#include "gtkmenuadaptor.h"

GtkMenuAdaptor::GtkMenuAdaptor(QObject *parent): MenuAdaptor (parent)
{

}

void GtkMenuAdaptor::updateApplicationMenu(const QString &serviceName, const QMap<MenuAdaptor::MenuObjectPathRole,QString>& objectPaths)
{
    m_appmenu.setBusType(DBusEnums::SessionBus);
    m_appmenu.setBusName(serviceName);
    m_appmenu.setObjectPath(objectPaths[MenuAdaptor::AppMenu]);
    m_menubar.setBusType(DBusEnums::SessionBus);
    m_menubar.setBusName(serviceName);
    m_menubar.setObjectPath(objectPaths[MenuAdaptor::MenuBar]);
    m_application.setBusType(DBusEnums::SessionBus);
    m_application.setBusName(serviceName);
    m_application.setObjectPath(objectPaths[MenuAdaptor::Application]);
    m_window.setBusType(DBusEnums::SessionBus);
    m_window.setBusName(serviceName);
    m_window.setObjectPath(objectPaths[MenuAdaptor::Window]);
    m_unity.setBusType(DBusEnums::SessionBus);
    m_unity.setBusName(serviceName);
    m_unity.setObjectPath(objectPaths[MenuAdaptor::Unity]);
}
