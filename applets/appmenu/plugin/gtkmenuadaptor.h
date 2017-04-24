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
#include "menuadaptor.h"
#include "qdbusmenumodel.h"
#include "qdbusactiongroup.h"

class GtkMenuAdaptor: public MenuAdaptor
{
    Q_OBJECT
public:
    explicit GtkMenuAdaptor(QObject *parent = 0);
    virtual ~GtkMenuAdaptor() = default;
    void updateApplicationMenu(const QString& serviceName, const QMap<MenuAdaptor::MenuObjectPathRole,QString>& objectPaths) override;
private:
    QDBusMenuModel m_appmenu;
    QDBusMenuModel m_menubar;
    QDBusActionGroup m_application;
    QDBusActionGroup m_window;
    QDBusActionGroup m_unity;
};

