
#include <QIcon>

#include <dbusmenuimporter.h>
#include <kdbusimporter.h>

#include "dbusmenuadaptor.h"

DBusMenuAdaptor::DBusMenuAdaptor(QObject *parent): MenuAdaptor (parent)
{

}

void DBusMenuAdaptor::updateApplicationMenu(const QString &serviceName, const QMap<MenuAdaptor::MenuObjectPathRole,QString>& objectPaths)
{
    if (m_serviceName == serviceName/* && m_menuObjectPath == menuObjectPath*/) {
        if (m_importer) {
            QMetaObject::invokeMethod(m_importer, "updateMenu", Qt::QueuedConnection);
        }
        return;
    }

    m_serviceName = serviceName;
    m_menuObjectPath = objectPaths[MenuObjectPathRole::MenuBar];

    if (m_importer) {
        m_importer->deleteLater();
    }

    m_importer = new KDBusMenuImporter(serviceName, m_menuObjectPath, this);
    QMetaObject::invokeMethod(m_importer, "updateMenu", Qt::QueuedConnection);

    connect(m_importer.data(), &DBusMenuImporter::menuUpdated, this, [=](QMenu *menu) {
        m_menu = m_importer->menu();
        if (m_menu.isNull() || menu != m_menu) {
            return;
        }

        //cache first layer of sub menus, which we'll be popping up
        for(QAction *a: m_menu->actions()) {
            if (a->menu()) {
                m_importer->updateMenu(a->menu());
            }
        }

        QObject::setProperty("menuAvailable",true);
        Q_EMIT adaptorUpdated();
    });
}
