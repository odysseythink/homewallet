#ifndef CURRENCY_H
#define CURRENCY_H

#include <QObject>
#include <QDebug>
#include <stdint.h>
#include <QSharedPointer>
#include <QPointer>
#include "hw-file.h"

struct _iso4217
{
    QString curr_iso_code;
    uint	   curr_frac_digit;
    QString curr_dec_char;
    QString curr_grp_char;
    bool   curr_is_prefix;
    QString curr_symbol;
    QString name;
};
typedef struct _iso4217		Currency4217;

class Currency : public IHWFileIO
{
    Q_OBJECT
public:
    explicit Currency(const QString iso_code, const QString symbol, const short frac_digits, QObject *parent = nullptr);
    explicit Currency(QObject *parent = nullptr);
    ~Currency(){}
    virtual QString CreateTableSql() override;
    virtual QString DataSql() override;
    virtual QString Tabname() override;
    virtual void LoadFromSqlQuery(QSqlQuery& query) override;
    virtual QString PrimaryKey() override{return "iso_code";}
    void initformat();
    void init_from4217(Currency4217 *curfmt);

    static QString get_system_iso();
    static QList<QSharedPointer<Currency> > get_all_currencies(bool from_xhw = false);
    static Currency4217 * iso4217format_get(QString code);
    static Currency *add_from_user(Currency4217 *curfmt);

signals:

public:
    ushort 	flags;
    QString name;
    QString iso_code;
    bool	sym_prefix;
    QString symbol;			/* max symbol is 3 digits in unicode but mostly is 1 digit */
    QString decimal_char;
    QString grouping_char;
    short		frac_digits;
    short		_pad1;
    double		rate;
    uint32_t		mdate;
    /* unsaved datas */
    char		format[8];			/* hold decimal format: '%.xf' */
    char		monfmt[32];
};

#endif // CURRENCY_H
