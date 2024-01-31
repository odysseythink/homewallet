#include "home_wallet.h"
#include "hw-tag.h"


/* our global datas */
static const char* create_table_sql = "CREATE TABLE t_tags ("
"    id VARCHAR (36)  PRIMARY KEY,"
"    name  VARCHAR (128)"
");";


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */


QString Tag::CreateTableSql()
{
    return create_table_sql;
}


QString Tag::DataSql()
{
    QString sql = QString("REPLACE INTO t_tags (id, name) values ('%1', '%2');").arg(id).arg(name);
    return sql;
}

QString Tag::Tabname()
{
    return "t_tags";
}

void Tag::LoadFromSqlQuery(QSqlQuery &query)
{
    int idx = 0;
    id       = query.value(idx++).toString();
    name     = query.value(idx++).toString();
}


QString Tag::tags_tostring(QStringList tags)
{
    int i;
    QStringList tptr;
    QString tagstring;

    qDebug("\n[tags] tostring\n" );
    if( tags.size() == 0 )
    {
        return "";
    }
    else
    {
        for(i=0;i<tags.size();i++)
        {
            HomeWallet::Instance()->h_tag[tags[i]];
            if( HomeWallet::Instance()->h_tag.contains(tags[i]) && HomeWallet::Instance()->h_tag[tags[i]] != nullptr)
            {
                tptr << HomeWallet::Instance()->h_tag[tags[i]]->name;
            }
        }
        tagstring = tptr.join(" ");
    }
    return tagstring;
}
