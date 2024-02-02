#ifndef __HOMEWALLET_H__
#define __HOMEWALLET_H__

#include <QObject>
#include <QSharedPointer>
#include <QObject>
#include <QString>
#include <QMap>
#include <QWidget>
#include <QSet>
#include <QList>
#include <QDir>
#include <QFile>
#include <QPair>
#include <errno.h>
#include <math.h>		/* floor */
//#include <libintl.h>	/* gettext */
#include <locale.h>
#include <stdlib.h>		/* atoi, atof, atol */
#include <string.h>		/* memset, memcpy, strcmp, strcpy */
//#include <time.h>
#include "enums.h"
#include "hw-preferences.h"
#include "hw-category.h"
#include "hw-account.h"
#include "hw-archive.h"
#include "hw-transaction.h"
#include "hw-payee.h"
#include "hw-assign.h"
#include "hw-filter.h"
#include "hw-misc.h"
#include "hw-group.h"
#include "hw-tag.h"


#define HBTK_IS_SEPARATOR -66


typedef enum {
    DATE_RANGE_BUDGET_MODE    = 1 << 1,
    DATE_RANGE_CUSTOM_HIDDEN  = 1 << 8,
    DATE_RANGE_CUSTOM_DISABLE = 1 << 9
} HbDateRangeFlags;



#define HB_UNSTABLE			FALSE
#define HB_UNSTABLE_SHOW	FALSE


#define HOMEWALLET_MAJOR	0
#define HOMEWALLET_MINOR	0
#define HOMEWALLET_MICRO	1

#define HW_VERSION		"0.0.1"
#define HW_VERSION_NUM	(HOMEWALLET_MAJOR*10000) + (HOMEWALLET_MINOR*100) + HOMEWALLET_MICRO

#define FILE_VERSION		1.4
#define PREF_VERSION		572

#if HB_UNSTABLE == FALSE
    #define	PROGNAME		"homewallet"
    #define HB_DATA_PATH	"homewallet"
#else
    #define	PROGNAME		"homewallet " HW_VERSION " (unstable)"
    #define HB_DATA_PATH	"homewallet_unstable"
#endif


#ifdef Q_OS_WIN
    #define GETTEXT_PACKAGE "homewallet"
    #define LOCALE_DIR      "locale"
    #define PIXMAPS_DIR     "images"
    #define HELP_DIR        "help"
    #define PACKAGE_VERSION HW_VERSION
    #define PACKAGE         "homewallet"
    #define VERSION         HW_VERSION

    //#define PORTABLE_APP
    //#define NOOFX

    #define ENABLE_NLS 1
#endif


/* container spacing */
#define SPACING_TINY		3
#define SPACING_SMALL		6
#define SPACING_MEDIUM		12
#define SPACING_LARGE		18
#define SPACING_POPOVER		10


#define HB_DATE_MAX_GAP	7

// those 2 line are duplicated into dateentry
#define HB_MINDATE  693596	  //01/01/1900
#define HB_MAXDATE  803533	  //31/12/2200

/* widget minimum width */
#define HB_MINWIDTH_LIST	161
#define HB_MINHEIGHT_LIST	260

#define HB_MINWIDTH_SEARCH	240
#define HB_MINWIDTH_COLUMN  48


/* miscellaneous */
#define PHI 1.61803399



#define HB_NUMBER_SAMPLE	1234567.89

/* hbfile/account/import update flags */
enum
{
    UF_TITLE     	= 1 << 0,	//1
    UF_SENSITIVE 	= 1 << 1,	//2
    UF_VISUAL   	= 1 << 2,	//4
    UF_REFRESHALL   = 1 << 3,	//8
//			= 1 << 4	//16
};



typedef enum
{
    FILETYPE_UNKNOWN,
    FILETYPE_HOMEWALLET,
    FILETYPE_OFX,
    FILETYPE_QIF,
    FILETYPE_CSV_HB,
//	FILETYPE_AMIGA_HB,
    NUM_FILETYPE
} HbFileType;


/* ---- icon size as defined into gtkiconfactory.c ---- */
/* GTK_ICON_SIZE_MENU 16
 * GTK_ICON_SIZE_BUTTON 20
 * GTK_ICON_SIZE_SMALL_TOOLBAR 18
 * GTK_ICON_SIZE_LARGE_TOOLBAR 24 (default for toolbar)
 * GTK_ICON_SIZE_DND 32
 * GTK_ICON_SIZE_DIALOG 48
 */


/* -------- named icons (Standard Icon Name) -------- */


//obsolete, as since since gtk3.10 : no more icons for dialogs and menu
#define ICONNAME_SAVE_AS			"document-save-as"	  //obsolete
#define ICONNAME_REVERT		    "document-revert"	  //obsolete
#define ICONNAME_PROPERTIES			"document-properties"   //obsolete
#define ICONNAME_CLOSE				"window-close"	  //obsolete
#define ICONNAME_QUIT				"application-exit"	  //obsolete
#define ICONNAME_HELP				"help-browser"	  //obsolete
#define ICONNAME_ABOUT				"help-about"	  //obsolete
#define ICONNAME_PREFERENCES		"preferences-system"	  //obsolete


//#define ICONNAME_FIND				"edit-find"				//unused
//#define ICONNAME_CLEAR			"edit-clear"			//unused
//#define ICONNAME_HB_SCHED_SKIP		"media-skip-forward"
//#define ICONNAME_HB_SCHED_POST		"media-playback-start"

//in 5.2 no themeable icon to keep a consistent iconset

#define ICONNAME_WARNING			"dialog-warning"
#define ICONNAME_ERROR				"dialog-error"
#define ICONNAME_INFO				"dialog-information"


#define ICONNAME_FOLDER				"folder-symbolic"
#define ICONNAME_EMBLEM_OK			"emblem-ok-symbolic"
#define ICONNAME_EMBLEM_SYSTEM		"emblem-system-symbolic"
#define ICONNAME_WINDOW_CLOSE		"window-close-symbolic"
#define ICONNAME_LIST_ADD			"list-add-symbolic"
#define ICONNAME_LIST_EDIT			"document-edit-symbolic"
#define ICONNAME_LIST_DUPLICATE		"list-duplicate-symbolic"
#define ICONNAME_LIST_DELETE		"list-remove-symbolic"
#define ICONNAME_LIST_DELETE_ALL	"list-remove-all-symbolic"
#define ICONNAME_LIST_MOVE_UP		"hb-go-up-symbolic"
#define ICONNAME_LIST_MOVE_DOWN		"hb-go-down-symbolic"
#define ICONNAME_LIST_MOVE_AFTER	"list-move-after-symbolic"
#define ICONNAME_CHANGES_PREVENT	"changes-prevent-symbolic"
#define ICONNAME_CHANGES_ALLOW  	"changes-allow-symbolic"
#define ICONNAME_SYSTEM_SEARCH		"system-search-symbolic"


// custom or gnome not found
#define ICONNAME_HB_BUTTON_MENU		"open-menu-symbolic"
#define ICONNAME_HB_BUTTON_BROWSER	"open-in-browser-symbolic"
#define ICONNAME_HB_BUTTON_COLLAPSE	"list-collapse-all-symbolic"
#define ICONNAME_HB_BUTTON_EXPAND	"list-expand-all-symbolic"
#define ICONNAME_HB_BUTTON_SPLIT	"edit-split-symbolic"
#define ICONNAME_HB_BUTTON_DELETE	"edit-delete-symbolic"
#define ICONNAME_HB_TOGGLE_SIGN		"toggle-sign-symbolic"
#define ICONNAME_HB_LIST_MERGE		"list-merge-symbolic"
#define ICONNAME_HB_BUTTON_HIDE		"eye-not-looking-symbolic"
#define ICONNAME_HB_BUTTON_USAGE	"data-usage-symbolic"

#define ICONNAME_HB_TEXT_CASE		"text-casesensitive-symbolic"
#define ICONNAME_HB_TEXT_REGEX		"text-regularexpression-symbolic"


/* -------- named icons (Custom to homewallet) -------- */


#define ICONNAME_HB_CURRENCY		"hb-currency"
#define ICONNAME_HB_ACCOUNT         "hb-account"
#define ICONNAME_HB_ARCHIVE         "hb-archive"
#define ICONNAME_HB_ASSIGN          "hb-assign"
#define ICONNAME_HB_BUDGET          "hb-budget"
#define ICONNAME_HB_CATEGORY        "hb-category"
#define ICONNAME_HB_PAYEE           "hb-payee"
#define ICONNAME_HB_OPE_SHOW        "hb-ope-show"   //? "view-register
#define ICONNAME_HB_REP_STATS       "hb-rep-stats"
#define ICONNAME_HB_REP_TIME        "hb-rep-time"
#define ICONNAME_HB_REP_BALANCE     "hb-rep-balance"
#define ICONNAME_HB_REP_BUDGET      "hb-rep-budget"
#define ICONNAME_HB_REP_CAR         "hb-rep-vehicle"
#define ICONNAME_HB_HELP            "hb-help"
#define ICONNAME_HB_DONATE          "hb-donate"

#define ICONNAME_HB_VIEW_LIST	    "hb-view-list"   //"view-list-text"
#define ICONNAME_HB_VIEW_BAR	    "hb-view-bar"    //"view-chart-bar"
#define ICONNAME_HB_VIEW_COLUMN	    "hb-view-column" //"view-chart-column"
#define ICONNAME_HB_VIEW_LINE	    "hb-view-line"   //"view-chart-line"
#define ICONNAME_HB_VIEW_PROGRESS	"hb-view-progress"  //"view-chart-progress"
#define ICONNAME_HB_VIEW_PIE	    "hb-view-pie"    //"view-chart-pie"
#define ICONNAME_HB_VIEW_DONUT	    "hb-view-donut"  //"view-chart-donut"
#define ICONNAME_HB_VIEW_STACK	    "hb-view-stack"  //"view-chart-stack"
#define ICONNAME_HB_VIEW_STACK100   "hb-view-stack100"  //"view-chart-stack100"
#define ICONNAME_HB_SHOW_LEGEND	    "hb-legend"		//"view-legend"
#define ICONNAME_HB_SHOW_RATE	    "hb-rate"	    	// obsolete ?
#define ICONNAME_HB_REFRESH		    "hb-view-refresh"	//"view-refresh"
#define ICONNAME_HB_FILTER		    "hb-filter"		//"edit-filter"

#define ICONNAME_HB_FILE_NEW		"hb-document-new"		//document-new
#define ICONNAME_HB_FILE_OPEN		"hb-document-open"	//document-open
#define ICONNAME_HB_FILE_SAVE		"hb-document-save"	//document-save
#define ICONNAME_HB_FILE_IMPORT		"hb-file-import"		//document-import
#define ICONNAME_HB_FILE_EXPORT		"hb-file-export"		//document-export
#define ICONNAME_HB_FILE_VALID		"hb-file-valid"
#define ICONNAME_HB_FILE_INVALID	"hb-file-invalid"

#define ICONNAME_HB_PRINT			"hb-document-print"

#define ICONNAME_HB_OPE_AUTO        "hb-ope-auto"   //?
#define ICONNAME_HB_OPE_BUDGET      "hb-ope-budget" //?
#define ICONNAME_HB_OPE_FORCED      "hb-ope-forced" //?
#define ICONNAME_HB_OPE_ADD         "hb-ope-add"	//? "edit-add"
#define ICONNAME_HB_OPE_HERIT       "hb-ope-herit"  //? "edit-clone"
#define ICONNAME_HB_OPE_EDIT        "hb-ope-edit"   //
#define ICONNAME_HB_OPE_MULTIEDIT   "hb-ope-multiedit"   //
#define ICONNAME_HB_OPE_DELETE      "hb-ope-delete" //? "edit-delete"
#define ICONNAME_CONVERT			"hb-ope-convert"	// obsolete ?
#define ICONNAME_HB_ASSIGN_RUN      "hb-assign-run"
#define ICONNAME_HB_OPE_MOVUP		"hb-go-up"
#define ICONNAME_HB_OPE_MOVDW		"hb-go-down"

#define ICONNAME_HB_OPE_NEW		    "hb-ope-new"
#define ICONNAME_HB_OPE_PREFILLED   "hb-ope-prefilled"
// edit is defined above
#define ICONNAME_HB_OPE_VOID        "hb-ope-void"
#define ICONNAME_HB_OPE_REMIND      "hb-ope-remind"
#define ICONNAME_HB_OPE_SIMILAR     "hb-ope-similar"

#define ICONNAME_HB_OPE_CLEARED     "hb-ope-cleared"
#define ICONNAME_HB_OPE_RECONCILED  "hb-ope-reconciled"
#define ICONNAME_HB_OPE_FUTURE      "hb-ope-future"

#define ICONNAME_HB_PM_INTXFER		"pm-intransfer"

#define WALLET_FILE_SUFFIX ".xhw"

typedef enum
{
    REPORT_SRC_NONE,
    REPORT_SRC_CATEGORY,
    //REPORT_SRC_SUBCATEGORY,
    REPORT_SRC_PAYEE,
    REPORT_SRC_ACCOUNT,
    REPORT_SRC_TAG,
    REPORT_SRC_MONTH,
    REPORT_SRC_YEAR,
} HbReportSrc;


typedef enum {
    REPORT_TYPE_EXPENSE,
    REPORT_TYPE_INCOME,
    REPORT_TYPE_TOTAL
} HbReportType;


typedef enum
{
    REPORT_INTVL_NONE,
    REPORT_INTVL_DAY,
    REPORT_INTVL_WEEK,
    REPORT_INTVL_FORTNIGHT,
    REPORT_INTVL_MONTH,
    REPORT_INTVL_QUARTER,
    REPORT_INTVL_HALFYEAR,
    REPORT_INTVL_YEAR,
} HbReportIntvl;


typedef enum
{
    REPORT_RESULT_TOTAL,
    REPORT_RESULT_CUMUL,
    REPORT_RESULT_BALANCE,
} HbReportResult;



class HomeWallet : public QObject
{
    Q_OBJECT

public:
    static HomeWallet* Instance(){
        if (m_iInstance == nullptr){
            m_iInstance = new HomeWallet();
        }
        return m_iInstance;
    }
    bool setup();

private:
    HomeWallet(QObject *parent = nullptr);
    ~HomeWallet();
    bool __check_app_dir_migrate_file(QString srcdir, QString dstdir, QString filename);

public:

public:
    static QString CHART_CATEGORY;
    static QStringList CYA_ASG_FIELD;
    static QStringList CYA_CAT_TYPE;
    static QStringList CYA_CATSUBCAT;
    static QStringList CYA_REPORT_MODE;
    static QMap<int, QString> CYA_REPORT_SRC;
    static QMap<int, QString> CYA_REPORT_TYPE;
    static QMap<int, QString> CYA_REPORT_SRC_TREND;
    static QMap<int, QString> CYA_REPORT_INTVL;
    static QMap<int, QString> CYA_ACC_TYPE;
    static QMap<int, QString> CYA_FLT_RANGE_DWF;
    static QMap<int, QString> CYA_FLT_RANGE_MQY;
    static QMap<int, QString> CYA_FLT_RANGE_NORMAL;
    static QMap<int, QString> CYA_FLT_RANGE_BUDGET;
    static QMap<int, QString> CYA_FLT_RANGE_CUSTOM;
    static QMap<int, QString> CYA_FLT_SCHEDULED;
    static QMap<int, QString> CYA_TXN_STATUSIMG;
    static QMap<int, QString> CYA_TXN_STATUS;
    static QMap<int, QString> CYA_TXN_IMG;
    static QStringList RA_REPORT_TIME_MODE;
    static QStringList RA_FILTER_MODE;
    static QStringList CYA_ARC_UNIT;
    static QStringList RA_ARC_WEEKEND;
    static QStringList CYA_KIND;
    static QStringList CYA_FLT_TYPE;
    static QStringList CYA_FLT_STATUS;
//    static QStringList OLD_CYA_FLT_RANGE;
    static QStringList CYA_WEEKDAY;
    static QStringList CYA_MONTHS;
    static QStringList CYA_ABMONTHS;
    static QMap<int, QString> CYA_TXN_PAYMODE_ICONS;
    static QMap<int, QString> CYA_TXN_PAYMODE_NAMES;
    static QMap<int, QString> CYA_FLT_RANGE;

private:
    static HomeWallet* m_iInstance;
    bool		first_run;
};



#endif // HOMEWALLET_H
