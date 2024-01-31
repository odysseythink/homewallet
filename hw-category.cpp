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
#include "hw-category.h"
#include <QDebug>


/****************************************************************************/
/* Debug macros										 */
/****************************************************************************/
/* our global datas */
static const char* create_table_sql = "CREATE TABLE t_categories ("
"id  VARCHAR (36)  PRIMARY KEY,"
"parent VARCHAR (36),"
"flags  INT,"
"name   VARCHAR (128),"
"type   VARCHAR (10)  NOT NULL"
");";


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

Category::Category(Category *other)
{
    qDebug("da_cat_clone\n");
    if(other)
    {
        //duplicate the string
        this->name = other->name;
        this->fullname = other->fullname;
    }
}

Category::~Category()
{
    printf("[%s %s:%d]\n", __FILE__, __FUNCTION__, __LINE__);
}

QString Category::CreateTableSql()
{
    return create_table_sql;
}

QString Category::DataSql()
{
    QString sql = QString("REPLACE INTO t_categories ("
"                          id,"
"                          parent,"
"                          flags,"
"                          name,type"
"                      ) VALUES ("
"                          '%1',"
"                          '%2',"
"                          %3,"
"                          '%4','%5');").arg(id).arg(parent).arg(flags).arg(name).arg(type);
    return sql;
}

QString Category::Tabname()
{
    return "t_categories";
}

void Category::LoadFromSqlQuery(QSqlQuery &query)
{
    int idx = 0;
    id = query.value(idx++).toString();
    parent = query.value(idx++).toString();
    flags = query.value(idx++).toInt();
    name = query.value(idx++).toString();
    type = query.value(idx++).toString();
}

QList<QSharedPointer<Category> > Category::get_all_by_type(const QString type)
{
    QString where = "";
    if (type != "") where= QString("where type='%1'").arg(type);
    return Preferences::get_all<Category>(where);
}



/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

bool Category::add_to_local(Category *item)
{
    QSqlDatabase db = Preferences::Instance()->currentDatabase();
    return item->save(db) == 0;
}

bool Category::del_from_local(const QString &id)
{
    if (id == ""){
        qWarning("invalid arg");
        return false;
    }
    QSqlDatabase db = Preferences::Instance()->currentDatabase();
    if(!db.isValid() || !db.isOpen()){
        qWarning("no valid QSqlDatabase");
        return false;
    }
    QSqlQuery query(db);
    QString sql = QString("delete from t_categories where id='%1';").arg(id);
    if (!query.exec(sql)){
        qWarning("exec(%s) failed:%s", sql.toStdString().c_str(), db.lastError().text().toStdString().c_str());
        return false;
    }
    return true;
}

bool Category::update_type_to_local(const QString &id, const QString &type)
{
    if (id == ""){
        qWarning("invalid arg");
        return false;
    }
    QSqlDatabase db = Preferences::Instance()->currentDatabase();
    if(!db.isValid() || !db.isOpen()){
        qWarning("no valid QSqlDatabase");
        return false;
    }
    QSqlQuery query(db);
    QString sql = QString("update t_categories set type='%1' where id='%2';").arg(type).arg(id);
    if (!query.exec(sql)){
        qWarning("exec(%s) failed:%s", sql.toStdString().c_str(), db.lastError().text().toStdString().c_str());
        return false;
    }
    return true;
}

QMap<QString, QString> Category::get_all_fullnames(const QString &type)
{
    printf("[%s %s:%d]\n", __FILE__, __FUNCTION__, __LINE__);
    QList<QSharedPointer<Category> > list = get_all_by_type(type);
    QMap<QString, QString> res;
    if (list.size() > 0){
        QMap<QString, QString> fullnames;
//        printf("[%s %s:%d]fullnames.size()=%d\n", __FILE__, __FUNCTION__, __LINE__, fullnames.size());
        auto items = ParseCategoriesToItems("", list, &fullnames);
        return fullnames;
    }
//    printf("[%s %s:%d]\n", __FILE__, __FUNCTION__, __LINE__);
    return res;
}

QMap<QString, QString> Category::get_all_leaf_fullnames(const QString &type)
{
    printf("[%s %s:%d]\n", __FILE__, __FUNCTION__, __LINE__);
    QList<QSharedPointer<Category> > list = get_all_by_type(type);
    QMap<QString, QString> res;
    if (list.size() > 0){
        QMap<QString, QString> fullnames;
        QStringList leafs;
//        printf("[%s %s:%d]fullnames.size()=%d\n", __FILE__, __FUNCTION__, __LINE__, fullnames.size());
        auto items = ParseCategoriesToItems("", list, &fullnames, &leafs);
        for(auto id : leafs){
            if(fullnames.contains(id)){
                qWarning("id(%s) not in fullnames", id.toStdString().c_str());
            } else {
                res[id] = fullnames[id];
            }
        }
        return res;
    }
//    printf("[%s %s:%d]\n", __FILE__, __FUNCTION__, __LINE__);
    return res;
}

QList<QStandardItem*> Category::ParseCategoriesToItems(const QString parentname,QList<QSharedPointer<Category> > &list, QMap<QString, QString>* fullnames, QStringList* leafs)
{
    qDebug("------------list.size()=%d", list.size());
    QList<QStandardItem*> items;
    if (list.size() == 0){
        return items;
    }
    QList<QSharedPointer<Category> >::iterator iter = list.begin();
    while(iter != list.end()){
        Category* pCategory = iter->get();
        if(parentname == pCategory->parent){
            QMap<QString, QVariant> data;
            data["id"] = pCategory->id;
            data["parent"] = pCategory->parent;
            data["flags"] = pCategory->flags;
            data["name"] = pCategory->name;
            data["type"] = pCategory->type;
            if (fullnames != nullptr){
//                printf("[%s %s:%d]pCategory->id=%s\n", __FILE__, __FUNCTION__, __LINE__, pCategory->id.toStdString().c_str());
                (*fullnames)[pCategory->id] =  (parentname == ""?"":(*fullnames)[pCategory->parent]+":") + pCategory->name;
            }
            QStandardItem* pItem = new QStandardItem(pCategory->name);
            pItem->setData(data);
            items.append(pItem);
            iter = list.erase(iter);
//            auto sublist = __ParseCategoriesToItems(pCategory->name, );
        } else {
            ++iter;
        }
    }
    for (int iLoop = 0; iLoop < items.size(); iLoop++){
        auto tmpsubitems = ParseCategoriesToItems(items[iLoop]->data().toMap()["id"].toString(), list, fullnames);
        if (tmpsubitems.size() > 0){
            items[iLoop]->appendRows(tmpsubitems);
        } else {
            if (leafs != nullptr){
                *leafs << items[iLoop]->data().toMap()["id"].toString();
            }
        }
    }
    return items;
}


