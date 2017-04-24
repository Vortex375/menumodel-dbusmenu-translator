/*
 * plasma-workspace-appmenu
 * Copyright (C) 2017 Konstantin Pugin <ria.freelander@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QObject>
#include <QByteArray>
#include <QPointer>
#include <QMenu>

#pragma once

static const QByteArray s_x11QtServiceNamePropertyName = QByteArrayLiteral("_KDE_NET_WM_APPMENU_SERVICE_NAME");
static const QByteArray s_x11QtObjectPathPropertyName = QByteArrayLiteral("_KDE_NET_WM_APPMENU_OBJECT_PATH");

static const QByteArray s_x11GtkServiceNamePropertyName = QByteArrayLiteral("_GTK_UNIQUE_BUS_NAME");
static const QByteArray s_x11GtkAppMenuObjectPathPropertyName = QByteArrayLiteral("_GTK_APP_MENU_OBJECT_PATH");
static const QByteArray s_x11GtkMenuBarObjectPathropertyName = QByteArrayLiteral("_GTK_MENUBAR_OBJECT_PATH");
static const QByteArray s_x11GtkApplicationObjectPathPropertyName = QByteArrayLiteral("_GTK_APPLICATION_OBJECT_PATH");
static const QByteArray s_x11GtkWindowObjectPathPropertyName = QByteArrayLiteral("_GTK_WINDOW_OBJECT_PATH");
static const QByteArray s_x11UnityObjectPathPropertyName = QByteArrayLiteral("_UNITY_OBJECT_PATH");

class MenuAdaptor : public QObject
{

    Q_OBJECT
    Q_PROPERTY(bool menuAvailable READ menuAvailable NOTIFY menuAvailableChanged)
    Q_PROPERTY(QMenu* menu READ menu)

public:
    enum MenuObjectPathRole
    {
        AppMenu,
        MenuBar,
        Application,
        Window,
        Unity
    };
    Q_ENUM(MenuObjectPathRole)
    explicit MenuAdaptor(QObject *parent = 0):QObject(parent){}
    virtual ~MenuAdaptor() = default;
    virtual void updateApplicationMenu(const QString& serviceName, const QMap<MenuObjectPathRole,QString>& objectPaths)
    {
        Q_UNUSED(serviceName);
        Q_UNUSED(objectPaths);
    }
    QMenu* menu()
    {
        return m_menu;
    }
    bool menuAvailable()
    {
        return m_menuAvailable;
    }

Q_SIGNALS:
    void adaptorUpdated();
    void menuAvailableChanged();
protected:
    QPointer<QMenu> m_menu;
    bool m_menuAvailable;
};
