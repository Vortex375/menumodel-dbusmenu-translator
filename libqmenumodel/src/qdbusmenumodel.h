/*
 * Copyright 2012 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *      Renato Araujo Oliveira Filho <renato@canonical.com>
 */

#ifndef QDBUSMENUMODEL_H
#define QDBUSMENUMODEL_H

#include "qdbusobject.h"
#include "qmenumodel.h"

class QDBusMenuModel : public QMenuModel, public QDBusObject
{
    Q_OBJECT
    Q_PROPERTY(int busType READ busType WRITE setIntBusType NOTIFY busTypeChanged)
    Q_PROPERTY(QString busName READ busName WRITE setBusName NOTIFY busNameChanged)
    Q_PROPERTY(QString objectPath READ objectPath WRITE setObjectPath NOTIFY objectPathChanged)
    Q_PROPERTY(int status READ status NOTIFY statusChanged)

public:
    QDBusMenuModel(QObject *parent=0);
    ~QDBusMenuModel();

Q_SIGNALS:
    void busTypeChanged(DBusEnums::BusType type);
    void busNameChanged(const QString &busNameChanged);
    void objectPathChanged(const QString &objectPath);
    void statusChanged(DBusEnums::ConnectionStatus status);

public Q_SLOTS:
    void start();
    void stop();

protected:
    virtual void serviceAppear(GDBusConnection *connection);
    virtual void serviceVanish(GDBusConnection *connection);

    virtual bool event(QEvent* e);

private:
    // workaround to support int as busType
    void setIntBusType(int busType);
};

#endif
