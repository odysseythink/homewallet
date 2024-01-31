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

#ifndef __HB_PAYEE_H__
#define __HB_PAYEE_H__

#include <QObject>
#include <QDebug>
#include <stdint.h>
#include <QUuid>
#include <QSharedPointer>
#include "hw-file.h"

struct Payee : public IHWFileIO
{
    Q_OBJECT
public:
    explicit Payee(QObject *parent = nullptr);
    ~Payee();
    virtual QString CreateTableSql() override;
    virtual QString DataSql() override;
    virtual QString Tabname() override;
    virtual void LoadFromSqlQuery(QSqlQuery& query) override;
    static QList<QSharedPointer<Payee> > get_all_by_category(const QString& cat);

    QString   	id;
	unsigned short		paymode;
	unsigned short 	flags;		//_pad1 prior 5.6
    QString		category;
    QString name;
    QString notes;		//added 5.6

	/* unsaved datas */
	//bool	flt_select;
	quint16		nb_use_txn;
	quint16		nb_use_all;
};

#define PF_HIDDEN	(1<<8)	//hidden by user



#endif
