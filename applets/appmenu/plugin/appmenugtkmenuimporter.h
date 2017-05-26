/* This file is part of the appmenu plugin
   Copyright 2017 Athor
   Author: Aurelien Gateau <ria.freelander@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License (LGPL) as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef APPMENUGTKMENUIMPORTER_H
#define APPMENUGTKMENUIMPORTER_H

#include <QtCore/QObject>
#include <glib.h>

class QAction;
class QDBusPendingCallWatcher;
class QDBusVariant;
class QIcon;
class QMenu;

class AppMenuGtkMenuImporterPrivate;


class AppMenuGtkMenuImporter: public QObject
{
    Q_OBJECT
public:
    AppMenuGtkMenuImporter(const QString &service, const QString &path, QObject *parent = 0);
    ~AppMenuGtkMenuImporter() override;
    QMenu *menu() const;
public Q_SLOTS:

    void updateMenu(QMenu *menu);
Q_SIGNALS:
    void menuUpdated(QMenu *);
    void actionActivationRequested(QAction *);

protected:
    virtual QMenu *createMenu(QWidget *parent) const;
    virtual QIcon iconForName(const QString &);

private:
    QMenu* m_menu;
    Q_DISABLE_COPY(AppMenuGtkMenuImporter)
};

#endif
