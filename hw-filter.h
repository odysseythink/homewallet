#ifndef FILTER_H
#define FILTER_H

#include <QObject>


enum
{
    FLT_OFF,
    FLT_INCLUDE,
    FLT_EXCLUDE
};


enum
{
    FLT_GRP_DATE,
    FLT_GRP_CATEGORY,
    FLT_GRP_PAYEE,
    FLT_GRP_ACCOUNT,
    FLT_GRP_TAG,
    FLT_GRP_TEXT,
    FLT_GRP_AMOUNT,
    FLT_GRP_PAYMODE,
    FLT_GRP_STATUS,
    FLT_GRP_TYPE,
    FLT_GRP_MAX
};


enum
{
    OLD56_FLT_RANGE_THISMONTH    = 0,
    OLD56_FLT_RANGE_LASTMONTH    = 1,
    OLD56_FLT_RANGE_THISQUARTER  = 2,
    OLD56_FLT_RANGE_LASTQUARTER  = 3,
    OLD56_FLT_RANGE_THISYEAR     = 4,
    OLD56_FLT_RANGE_LASTYEAR     = 5,	 //was not existing on 4.5
    // 6 separator
    OLD56_FLT_RANGE_LAST30DAYS   = 7,
    OLD56_FLT_RANGE_LAST60DAYS   = 8,
    OLD56_FLT_RANGE_LAST90DAYS   = 9,
    OLD56_FLT_RANGE_LAST12MONTHS = 10,
    // 11 separator
    OLD56_FLT_RANGE_OTHER        = 12,
    // 13 separator
    OLD56_FLT_RANGE_ALLDATE      = 14
};


enum
{
    FLT_RANGE_UNSET = 0,
    FLT_RANGE_MISC_CUSTOM,
    FLT_RANGE_MISC_ALLDATE,
    FLT_RANGE_MISC_30DAYS,

    FLT_RANGE_LAST_DAY = 20,
    FLT_RANGE_LAST_WEEK,
    FLT_RANGE_LAST_FORTNIGHT,
    FLT_RANGE_LAST_MONTH,
    FLT_RANGE_LAST_QUARTER,
    FLT_RANGE_LAST_YEAR,
    FLT_RANGE_LAST_30DAYS,
    FLT_RANGE_LAST_60DAYS,
    FLT_RANGE_LAST_90DAYS,
    FLT_RANGE_LAST_12MONTHS,
    FLT_RANGE_LAST_6MONTHS,

    FLT_RANGE_THIS_DAY = 40,
    FLT_RANGE_THIS_WEEK,
    FLT_RANGE_THIS_FORTNIGHT,
    FLT_RANGE_THIS_MONTH,
    FLT_RANGE_THIS_QUARTER,
    FLT_RANGE_THIS_YEAR,

    FLT_RANGE_NEXT_DAY = 60,
    FLT_RANGE_NEXT_WEEK,
    FLT_RANGE_NEXT_FORTNIGHT,
    FLT_RANGE_NEXT_MONTH,
    FLT_RANGE_NEXT_QUARTER,
    FLT_RANGE_NEXT_YEAR,

    FLT_RANGE_MAX
};




enum
{
    FLT_TYPE_ALL = 0,
    // 1 separator
    FLT_TYPE_EXPENSE = 2,
    FLT_TYPE_INCOME = 3,
    FLT_TYPE_INTXFER = 4,
};


enum
{
    FLT_STATUS_ALL = 0,
    // 1 separator
    FLT_STATUS_CLEARED = 2,
    FLT_STATUS_UNCLEARED = 3,
    FLT_STATUS_RECONCILED = 4,
    FLT_STATUS_UNRECONCILED = 5,
    // 6 separator
    FLT_STATUS_UNCATEGORIZED = 7,
};


enum
{
    FLT_QSEARCH_MEMO     = 1<<0,
    FLT_QSEARCH_INFO     = 1<<1,
    FLT_QSEARCH_PAYEE    = 1<<2,
    FLT_QSEARCH_CATEGORY = 1<<3,
    FLT_QSEARCH_TAGS     = 1<<4,
    FLT_QSEARCH_AMOUNT   = 1<<5
};


class Filter : public QObject
{
    Q_OBJECT
public:
    explicit Filter(QObject *parent = nullptr);

signals:

};

#endif // FILTER_H
