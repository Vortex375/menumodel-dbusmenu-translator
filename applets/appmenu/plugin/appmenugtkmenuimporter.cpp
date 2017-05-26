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

AppMenuGtkMenuImporter::AppMenuGtkMenuImporter(const QString &service, const QString &path,QObject *parent):QObject(parent)
{

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
