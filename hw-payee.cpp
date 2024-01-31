#include "home_wallet.h"
#include "hw-payee.h"



/* our global datas */
static const char* create_table_sql = "CREATE TABLE t_payee ("
"        id    VARCHAR (36)   PRIMARY KEY,"
"        flags    INT,"
"        name     VARCHAR (128),"
"        category VARCHAR (36),"
"        paymode  INT,"
"        notes    VARCHAR (1024) "
"    );";


Payee::Payee(QObject *parent)
    : IHWFileIO(parent)
{

}

Payee::~Payee()
{

}

QString Payee::CreateTableSql()
{
    return create_table_sql;
}

QString Payee::DataSql()
{
    QString sql = QString("REPLACE INTO t_payee ("
"                          id,"
"                          flags,"
"                          name,"
"                          category,"
"                          paymode,"
"                          notes"
"                      )"
"                      VALUES ("
"                          '%1',"
"                          %2,"
"                          '%3',"
"                          '%4',"
"                          %5,"
"                          '%6');").arg(id).arg(flags).arg(name).arg(category).arg(paymode).arg(notes);
    return sql;
}

QString Payee::Tabname()
{
    return "t_payee";
}

void Payee::LoadFromSqlQuery(QSqlQuery &query)
{
    int idx = 0;
    id       = query.value(idx++).toString();
    flags    = query.value(idx++).toInt();
    name     = query.value(idx++).toString();
    category = query.value(idx++).toString();
    paymode  = query.value(idx++).toInt();
    notes    = query.value(idx++).toString();
}


QList<QSharedPointer<Payee> > Payee::get_all_by_category(const QString &cat)
{
    QString where = QString("where category='%1'").arg(cat);
    return Preferences::get_all<Payee>(where);
}
