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

#include "hw-transaction.h"
#include "hw-tag.h"
#include "hw-split.h"
#include "enums.h"
#include "hw-preferences.h"


/* our global datas */
static const char* create_table_sql = "CREATE TABLE t_transactions ("
"id           VARCHAR (36)   PRIMARY KEY,"
"created_at   DATETIME,"
"amount       DOUBLE,"
"account      VARCHAR (36),"
"xfer_amount  DOUBLE,"
"xfer_account VARCHAR (36),"
"paymode      INT,"
"status       INT,"
"flags        INT,"
"payee        VARCHAR (36),"
"category     VARCHAR (36),"
"memo         VARCHAR (1024),"
"info         VARCHAR (1024),"
"tags         VARCHAR (1024),"
"xfer         VARCHAR (36),"
"scat         VARCHAR (1024),"
"samt         VARCHAR (1024),"
"smem         VARCHAR (1024),"
"type         VARCHAR (10) "
");";


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */
Transaction::Transaction(QObject* parent)
    : IHWFileIO(parent)
{

}

Transaction::Transaction(Transaction *src)
{
    if (src != nullptr){
        //duplicate the string
        this->memo = src->memo;
        this->info = src->info;

        //duplicate tags/splits
        //no g_free here to avoid free the src tags (memdup copied the ptr)
        this->tags = src->tags;

        if( this->splits.size() > 0 )
            this->flags |= OF_SPLIT; //Flag that Splits are active
    }
}

Transaction::~Transaction()
{
}

QString Transaction::CreateTableSql()
{
    return create_table_sql;
}


QString Transaction::DataSql()
{
    flags &= ~(OF_AUTO|OF_ADDED|OF_CHANGED);
    QString tagstr = Tag::tags_tostring(tags);
    QString cats, amounts, memos;

    QString sql = QString("REPLACE INTO t_transactions (id,"
"created_at,"
"amount,"
"account,"
"xfer_amount,"
"xfer_account,"
"paymode,"
"status,"
"flags,"
"payee,"
"category,"
"memo,"
"info,"
"tags,"
"xfer,"
"scat,"
"samt,"
"smem,type) VALUES ('%1',"
"'%2',"
"%3,"
"'%4',"
"%5,"
"'%6',"
"%7,"
"%8,"
"%9,"
"'%10',"
"'%11',"
"'%12',"
"'%13',"
"'%14',"
"'%15',"
"'%16',"
"'%17',"
"'%18', '%19');").arg(id)
          .arg(created_at.toString("yyyy-MM-dd hh:mm:ss"))
          .arg(amount)
          .arg(account)
          .arg(xfer_amount)
          .arg(xfer_account)
          .arg(paymode)
          .arg(status)
          .arg(flags)
          .arg(payee)
          .arg(category)
          .arg(memo)
          .arg(info)
          .arg(tagstr)
          .arg(kxfer)
          .arg(cats)
          .arg(amounts)
          .arg(memos)
          .arg(type);
    return sql;
}

QString Transaction::Tabname()
{
    return "t_transactions";
}

void Transaction::LoadFromSqlQuery(QSqlQuery &query)
{
    int idx = 0;
    id           = query.value(idx++).toString();
    created_at   = query.value(idx++).toDateTime();
    amount       = query.value(idx++).toDouble();
    account      = query.value(idx++).toString();
    xfer_amount  = query.value(idx++).toDouble();
    xfer_account = query.value(idx++).toString();
    paymode      = query.value(idx++).toInt();
    status       = query.value(idx++).toInt();
    flags        = query.value(idx++).toInt();
    payee        = query.value(idx++).toString();
    category     = query.value(idx++).toString();
    memo         = query.value(idx++).toString();
    info         = query.value(idx++).toString();
    tags         = query.value(idx++).toString().split(" ");
    kxfer        = query.value(idx++).toString();
    auto scat          = query.value(idx++).toString();
    auto samt          = query.value(idx++).toString();
    auto smem          = query.value(idx++).toString();
    type         = query.value(idx++).toString();
}


const Transaction &Transaction::operator =(const Transaction &a)
{
    if(this == &a){
        return *this;
    }

    //duplicate the string
    this->memo = a.memo;
    this->info = a.info;

    //duplicate tags/splits
    //no g_free here to avoid free the src tags (memdup copied the ptr)
    this->tags = a.tags;

    if( this->splits.size() > 0 )
        this->flags |= OF_SPLIT; //Flag that Splits are active
    return *this;
}


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */


bool Transaction::is_balanceable()
{
bool retval = true;

    if( status ==  TXN_STATUS_VOID )
		retval = false;
	//#1812598
    else if( (status == TXN_STATUS_REMIND) && (Preferences::Instance()->includeremind() == false) )
		retval = false;

	   return retval;
}


bool Transaction::add_to_local()
{
    QSqlDatabase db = Preferences::Instance()->currentDatabase();
    return save(db) == 0;
}

QList<QSharedPointer<Transaction> > Transaction::get_all_by_account(const QString &acc)
{
    QString where = "";
    if (acc != "") where= QString("where account='%1'").arg(acc);
    return Preferences::get_all<Transaction>(where);
}

bool Transaction::sum_by_account(const QString& acc, double &bal_future, double& bal_today, double& bal_clear, double& bal_recon)
{
    if (Preferences::Instance()->XHWFilepath() == ""){
        qWarning("no xhw file defined");
        return false;
    }
    {
        QSqlDatabase db = Preferences::Instance()->currentDatabase();
        if (db.isOpen()) {
            qDebug() << "Database opened successfully！";
        } else {
            qDebug("can't open database(%s) because:%s",Preferences::Instance()->XHWFilepath().toStdString().c_str(), db.lastError().text().toStdString().c_str());
            return false;
        }
        QSqlQuery query(db);
        QString sql = QString("select status,sum(amount) from t_transactions where account = '%1' group by status;").arg(acc);
        if (!query.exec(sql)){
            qDebug("query sql(%s) failed:%s",sql.toStdString().c_str(), db.lastError().text().toStdString().c_str());
            return false;
        }
        bal_clear = 0;
        bal_recon = 0;
        bal_today = 0;
        bal_future = 0;
        while (query.next()){
            int status = query.value(0).toInt();
            double amount = query.value(1).toDouble();
            bal_today += amount;
            bal_future += amount;

            if(status == TXN_STATUS_CLEARED)
                bal_clear += amount;

            if(status == TXN_STATUS_RECONCILED)
            {
                bal_recon += amount;
                bal_clear += amount;
            }
        }
        sql = QString("select status,sum(amount) from t_transactions where xfer_account = '%1' and type = '%2' group by status;").arg(acc).arg(TXN_TYPE_TRANSFER);
        if (!query.exec(sql)){
            qDebug("query sql(%s) failed:%s",sql.toStdString().c_str(), db.lastError().text().toStdString().c_str());
            return false;
        }
        while (query.next()){
            int status = query.value(0).toInt();
            double amount = query.value(1).toDouble();
            bal_today -= amount;
            bal_future -= amount;

            if(status == TXN_STATUS_CLEARED)
                bal_clear -= amount;

            if(status == TXN_STATUS_RECONCILED)
            {
                bal_recon -= amount;
                bal_clear -= amount;
            }
        }
    }

    return true;
}

bool Transaction::sum_by_category(const QString &cat, double &amount)
{
    if (Preferences::Instance()->XHWFilepath() == ""){
        qWarning("no xhw file defined");
        return false;
    }
    {
        QSqlDatabase db = Preferences::Instance()->currentDatabase();
        if (db.isOpen()) {
            qDebug() << "Database opened successfully！";
        } else {
            qDebug("can't open database(%s) because:%s",Preferences::Instance()->XHWFilepath().toStdString().c_str(), db.lastError().text().toStdString().c_str());
            return false;
        }
        QSqlQuery query(db);
        QString sql = QString("select sum(amount) from t_transactions where category = '%1';").arg(cat);
        if (!query.exec(sql)){
            qDebug("query sql(%s) failed:%s",sql.toStdString().c_str(), db.lastError().text().toStdString().c_str());
            return false;
        }
        amount = 0;
        if (query.next()){
            amount = query.value(0).toDouble();
        }
    }

    return true;
}
