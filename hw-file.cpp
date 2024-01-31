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
#include "hw-currency.h"
//#include "hb-hbfile.h"
//#include "hw-archive.h"
//#include "hw-transaction.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QException>


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =*/


void IHWFileIO::setup(bool file_clear)
{

    qDebug("setup\n");
    qDebug("- file clear is %d", file_clear);
}

int IHWFileIO::save(QSqlDatabase& db)
{
    QStringList tables = db.tables();
    QSqlQuery query(db);
    if (!tables.contains(Tabname())){
        query.exec(CreateTableSql());
    }
    QString sql = DataSql();
    if (!query.exec(sql)) {
        qWarning("sql(%s) exec failed:%s", sql.toStdString().c_str(), db.lastError().text().toStdString().c_str());
        return -2;
    }
    return 0;
}

bool IHWFileIO::table_exists(QString table) {
    try {
        QSqlQuery query;
        query.exec(QString("SELECT name FROM sqlite_master WHERE name='%1';").arg(table));
        if (query.next()){
            return true;
        }
    } catch (QException e) {
        qDebug("exception because %s", e.what());
        return false;
    }
    return false;
}

const char* properties_create_sql = "CREATE TABLE t_properties ("
"        id               VARCHAR (36)  PRIMARY KEY,"
"        title            VARCHAR (128),"
"        currency         VARCHAR (36),"
"        vehicle_category VARCHAR (128),"
"        auto_smode       INT,"
"        auto_weekday     INT,"
"        auto_nbmonths    INT,"
"        auto_nbdays      INT"
"    );";

/*
** XML properties save
*/
static int homebank_save_xml_prop()
{
    if (!IHWFileIO::table_exists("t_properties")){
        QSqlQuery query;
        query.exec(properties_create_sql);
    }
    int retval = 0;
    QString title = HomeWallet::Instance()->owner == NULL ? "" : HomeWallet::Instance()->owner;
    QString sql = QString("REPLACE INTO t_properties (id, title, currency,vehicle_category,auto_smode, auto_weekday,auto_nbmonths, auto_nbdays) values ('1', '%1', '%2', '%3', %4,%5,%6,%7);").arg(title).arg(HomeWallet::Instance()->kcur).arg(HomeWallet::Instance()->vehicle_category).arg(HomeWallet::Instance()->auto_smode).arg(HomeWallet::Instance()->auto_weekday).arg(HomeWallet::Instance()->auto_nbmonths).arg(HomeWallet::Instance()->auto_nbdays);
    QSqlQuery query;
    if (!query.exec(sql)) {
        retval = -2;
    }
    return retval;
}

/*
** XML currency save
*/
static int homebank_save_xml_cur(QSqlDatabase& DB)
{
    int retval = 0;
    QMap<QString, QSharedPointer<Currency> >::const_iterator iter = HomeWallet::Instance()->h_cur.constBegin();
    while (iter != HomeWallet::Instance()->h_cur.constEnd()) {
        Currency* item = iter.value().get();
        if(item != nullptr){
            retval = item->save(DB);
            if (retval != 0) {
                break;
            }
        }
        ++iter;
    }
    return retval;
}

/*
** XML grp save
*/
static int homebank_save_xml_grp(QSqlDatabase& DB)
{
    int retval = 0;
    QMap<QString, QSharedPointer<Group> >::const_iterator iter = HomeWallet::Instance()->h_grp.constBegin();
    while (iter != HomeWallet::Instance()->h_grp.constEnd()) {
        Group* item = iter.value().get();
        if(item != nullptr){
            retval = item->save(DB);
            if (retval != 0) {
                break;
            }
        }
        ++iter;
    }
    return retval;
}

/*
** XML account save
*/
static int homebank_save_xml_acc(QSqlDatabase& DB)
{
    int retval = 0;
    QMap<QString, QSharedPointer<Account> >::const_iterator iter = HomeWallet::Instance()->h_acc.constBegin();
    while (iter != HomeWallet::Instance()->h_acc.constEnd()) {
        Account* item = iter.value().get();
        if(item != nullptr){
            retval = item->save(DB);
            if (retval != 0) {
                break;
            }
        }
        ++iter;
    }
    return retval;
}

/*
** XML payee save
*/
static int homebank_save_xml_pay(QSqlDatabase& DB)
{
    int retval = 0;
    QMap<QString, QSharedPointer<Payee> >::const_iterator iter = HomeWallet::Instance()->h_pay.constBegin();
    while (iter != HomeWallet::Instance()->h_pay.constEnd()) {
        Payee* item = iter.value().get();
        if(item != nullptr){
            retval = item->save(DB);
            if (retval != 0) {
                break;
            }
        }
        ++iter;
    }
    return retval;
}

/*
** XML category save
*/
static int homebank_save_xml_cat(QSqlDatabase& DB)
{
    int retval = 0;
    QMap<QString, QSharedPointer<Category> >::const_iterator iter = HomeWallet::Instance()->h_cat.constBegin();
    while (iter != HomeWallet::Instance()->h_cat.constEnd()) {
        Category* item = iter.value().get();
        if(item != nullptr){
            retval = item->save(DB);
            if (retval != 0) {
                break;
            }
        }
        ++iter;
    }
    return retval;
}

/*
** XML tag save
*/
static int homebank_save_xml_tag(QSqlDatabase& DB)
{
    int retval = 0;
    QMap<QString, QSharedPointer<Tag> >::const_iterator iter = HomeWallet::Instance()->h_tag.constBegin();
    while (iter != HomeWallet::Instance()->h_tag.constEnd()) {
        Tag* item = iter.value().get();
        if(item != nullptr){
            retval = item->save(DB);
            if (retval != 0) {
                break;
            }
        }
        ++iter;
    }
    return retval;
}


/*
** XML assign save
*/
static int homebank_save_xml_asg(QSqlDatabase& DB)
{
    int retval = 0;
    QMap<QString, QSharedPointer<Assign> >::const_iterator iter = HomeWallet::Instance()->h_rul.constBegin();
    while (iter != HomeWallet::Instance()->h_rul.constEnd()) {
        Assign* item = iter.value().get();
        if(item != nullptr){
            retval = item->save(DB);
            if (retval != 0) {
                break;
            }
        }
        ++iter;
    }
    return retval;
}


/*
** XML archive save
*/
static int homebank_save_xml_arc(QSqlDatabase& DB)
{
    int retval = 0;
    QList<QSharedPointer<Archive> >::const_iterator iter = HomeWallet::Instance()->arc_list.constBegin();
    while (iter != HomeWallet::Instance()->arc_list.constEnd()) {
        Archive* item = iter->get();
        if(item != nullptr){
            retval = item->save(DB);
            if (retval != 0) {
                break;
            }
        }
        ++iter;
    }
    return retval;
}


static int homebank_save_xml_ope(QSqlDatabase& DB)
{
    int retval = 0;
    QMap<QString, QSharedPointer<Account> >::const_iterator acciter = HomeWallet::Instance()->h_acc.constBegin();
    while (acciter != HomeWallet::Instance()->h_acc.constEnd()) {
        Account* acc = acciter.value().get();
        QList<QSharedPointer<Transaction> >::const_iterator transiter = acc->txn_queue.constBegin();
        while (transiter != acc->txn_queue.constEnd()) {
            Transaction *item = transiter->get();
            if(item != nullptr){
                retval = item->save(DB);
                if (retval != 0) {
                    break;
                }
            }
            ++transiter;
        }
        ++acciter;
    }
    return retval;
}

/*
** XML save homebank file: hbfile
*/
int homebank_save_xml(const QString filename)
{
    int retval = 0;
    //打开数据库
    QSqlDatabase db = Preferences::Instance()->currentDatabase();
    if (db.isOpen()) {
        qDebug() << "Database opened successfully！";
    } else {
        qDebug() << "无法打开数据库：" << db.lastError().text();
        return -1;
    }
    retval = homebank_save_xml_prop();
    retval = homebank_save_xml_cur(db);
    retval = homebank_save_xml_grp(db);
    retval = homebank_save_xml_acc(db);
    retval = homebank_save_xml_pay(db);
    retval = homebank_save_xml_cat(db);
    retval = homebank_save_xml_tag(db);
    retval = homebank_save_xml_asg(db);
    retval = homebank_save_xml_arc(db);
    retval = homebank_save_xml_ope(db);

    return retval;
}

/*
** XML save homebank file: hbfile
*/
int IHWFileIO::new_xml(const QString filename)
{
    int retval = 0;
    {
        //打开数据库
        Preferences::Instance()->addConnection("QSQLITE", filename, "","", "", 0);

        QSqlDatabase db = Preferences::Instance()->currentDatabase();
        if (db.isOpen()) {
            qDebug() << "Database opened successfully！";
        } else {
            goto End;
            retval = -1;
        }
        QSqlQuery query(db);
        query.exec(properties_create_sql);
        QList<IHWFileIO*> ios;
        ios << new Currency();
        ios << new Group();
        ios << new Account();
        ios << new Payee();
        ios << new Category();
        ios << new Tag();
        ios << new Assign();
        ios << new Archive();
        ios << new Transaction();
        for(int iLoop = 0; iLoop < ios.size(); iLoop++){
            query.exec(ios[iLoop]->CreateTableSql());
            delete ios[iLoop];
            ios[iLoop] = nullptr;
        }
        ios.clear();
        auto list = Currency::get_all_currencies();
        QList<QSharedPointer<Currency>>::iterator iter = list.begin();
        qDebug("---------0000, list.size()=%d", list.size());
        while (iter != list.end()) {
            qDebug("---------11111");
            Currency* item = iter->get();
            if(item != nullptr){
                item->save(db);
            }
            ++iter;
        }
        iter = list.begin();
        while (iter != list.end()) {
            Currency* item = iter->get();
            if(item != nullptr){
                QString sql = QString("select * from t_currencies where iso_code='%1'").arg(item->iso_code);
                query.exec(sql);
                if (!query.next()) {
                    qWarning("sql(%s) exec failed:%s", sql.toStdString().c_str(), db.lastError().text().toStdString().c_str());
                    qDebug("%s", item->DataSql().toStdString().c_str());
                }
            }
            ++iter;
        }
        list.clear();
    }
End:
    return retval;
}
