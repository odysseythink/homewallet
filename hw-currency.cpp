#include "hw-currency.h"
#include <QLocale>
#include "home_wallet.h"

static const char* create_table_sql = "CREATE TABLE t_currencies ("
"        flags         INT,"
"        iso_code      VARCHAR (128) NOT NULL PRIMARY KEY,"
"        name          VARCHAR (128) NOT NULL,"
"        symbol        VARCHAR (128),"
"        sym_prefix    BOOLEAN,"
"        decimal_char  VARCHAR (128),"
"        grouping_char VARCHAR (128),"
"        frac_digits   INT,"
"        rate          VARCHAR (128),"
"        mdate         INT"
"    );";


Currency4217 iso4217cur[] =
{
    { "AED", 2, ".", ",", true, "د.إ.‏", "UAE Dirham" },
    { "AFN", 2, ",", ",", true, "؋", "Afghani" },
    { "ALL", 2, ",", " ", false, "Lekë", "Lek" },
    { "AMD", 2, ".", ",", false, "֏", "Armenian Dram" },
    { "ANG", 2, ",", ",", true, "NAf.", "Netherlands Antillian Guilder" },
    { "AOA", 2, ",", " ", false, "Kz", "Kwanza" },
    { "ARS", 2, ",", ".", true, "$", "Argentine Peso" },
    { "AUD", 2, ".", ",", true, "$", "Australian Dollar" },
    { "AWG", 2, ",", ".", true, "Afl.", "Aruban Guilder" },
    { "AZN", 2, ",", " ", true, "₼", "Azerbaijanian Manat" },
    { "BAM", 2, ",", ".", false, "KM", "Convertible Marks" },
    { "BBD", 2, ".", ",", true, "$", "Barbados Dollar" },
    { "BDT", 2, ".", ",", true, "৳", "Taka" },
    { "BGN", 2, ",", " ", false, "лв.", "Bulgarian Lev" },
    { "BHD", 3, ".", ",", true, "د.ب.‏", "Bahraini Dinar" },
    { "BIF", 0, ",", " ", false, "FBu", "Burundi Franc" },
    { "BMD", 2, ".", ",", true, "$", "Bermudian Dollar" },
    { "BND", 2, ",", ".", true, "$", "Brunei Dollar" },
    { "BOB", 2, ",", ".", true, "Bs", "Boliviano" },
    { "BOV", 2, ".", "", false, "BOV", "Mvdol" },
    { "BRL", 2, ",", ".", true, "R$", "Brazilian Real" },
    { "BSD", 2, ".", ",", true, "$", "Bahamian Dollar" },
    { "BTN", 2, ".", ",", true, "Nu.", "Ngultrum" },
    { "BWP", 2, ".", " ", true, "P", "Pula" },
    { "BYN", 0, ",", " ", false, "Br", "Belarussian Ruble" },
    { "BYR", 0, ",", " ", false, "Br", "Old Belarussian Ruble" },
    { "BZD", 2, ".", ",", true, "$", "Belize Dollar" },
    { "CAD", 2, ",", " ", true, "$", "Canadian Dollar" },
    { "CDF", 2, ",", " ", true, "FC", "Congolese Franc" },
    { "CHE", 2, ".", "", false, "CHE", "WIR Euro" },
    { "CHF", 2, ",", "''", true, "CHF", "Swiss Franc" },
    { "CHW", 2, ".", "", false, "CHW", "WIR Franc" },
    { "CLF", 4, ".", "", false, "CLF", "Unidades de fomento" },
    { "CLP", 0, ",", ".", true, "$", "Chilean Peso" },
    { "CNY", 2, ".", ",", true, "¥", "Yuan Renminbi" },
    { "COP", 2, ",", ".", true, "$", "Colombian Peso" },
    { "COU", 2, ".", "", false, "COU", "Unidad de Valor Real" },
    { "CRC", 0, ",", ".", true, "₡", "Costa Rican Colon" },
    { "CUP", 2, ".", ",", true, "$", "Cuban Peso" },
    { "CVE", 2, "$", " ", false, "​", "Cape Verde Escudo" },
    { "CYP", 2, ".", "", false, "CYP", "Cyprus Pound" },
    { "CZK", 2, ",", " ", false, "Kč", "Czech Koruna" },
    { "DJF", 0, ",", " ", true, "Fdj", "Djibouti Franc" },
    { "DKK", 2, ",", ".", true, "kr", "Danish Krone" },
    { "DOP", 2, ".", ",", true, "$", "Dominican Peso" },
    { "DZD", 2, ",", " ", false, "DA", "Algerian Dinar" },
    { "EEK", 2, ".", "", false, "EEK", "Kroon" },
    { "EGP", 2, ".", ",", true, "ج.م.‏", "Egyptian Pound" },
    { "ERN", 2, ".", ",", true, "Nfk", "Nakfa" },
    { "ETB", 2, ".", ",", true, "Br", "Ethiopian Birr" },
    { "EUR", 2, ",", " ", true, "€", "Euro" },
    { "FJD", 2, ".", ",", true, "$", "Fiji Dollar" },
    { "FKP", 2, ".", ",", true, "£", "Falkland Islands Pound" },
    { "GBP", 2, ".", ",", true, "£", "Pound Sterling" },
    { "GEL", 2, ",", " ", true, "₾", "Lari" },
    { "GHS", 2, ".", ",", true, "GH₵", "Ghana Cedi" },
    { "GIP", 2, ".", ",", true, "£", "Gibraltar Pound" },
    { "GMD", 2, ".", ",", true, "D", "Dalasi" },
    { "GNF", 0, ",", " ", true, "FG", "Guinea Franc" },
    { "GTQ", 2, ".", ",", true, "Q", "Quetzal" },
    { "GYD", 2, ".", ",", true, "$", "Guyana Dollar" },
    { "HKD", 2, ".", ",", true, "$", "Hong Kong Dollar" },
    { "HNL", 2, ".", ",", true, "L", "Lempira" },
    { "HRK", 2, ",", ".", false, "kn", "Croatian Kuna" },
    { "HTG", 2, ",", " ", false, "G", "Gourde" },
    { "HUF", 2, ",", " ", false, "HUF", "Forint" },
    { "IDR", 2, ",", ".", true, "Rp", "Rupiah" },
    { "ILS", 2, ".", ",", true, "₪", "New Israeli Sheqel" },
    { "INR", 2, ".", ",", true, "₹", "Indian Rupee" },
    { "IQD", 3, ".", ",", true, "د.ع.‏", "Iraqi Dinar" },
    { "IRR", 2, "/", ",", true, "ريال", "Iranian Rial" },
    { "ISK", 0, ",", ".", false, "ISK", "Iceland Krona" },
    { "JMD", 2, ".", ",", true, "$", "Jamaican Dollar" },
    { "JOD", 3, ".", ",", true, "د.ا.‏", "Jordanian Dinar" },
    { "JPY", 0, ".", ",", true, "¥", "Yen" },
    { "KES", 2, ".", ",", true, "Ksh", "Kenyan Shilling" },
    { "KGS", 2, ",", " ", false, "сом", "Som" },
    { "KHR", 2, ".", ",", false, "៛", "Riel" },
    { "KMF", 0, ",", " ", true, "CF", "Comoro Franc" },
    { "KPW", 2, ".", "", false, "KPW", "North Korean Won" },
    { "KRW", 0, ".", ",", true, "₩", "Won" },
    { "KWD", 3, ".", ",", true, "د.ك.‏", "Kuwaiti Dinar" },
    { "KYD", 2, ".", ",", true, "$", "Cayman Islands Dollar" },
    { "KZT", 2, ",", " ", false, "₸", "Tenge" },
    { "LAK", 2, ",", ".", true, "₭", "Kip" },
    { "LBP", 2, ".", ",", true, "ل.ل.‏", "Lebanese Pound" },
    { "LKR", 2, ".", ",", true, "Rs.", "Sri Lanka Rupee" },
    { "LRD", 2, ".", ",", true, "$", "Liberian Dollar" },
    { "LSL", 2, ".", "", false, "LSL", "Loti" },
    { "LTL", 2, ".", "", false, "LTL", "Lithuanian Litas" },
    { "LVL", 2, ".", "", false, "LVL", "Latvian Lats" },
    { "LYD", 3, ".", ",", true, "د.ل.‏", "Libyan Dinar" },
    { "MAD", 2, ",", " ", false, "DH", "Moroccan Dirham" },
    { "MDL", 2, ",", " ", false, "L", "Moldovan Leu" },
    { "MGA", 2, ",", " ", true, "Ar", "Malagasy Ariary" },
    { "MKD", 2, ",", " ", true, "den", "Denar" },
    { "MMK", 2, ".", ",", true, "K", "Kyat" },
    { "MNT", 2, ".", ",", true, "₮", "Tugrik" },
    { "MOP", 2, ",", " ", true, "MOP", "Pataca" },
    { "MRO", 0, ",", " ", true, "UM", "Ouguiya" },
    { "MTL", 2, ".", "", false, "MTL", "Maltese Lira" },
    { "MUR", 2, ",", " ", true, "Rs", "Mauritius Rupee" },
    { "MVR", 2, ".", ",", false, "ރ.", "Rufiyaa" },
    { "MWK", 2, ".", ",", true, "MK", "Kwacha" },
    { "MXN", 2, ".", ",", true, "$", "Mexican Peso" },
    { "MXV", 2, ".", "", false, "MXV", "Mexican Unidad de Inversion (UDI)" },
    { "MYR", 2, ".", ",", true, "RM", "Malaysian Ringgit" },
    { "MZN", 2, ",", " ", false, "MTn", "Metical" },
    { "NAD", 2, ",", " ", true, "$", "Namibia Dollar" },
    { "NGN", 2, ".", ",", true, "₦", "Naira" },
    { "NIO", 2, ".", ",", true, "C$", "Cordoba Oro" },
    { "NOK", 2, ",", " ", true, "kr", "Norwegian Krone" },
    { "NPR", 2, ".", ",", true, "रु", "Nepalese Rupee" },
    { "NZD", 2, ".", ",", true, "$", "New Zealand Dollar" },
    { "OMR", 3, ".", ",", true, "ر.ع.‏", "Rial Omani" },
    { "PAB", 2, ".", ",", true, "B/.", "Balboa" },
    { "PEN", 2, ".", ",", true, "S/.", "Nuevo Sol" },
    { "PGK", 2, ".", ",", true, "K", "Kina" },
    { "PHP", 2, ",", ",", true, "₱", "Philippine Peso" },
    { "PKR", 2, ".", ",", true, "Rs", "Pakistan Rupee" },
    { "PLN", 2, ",", " ", false, "zł", "Zloty" },
    { "PYG", 0, ",", ".", true, "₲", "Guarani" },
    { "QAR", 2, ".", ",", true, "ر.ق.‏", "Qatari Rial" },
    { "RON", 2, ",", ".", false, "RON", "New Leu" },
    { "RSD", 2, ",", ".", false, "RSD", "Serbian Dinar" },
    { "RUB", 2, ",", " ", true, "₽", "Russian Ruble" },
    { "RWF", 0, ",", " ", true, "RF", "Rwanda Franc" },
    { "SAR", 2, ".", ",", true, "ر.س.‏", "Saudi Riyal" },
    { "SBD", 2, ".", ",", true, "$", "Solomon Islands Dollar" },
    { "SCR", 2, ",", " ", true, "SR", "Seychelles Rupee" },
    { "SDG", 2, ".", ",", true, "SDG", "Sudanese Pound" },
    { "SEK", 2, ",", ".", false, "kr", "Swedish Krona" },
    { "SGD", 2, ".", ",", true, "$", "Singapore Dollar" },
    { "SHP", 2, ".", ",", true, "£", "Saint Helena Pound" },
    { "SLL", 2, ".", ",", true, "Le", "Leone" },
    { "SOS", 2, ".", ",", true, "S", "Somali Shilling" },
    { "SRD", 2, ",", ".", true, "$", "Surinam Dollar" },
    { "STD", 0, ",", " ", false, "Db", "Dobra" },
    { "SVC", 2, ".", "", false, "SVC", "El Salvador Colon" },
    { "SYP", 2, ",", " ", true, "LS", "Syrian Pound" },
    { "SZL", 2, ",", " ", true, "E", "Lilangeni" },
    { "THB", 2, ".", ",", true, "฿", "Baht" },
    { "TJS", 2, ",", " ", false, "смн", "Somoni" },
    { "TMM", 2, ".", "", false, "TMM", "Manat" },
    { "TND", 3, ",", " ", true, "DT", "Tunisian Dinar" },
    { "TOP", 2, ".", ",", true, "T$", "Pa''anga" },
    { "TRY", 2, ",", ".", false, "₺", "New Turkish Lira" },
    { "TTD", 2, ".", ",", true, "$", "Trinidad and Tobago Dollar" },
    { "TWD", 2, ".", ",", true, "NT$", "New Taiwan Dollar" },
    { "TZS", 2, ".", ",", true, "TSh", "Tanzanian Shilling" },
    { "UAH", 2, ",", " ", false, "₴", "Hryvnia" },
    { "UGX", 0, ".", ",", true, "USh", "Uganda Shilling" },
    { "USD", 2, ",", " ", true, "$", "US Dollar" },
    { "USN", 2, ".", "", false, "USN", "US Dollar (Next day)" },
    { "USS", 2, ".", "", false, "USS", "US Dollar (Same day)" },
    { "UYI", 0, ".", "", false, "UYI", "Uruguay Peso en Unidades Indexadas" },
    { "UYU", 2, ",", ".", true, "$", "Peso Uruguayo" },
    { "UZS", 2, ",", " ", true, "soʻm", "Uzbekistan Sum" },
    { "VEF", 2, ",", ".", true, "Bs.", "Bolivar Fuerte" },
    { "VND", 0, ",", ".", false, "₫", "Dong" },
    { "VUV", 0, ",", " ", true, "VT", "Vatu" },
    { "WST", 2, ".", ",", true, "WS$", "Tala" },
    { "XAF", 0, ",", " ", true, "FCFA", "CFA Franc BEAC" },
    { "XAG", 2, ".", "", false, "XAG", "Silver" },
    { "XAU", 2, ".", "", false, "XAU", "Gold" },
    { "XBA", 2, ".", "", false, "XBA", "European Composite Unit (EURCO)" },
    { "XBB", 2, ".", "", false, "XBB", "European Monetary Unit (E.M.U.-6)" },
    { "XBC", 2, ".", "", false, "XBC", "European Unit of Account 9 (E.U.A.-9)" },
    { "XBD", 2, ".", "", false, "XBD", "European Unit of Account 17 (E.U.A.-17)" },
    { "XCD", 2, ",", " ", true, "$", "East Caribbean Dollar" },
    { "XDR", 2, ",", " ", true, "XDR", "Special Drawing Rights" },
    { "XFO", 0, ".", "", false, "XFO", "Gold-Franc" },
    { "XFU", 2, ".", "", false, "XFU", "UIC-Franc" },
    { "XOF", 0, ",", " ", true, "CFA", "CFA Franc BCEAO" },
    { "XPD", 2, ".", "", false, "XPD", "Palladium" },
    { "XPF", 0, ",", " ", false, "FCFP", "CFP Franc" },
    { "XPT", 2, ".", "", false, "XPT", "Platinum" },
    { "XTS", 2, ".", "", false, "XTS", "Code for testing purposes" },
    { "XXX", 2, ".", "", false, "XXX", "No currency" },
    { "YER", 2, ".", ",", true, "ر.ي.‏", "Yemeni Rial" },
    { "ZAR", 2, ",", " ", true, "R", "Rand" },
    { "ZMK", 2, ".", "", false, "ZMK", "Zambian Kwacha" },
    { "ZWD", 2, ".", "", false, "ZWD", "Zimbabwe Dollar" }
};
int n_iso4217cur = sizeof(iso4217cur)/sizeof (iso4217cur[0]);


Currency::Currency(const QString iso_code, const QString symbol, const short frac_digits, QObject *parent)
    : IHWFileIO(parent), iso_code(iso_code), symbol(symbol), frac_digits(frac_digits)
{
    qDebug(" iso:'%s' symbol:'%s' digit:%d\n", this->iso_code.toStdString().c_str(), this->symbol.toStdString().c_str(), this->frac_digits);

    // for formatd
    memset(this->format, 0, sizeof(this->format));
    snprintf(this->format , 8-1, "%%.%df", this->frac_digits);

    memset(this->monfmt, 0, sizeof(this->monfmt));
    if(this->sym_prefix == true)
        snprintf(this->monfmt , 32-1, "%s %%s", this->symbol.toStdString().c_str());
    else
        snprintf(this->monfmt , 32-1, "%%s %s", this->symbol.toStdString().c_str());

    qDebug(" numfmt '%s'\n", this->format);
    qDebug(" monfmt '%s'\n", this->monfmt);
}

Currency::Currency(QObject *parent)
    : IHWFileIO(parent)
{
    flags = 0;
    name = "";
    iso_code = "";
    sym_prefix = false;
    symbol = "";			/* max symbol is 3 digits in unicode but mostly is 1 digit */
    decimal_char = "";
    grouping_char = "";
    frac_digits = 0;
    _pad1 = 0;
    rate = 0.;
    mdate = 0;
    /* unsaved datas */
    memset(this->format, 0, sizeof(this->format));
    snprintf(this->format , 8-1, "%%.%df", this->frac_digits);
    memset(this->monfmt, 0, sizeof(this->monfmt));
    if(this->sym_prefix == true)
        snprintf(this->monfmt , 32-1, "%s %%s", this->symbol.toStdString().c_str());
    else
        snprintf(this->monfmt , 32-1, "%%s %s", this->symbol.toStdString().c_str());
}

QString Currency::CreateTableSql()
{
    return create_table_sql;
}

QString Currency::DataSql()
{
    QString sql = QString("REPLACE INTO t_currencies (flags, iso_code,name,symbol, sym_prefix,decimal_char, grouping_char,frac_digits,rate,mdate) values (%2, '%3', '%4', '%5',%6,'%7','%8',%9, '%10', %11);").arg(flags).arg(iso_code).arg(name).arg(symbol).arg(sym_prefix).arg(decimal_char).arg(grouping_char).arg(frac_digits).arg(rate).arg(mdate);
    return sql;
}

QString Currency::Tabname()
{
    return "t_currencies";
}

void Currency::LoadFromSqlQuery(QSqlQuery &query)
{
    int idx = 0;
    flags         = query.value(idx++).toInt();
    iso_code      = query.value(idx++).toString();
    name          = query.value(idx++).toString();
    symbol        = query.value(idx++).toString();
    sym_prefix    = query.value(idx++).toBool();
    decimal_char  = query.value(idx++).toString();
    grouping_char = query.value(idx++).toString();
    frac_digits   = query.value(idx++).toInt();
    rate          = query.value(idx++).toInt();
    mdate         = query.value(idx++).toInt();
}

void Currency::initformat()
{
    qDebug("init format");
    qDebug("iso:'%s' symbol:'%s' digit:%d\n", this->iso_code.toStdString().c_str(), this->symbol.toStdString().c_str(), this->frac_digits);

    // for formatd
    memset(this->format, 0, sizeof(this->format));
    snprintf(this->format , 8-1, "%%.%df", this->frac_digits);

    memset(this->monfmt, 0, sizeof(this->monfmt));
    if(this->sym_prefix == true)
        snprintf(this->monfmt , 32-1, "%s %%s", this->symbol.toStdString().c_str());
    else
        snprintf(this->monfmt , 32-1, "%%s %s", this->symbol.toStdString().c_str());

    qDebug(" numfmt '%s'\n", this->format);
    qDebug(" monfmt '%s'\n", this->monfmt);
}

QString Currency::get_system_iso()
{
    qDebug("get system ISO");
    QLocale local = QLocale::system();
    qDebug("raw currency Symbol: '%hs'\n", local.currencySymbol(QLocale::CurrencyIsoCode).toStdString().c_str());
    return local.currencySymbol(QLocale::CurrencyIsoCode);
}

QList<QSharedPointer<Currency> > Currency::get_all_currencies(bool from_xhw)
{
    if (!from_xhw){
        QList<QSharedPointer<Currency> > list;
        for (int iLoop = 0; iLoop < n_iso4217cur; iLoop++) {
            Currency* cur = add_from_user(&iso4217cur[iLoop]);
            if (cur != nullptr){
                list.append(QSharedPointer<Currency>(cur));
            }
        }
        return list;
    } else {
        QString where = "";
        return Preferences::get_all<Currency>(where);
    }
}

void Currency::init_from4217(Currency4217 *curfmt)
{
    this->symbol        = curfmt->curr_symbol;
    this->sym_prefix    = curfmt->curr_is_prefix;
    this->decimal_char  = curfmt->curr_dec_char;
    this->grouping_char = curfmt->curr_grp_char;
    this->frac_digits   = curfmt->curr_frac_digit;
    initformat();
}



/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */



///* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */


Currency *Currency::add_from_user(Currency4217 *curfmt)
{
    Currency *item;

    qDebug("found adding %s\n", curfmt->curr_iso_code.toStdString().c_str());

    item = new Currency();
    //no mem alloc here
    //item->key = i;
    if(curfmt != nullptr)
    {
        item->name = curfmt->name;
        //item->country = cur.country_name;
        item->iso_code = curfmt->curr_iso_code;

        //init from our internal iso 4217 preset
        item->frac_digits   = curfmt->curr_frac_digit;
        item->symbol        = curfmt->curr_symbol;
        item->sym_prefix    = curfmt->curr_is_prefix;
        item->decimal_char  = curfmt->curr_dec_char;
        item->grouping_char = curfmt->curr_grp_char;
    }
    else
    {
        item->name        = "unknown";
        //item->country     = cur.country_name;
        item->iso_code    = "XXX";
        item->frac_digits = 2;
        item->sym_prefix  = false;
    }

    //#1862540 if symbol/decimal/grouping char are null = crash on hb_str_formatd()
    if(item->symbol == "")
        item->symbol = "XXX";

    if(item->decimal_char == "")
        item->decimal_char = ".";

    qDebug("        symbol ='%s'", item->symbol.toStdString().c_str());
    qDebug("    sym_prefix ='%d'", item->sym_prefix);
    qDebug("  decimal_char ='%s'", item->decimal_char.toStdString().c_str());
    qDebug(" grouping_char ='%s'", item->grouping_char.toStdString().c_str());
    qDebug("   frac_digits ='%d'", item->frac_digits);

//    da_append(item);

    return item;
}

