#include "home_wallet.h"
#include "hw-archive.h"
#include "hw-split.h"
#include "hw-transaction.h"


/* our global datas */
static const char* create_table_sql = "CREATE TABLE t_archives ("
"    id        VARCHAR (36)   PRIMARY KEY,"
"    amount       DOUBLE,"
"    account      VARCHAR (36),"
"    xfer_amount  DOUBLE,"
"    xfer_account VARCHAR (36),"
"    paymode      INT,"
"    status       INT,"
"    flags        INT,"
"    payee        VARCHAR (36),"
"    category     VARCHAR (36),"
"    memo         VARCHAR (128),"
"    info         VARCHAR (128),"
"    tags         VARCHAR (1024),"
"    nextdate     INT,"
"    every        INT,"
"    unit         INT,"
"    [limit]      INT,"
"    weekend      INT,"
"    daygap       INT,"
"    scat         VARCHAR (1024),"
"    samt         VARCHAR (1024),"
"    smem         VARCHAR (1024)"
");";


/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

Archive::Archive(QObject *parent)
    : IHWFileIO(parent)
{
    id = 1;
}

Archive::~Archive()
{
    clean();
}

QString Archive::CreateTableSql()
{
    return create_table_sql;
}


QString Archive::DataSql()
{
    QString cats, amounts, memos;

    QString tagstr = Tag::tags_tostring(tags);
    QString sql = QString("REPLACE INTO t_archives (id,"
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
"nextdate,"
"every,"
"unit,"
"[limit],"
"weekend,"
"daygap,"
"scat,"
"samt,"
"smem) VALUES ('%1',"
"%2,"
"'%3',"
"%4,"
"'%5',"
"%6,"
"%7,"
"%8,"
"'%9',"
"'%10',"
"'%11',"
"'%12',"
"'%13',"
"%14,"
"%15,"
"%16,"
"%17,"
"%18,"
"%19,"
"'%20',"
"'%21',"
"'%22');").arg(id)
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
            .arg(nextdate)
            .arg(every)
            .arg(unit)
            .arg(limit)
            .arg(weekend)
            .arg(daygap)
            .arg(cats)
            .arg(amounts)
            .arg(memos);
    return sql;
}

QString Archive::Tabname()
{
    return "t_archives";
}

void Archive::LoadFromSqlQuery(QSqlQuery &query)
{
    int idx = 0;
    id          = query.value(idx++).toString();
    amount      = query.value(idx++).toDouble();
    account     = query.value(idx++).toString();
    xfer_amount = query.value(idx++).toDouble();
    xfer_account= query.value(idx++).toString();
    paymode     = query.value(idx++).toInt();
    status      = query.value(idx++).toInt();
    flags       = query.value(idx++).toInt();
    payee       = query.value(idx++).toString();
    category    = query.value(idx++).toString();
    memo        = query.value(idx++).toString();
    info        = query.value(idx++).toString();
    tags        = query.value(idx++).toString().split(" ");
    nextdate    = query.value(idx++).toInt();
    every       = query.value(idx++).toInt();
    unit        = query.value(idx++).toInt();
    limit       = query.value(idx++).toInt();
    weekend     = query.value(idx++).toInt();
    daygap      = query.value(idx++).toInt();
    auto scat   = query.value(idx++).toString();
    auto samt   = query.value(idx++).toString();
    auto smem   = query.value(idx++).toString();

}


void Archive::clean()
{
    memo = "";

    info = "";

    //5.3 added as it was a leak
    tags.clear();

    if(splits.size() > 0)
    {
        flags &= ~(OF_SPLIT); //Flag that Splits are cleared
    }
}

const Archive &Archive::operator=(const Archive &a)
{
    if(this == &a){
        return *this;
    }
    //duplicate the string
    this->memo = a.memo;
    this->info = a.info;

    //duplicate tags
    //no g_free here to avoid free the src tags (memdup copie dthe ptr)
    this->tags = a.tags;
    return *this;
}






/* = = = = = = = = = = = = = = = = = = = = */

