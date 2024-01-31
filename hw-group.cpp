#include "home_wallet.h"
#include "hw-group.h"

/* our global datas */

static const char* create_table_sql = "CREATE TABLE t_groups ("
"        id VARCHAR (36)  PRIMARY KEY,"
"        name  VARCHAR (128) NOT NULL"
"    );";



Group::Group(QObject *parent)
    : IHWFileIO(parent)
{

}

Group::~Group()
{

}

QString Group::CreateTableSql()
{
    return create_table_sql;
}


QString Group::DataSql()
{
    QString sql = QString("REPLACE INTO t_groups (id, name) values ('%1', '%2');").arg(id).arg(name);
    return sql;
}

QString Group::Tabname()
{
    return "t_groups";
}

void Group::LoadFromSqlQuery(QSqlQuery &query)
{
    int idx = 0;
    id       = query.value(idx++).toString();
    name     = query.value(idx++).toString();
}
