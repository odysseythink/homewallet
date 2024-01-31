#ifndef __HB_TAG_H__
#define __HB_TAG_H__

#include <QtGlobal>
#include "hw-file.h"

class Tag: public IHWFileIO
{
    Q_OBJECT
public:
    Tag(QObject* parent = nullptr):IHWFileIO(parent){}
    ~Tag(){}
    virtual QString CreateTableSql() override;
    virtual QString DataSql() override;
    virtual QString Tabname() override;
    virtual void LoadFromSqlQuery(QSqlQuery& query) override;

public:
    static QString tags_tostring(QStringList tags);

    QString id;
    QString name;

	/* unsaved datas */
	//bool	flt_select;
	unsigned int		usage_count;
};


#endif

