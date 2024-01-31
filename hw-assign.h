#ifndef __HB_ASSIGN_H__
#define __HB_ASSIGN_H__

#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QWidget>
#include <QList>
#include <QSharedPointer>
#include <QDate>
#include "hw-file.h"


struct Assign: public IHWFileIO
{
    Q_OBJECT
public:
    Assign(QObject* parent = nullptr);
    ~Assign();
    virtual QString CreateTableSql() override;
    virtual QString DataSql() override;
    virtual QString Tabname() override;
    virtual void LoadFromSqlQuery(QSqlQuery& query) override;
    QString   	id;
	unsigned short		flags;
	unsigned short		field;		//0:memo / 1:payee
    QString search;
    QString notes;
    QString		payee;		//dst payee
    QString		category;		//dst category
	unsigned short		paymode;	//dst_payment mode
	quint32		pos;		//position
	double		amount;		//5.6 #1710085 assignment based on amount
};


#define ASGF_EXACT	(1<<0)
#define ASGF_DOPAY	(1<<1)
#define ASGF_DOCAT	(1<<2)
#define ASGF_DOMOD	(1<<3)
//#define ASGF_DOTAG (later)

#define ASGF_PREFILLED	(1<<5)

#define ASGF_AMOUNT	(1<<7)	//5.6
#define ASGF_REGEX	(1<<8)
#define ASGF_OVWPAY	(1<<9)
#define ASGF_OVWCAT	(1<<10)
#define ASGF_OVWMOD	(1<<11)
//#define ASGF_OVWTAG (later)


#endif

