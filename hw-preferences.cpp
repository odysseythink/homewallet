#include "hw-preferences.h"
#include "hw-filter.h"
#include "hw-archive.h"
#include "hw-currency.h"
#include "hw-misc.h"
#include "config.h"
#include <QDir>
#include <QCoreApplication>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include "home_wallet.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif


/* our global datas */
//extern struct HomeBank *GLOBALS;
//extern struct Preferences *PREFS;

/* = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

static bool readJsonFile(QIODevice &device, QSettings::SettingsMap &map)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(device.readAll(), &jsonError));
    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug()<< "json error:" << jsonError.errorString();
        return false;
    }
    map = jsonDoc.toVariant().toMap();
    //    for(QMap<QString, QVariant>::const_iterator iter = map1.begin(); iter != map1.end(); ++iter){
    //        map[iter.key()] = iter.value();
    //    }
    return true;
}

static bool writeJsonFile(QIODevice &device, const QSettings::SettingsMap &map)
{
    bool ret = false;

    QJsonObject rootObj;
    /*QJsonDocument jsonDocument; = QJsonDocument::fromVariant(QVariant::fromValue(map));
    if ( device.write(jsonDocument.toJson()) != -1 )
        ret = true;*/
    for(QMap<QString, QVariant>::const_iterator iter = map.begin(); iter != map.end(); ++iter){
        rootObj[iter.key()] = QJsonValue::fromVariant(iter.value());
    }
    QJsonDocument jsonDoc;
    jsonDoc.setObject(rootObj);
    if ( device.write(jsonDoc.toJson()) != -1 )
        ret = true;
    return ret;
}

void Preferences::__init_wingeometry(struct WinGeometry *wg, int l, int t, int w, int h)
{
    wg->l = l;
    wg->t = t;
    wg->w = w;
    wg->h = h;
    wg->s = 0;
}


////vehicle_unit_100
////vehicle_unit_distbyvol
////=> used for column title

void Preferences::__init_measurement_units(void)
{
    // unit is kilometer
    if(!vehicle_unit_ismile)
    {
        vehicle_unit_dist0 = "%d km";
        vehicle_unit_dist1 = "%.1f km";
        vehicle_unit_100  = "100 km";
    }
    // unit is miles
    else
    {
        vehicle_unit_dist0 = "%d mi.";
        vehicle_unit_dist1 = "%.1f mi.";
        vehicle_unit_100  = "100 mi.";
    }

    // unit is Liters
    if(!vehicle_unit_isgal)
    {
        //TRANSLATORS: format a liter number with l/L as abbreviation
        vehicle_unit_vol  = "%.2f l";
        if(!vehicle_unit_ismile)
            //TRANSLATORS: kilometer per liter
            vehicle_unit_distbyvol  = "km/l";
        else
            //TRANSLATORS: miles per liter
            vehicle_unit_distbyvol  = "mi./l";
    }
    // unit is gallon
    else
    {
        vehicle_unit_vol  = "%.2f gal.";
        if(!vehicle_unit_ismile)
            vehicle_unit_distbyvol  = "km/gal.";
        else
            vehicle_unit_distbyvol  = "mi./gal.";
    }
}


//void homebank_pref_free(void)
//{
//	qDebug("\n[preferences] free\n") );


//	g_free(PREFS->date_format);

//	g_free(PREFS->api_rate_url);
//	g_free(PREFS->api_rate_key);

//	g_free(PREFS->color_exp);
//	g_free(PREFS->color_inc);
//	g_free(PREFS->color_warn);

//	g_free(PREFS->path_hbfile);
//	g_free(PREFS->path_import);
//	g_free(PREFS->path_export);
//	//g_free(PREFS->path_navigator);

//	g_free(PREFS->language);

//	g_free(PREFS->pnl_list_tab);

//	g_free(PREFS->minor_cur.symbol);
//	g_free(PREFS->minor_cur.decimal_char);
//	g_free(PREFS->minor_cur.grouping_char);

//	memset(PREFS, 0, sizeof(struct Preferences));
//}


int Preferences::list_column_get(int *cols_id, int uid, int maxcol)
{
    int i;

    for(i=0; i < maxcol ; i++ )
    {
        if( uid == qAbs(cols_id[i]) )
            return cols_id[i];

        /* secure point */
        if( i > 50)
            break;
    }
    return uid;
}


void Preferences::setdefault_lst_det_columns()
{
    int i = 0;

    lst_det_columns[i++] = LST_DSPOPE_STATUS;  //always displayed
    lst_det_columns[i++] = LST_DSPOPE_DATE;	  //always displayed
    lst_det_columns[i++] = -LST_DSPOPE_INFO;
    lst_det_columns[i++] = LST_DSPOPE_PAYEE;
    lst_det_columns[i++] = LST_DSPOPE_CATEGORY;
    lst_det_columns[i++] = -LST_DSPOPE_TAGS;
    lst_det_columns[i++] = LST_DSPOPE_CLR;
    lst_det_columns[i++] = LST_DSPOPE_AMOUNT;
    lst_det_columns[i++] = -LST_DSPOPE_EXPENSE;
    lst_det_columns[i++] = -LST_DSPOPE_INCOME;
    lst_det_columns[i++] = -LST_DSPOPE_BALANCE;
    lst_det_columns[i++] = LST_DSPOPE_MEMO;
    lst_det_columns[i++] = LST_DSPOPE_ACCOUNT;
    lst_det_columns[i++] = LST_DSPOPE_MATCH;

    for( i=0;i<NUM_LST_DSPOPE;i++)
        lst_det_col_width[i] = -1;
}


void Preferences::setdefault_lst_ope_columns()
{
int i = 0;

    lst_ope_columns[i++] = LST_DSPOPE_STATUS;  //always displayed
    lst_ope_columns[i++] = LST_DSPOPE_DATE;	  //always displayed
    lst_ope_columns[i++] = LST_DSPOPE_INFO;
    lst_ope_columns[i++] = LST_DSPOPE_PAYEE;
    lst_ope_columns[i++] = LST_DSPOPE_CATEGORY;
    lst_ope_columns[i++] = LST_DSPOPE_TAGS;
    lst_ope_columns[i++] = LST_DSPOPE_CLR;
    lst_ope_columns[i++] = -LST_DSPOPE_AMOUNT;
    lst_ope_columns[i++] = LST_DSPOPE_EXPENSE;
    lst_ope_columns[i++] = LST_DSPOPE_INCOME;
    lst_ope_columns[i++] = LST_DSPOPE_BALANCE;
    lst_ope_columns[i++] = LST_DSPOPE_MEMO;
    lst_ope_columns[i++] = -LST_DSPOPE_ACCOUNT;
    lst_ope_columns[i++] = -LST_DSPOPE_MATCH;

    lst_ope_sort_id    = LST_DSPOPE_DATE;
    lst_ope_sort_order = SORT_ASCENDING;

    for( i=0;i<NUM_LST_DSPOPE;i++)
        lst_ope_col_width[i] = -1;

}

Preferences::Preferences()
    : QSettings(HomeWallet::Instance()->app_get_config_dir() +"/"+ "preferences.json" ,QSettings::registerFormat("json", readJsonFile, writeJsonFile))
{
    m_iAccountsModel = nullptr;
    m_iPayeeModel = nullptr;
    sync();
    setdefault();
    load();
    if (XHWFilepath() != ""){
        QFileInfo fi(XHWFilepath());
        if (!fi.exists()){
            SetXHWFilepath("");
        } else {
            addConnection("QSQLITE", XHWFilepath(), "","", "", 0);
        }
        HomeWallet::Instance()->kcur = Currency::get_system_iso();
    }
}

QSqlError Preferences::addConnection(const QString &driver, const QString &dbName, const QString &host,
                            const QString &user, const QString &passwd, int port)
{
    __removeOldConnection();
    connectionname = QUuid::createUuid().toString();
    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(driver, connectionname);
    db.setDatabaseName(dbName);
    db.setHostName(host);
    db.setPort(port);
    if (!db.open(user, passwd)) {
        err = db.lastError();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(connectionname);
        connectionname = "";
    }
    if (m_iAccountsModel != nullptr){
        delete m_iAccountsModel;
        m_iAccountsModel = nullptr;
    }
    m_iAccountsModel = new QSqlTableModel(nullptr, db);
    m_iAccountsModel->setTable("t_accounts");
    if (m_iPayeeModel != nullptr){
        delete m_iPayeeModel;
        m_iPayeeModel = nullptr;
    }
    m_iPayeeModel = new QSqlTableModel(nullptr, db);
    m_iPayeeModel->setTable("t_payee");

    return err;
}

void Preferences::__removeOldConnection()
{
    QStringList connectionNames = QSqlDatabase::connectionNames();
    for (int i = 0; i < connectionNames.count(); ++i) {
        QSqlDatabase::removeDatabase(connectionNames.at(i));
    }
    connectionname = "";
    if (m_iAccountsModel != nullptr){
        delete m_iAccountsModel;
        m_iAccountsModel = nullptr;
    }
    if (m_iPayeeModel != nullptr){
        delete m_iPayeeModel;
        m_iPayeeModel = nullptr;
    }
}

void Preferences::setdefault_win()
{
int w = 1024, h = 600;

    // windows position/size 1024x600 for netbook
    // see https://gs.statcounter.com/screen-resolution-stats/desktop/worldwide
    // and gnome HIG

#if HB_UNSTABLE == true
    w = 1366; h = 768;
#endif

    Preferences::__init_wingeometry(&wal_wg, 0, 0, w, h);
    Preferences::__init_wingeometry(&acc_wg, 0, 0, w, h);

    w = (w * 0.8);
    h = (h * 0.8);
    Preferences::__init_wingeometry(&sta_wg, 0, 0, w, h);
    Preferences::__init_wingeometry(&tme_wg, 0, 0, w, h);
    Preferences::__init_wingeometry(&ove_wg, 0, 0, w, h);
    Preferences::__init_wingeometry(&bud_wg, 0, 0, w, h);
    Preferences::__init_wingeometry(&cst_wg, 0, 0, w, h);

    Preferences::__init_wingeometry(&txn_wg, 0, 0, -1, -1);
}


void Preferences::setdefault()
{
    int i;

    qDebug("setdefault starting......");

    language = "";

    date_format = DEFAULT_FORMAT_DATE;

    path_hbfile = QString::asprintf("%s", QDir::homePath().toStdString().c_str());
    path_hbbak  = QString::asprintf("%s", QDir::homePath().toStdString().c_str());
    path_import = QString::asprintf("%s", QDir::homePath().toStdString().c_str());
    path_export = QString::asprintf("%s", QDir::homePath().toStdString().c_str());
    //path_navigator = DEFAULT_PATH_NAVIGATOR;

    showsplash = true;
    showwelcome = true;
    loadlast = true;
    appendscheduled = false;
    do_update_currency = false;

    bak_is_automatic = true;
    bak_max_num_copies = 5;

    heritdate = false;
    txn_showconfirm = false;
    hidereconciled = false;
    showremind = true;
    //#1918334 no reason to show void by default
    showvoid = false;
    includeremind = false;
    //#1980562
    lockreconciled = true;
    //#1673048
    txn_memoacp = true;
    txn_memoacp_days = 365;
    //#1887212
    txn_xfer_daygap = 2;
    txn_xfer_syncstat = false;

    toolbar_style = 4;	//text beside icons
    grid_lines = 0;

    gtk_override = false;
    gtk_fontsize = 10;

    custom_colors = true;
    color_exp  = DEFAULT_EXP_COLOR;
    color_inc  = DEFAULT_INC_COLOR;
    color_warn = DEFAULT_WARN_COLOR;

    /* fiscal year */
    fisc_year_day = 1;
    fisc_year_month = 1;

    setdefault_win();

    IntCurrSymbol = "";
    IntCurrSymbol = Currency::get_system_iso();

    wal_toolbar = true;
    wal_totchart = true;
    wal_timchart = true;
    wal_upcoming = true;
    wal_vpaned = 600/2;
    wal_hpaned = 1024/2;

    pnl_acc_col_acc_width = -1;
    pnl_acc_show_by = DSPACC_GROUP_BY_TYPE;

    hub_tot_view  = 1;
    hub_tot_range = FLT_RANGE_THIS_MONTH;
    hub_tim_view  = 1;
    hub_tim_range = FLT_RANGE_LAST_12MONTHS;

    i = 0;
    lst_acc_columns[i++] = COL_DSPACC_STATUS;
    lst_acc_columns[i++] = COL_DSPACC_ACCOUNTS;
    lst_acc_columns[i++] = COL_DSPACC_CLEAR;
    lst_acc_columns[i++] = COL_DSPACC_RECON;
    lst_acc_columns[i++] = COL_DSPACC_TODAY;
    lst_acc_columns[i++] = COL_DSPACC_FUTURE;

    pnl_upc_col_pay_show  = 1;
    pnl_upc_col_pay_width = -1;
    pnl_upc_col_cat_show  = 1;
    pnl_upc_col_cat_width = -1;
    pnl_upc_col_mem_show  = 1;
    pnl_upc_col_mem_width = -1;
    pnl_upc_range = FLT_SCHEDULED_ALLDATE;

    i = 0;
    lst_impope_columns[i++] = LST_DSPOPE_DATE;	  //always displayed
    lst_impope_columns[i++] = LST_DSPOPE_MEMO;
    lst_impope_columns[i++] = LST_DSPOPE_AMOUNT;
    lst_impope_columns[i++] = LST_DSPOPE_INFO;
    lst_impope_columns[i++] = LST_DSPOPE_PAYEE;
    lst_impope_columns[i++] = LST_DSPOPE_CATEGORY;
    lst_impope_columns[i++] = -LST_DSPOPE_CLR;
    lst_impope_columns[i++] = -LST_DSPOPE_STATUS;  //always displayed
    lst_impope_columns[i++] = -LST_DSPOPE_EXPENSE;
    lst_impope_columns[i++] = -LST_DSPOPE_INCOME;
    lst_impope_columns[i++] = -LST_DSPOPE_BALANCE;
    lst_impope_columns[i++] = -LST_DSPOPE_ACCOUNT;
    lst_impope_columns[i++] = -LST_DSPOPE_MATCH;

    //book list column
    setdefault_lst_ope_columns();

    //detail list column
    setdefault_lst_det_columns();

    //base_cur.nbdecimal = 2;
    //base_cur.separator = true;

    //date_range_wal = FLT_RANGE_LASTMONTH;
    //date_range_txn = FLT_RANGE_LAST12MONTHS;
    //date_range_rep = FLT_RANGE_THISYEAR;

    //v5.2 change to let the example file show things
    //date_range_wal = FLT_RANGE_MISC_ALLDATE;
    date_range_txn = FLT_RANGE_MISC_ALLDATE;
    date_range_rep = FLT_RANGE_MISC_ALLDATE;
    date_future_nbdays = 0;
    rep_maxspenditems = 10;

    //forecast
    rep_forcast = true;
    rep_forecat_nbmonth = 6;

    //import/export
    dtex_nointro = true;
    dtex_dodefpayee = false;
    dtex_doautoassign = false;

    dtex_ucfirst = false;
    //#2040010
    dtex_datefmt = PRF_DATEFMT_YMD;
    dtex_ofxname = 1;
    dtex_ofxmemo = 2;
    dtex_qifmemo = true;
    dtex_qifswap = false;
    dtex_csvsep =  Preferences::DTEX_CSVSEP_SEMICOLON;

    //currency api
    api_rate_url = "https://api.frankfurter.app/latest";
    api_rate_key = "";

    //todo: add intelligence here
    euro_active  = false;

    euro_country = 0;
    euro_value   = 1.0;

    minor_cur = nullptr;
//    da_cur_initformat(&minor_cur);

    //euro_nbdec   = 2;
    //euro_thsep   = true;
    //euro_symbol	= g_strdup("??");

    stat_byamount    = false;
    stat_showdetail  = false;
    stat_showrate    = false;
    stat_includexfer = false;
    budg_showdetail  = false;
    report_color_scheme = CHART_COLMAP_HOMEBANK;

    //chart_legend = false;

    vehicle_unit_ismile = false;
    vehicle_unit_isgal  = false;

    __init_measurement_units();
    qDebug("setdefault finished");
}


///*
//** load preference from homedir/.homebank (HB_DATA_PATH)
//*/
void Preferences::__get_wingeometry(QString strVal,  struct WinGeometry *storage)
{
    if (strVal == "" || storage == nullptr){
        qWarning("strVal(%s) must be a string like(int;int;int;int;int;) and storage must be nullptr", strVal.toStdString().c_str());
        return;
    }
    auto src = __get_integer_list(strVal);
    int length = src.size();
    if (src.size() != 5){
        qWarning("strVal(%s) must have the format(int;int;int;int;int;)", strVal.toStdString().c_str());
        return;
    }
    storage->l = src[0];
    storage->t = src[1];
    storage->w = src[2];
    storage->h = src[3];
    storage->s = src[4];

    // #606613 ensure left/top to be > 0
    if(storage->l < 0)
        storage->l = 0;

    if(storage->t < 0)
        storage->t = 0;
}

QList<int> Preferences::__get_integer_list(QString strVal)
{
    QList<int> src;
    QStringList tmplist = strVal.split(";");
    for(auto tmp : tmplist){
        if (tmp != ""){
            bool ok;
            int ival = tmp.toInt(&ok);
            if (ok){
                src.append(ival);
            }
        }
    }
    return src;
}




//static void homebank_pref_get_boolean(
//	GKeyFile *key_file,
//    const gchar *group_name,
//    const gchar *key,
//	bool *storage)
//{
//	qDebug(" search %s in %s\n", key, group_name) );

//	if( g_key_file_has_key(key_file, group_name, key, NULL) )
//	{
//		*storage = g_key_file_get_boolean(key_file, group_name, key, NULL);
//		qDebug(" > stored boolean %d for %s at %p\n", *storage, key, storage) );
//	}
//}

//static void homebank_pref_get_integer(
//	GKeyFile *key_file,
//    const gchar *group_name,
//    const gchar *key,
//	int *storage)
//{
//	qDebug(" search %s in %s\n", key, group_name) );

//	if( g_key_file_has_key(key_file, group_name, key, NULL) )
//	{
//		*storage = g_key_file_get_integer(key_file, group_name, key, NULL);
//		qDebug(" > stored integer %d for %s at %p\n", *storage, key, storage) );
//	}
//}

//static void homebank_pref_get_uint32_t(
//	GKeyFile *key_file,
//    const gchar *group_name,
//    const gchar *key,
//	uint32_t *storage)
//{
//	qDebug(" search %s in %s\n", key, group_name) );

//	if( g_key_file_has_key(key_file, group_name, key, NULL) )
//	{
//		*storage = g_key_file_get_integer(key_file, group_name, key, NULL);
//		qDebug(" > stored uint32_t %d for %s at %p\n", *storage, key, storage) );
//	}
//}

//static void homebank_pref_get_short(
//	GKeyFile *key_file,
//    const gchar *group_name,
//    const gchar *key,
//	gshort *storage)
//{
//	qDebug(" search %s in %s\n", key, group_name) );

//	if( g_key_file_has_key(key_file, group_name, key, NULL) )
//	{
//		*storage = (gshort)g_key_file_get_integer(key_file, group_name, key, NULL);
//		qDebug(" > stored short %d for %s at %p\n", *storage, key, storage) );
//	}
//}

//static void homebank_pref_get_string(
//	GKeyFile *key_file,
//    const gchar *group_name,
//    const gchar *key,
//	gchar **storage)
//{
//gchar *string;

//	qDebug(" search %s in %s\n", key, group_name) );

//	if( g_key_file_has_key(key_file, group_name, key, NULL) )
//	{
//		/* free any previous string */
//		if( *storage != NULL )
//		{
//			qDebug(" storage was not null, freeing\n") );

//			g_free(*storage);
//		}

//		*storage = NULL;

//		string = g_key_file_get_string(key_file, group_name, key, NULL);
//		if( string != NULL )
//		{
//			//*storage = g_strdup(string);
//			//leak
//			*storage = string;	//already a new allocated string

//			qDebug(" > stored '%s' for %s at %p\n", *storage, key, *storage) );
//		}
//	}

///*
//	if (error)
//    {
//      g_warning ("error: %s\n", error->message);
//      g_error_free(error);
//      error = NULL;
//    }
//*/


//}


int Preferences::__upgrade_560_daterange(int oldrange)
{
int newrange = FLT_RANGE_UNSET;

    switch(oldrange)
    {
        case OLD56_FLT_RANGE_THISMONTH:    newrange = FLT_RANGE_THIS_MONTH; break;
        case OLD56_FLT_RANGE_LASTMONTH:    newrange = FLT_RANGE_LAST_MONTH; break;
        case OLD56_FLT_RANGE_THISQUARTER:  newrange = FLT_RANGE_THIS_QUARTER; break;
        case OLD56_FLT_RANGE_LASTQUARTER:  newrange = FLT_RANGE_LAST_QUARTER; break;
        case OLD56_FLT_RANGE_THISYEAR:     newrange = FLT_RANGE_THIS_YEAR; break;
        case OLD56_FLT_RANGE_LASTYEAR:     newrange = FLT_RANGE_LAST_YEAR; break;
        case OLD56_FLT_RANGE_LAST30DAYS:   newrange = FLT_RANGE_LAST_30DAYS; break;
        case OLD56_FLT_RANGE_LAST60DAYS:   newrange = FLT_RANGE_LAST_60DAYS; break;
        case OLD56_FLT_RANGE_LAST90DAYS:   newrange = FLT_RANGE_LAST_90DAYS; break;
        case OLD56_FLT_RANGE_LAST12MONTHS: newrange = FLT_RANGE_LAST_12MONTHS; break;
        case OLD56_FLT_RANGE_ALLDATE:      newrange = FLT_RANGE_MISC_ALLDATE; break;
    }
    qDebug(" %d => %d", oldrange, newrange);
    return newrange;
}


void Preferences::__currfmt_convert(Currency *cur, QString prefix, QString suffix)
{
    if(prefix != "") {
        cur->symbol = prefix;
        cur->sym_prefix = true;
    } else if (suffix != "") {
        cur->symbol = suffix;
        cur->sym_prefix = false;
    }
}


void Preferences::__loadGeneral(uint32_t& version){
    qDebug(" -> ** General");
    if (!contains("General")){
        qWarning("no General group in preference");
        return;
    }
    QVariantMap groupsetting = value("General").toMap();

    //since 4.51 version is integer
    if(groupsetting.contains("Version")) version = groupsetting.value("Version").toUInt();
    qDebug(" - version: %d\n", version);
    if(groupsetting.contains("Language")) language = groupsetting.value("Language").toString();
    if(groupsetting.contains("BarStyle")) toolbar_style = groupsetting.value("BarStyle").toInt();
    if(version <= 6 && toolbar_style == 0)	// force system to text beside
    {
        toolbar_style = 4;
    }


    if(version <= 2)	// retrieve old settings
    {
        if(groupsetting.contains("ColorExp")) color_exp = groupsetting.value("ColorExp").toString();
        if(groupsetting.contains("ColorInc")) color_inc = groupsetting.value("ColorInc").toString();
        if(groupsetting.contains("ColorWarn")) color_warn = groupsetting.value("ColorWarn").toString();
    }
    else
    {
        if(groupsetting.contains("CustomColors")) custom_colors = groupsetting.value("CustomColors").toBool();
        if(groupsetting.contains("ColorExp")) color_exp = groupsetting.value("ColorExp").toString();
        if(groupsetting.contains("ColorInc")) color_inc = groupsetting.value("ColorInc").toString();
        if(groupsetting.contains("ColorWarn")) color_warn = groupsetting.value("ColorWarn").toString();

        if( version <= 500 )
        {
            bool rules_hint = false;
            rules_hint = groupsetting.value("RulesHint").toBool();
            if( rules_hint == true )
                grid_lines = 0;//GTK_TREE_VIEW_GRID_LINES_HORIZONTAL;

        }
        else
            if(groupsetting.contains("GridLines")) grid_lines = groupsetting.value("GridLines").toInt();

        //we disable showwelcome for old users
        if( version < 540 )
            showwelcome = false;
    }

    qDebug(" - color exp: %s\n", color_exp.toStdString().c_str());
    qDebug(" - color inc: %s\n", color_inc.toStdString().c_str());
    qDebug(" - color wrn: %s\n", color_warn.toStdString().c_str());

    if(groupsetting.contains("WalletPath")) path_hbfile = groupsetting.value("WalletPath").toString();
    if(groupsetting.contains("BackupPath")) path_hbbak = groupsetting.value("BackupPath").toString();
    //#1870433 default backup path folder not initialized with wallet folder
    if( version < 530 )
    {
        if(groupsetting.contains("WalletPath")) path_hbbak = groupsetting.value("WalletPath").toString();
    }
    if(groupsetting.contains("ImportPath")) path_import = groupsetting.value("ImportPath").toString();
    if(groupsetting.contains("ExportPath")) path_export = groupsetting.value("ExportPath").toString();

    if(groupsetting.contains("ShowSplash")) showsplash = groupsetting.value("ShowSplash").toBool();
    if(groupsetting.contains("ShowWelcome")) showwelcome = groupsetting.value("ShowWelcome").toBool();
    if(groupsetting.contains("LoadLast")) loadlast = groupsetting.value("LoadLast").toBool();
    if(groupsetting.contains("AppendScheduled")) appendscheduled = groupsetting.value("AppendScheduled").toBool();
    if(groupsetting.contains("UpdateCurrency")) do_update_currency = groupsetting.value("UpdateCurrency").toBool();
    if(groupsetting.contains("BakIsAutomatic")) bak_is_automatic = groupsetting.value("BakIsAutomatic").toBool();
    if(groupsetting.contains("BakMaxNumCopies")) bak_max_num_copies = groupsetting.value("BakMaxNumCopies").toInt();

    if(groupsetting.contains("HeritDate")) heritdate = groupsetting.value("HeritDate").toBool();
    if(groupsetting.contains("ShowConfirm")) txn_showconfirm = groupsetting.value("ShowConfirm").toBool();
    if(groupsetting.contains("HideReconciled")) hidereconciled = groupsetting.value("HideReconciled").toBool();
    if(groupsetting.contains("ShowRemind")) showremind = groupsetting.value("ShowRemind").toBool();
    if(groupsetting.contains("ShowVoid")) showvoid = groupsetting.value("ShowVoid").toBool();
    if(groupsetting.contains("IncludeRemind")) includeremind = groupsetting.value("IncludeRemind").toBool();
    if(groupsetting.contains("LockReconciled")) lockreconciled = groupsetting.value("LockReconciled").toBool();
    if(groupsetting.contains("TxnMemoAcp")) txn_memoacp = groupsetting.value("TxnMemoAcp").toBool();
    if(groupsetting.contains("TxnMemoAcpDays")) txn_memoacp_days = groupsetting.value("TxnMemoAcpDays").toInt();
    if(groupsetting.contains("TxnXferDayGap")) txn_xfer_daygap = groupsetting.value("TxnXferDayGap").toInt();
    if(groupsetting.contains("TxnXferSyncStatus")) txn_xfer_syncstat = groupsetting.value("TxnXferSyncStatus").toBool();

    if(groupsetting.contains("ColumnsOpe")) {
        bool *bsrc;
        int i, j;
        auto src = __get_integer_list(groupsetting.value("ColumnsOpe").toString());
        int length = src.size();

        qDebug(" - length %d (max=%d)\n", (int)length, NUM_LST_DSPOPE);
        memset(lst_ope_columns, 0, sizeof (lst_ope_columns));

        if( length == NUM_LST_DSPOPE )
        {
            qDebug(" - copying column order from pref file\n");
            for (i = 0; i < length; i++){
                lst_ope_columns[i] = src[i];
            }
        }
        else
        {
            if(version <= 7)
            {
                if( length == NUM_LST_DSPOPE-2 )	//1 less column before v4.5.1
                {
                    qDebug(" - upgrade from v7\n");
                    qDebug(" - copying column order from pref file\n");
                    for (i = 0; i < length; i++){
                        lst_ope_columns[i] = src[i];
                    }
                    //append balance column
                    lst_ope_columns[10] = LST_DSPOPE_BALANCE;
                }
            }

            if(version < 500)
            {
                if( length == NUM_LST_DSPOPE-2 )	//1 less column before v4.5.1
                {
                    qDebug(" - upgrade prior v5.0\n");
                    qDebug(" - copying column order from pref file\n");
                    bool added = false;
                    for(i=0,j=0; i<NUM_LST_DSPOPE-1 ; i++)
                    {
                        if( added == false &&
                            (qAbs(src[i]) == LST_DSPOPE_AMOUNT ||
                            qAbs(src[i]) == LST_DSPOPE_EXPENSE ||
                            qAbs(src[i]) == LST_DSPOPE_INCOME) )
                        {
                            lst_ope_columns[j++] = LST_DSPOPE_CLR;
                            added = true;
                        }
                        lst_ope_columns[j++] = src[i];
                    }
                }
            }

        }

    }
    if(groupsetting.contains("ColumnsOpeWidth")) {
        auto src = __get_integer_list(groupsetting.value("ColumnsOpeWidth").toString());
        int length = src.size();
        qDebug(" - length %d (max=%d)\n", (int)length, NUM_LST_DSPOPE);
        memset(lst_ope_col_width, 0, sizeof(lst_ope_col_width));
        if( length == NUM_LST_DSPOPE )
        {
            qDebug(" - copying column width from pref file\n");
            for (int i = 0; i < length; i++){
                lst_ope_col_width[i] = src[i];
            }
        }
    }
    if(groupsetting.contains("OpeSortId")) lst_ope_sort_id = groupsetting.value("OpeSortId").toInt();
    if(groupsetting.contains("OpeSortOrder")) lst_ope_sort_order = groupsetting.value("OpeSortOrder").toInt();

    qDebug(" - set sort to %d %d\n", lst_ope_sort_id, lst_ope_sort_order);

    //detail list
    if(groupsetting.contains("ColumnsDet")) {
        auto src = __get_integer_list(groupsetting.value("ColumnsDet").toString());
        int length = src.size();
        memset(lst_det_columns, 0, sizeof (lst_det_columns));
        qDebug(" - length %d (max=%d)\n", (int)length, NUM_LST_DSPOPE);
        if( length == NUM_LST_DSPOPE )
        {
            qDebug(" - copying column order from pref file\n");
            for (int i = 0; i < length; i++){
                lst_det_columns[i] = src[i];
            }
        }
    }
    if(groupsetting.contains("ColumnsDetWidth")){
        auto src = __get_integer_list(groupsetting.value("ColumnsDetWidth").toString());
        int length = src.size();
        memset(lst_det_col_width, 0, sizeof (lst_det_col_width));
        qDebug(" - length %d (max=%d)\n", (int)length, NUM_LST_DSPOPE);

        if( length == NUM_LST_DSPOPE )
        {
            qDebug(" - copying column width from pref file\n");
            for (int i = 0; i < length; i++){
                lst_det_col_width[i] = src[i];
            }
        }
    }

    if(groupsetting.contains("FiscYearDay")) fisc_year_day = groupsetting.value("FiscYearDay").toInt();
    if(groupsetting.contains("FiscYearMonth")) fisc_year_month = groupsetting.value("FiscYearMonth").toInt();
}

void Preferences::__loadWindows(uint32_t &version)
{
    if (!contains("Windows")){
        qWarning("no Windows group in preference");
        return;
    }
    QVariantMap groupsetting = value("Windows").toMap();
    qDebug(" -> ** Windows");
    if (groupsetting.contains("Wal"))__get_wingeometry(groupsetting.value("Wal").toString(), &wal_wg);
    if (groupsetting.contains("Acc"))__get_wingeometry(groupsetting.value("Acc").toString(), &acc_wg);
    if (groupsetting.contains("Sta"))__get_wingeometry(groupsetting.value("Sta").toString(), &sta_wg);
    if (groupsetting.contains("Tme"))__get_wingeometry(groupsetting.value("Tme").toString(), &tme_wg);
    if (groupsetting.contains("Ove"))__get_wingeometry(groupsetting.value("Ove").toString(), &ove_wg);
    if (groupsetting.contains("Bud"))__get_wingeometry(groupsetting.value("Bud").toString(), &bud_wg);
    if (groupsetting.contains("Car"))__get_wingeometry(groupsetting.value("Car").toString(), &cst_wg);

    if (groupsetting.contains("Txn"))__get_wingeometry(groupsetting.value("Txn").toString(), &txn_wg);
    if (groupsetting.contains("DBud"))__get_wingeometry(groupsetting.value("DBud").toString(), &dbud_wg);

    if(version <= 7)	//set maximize to 0
    {
        wal_wg.s = 0;
        acc_wg.s = 0;
        txn_wg.s = 0;
        sta_wg.s = 0;
        tme_wg.s = 0;
        ove_wg.s = 0;
        bud_wg.s = 0;
        cst_wg.s = 0;
    }
    if(groupsetting.contains("WalVPaned")) wal_vpaned = groupsetting.value("WalVPaned").toInt();
    if(groupsetting.contains("WalHPaned")) wal_hpaned = groupsetting.value("WalHPaned").toInt();
    if(groupsetting.contains("WalToolbar")) wal_toolbar = groupsetting.value("WalToolbar").toBool();
    if(groupsetting.contains("WalTotalChart")) wal_totchart = groupsetting.value("WalTotalChart").toBool();
    if(groupsetting.contains("WalTimeChart")) wal_timchart = groupsetting.value("WalTimeChart").toBool();
    if(groupsetting.contains("WalUpcoming")) wal_upcoming = groupsetting.value("WalUpcoming").toBool();
    if( version < 570 )
    {
        wal_totchart = groupsetting.value("WalSpending").toBool();
    }
}

void Preferences::__loadPanels(uint32_t &version)
{
    //since 5.1.3
    if (!contains("Panels")){
        qWarning("no Panels group in preference");
        return;
    }
    QVariantMap groupsetting = value("Panels").toMap();
    qDebug(" -> ** Panels");


    if(groupsetting.contains("AccColAccW")) pnl_acc_col_acc_width = groupsetting.value("AccColAccW").toInt();
    if(groupsetting.contains("AccShowBy")) pnl_acc_show_by = groupsetting.value("AccShowBy").toInt();

    if(groupsetting.contains("AccColumns")) {
        auto src = __get_integer_list(groupsetting.value("AccColumns").toString());
        int length = src.size();
        memset(lst_acc_columns, 0, sizeof(lst_acc_columns));
        qDebug(" - length %d (max=%d)\n", (int)length, NUM_LST_DSPOPE);

        if( length == NUM_LST_COL_DSPACC )
        {
            qDebug(" - copying column order from pref file\n");
            for(int iLoop = 0; iLoop < src.size(); iLoop++)lst_acc_columns[iLoop]=src[iLoop];
        }
    }

    //hub total/time
    if(groupsetting.contains("HubTotView")) hub_tot_view = groupsetting.value("HubTotView").toInt();
    if(groupsetting.contains("HubTotViewRange")) hub_tot_range = groupsetting.value("HubTotViewRange").toInt();
    if(groupsetting.contains("HubTimView")) hub_tim_view = groupsetting.value("HubTimView").toInt();
    if(groupsetting.contains("HubTimViewRange")) hub_tim_range = groupsetting.value("HubTimViewRange").toInt();

    //upcoming
    if(groupsetting.contains("UpcColPayV")) pnl_upc_col_pay_show = groupsetting.value("UpcColPayV").toInt();
    if(groupsetting.contains("UpcColCatV")) pnl_upc_col_cat_show = groupsetting.value("UpcColCatV").toInt();
    if(groupsetting.contains("UpcColMemV")) pnl_upc_col_mem_show = groupsetting.value("UpcColMemV").toInt();
    if(groupsetting.contains("UpcColPayW")) pnl_upc_col_pay_width = groupsetting.value("UpcColPayW").toInt();
    if(groupsetting.contains("UpcColCatW")) pnl_upc_col_cat_width = groupsetting.value("UpcColCatW").toInt();
    if(groupsetting.contains("UpcColMemW")) pnl_upc_col_mem_width = groupsetting.value("UpcColMemW").toInt();
    if(groupsetting.contains("UpcRange")) pnl_upc_range = groupsetting.value("UpcRange").toInt();

    if(groupsetting.contains("PnlLstTab")) pnl_list_tab = groupsetting.value("PnlLstTab").toString();
}

void Preferences::__loadFormat(uint32_t &version)
{
    if (!contains("Format")){
        qWarning("no Format group in preference");
        return;
    }
    QVariantMap groupsetting = value("Format").toMap();
    qDebug(" -> ** Format");

    if(groupsetting.contains("DateFmt")) date_format = groupsetting.value("DateFmt").toString();

    if(version < 460)
    {
        bool useimperial = false;

        if(groupsetting.contains("UKUnits")) useimperial = groupsetting.value("UKUnits").toBool();
        if(useimperial)
        {
            vehicle_unit_ismile = true;
            vehicle_unit_isgal = true;
        }
    }

    if(groupsetting.contains("UnitIsMile")) vehicle_unit_ismile = groupsetting.value("UnitIsMile").toBool();
    if(groupsetting.contains("UnitIsGal")) vehicle_unit_isgal = groupsetting.value("UnitIsGal").toBool();
}

void Preferences::__loadFilter(uint32_t &version)
{
    if (!contains("Filter")){
        qWarning("no Filter group in preference");
        return;
    }
    QVariantMap groupsetting = value("Filter").toMap();
    qDebug(" -> ** Filter");

    //homebank_pref_get_integer(keyfile, group, "DateRangeWal", &PREFS->date_range_wal);
    if(groupsetting.contains("DateRangeTxn")) date_range_txn = groupsetting.value("DateRangeTxn").toInt();
    if(groupsetting.contains("DateFutureNbDays")) date_future_nbdays = groupsetting.value("DateFutureNbDays").toInt();
    if(groupsetting.contains("DateRangeRep")) date_range_rep = groupsetting.value("DateRangeRep").toInt();

    if(version <= 7)
    {
        // shift date range >= 5, since we inserted a new one at position 5
        //if(PREFS->date_range_wal >= OLD56_FLT_RANGE_LASTYEAR)
        //	PREFS->date_range_wal++;
        if(date_range_txn >= OLD56_FLT_RANGE_LASTYEAR)
            date_range_txn++;
        if(date_range_rep >= OLD56_FLT_RANGE_LASTYEAR)
            date_range_rep++;
    }
}

void Preferences::__loadAPI(uint32_t &version)
{
    if (!contains("API")){
        qWarning("no API group in preference");
        return;
    }
    QVariantMap groupsetting = value("API").toMap();
    qDebug(" -> ** API");

    if(groupsetting.contains("APIRateUrl")) api_rate_url = groupsetting.value("APIRateUrl").toString();
    if(groupsetting.contains("APIRateKey")) api_rate_key = groupsetting.value("APIRateKey").toString();

    //5.7.2 fix wrong host set as defaut in 5.7
    if(version < 572)
    {
        if( "https://api.exchangerate.host" == api_rate_url)
        {
            qDebug(" fix bad host in 5.7\n");
            api_rate_url = "https://api.frankfurter.app/latest";
        }
    }
}

void Preferences::__loadEuro(uint32_t &version)
{
    if (!contains("Euro")){
        qWarning("no Euro group in preference");
        return;
    }
    QVariantMap groupsetting = value("Euro").toMap();
    qDebug(" -> ** Euro");

    //homebank_pref_get_string(keyfile, group, "DefCurrency" , &PREFS->curr_default);

    if(groupsetting.contains("Active")) euro_active = groupsetting.value("Active").toBool();
    if(groupsetting.contains("Country")) euro_country = groupsetting.value("Country").toInt();

    if(groupsetting.contains("ChangeRate")) euro_value = groupsetting.value("ChangeRate").toDouble();

    if(version <= 1)
    {
        if(groupsetting.contains("ChangeRate") && minor_cur != nullptr) minor_cur->symbol = groupsetting.value("Symbol").toString();
        if(groupsetting.contains("ChangeRate") && minor_cur != nullptr) minor_cur->frac_digits =  groupsetting.value("NBDec").toInt();

        //PREFS->euro_nbdec = g_key_file_get_integer (keyfile, group, "NBDec", NULL);
        //PREFS->euro_thsep = g_key_file_get_boolean (keyfile, group, "Sep", NULL);
        //gchar *tmpstr = g_key_file_get_string  (keyfile, group, "Symbol", &error);
    }
    else
    {
        if(version < 460)
        {
            QString prefix = "";
            QString suffix = "";
            if(groupsetting.contains("PreSymbol")) prefix = groupsetting.value("PreSymbol").toString();
            if(groupsetting.contains("SufSymbol")) suffix = groupsetting.value("SufSymbol").toString();

            __currfmt_convert(minor_cur, prefix, suffix);
        }
        else
        {
            if(groupsetting.contains("Symbol") && minor_cur != nullptr) minor_cur->symbol = groupsetting.value("Symbol").toString();
            if(groupsetting.contains("IsPrefix") && minor_cur != nullptr) minor_cur->sym_prefix = groupsetting.value("IsPrefix").toBool();
        }
        if(groupsetting.contains("DecChar") && minor_cur != nullptr) minor_cur->decimal_char = groupsetting.value("DecChar").toString();
        if(groupsetting.contains("GroupChar") && minor_cur != nullptr) minor_cur->grouping_char = groupsetting.value("GroupChar").toString();
        if(groupsetting.contains("FracDigits") && minor_cur != nullptr) minor_cur->frac_digits = groupsetting.value("FracDigits").toInt();

        //fix 378992/421228
        if(minor_cur != nullptr && minor_cur->frac_digits > MAX_FRAC_DIGIT )
           minor_cur->frac_digits = MAX_FRAC_DIGIT;
        if(minor_cur != nullptr) minor_cur->initformat();
    }

    //PREFS->euro_symbol = g_locale_to_utf8(tmpstr, -1, NULL, NULL, NULL);

}

void Preferences::__loadReport(uint32_t &version)
{
    if (!contains("Report")){
        qWarning("no Report group in preference");
        return;
    }
    QVariantMap groupsetting = value("Report").toMap();
    qDebug(" -> ** Report");

    if(groupsetting.contains("StatByAmount")) stat_byamount = groupsetting.value("StatByAmount" ).toBool();
    if(groupsetting.contains("StatDetail")) stat_showdetail = groupsetting.value("StatDetail"   ).toBool();
    if(groupsetting.contains("StatRate")) stat_showrate = groupsetting.value("StatRate"         ).toBool();
    if(groupsetting.contains("StatIncXfer")) stat_includexfer = groupsetting.value("StatIncXfer").toBool();
    if(groupsetting.contains("BudgDetail")) budg_showdetail = groupsetting.value("BudgDetail"   ).toBool();
    if(groupsetting.contains("SmallFont")) rep_smallfont = groupsetting.value("SmallFont"       ).toBool();
    if(groupsetting.contains("Forecast")) rep_forcast = groupsetting.value("Forecast"           ).toBool();

    if(groupsetting.contains("ForecastNbMonth")) rep_forecat_nbmonth = groupsetting.value("ForecastNbMonth").toInt();
    if(groupsetting.contains("MaxSpendItems")) rep_maxspenditems = groupsetting.value("MaxSpendItems"      ).toInt();
    if(groupsetting.contains("ColorScheme")) report_color_scheme = groupsetting.value("ColorScheme"        ).toInt();
}

void Preferences::__loadExchange(uint32_t &version)
{
    if (!contains("Exchange")){
        qWarning("no Exchange group in preference");
        return;
    }
    QVariantMap groupsetting = value("Exchange").toMap();
    qDebug(" -> ** Exchange");

    if(groupsetting.contains("DoIntro"     )) dtex_nointro      = groupsetting.value("DoIntro"     ).toBool();
    if(groupsetting.contains("UcFirst"     )) dtex_ucfirst      = groupsetting.value("UcFirst"     ).toBool();
    if(groupsetting.contains("QifMemo"     )) dtex_qifmemo      = groupsetting.value("QifMemo"     ).toBool();
    if(groupsetting.contains("QifSwap"     )) dtex_qifswap      = groupsetting.value("QifSwap"     ).toBool();
    if(groupsetting.contains("DoDefPayee"  )) dtex_dodefpayee   = groupsetting.value("DoDefPayee"  ).toBool();
    if(groupsetting.contains("DoAutoAssign")) dtex_doautoassign = groupsetting.value("DoAutoAssign").toBool();

    if(groupsetting.contains("DateFmt")) dtex_datefmt = groupsetting.value("DateFmt").toInt();
    if(groupsetting.contains("DayGap" )) dtex_daygap  = groupsetting.value("DayGap" ).toInt();
    if(groupsetting.contains("OfxName")) dtex_ofxname = groupsetting.value("OfxName").toInt();
    if(groupsetting.contains("OfxMemo")) dtex_ofxmemo = groupsetting.value("OfxMemo").toInt();
    if(groupsetting.contains("CsvSep" )) dtex_csvsep  = groupsetting.value("CsvSep" ).toInt();
}

void Preferences::__loadChart(uint32_t &version)
{
    if (!contains("Chart")){
        qWarning("no Chart group in preference");
        return;
    }
    QVariantMap groupsetting = value("Chart").toMap();
    qDebug(" -> ** Chart");
//    if(groupsetting.contains("Legend")) chart_legend = groupsetting.value("Legend").toBool();
}

bool Preferences::load()
{
//    GKeyFile *keyfile;
    bool retval = false;
    QString group, filename;
    uint32_t version = 0;
    bool loaded;
    //GError *error = NULL;

    qDebug("starting......");

    {
        filename = HomeWallet::Instance()->app_get_config_dir() +"/"+ "preferences";

        qDebug(" - filename: %s", filename.toStdString().c_str());

        {
            __loadGeneral(version);
            __loadWindows(version);
            __loadPanels(version);
            __loadFormat(version);
            __loadFilter(version);
            __loadAPI(version);
            __loadEuro(version);
            __loadReport(version);
            __loadExchange(version);
            __loadChart(version);


            /* file upgrade */
            if(version < 560)
            {
                qDebug(" ugrade 5.6 daterange\n");
                //convert old daterange
                //date_range_wal = __upgrade_560_daterange(date_range_wal);	//top spending
                date_range_txn = __upgrade_560_daterange(date_range_txn);	//transactions
                date_range_rep = __upgrade_560_daterange(date_range_rep);	//report options
            }

        }

        __init_measurement_units();
    }

    return retval;
}


Preferences* Preferences::m_iInstance = nullptr;
