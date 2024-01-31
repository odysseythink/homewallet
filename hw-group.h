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

#ifndef __HB_GROUP_H__
#define __HB_GROUP_H__

#include <QObject>
#include <QDebug>
#include <stdint.h>
#include "hw-file.h"


class Group : public IHWFileIO
{
    Q_OBJECT
public:
    explicit Group(QObject *parent = nullptr);
    ~Group();
    virtual QString CreateTableSql() override;
    virtual QString DataSql() override;
    virtual QString Tabname() override;
    virtual void LoadFromSqlQuery(QSqlQuery& query) override;

    QString   	id;
	//gushort		flags;
	//gushort		type;
    QString name;
};


/*typedef enum
{
	GRP_TYPE_ACC = 1,
	//GRP_TYPE_PAY
	//GRP_TYPE_CAT
} HbGroupType;
*/


#endif
