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

#include "home_wallet.h"
#include "hw-account.h"
#include <QtAlgorithms>
#include "hw-misc.h"


static const char* create_table_sql = "CREATE TABLE t_accounts ("
"id    VARCHAR (36)   PRIMARY KEY,"
"name     VARCHAR (128),"
"type     INTEGER DEFAULT (0),"
"flags    INTEGER,"
"pos      INTEGER,"
"currency VARCHAR (36),"
"number   VARCHAR (128),"
"bankname VARCHAR (128),"
"initial  DOUBLE DEFAULT (0.0),"
"minimum  DOUBLE DEFAULT (0.0),"
"maximum  DOUBLE DEFAULT (0.0),"
"cheque1  INTEGER,"
"cheque2  INTEGER,"
"website  VARCHAR (128),"
"notes    VARCHAR (1024),"
"archive  VARCHAR (36),"
"[group]  VARCHAR (36),"
"cccday   INTEGER,"
"rdate    INTEGER,"
"is_closed                       BOOLEAN DEFAULT (false),"
"is_exclude_from_account_summary BOOLEAN DEFAULT (false),"
"is_outflow_in_summary           BOOLEAN DEFAULT (false),"
"is_exclude_from_budget          BOOLEAN DEFAULT (false),"
"is_exclude_from_any_reports     BOOLEAN DEFAULT (false),"
"bal_clear  DOUBLE DEFAULT (0.0),"
"bal_recon  DOUBLE DEFAULT (0.0),"
"bal_today  DOUBLE DEFAULT (0.0),"
"bal_future DOUBLE DEFAULT (0.0) "
");";


/* our global datas */
/* = = = = = = = = = = = = = = = = */

Account::Account(const QString& name, QObject *parent)
    : IHWFileIO(parent), name(name)
{

    printf("da_acc_malloc\n");
    this->currency = Preferences::Instance()->currency;

    id = "";
    flags = 0;
    type = 0;
            pos = 0;		//display position
            currency = "";
     number = "";
     bankname = "";
            group = "";
        initial = 0.;
        minimum = 0.;
        maximum = 0.;
        cheque1 = 0;
        cheque2 = 0;
    website = "";		//5.7 add
    notes = "";
        archive = "";
        cccday = 0;		//creditcard close day
        rdate = 0;		//last reconciled date
    window = nullptr;	//dsp_account window opened

    bal_recon  = 0.;	//bank balance (reconciled transaction)
    bal_clear  = 0.;	//cleared
    bal_today  = 0.;	//today balance (every transaction until today)
    bal_future = 0.;	//future balance (every transaction)

    xferincname = "";	//xfer payee display name: '< account'
    xferexpname = "";	//xfer payee display name: '> account'

    is_closed                       = false;
    is_exclude_from_account_summary = false;
    is_outflow_in_summary           = false;
    is_exclude_from_budget          = false;
    is_exclude_from_any_reports     = false;
}

Account::~Account()
{
    printf("da_acc_free\n");
        printf(" => %s, %s\n", this->id.toStdString().c_str(), this->name.toStdString().c_str());

}

QString Account::CreateTableSql()
{
    return create_table_sql;
}

QString Account::DataSql()
{
    if (id == ""){
        id = QUuid::createUuid().toString().remove("{").remove("}");
    }
    QString sql = QString("REPLACE INTO t_accounts VALUES ("
"'%1',"
"'%2',"
"%3,"
"%4,"
"'%5',"
"'%6',"
"'%7',"
"'%8',"
"%9,"
"%10,"
"%11,"
"%12,"
"%13,"
"'%14',"
"'%15',"
"'%16',"
"'%17',"
"%18,"
"%19,"
"%20,"
"%21,"
"%22,"
"%23,"
"%24,"
"%25,"
"%26,"
"%27,"
"%28);").arg(id                             )
            .arg(name                           )
            .arg(type                           )
            .arg(flags                          )
            .arg(pos                            )
            .arg(currency                       )
            .arg(number                         )
            .arg(bankname                       )
            .arg(initial                        )
            .arg(minimum                        )
            .arg(maximum                        )
            .arg(cheque1                        )
            .arg(cheque2                        )
            .arg(website                        )
            .arg(notes                          )
            .arg(archive                        )
            .arg(group                        )
            .arg(cccday                         )
            .arg(rdate                          )
            .arg(is_closed                      )
            .arg(is_exclude_from_account_summary)
            .arg(is_outflow_in_summary          )
            .arg(is_exclude_from_budget         )
            .arg(is_exclude_from_any_reports    )
            .arg(bal_clear                      )
            .arg(bal_recon                      )
            .arg(bal_today                      )
            .arg(bal_future                     );

    return sql;
}

QString Account::Tabname()
{
    return "t_accounts";
}

void Account::LoadFromSqlQuery(QSqlQuery &query)
{
    int idx = 0;
    id                              = query.value(idx++).toString();
    name                            = query.value(idx++).toString();
    type                            = query.value(idx++).toInt();
    flags                           = query.value(idx++).toInt();
    pos                             = query.value(idx++).toInt();
    currency                        = query.value(idx++).toString();
    number                          = query.value(idx++).toString();
    bankname                        = query.value(idx++).toString();
    initial                         = query.value(idx++).toDouble();
    minimum                         = query.value(idx++).toDouble();
    maximum                         = query.value(idx++).toDouble();
    cheque1                         = query.value(idx++).toInt();
    cheque2                         = query.value(idx++).toInt();
    website                         = query.value(idx++).toString();
    notes                           = query.value(idx++).toString();
    archive                         = query.value(idx++).toString();
    group                           = query.value(idx++).toString();
    cccday                          = query.value(idx++).toInt();
    rdate                           = query.value(idx++).toInt();
    is_closed                       = query.value(idx++).toBool();
    is_exclude_from_account_summary = query.value(idx++).toBool();
    is_outflow_in_summary           = query.value(idx++).toBool();
    is_exclude_from_budget          = query.value(idx++).toBool();
    is_exclude_from_any_reports     = query.value(idx++).toBool();
    bal_clear                       = query.value(idx++).toDouble();
    bal_recon                       = query.value(idx++).toDouble();
    bal_today                       = query.value(idx++).toDouble();
    bal_future                      = query.value(idx++).toDouble();
}

bool Account::has_website()
{
bool retval = false;

    if( this->website != nullptr )
	{
		//TODO: reinforce controls here
        if( this->website.size() > 4 )
			retval = true;
	}
	return retval;
}


/* 
 * change the account currency
 * change every txn to currency
 * #2026594 no more change target currency
 * #1673260 internal transfer with different currency
 *  => no more ensure dst xfer transaction account will be set to same currency
 */
void Account::set_currency(const QString kcur)
{
/*Account *dstacc;
bool *xfer_list;
quint32 maxkey, i;*/

	printf("\n[account] set currency\n");

    if(this->currency == kcur)
	{
		printf(" - already ok, return\n");
		return;
	}

    printf(" - set for '%s'\n", this->name.toStdString().c_str());

    QQueue<QSharedPointer<Transaction> >::const_iterator tIter = this->txn_queue.constBegin();
    while (tIter != this->txn_queue.constEnd()) {
        Transaction *txn = tIter->get();

        txn->kcur = kcur;
        /*if( (txn->flags & OF_INTXFER) && (txn->kxferacc > 0) && (txn->kxfer > 0) )
        {
            xfer_list[txn->kxferacc] = true;
        }*/

        ++tIter;
    }

    this->currency = kcur;
    printf(" - '%s'\n", this->name.toStdString().c_str());
	
	//#1673260 internal transfer with different currency
	/*for(i=1;i<maxkey;i++)
	{
		printf(" - %d '%d'\n", i, xfer_list[i]) );
		if( xfer_list[i] == true )
		{
            dstacc = Account::get(i);
            dstacc->set_currency(kcur);
		}
	}

	g_free(xfer_list);*/

}

bool Account::save_to_local()
{
    QSqlDatabase db = Preferences::Instance()->currentDatabase();
    return save(db) == 0;
}



void Account::compute_balances(const QString& accID)
{
    if (accID == ""){
        QList<QSharedPointer<Account>> acclist = Account::get_all_by_type();
        QList<QSharedPointer<Account>>::iterator iter = acclist.begin();
        while(iter != acclist.end()){
            qDebug("compute_balances account(%s)", iter->get()->name.toStdString().c_str());
            double clear = 0, recon = 0, today = 0, future = 0;
            Transaction::sum_by_account(iter->get()->id, future, today, clear, recon);
            iter->get()->bal_clear = iter->get()->initial + clear;
            iter->get()->bal_recon = iter->get()->initial + recon;
            iter->get()->bal_today = iter->get()->initial + today;
            iter->get()->bal_future = iter->get()->initial + future;
            printf("[%s %s:%d]\n", __FILE__, __FUNCTION__, __LINE__);
            iter->get()->save_to_local();
            printf("[%s %s:%d]\n", __FILE__, __FUNCTION__, __LINE__);
            ++iter;
        }
    } else {
        auto acc = Preferences::get_item<Account>(accID);
        if (acc == nullptr) return;
        double clear = 0, recon = 0, today = 0, future = 0;
        Transaction::sum_by_account(acc->id, future, today, clear, recon);
        acc->bal_clear = acc->initial + clear;
        acc->bal_recon = acc->initial + recon;
        acc->bal_today = acc->initial + today;
        acc->bal_future = acc->initial + future;
        acc->save_to_local();
        delete acc;
    }
}

void Account::update_to_local(Account *acc)
{
    acc->save_to_local();
}

void Account::del_from_local(Account *acc)
{
    if (Preferences::Instance()->XHWFilepath() == ""){
        qWarning("no xhw file defined");
        return;
    }
    if(acc == nullptr){
        qWarning("invalid arg");
        return;
    }
    {
        QSqlDatabase db = Preferences::Instance()->currentDatabase();
        if (db.isOpen()) {
            qDebug() << "Database opened successfully！";
        } else {
            qDebug("can't open database(%s) because:%s",Preferences::Instance()->XHWFilepath().toStdString().c_str(), db.lastError().text().toStdString().c_str());
            goto End;
        }
        QString sql = QString("delete from t_accounts where id='%1';").arg(acc->id);
        QSqlQuery query(db);
        if(!query.exec(sql)){
            qWarning("exec sql(%s) failed, because:%s", sql.toStdString().c_str(), db.lastError().text().toStdString().c_str());
        }
    }
End:
    return;
}

QList<QSharedPointer<Account> > Account::get_all_by_type(int type)
{
    QString where = "";
    if (type != ACC_TYPE_MAXVALUE) where= QString("where type=%1").arg(type);
    return Preferences::get_all<Account>(where);
}


