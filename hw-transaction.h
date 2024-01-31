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


#ifndef __HB_TRANSACTION_H__
#define __HB_TRANSACTION_H__

#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QWidget>
#include <QList>
#include <QSharedPointer>
#include <QQueue>
#include <QDateTime>
#include "hw-split.h"
#include "hw-archive.h"
#include "hw-account.h"
#include "hw-file.h"

class Transaction: public IHWFileIO
{
    Q_OBJECT
public:
    Transaction(QObject* parent = nullptr);
    Transaction(Transaction* src);
    ~Transaction();
    virtual QString CreateTableSql() override;
    virtual QString DataSql() override;
    virtual QString Tabname() override;
    virtual void LoadFromSqlQuery(QSqlQuery& query) override;
    const Transaction& operator=(const Transaction& a);
    bool is_balanceable();
    bool add_to_local();
    static QList<QSharedPointer<Transaction>> get_all_by_account(const QString& acc = "");

public:
    QString id;
    double		amount;
    QString		account;
    unsigned short		paymode;
    unsigned short		flags;
    QString		payee;
    QString		category;
    QString memo;

    QDateTime		created_at;
    unsigned short		pos;
    unsigned short     status;
    QString info;
    QStringList tags;
    QString		kxfer;		//strong link xfer key
    QString		xfer_account;
    double		xfer_amount;	//xfer target amount

    QList<QSharedPointer<Split> > splits;

	/* unsaved datas */
    QString		kcur;		//init at loadxml (preprend) + add
    unsigned short		dspflags;	//
    unsigned short		matchrate;	//used only when find xfer target
    double		balance;	//init at dsp balance refresh
	//GList		*same;		//used for import todo: change this
    QString type;
};

#include "hw-archive.h"

// display flags (not related to below real flags)
#define	TXN_DSPFLG_OVER		(1<<1)
#define	TXN_DSPFLG_LOWBAL	(1<<2)

#define	TXN_DSPFLG_DUPSRC	(1<<9)
#define	TXN_DSPFLG_DUPDST	(1<<10)


// data flags
//unsigned short is 2 bytes / 16 bits
//FREE (1<<0) 
#define OF_INCOME	(1<<1)
#define OF_AUTO		(1<<2)	//scheduled
#define OF_INTXFER	(1<<3)
#define OF_ADVXFER  (1<<4)	//xfer with != kcur
//FREE (1<<5)
#define OF_CHEQ2	(1<<6)
#define OF_LIMIT	(1<<7)	//scheduled
#define OF_SPLIT	(1<<8)

//unsaved tmp flags
#define OF_ADDED		(1<<9)  //was 1<<3 < 5.3
#define OF_CHANGED		(1<<10) //was 1<<4 < 5.3
#define OF_PREFILLED	(1<<11)

//deprecated since 5.x
#define OLDF_VALID	(1<<0)  
#define OLDF_REMIND	(1<<5)


typedef enum {
	TXN_STATUS_NONE,
	TXN_STATUS_CLEARED,
	TXN_STATUS_RECONCILED,
	TXN_STATUS_REMIND,
	TXN_STATUS_VOID
} HbTxnStatus;

enum {
	TXN_MARK_NONE,
	TXN_MARK_DUPSRC,
	TXN_MARK_DUPDST
};


typedef enum
{
	TXN_DLG_ACTION_NONE,
	TXN_DLG_ACTION_ADD,
	TXN_DLG_ACTION_INHERIT,
	TXN_DLG_ACTION_EDIT
} HbTxnDlgAction;


typedef enum
{
	TXN_DLG_TYPE_NONE,
	TXN_DLG_TYPE_TXN,
	TXN_DLG_TYPE_TPL,
	TXN_DLG_TYPE_SCH
} HbTxnDlgType;

#endif
