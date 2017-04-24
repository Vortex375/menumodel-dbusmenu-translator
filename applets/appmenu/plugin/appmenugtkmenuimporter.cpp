 #include "appmenugtkmenuimporter.h"

#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusVariant>
#include <QFont>
#include <QMenu>
#include <QPointer>
#include <QTime>
#include <QTimer>
#include <QToolButton>
#include <QWidgetAction>
#include <QSet>
#include <QDebug>



AppMenuGtkMenuImporter::AppMenuGtkMenuImporter(const QString &service, const QString &menubar_path, const QString &appmenu_path,
                                               const QString &application_path, const QString& window_path, const QString& unity_path,
                                               QObject *parent):QObject(parent)
{
    this->m_application.setBusName(service);
    this->m_window.setBusName(service);
    this->m_unity.setBusName(service);
    this->m_appmenu.setBusName(service);
    this->m_menubar.setBusName(service);
    this->m_appmenu.setObjectPath(appmenu_path);
    this->m_menubar.setObjectPath(menubar_path);
    this->m_application.setObjectPath(application_path);
    this->m_window.setObjectPath(window_path);
    this->m_unity.setObjectPath(unity_path);
}

AppMenuGtkMenuImporter::~AppMenuGtkMenuImporter()
{

}

QMenu* AppMenuGtkMenuImporter::menu() const
{
    return this->createMenu(nullptr);
}

void AppMenuGtkMenuImporter::updateMenu(QMenu* menu)
{

}

QMenu *AppMenuGtkMenuImporter::createMenu(QWidget *parent) const
{
    return new QMenu(parent);
}

QIcon AppMenuGtkMenuImporter::iconForName(const QString &/*name*/)
{
    return QIcon();
}
