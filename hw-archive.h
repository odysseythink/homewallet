#ifndef __HB_ARCHIVE_H__
#define __HB_ARCHIVE_H__

#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QWidget>
#include <QList>
#include <QSharedPointer>
#include <QDate>
//#include "hw-transaction.h"
#include "hw-split.h"
#include "hw-file.h"

#define CLAMP(val, min, max) val < min ? min: (val > max?max:val)


class Transaction;
class Archive: public IHWFileIO
{
    Q_OBJECT
public:
    Archive(QObject* parent = nullptr);
    ~Archive();
    virtual QString CreateTableSql() override;
    virtual QString DataSql() override;
    virtual QString Tabname() override;
    virtual void LoadFromSqlQuery(QSqlQuery& query) override;
    void clean();
    const Archive& operator=(const Archive& a);

public:
    QString		id;

	double		amount;
    QString		account;
	unsigned short		paymode;
	unsigned short		flags;
    QString		payee;
    QString		category;
    QString memo;

	//quint32		date;
	//unsigned short		pos;
	unsigned short     status;
    QString info;
    QStringList tags;
	//quint32		kxfer;		//strong link xfer key
    QString		xfer_account;
    double		xfer_amount;	//xfer target alount
	
    QList<QSharedPointer<Split> > splits;

	quint32		nextdate;
	unsigned short		daygap;
	unsigned short		every;
	unsigned short		unit;
	unsigned short		limit;
	unsigned short		weekend;
};


/* list define archive (defarchive) */
enum
{
	LST_DEFARC_DATAS,
//	LST_DEFARC_OLDPOS,
//	LST_DEFARC_AUTO,
	NUM_LST_DEFARC
};


enum
{
	ARC_POSTMODE_DUEDATE,
	ARC_POSTMODE_PAYOUT,
	ARC_POSTMODE_ADVANCE
};


/*
** scheduled unit
*/
enum
{
	AUTO_UNIT_DAY,
	AUTO_UNIT_WEEK,
	AUTO_UNIT_MONTH,
	//AUTO_UNIT_QUARTER,
	AUTO_UNIT_YEAR
};


enum
{
	ARC_WEEKEND_POSSIBLE,
	ARC_WEEKEND_BEFORE,
	ARC_WEEKEND_AFTER,
	ARC_WEEKEND_SKIP
};


enum
{
	FLT_SCHEDULED_THISMONTH = 1,
	FLT_SCHEDULED_NEXTMONTH,
	FLT_SCHEDULED_NEXT30DAYS,
	FLT_SCHEDULED_NEXT60DAYS,
	FLT_SCHEDULED_NEXT90DAYS,
	FLT_SCHEDULED_ALLDATE,
	//added 5.7
	FLT_SCHEDULED_MAXPOSTDATE
};


#endif

