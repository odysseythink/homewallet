#include "home_wallet.h"
#include "hw-assign.h"


/* our global datas */
static const char* create_table_sql = "CREATE TABLE t_assigns ("
"    id    VARCHAR (36)   PRIMARY KEY,"
"    flags    INT,"
"    pos      INT,"
"    field    INT,"
"    search   VARCHAR (128),"
"    notes    VARCHAR (1024),"
"    payee    VARCHAR (36),"
"    category VARCHAR (36),"
"    paymode  INT,"
"    amount   DOUBLE"
");";


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

Assign::Assign(QObject *parent)
    : IHWFileIO(parent)
{

}

Assign::~Assign()
{

}

QString Assign::CreateTableSql()
{
    return create_table_sql;
}

QString Assign::DataSql()
{
    QString sql = QString("REPLACE INTO t_assigns ("
"id,"
"flags,"
"pos,"
"field,"
"search,"
"notes,"
"payee,"
"category,"
"paymode,"
"amount) VALUES ('%1',"
"%2,"
"%3,"
"%4,"
"'%5',"
"'%6',"
"'%7',"
"'%8',"
"%9,"
"%10);").arg(id)
            .arg(flags)
            .arg(pos)
            .arg(field)
            .arg(search)
            .arg(notes)
            .arg(payee)
         .arg(category)
            .arg(paymode)
            .arg(amount);
    return sql;
}

QString Assign::Tabname()
{
    return "t_assigns";
}
void Assign::LoadFromSqlQuery(QSqlQuery &query)
{
    int idx = 0;
    id       = query.value(idx++).toString();
    flags    = query.value(idx++).toInt();
    pos      = query.value(idx++).toInt();
    field    = query.value(idx++).toInt();
    search   = query.value(idx++).toString();
    notes    = query.value(idx++).toString();
    payee    = query.value(idx++).toString();
    category = query.value(idx++).toString();
    paymode  = query.value(idx++).toInt();
    amount   = query.value(idx++).toDouble();
}
