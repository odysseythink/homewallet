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

#ifndef __HB_ACCOUNT_H__
#define __HB_ACCOUNT_H__

#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QWidget>
#include <QList>
#include <QQueue>
#include "hw-file.h"


// data flags
//unsigned short is 2 bytes / 16 bits
//FREE (1<<0)
#define AF_CLOSED		(1<<1)

#define AF_NOSUMMARY	(1<<4)
#define AF_NOBUDGET		(1<<5)
#define AF_NOREPORT		(1<<6)
#define AF_OUTFLOWSUM	(1<<7)

//unsaved tmp flags
#define AF_ADDED		(1<<9)	//was 1<<2 < 5.5.3
#define AF_CHANGED		(1<<10)	//was 1<<3 < 5.5.3

//deprecated
#define AF_OLDBUDGET	(1<<0)


enum
{
// + https://www.kashoo.com/blog/what-are-the-different-account-types-in-accounting/

    ACC_TYPE_NONE       = 0,
    ACC_TYPE_BANK       = 1,	//Banque
    ACC_TYPE_CASH       = 2,	//Espèce
    ACC_TYPE_ASSET      = 3,	//Actif (avoir)
    ACC_TYPE_CREDITCARD = 4,	//Carte crédit
    ACC_TYPE_LIABILITY  = 5,	//Passif (dettes)
    ACC_TYPE_CHECKING	= 6, 	//OFX A standard checking account
    ACC_TYPE_SAVINGS 	= 7,	//OFX A standard savings account
    //	OFX_MONEYMRKT 	OFX A money market account
    //	OFX_CREDITLINE 	OFX A line of credit
    //	OFX_INVESTMENT 	OFX An investment account
    //	ACC_TYPE_STOCK      = 11,	//Actions
    //ACC_TYPE_MUTUALFUND = 12,	//Fond de placement
    //ACC_TYPE_INCOME     = 13,	//Revenus
    //ACC_TYPE_EXPENSE    = 14,	//Dépenses
    //ACC_TYPE_EQUITY     = 15,	//Capitaux propres
//	ACC_TYPE_,
    ACC_TYPE_MAXVALUE
};


enum {
    ACC_USAGE_NONE,
    ACC_USAGE_TXN,
    ACC_USAGE_TXN_XFER,
    ACC_USAGE_ARC,
    ACC_USAGE_ARC_XFER
};



class Transaction;
class Account: public IHWFileIO
{
    Q_OBJECT
public:
    Account(const QString& name = "", QObject* parent = nullptr);
    ~Account();
    virtual QString CreateTableSql() override;
    virtual QString DataSql() override;
    virtual QString Tabname() override;
    virtual void LoadFromSqlQuery(QSqlQuery& query) override;

    bool has_website();
    void set_currency(const QString currency);
    static void compute_balances(Transaction *txn, Account* acc = nullptr);
    static void compute_balances(const QString& acc = "");
    bool balances_add(Transaction *txn);
    bool balances_sub(Transaction *txn);
    void convert_euro();
    static void update_to_local(Account* acc);
    static void del_from_local(Account* acc);
    static QList<QSharedPointer<Account>> get_all_by_type(int type = ACC_TYPE_MAXVALUE);
    bool save_to_local();

public:
    QString		id;
	unsigned short		flags;
	unsigned short		type;
	quint32		pos;		//display position
    QString		currency;
    QString name;
    QString number;
    QString bankname;
    QString		group;
	double		initial;
	//double		warning;
	double		minimum;
	double		maximum;
	quint32		cheque1;
	quint32		cheque2;
    QString website;		//5.7 add
    QString notes;
    QString		archive;
    quint16		cccday;		//creditcard close day
	quint32		rdate;		//last reconciled date

	/* unsaved datas */
    QQueue<QSharedPointer<Transaction>>		txn_queue;
    QWidget	*window;	//dsp_account window opened


    double bal_clear ;	//cleared
    double bal_recon ;	//bank balance (reconciled transaction)
    double bal_today ;	//today balance (every transaction until today)
    double bal_future;	//future balance (every transaction)

    QString xferincname;	//xfer payee display name: '< account'
    QString xferexpname;	//xfer payee display name: '> account'

	//bool	flt_select;		//true if selected into filter
    bool is_closed                      ;
    bool is_exclude_from_account_summary;
    bool is_outflow_in_summary          ;
    bool is_exclude_from_budget         ;
    bool is_exclude_from_any_reports    ;
};




#endif
