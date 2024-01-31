/*  HomeBank -- Free, easy, personal accounting for everyone.
 *  Copyright (C) 1995-2023 Maxime DOYEN
 *
 *  This file is part of HomeBank.
 *
 *  HomeBank is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  HomeBank is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __HB_HBFILE_H__
#define __HB_HBFILE_H__

#include <QObject>
#include <QString>
#include <QUuid>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QException>
#include "enums.h"

class IHWFileIO: public QObject
{
    Q_OBJECT
public:
    IHWFileIO(QObject* parent = nullptr):QObject(parent){}
    ~IHWFileIO(){}
    virtual QString CreateTableSql() = 0;
    virtual QString DataSql() = 0;
    virtual QString Tabname() = 0;
    virtual void LoadFromSqlQuery(QSqlQuery& query) = 0;
    virtual QString PrimaryKey(){return "id";}
    virtual int save(QSqlDatabase& db);
    static bool table_exists(QString table);

    static int new_xml(const QString filename);
    static void setup(bool file_clear);
};

int homebank_save_xml(const QString filename);
int homebank_new_xml(const QString filename);


#endif

