/*  HomeBank -- Free, easy, personal accounting for everyone.
 *  Copyright (C) 1995-2023 Maxime DOYEN
 *
 *  This file is part of HomeBank.
 *
 *  HomeBank is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  HomeBank is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __HB_PREFERENCES_H__
#define __HB_PREFERENCES_H__

#include "hw-currency.h"
#include "enums.h"
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QDebug>
#include <QList>
#include <QMap>
#include <QMessageBox>
#include <QUuid>
#include <QFileInfo>
#include <QDir>
#include <QMutex>
#include <QDateTime>
#include <QSharedPointer>
#include <QObject>
#include <QString>
#include <QUuid>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QException>
#include <QSqlTableModel>



#define DEFAULT_FORMAT_DATE			"%x"

#define MAX_FRAC_DIGIT		6

//Tango light
#define LIGHT_EXP_COLOR		"#fcaf3e"	//Orange
#define LIGHT_INC_COLOR		"#8ae234"	//Chameleon
#define LIGHT_WARN_COLOR	"#ef2929"	//Scarlett Red

//Tango medium
#define MEDIUM_EXP_COLOR	"#f57900"	//Orange
#define MEDIUM_INC_COLOR	"#73d216"	//Chameleon
#define MEDIUM_WARN_COLOR	"#cc0000"	//Scarlett Red

//Tango dark
#define DEFAULT_EXP_COLOR		"#ce5c00"	//Orange
#define DEFAULT_INC_COLOR		"#4e9a36"	//Chameleon
#define DEFAULT_WARN_COLOR		"#a40000"	//Scarlett Red


#define PRF_DTEX_CSVSEP_BUFFER "\t,; "



enum
{
    DSPACC_GROUP_BY_TYPE,
    DSPACC_GROUP_BY_BANK,
    DSPACC_GROUP_BY_GROUP
};


/* lst acc datatype */
enum
{
    DSPACC_TYPE_NORMAL,
    DSPACC_TYPE_HEADER,
    DSPACC_TYPE_SUBTOTAL,
    DSPACC_TYPE_TOTAL
};


/* list display account */
enum
{
    LST_DSPACC_DATATYPE,
    LST_DSPACC_POS,
    LST_DSPACC_DATAS,
    NUM_LST_DSPACC
};

enum
{
    COL_DSPACC_STATUS,
    COL_DSPACC_ACCOUNTS,
    COL_DSPACC_RECON,
    COL_DSPACC_CLEAR,
    COL_DSPACC_TODAY,
    COL_DSPACC_FUTURE,
    NUM_LST_COL_DSPACC
};



/*
** Preference datas
*/

struct WinGeometry
{
    int		l, t, w, h, s;
};




class Preferences : public QSettings
{
    Q_OBJECT

public:
    static Preferences* Instance(){
        if (m_iInstance == nullptr){
            m_iInstance = new Preferences();
        }
        return m_iInstance;
    }
    int list_column_get(int *cols_id, int uid, int maxcol);
    void setdefault_lst_det_columns();
    void setdefault_lst_ope_columns();
    void setdefault_win();
    bool load();
    void setdefault();

    enum
    {
      SORT_ASCENDING,
      SORT_DESCENDING
    } ;
    enum {
        DTEX_CSVSEP_TAB,
        DTEX_CSVSEP_COMMA,
        DTEX_CSVSEP_SEMICOLON,
        DTEX_CSVSEP_SPACE,
    };
    QString XHWFilepath(){
        if (!contains("General")){
            qWarning("no General group in preference");
            return "";
        }
        QVariantMap groupsetting = value("General").toMap();
        if(groupsetting.contains("xhw_filepath")) return groupsetting.value("xhw_filepath").toString();
        else return "";
    }
    void SetXHWFilepath(QString filepath){
        QVariantMap groupsetting;
        if (contains("General")){
            groupsetting = value("General").toMap();
        }
        groupsetting["xhw_filepath"] = filepath;
        setValue("General", groupsetting);
    }
    QSqlError addConnection(const QString &driver, const QString &dbName, const QString &host,
                                const QString &user, const QString &passwd, int port);
    QSqlDatabase currentDatabase() const
    {
        return QSqlDatabase::database(connectionname);
    }
    template<typename T>
    static QList<QSharedPointer<T> > get_all(QString& where){
        QList<QSharedPointer<T> > retval;
        T* pItem = new T();
        IHWFileIO* pIOObject = dynamic_cast<IHWFileIO *>(pItem);
        if (pIOObject == nullptr){
            return retval;
        }

        if (Preferences::Instance()->XHWFilepath() == ""){
            qWarning("no xhw file defined");
            delete pItem;
            return retval;
        }
        {
            QSqlDatabase db = Preferences::Instance()->currentDatabase();
            if (db.isOpen()) {
                qDebug() << "Database opened successfully！";
            } else {
                qDebug("can't open database(%s) because:%s",Preferences::Instance()->XHWFilepath().toStdString().c_str(), db.lastError().text().toStdString().c_str());
                goto End;
            }
            QSqlQuery query(db);
            QString sql = "";
            if (where != "") {
                if (where.endsWith(";"))where = where.left(where.size()-1);
                if (!where.startsWith("where"))sql=QString("select * from %1 where %2;").arg(pIOObject->Tabname()).arg(where);
                else sql=QString("select * from %1 %2;").arg(pIOObject->Tabname()).arg(where);
            } else {
                sql=QString("select * from %1;").arg(pIOObject->Tabname());
            }
            if (sql.endsWith(";;")) sql = sql.left(sql.size()-1);
            qDebug("IHWFileIO::get_all sql=%s", sql.toStdString().c_str());
            query.exec(sql);
            while (query.next()) {
                T *pNewItem = new T();
                IHWFileIO* pNewIOObject = dynamic_cast<IHWFileIO *>(pNewItem);
                pNewIOObject->LoadFromSqlQuery(query);
                retval.append(QSharedPointer<T>(pNewItem));
            }
        }
    End:
        delete pItem;
        return retval;
    }
    template<typename T>
    static T* get_item(const QString& key){
        printf("[%s %s:%d]\n", __FILE__, __FUNCTION__, __LINE__);
        T* pItem = new T();
        IHWFileIO* pIOObject = dynamic_cast<IHWFileIO *>(pItem);
        if (pIOObject == nullptr){
            delete pItem;
            return nullptr;
        }
        printf("[%s %s:%d]\n", __FILE__, __FUNCTION__, __LINE__);
        QString where = QString("where %1='%2'").arg(pIOObject->PrimaryKey()).arg(key);
        if (Preferences::Instance()->XHWFilepath() == ""){
            qWarning("no xhw file defined");
            delete pItem;
            return nullptr;
        }
        {
            QSqlDatabase db = Preferences::Instance()->currentDatabase();
            if (db.isOpen()) {
                qDebug() << "Database opened successfully！";
            } else {
                qDebug("can't open database(%s) because:%s",Preferences::Instance()->XHWFilepath().toStdString().c_str(), db.lastError().text().toStdString().c_str());
                delete pItem;
                return nullptr;
            }
            QSqlQuery query(db);
            QString sql = QString("select * from %1 where %2='%3';").arg(pIOObject->Tabname()).arg(pIOObject->PrimaryKey()).arg(key);
            qDebug("IHWFileIO::get_item sql=%s", sql.toStdString().c_str());
            query.exec(sql);
            if (query.next()) {
                pIOObject->LoadFromSqlQuery(query);
                pItem;
            }
        }
        delete pItem;
        return nullptr;
    }

private:
    static void __init_wingeometry(struct WinGeometry *wg, int l, int t, int w, int h);
    void __init_measurement_units();
    void __loadGeneral(uint32_t& version);
    void __loadWindows(uint32_t& version);
    void __loadPanels(uint32_t& version);
    void __loadFormat(uint32_t& version);
    void __loadFilter(uint32_t& version);
    void __loadAPI(uint32_t& version);
    void __loadEuro(uint32_t& version);
    void __loadReport(uint32_t& version);
    void __loadExchange(uint32_t& version);
    void __loadChart(uint32_t& version);
    void __get_wingeometry(QString strVal,  struct WinGeometry *storage);
    QList<int> __get_integer_list(QString strVal);
    void __currfmt_convert(Currency *cur, QString prefix, QString suffix);
    int __upgrade_560_daterange(int oldrange);
    void __removeOldConnection();

public:
    QSqlTableModel* m_iAccountsModel;
    QSqlTableModel* m_iPayeeModel;
    QSqlTableModel* m_iTransactionModel;
    QString connectionname;
	//general
    bool	showsplash;
    bool	showwelcome;
    bool	loadlast;
    bool	appendscheduled;
    bool	do_update_currency;

	//top spending
    //int		date_range_wal;
    int		rep_maxspenditems;

	//interface
    short		toolbar_style;
    short		grid_lines;
    bool	gtk_override;
    short		gtk_fontsize;
    bool	gtk_darktheme;

    bool	icon_symbolic;
    bool	custom_colors;
    QString color_exp;
    QString color_inc;
    QString color_warn;

	//locale
    QString language;
    QString date_format;
    short		fisc_year_day;
    short		fisc_year_month;
    bool	vehicle_unit_ismile;	// true if unit is mile, default Km
    bool	vehicle_unit_isgal;		// true if unit is gallon, default Liter

	//transactions
	//-- register
    int		date_range_txn;
    int		date_future_nbdays;
    bool	hidereconciled;
    bool    showremind;
    bool    showvoid;
    bool	includeremind;
    bool	lockreconciled;
	//-- dialog
    bool	heritdate;
    bool	txn_memoacp;
    short		txn_xfer_daygap;
    bool	txn_xfer_syncstat;
    short		txn_memoacp_days;
    bool	txn_showconfirm;

	//import/export
    int		dtex_datefmt;
    int		dtex_daygap;
    int		dtex_ofxname;
    int		dtex_ofxmemo;
    bool	dtex_qifmemo;
    bool	dtex_qifswap;
    bool	dtex_ucfirst;
    int		dtex_csvsep;

	//report options
    int		date_range_rep;
    int		report_color_scheme;
    bool	rep_smallfont;
    bool	stat_byamount;
    bool	stat_showrate;
    bool	stat_showdetail;
    bool	stat_includexfer;
    bool	budg_showdetail;
	//5.7
    bool	rep_forcast;
    int		rep_forecat_nbmonth;
		

	//backup option
    bool	bak_is_automatic;
    short		bak_max_num_copies;

	//folders
    QString path_hbfile;
    QString path_hbbak;
    QString path_import;
    QString path_export;
    QString path_attach;

	//currency api
    QString api_rate_url;
    QString api_rate_key;

	//euro zone
    bool	euro_active;
    int		euro_country;
    double		euro_value;
    Currency	*minor_cur;


	//---- others data (not in pref dialog) -----
    bool	dtex_nointro;
    bool	dtex_dodefpayee;
    bool	dtex_doautoassign;

    QString	    IntCurrSymbol;

    int 		lst_impope_columns[NUM_LST_DSPOPE+1];

	//register list column
    int 		lst_ope_columns[NUM_LST_DSPOPE+1];
    int 		lst_ope_col_width[NUM_LST_DSPOPE+1];
    int		lst_ope_sort_id;	// -- implicit --
    int		lst_ope_sort_order; // -- implicit --

	//detail list column
    int 		lst_det_columns[NUM_LST_DSPOPE+1];
    int 		lst_det_col_width[NUM_LST_DSPOPE+1];

	/* windows/dialogs size an position */
	struct WinGeometry	wal_wg;
	struct WinGeometry	acc_wg;
	
	struct WinGeometry	sta_wg;
	struct WinGeometry	tme_wg;
	struct WinGeometry	ove_wg;
	struct WinGeometry	bud_wg;
	struct WinGeometry	cst_wg;

	struct WinGeometry	txn_wg;
	struct WinGeometry	dbud_wg;

	// main window stuffs 
    bool	wal_toolbar;
    bool	wal_totchart;
    bool	wal_timchart;
    bool	wal_upcoming;

    int		wal_vpaned;
    int		wal_hpaned;

	//home panel
    short		pnl_acc_col_acc_width;
    int 		lst_acc_columns[NUM_LST_COL_DSPACC+1];
    short		pnl_acc_show_by;

	//hub total/time
    short		hub_tot_view;
    short		hub_tot_range;
    short		hub_tim_view;
    short		hub_tim_range;

    short		pnl_upc_col_pay_show;
    short		pnl_upc_col_pay_width;
    short		pnl_upc_col_cat_show;
    short		pnl_upc_col_cat_width;
    short		pnl_upc_col_mem_show;
    short		pnl_upc_col_mem_width;
    int		pnl_upc_range;
	
    QString pnl_list_tab;

	//vehiclecost units (mile/gal or km/liters)
	
    QString vehicle_unit_dist0;
    QString vehicle_unit_dist1;
    QString vehicle_unit_vol;
    QString vehicle_unit_100;
    QString vehicle_unit_distbyvol;

private:
    void __NoLink();
    explicit Preferences();
    ~Preferences(){}
    static Preferences* m_iInstance;
    QSqlDatabase m_DB;
};

//int homebank_pref_list_column_get(int *cols_id, int uid, int maxcol);

//void homebank_pref_setdefault_lst_ope_columns(void);
//void homebank_pref_setdefault_lst_det_columns(void);
//void homebank_pref_setdefault_win(void);
//void homebank_prefs_set_default(void);
//void homebank_pref_free(void);
//void homebank_pref_createformat(void);
//void homebank_pref_init_measurement_units(void);

//void homebank_pref_apply(void);
//bool homebank_pref_load(void);
//bool homebank_pref_save(void);
//void homebank_pref_setdefault(void);

#endif
