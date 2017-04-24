/******************************************************************
 * Copyright 2016 Kai Uwe Broulik <kde@privat.broulik.de>
 * Copyright 2016 Chinmoy Ranjan Pradhan <chinmoyrp65@gmail.com>
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************/

#include "appmenumodel.h"

#include <config-X11.h>

#if HAVE_X11
#include <QX11Info>
#include <xcb/xcb.h>
#endif

#include <QAction>
#include <QMenu>
#include <QDebug>
#include <QDBusConnection>
#include <QDBusConnectionInterface>

#include <dbusmenuimporter.h>

#include "dbusmenuadaptor.h"
#include "gtkmenuadaptor.h"


AppMenuModel::AppMenuModel(QObject *parent)
            : QAbstractListModel(parent)
{
    connect(KWindowSystem::self(), &KWindowSystem::activeWindowChanged, this, &AppMenuModel::onActiveWindowChanged);
    connect(this, &AppMenuModel::modelNeedsUpdate, this, &AppMenuModel::update, Qt::UniqueConnection);
    onActiveWindowChanged(KWindowSystem::activeWindow());

    //if our current DBus connection gets lost, close the menu
    //we'll select the new menu when the focus changes
    connect(QDBusConnection::sessionBus().interface(), &QDBusConnectionInterface::serviceOwnerChanged, this, [this](const QString &serviceName, const QString &oldOwner, const QString &newOwner)
    {
        Q_UNUSED(oldOwner);
        if ((serviceName == m_serviceName_gtk || serviceName == m_serviceName_kde) && newOwner.isEmpty()) {
            m_adaptor = nullptr;
            Q_EMIT modelNeedsUpdate();
        }
    });
}

AppMenuModel::~AppMenuModel() = default;

bool AppMenuModel::menuAvailable() const
{
    if(m_adaptor)
        return m_adaptor->menuAvailable();
    else
        return false;
}

int AppMenuModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_activeMenu.count();
}

void AppMenuModel::update()
{
    beginResetModel();
    if (!m_activeMenu.isEmpty() && !m_activeActions.isEmpty()) {
        m_activeMenu.clear();
        m_activeActions.clear();
    }

    if (m_adaptor && m_adaptor->menu() && m_adaptor->menuAvailable()) {
        const auto &actions = m_adaptor->menu()->actions();
        for (QAction *action : actions) {
            m_activeActions.append(action);
            m_activeMenu.append(action->text());
        }
    }

    endResetModel();
}


void AppMenuModel::onActiveWindowChanged(WId id)
{
#if HAVE_X11
    if (KWindowSystem::isPlatformX11()) {
        auto *c = QX11Info::connection();

        static QHash<QByteArray, xcb_atom_t> s_atoms;

        auto getWindowPropertyString = [c,this](WId id, const QByteArray &name) -> QByteArray {
            QByteArray value;
            if (!s_atoms.contains(name)) {
                const xcb_intern_atom_cookie_t atomCookie = xcb_intern_atom(c, false, name.length(), name.constData());
                QScopedPointer<xcb_intern_atom_reply_t, QScopedPointerPodDeleter> atomReply(xcb_intern_atom_reply(c, atomCookie, Q_NULLPTR));
                if (atomReply.isNull()) {
                    return value;
                }

                s_atoms[name] = atomReply->atom;
                if (s_atoms[name] == XCB_ATOM_NONE) {
                     return value;
                }
            }

            static const long MAX_PROP_SIZE = 10000;
            auto propertyCookie = xcb_get_property(c, false, id, s_atoms[name], XCB_ATOM_STRING, 0, MAX_PROP_SIZE);
            QScopedPointer<xcb_get_property_reply_t, QScopedPointerPodDeleter> propertyReply(xcb_get_property_reply(c, propertyCookie, NULL));
            if (propertyReply.isNull()) {
                return value;
            }

            if (propertyReply->type == XCB_ATOM_STRING && propertyReply->format == 8 && propertyReply->value_len > 0) {
                const char *data = (const char *) xcb_get_property_value(propertyReply.data());
                int len = propertyReply->value_len;
                if (data) {
                    value = QByteArray(data, data[len - 1] ? len : len - 1);
                }
            }

            return value;
        };

        auto updateMenuFromWindowIfHasMenu = [this, &getWindowPropertyString](WId id) {
            const QString serviceNameKde = QString::fromUtf8(getWindowPropertyString(id, s_x11QtServiceNamePropertyName));
            if (!serviceNameKde.isEmpty()) {
                m_adaptor = new DBusMenuAdaptor();
                connect(m_adaptor,&MenuAdaptor::menuAvailableChanged,this,[this](){
                    Q_EMIT this->menuAvailableChanged();
                });
                QMap<MenuAdaptor::MenuObjectPathRole,QString> objectPaths;
                const QString objectPathKde = QString::fromUtf8(getWindowPropertyString(id, s_x11QtObjectPathPropertyName));
                objectPaths[MenuAdaptor::MenuBar] = objectPathKde;
                m_adaptor->updateApplicationMenu(serviceNameKde,objectPaths);
                return true;
            }
            const QString serviceNameGtk = QString::fromUtf8(getWindowPropertyString(id, s_x11GtkServiceNamePropertyName));
            if (!serviceNameGtk.isEmpty())
            {
                m_adaptor = new GtkMenuAdaptor();
                QMap<MenuAdaptor::MenuObjectPathRole,QString> objectPaths;                ;
                const QString objectPathGtkAppmenu = QString::fromUtf8(getWindowPropertyString(id, s_x11GtkAppMenuObjectPathPropertyName));
                objectPaths[MenuAdaptor::AppMenu] = (objectPathGtkAppmenu);
                const QString objectPathGtkMenubar = QString::fromUtf8(getWindowPropertyString(id, s_x11GtkMenuBarObjectPathropertyName));
                objectPaths[MenuAdaptor::MenuBar] = objectPathGtkMenubar;
                const QString objectPathGtkApp = QString::fromUtf8(getWindowPropertyString(id, s_x11GtkApplicationObjectPathPropertyName));
                objectPaths[MenuAdaptor::Application] = objectPathGtkApp;
                const QString objectPathGtkWin = QString::fromUtf8(getWindowPropertyString(id, s_x11GtkWindowObjectPathPropertyName));
                objectPaths[MenuAdaptor::Window] = (objectPathGtkWin);
                const QString objectPathUnity = QString::fromUtf8(getWindowPropertyString(id, s_x11UnityObjectPathPropertyName));
                objectPaths[MenuAdaptor::Unity] = (objectPathUnity);
                m_adaptor->updateApplicationMenu(serviceNameGtk,objectPaths);
                return true;
            }
            return false;
        };

        KWindowInfo info(id, NET::WMState | NET::WMWindowType, NET::WM2TransientFor);
        if (info.hasState(NET::SkipTaskbar) ||
                info.windowType(NET::UtilityMask) == NET::Utility ||
                info.windowType(NET::DesktopMask) == NET::Desktop) {
            return;
        }

        WId transientId = info.transientFor();
        // lok at transient windows first
        while (transientId) {
            if (updateMenuFromWindowIfHasMenu(transientId)) {
                return;
            }
            transientId = KWindowInfo(transientId, 0, NET::WM2TransientFor).transientFor();
        }

        if (updateMenuFromWindowIfHasMenu(id)) {
            return;
        }

        //no menu found, set it to unavailable
        m_adaptor = nullptr;
        Q_EMIT modelNeedsUpdate();
    }
#endif

}


QHash<int, QByteArray> AppMenuModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames[MenuRole] = "activeMenu";
    roleNames[ActionRole] = "activeActions";
    return roleNames;
}

QVariant AppMenuModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 ) {
        return QVariant();
    }

    if (role == MenuRole) {
        return m_activeMenu.at(row);
    } else if(role == ActionRole) {
        const QVariant data = qVariantFromValue((void *) m_activeActions.at(row));
        return data;
    }

    return QVariant();
}
