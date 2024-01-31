#include "home_wallet.h"


/* = = = = = = = = = = = = = = = = = = = = */

QString HomeWallet::CHART_CATEGORY = "Category";

QStringList HomeWallet::CYA_ASG_FIELD = {
    "Memo",
    "Payee",
};


QStringList HomeWallet::CYA_CAT_TYPE = {
    "Expense",
    "Income",
};


QStringList HomeWallet::CYA_CATSUBCAT = {
    "Category",
    "Subcategory",
};


QStringList HomeWallet::CYA_REPORT_MODE =
{
    "Total",
    "Time",
};


QMap<int, QString> HomeWallet::CYA_REPORT_SRC = {
    { REPORT_SRC_CATEGORY,		"Category" },
    //{ REPORT_SRC_SUBCATEGORY,	"Subcategory" },
    { REPORT_SRC_PAYEE,			"Payee" },
    { REPORT_SRC_ACCOUNT,		"Account" },
    { REPORT_SRC_TAG,			"Tag" },
    { REPORT_SRC_MONTH,			"Month" },
    { REPORT_SRC_YEAR,			"Year" },
};


QMap<int, QString> HomeWallet::CYA_REPORT_TYPE = {
    { REPORT_TYPE_EXPENSE,	"Expense" },
    { REPORT_TYPE_INCOME,	"Income" },
    { REPORT_TYPE_TOTAL,	"Total"} ,
};


QMap<int, QString> HomeWallet::CYA_REPORT_SRC_TREND = {
    { REPORT_SRC_ACCOUNT, 	"Account" },
    { REPORT_SRC_CATEGORY,	"Category" },
    { REPORT_SRC_PAYEE,		"Payee" },
    { REPORT_SRC_TAG,		"Tag" },
};


QMap<int, QString> HomeWallet::CYA_REPORT_INTVL = {
    { REPORT_INTVL_DAY,		  "Day" },
    { REPORT_INTVL_WEEK,	  "Week" },
    { REPORT_INTVL_FORTNIGHT, "Fortnight" },
    { REPORT_INTVL_MONTH,	  "Month" },
    { REPORT_INTVL_QUARTER,	  "Quarter" },
    { REPORT_INTVL_HALFYEAR,  "Half Year" },
    { REPORT_INTVL_YEAR,	  "Year" },
};


//soon obsolete for merged trendtime
QStringList HomeWallet::RA_REPORT_TIME_MODE =  {
    "Total",
    "Trend",
    "Balance",
};


QStringList HomeWallet::RA_FILTER_MODE = 
{
    "Include",
    "Exclude",
};


/* = = = = = = = = = = = = = = = = = = = = */

QMap<int, QString> HomeWallet::CYA_TXN_PAYMODE_ICONS =
{
    { PAYMODE_NONE,			"://img/hicolor_status_16x16_hb-pm-none.png" },
    { PAYMODE_CCARD,		"://img/hicolor_status_16x16_hb-pm-ccard.png" },
    { PAYMODE_CHECK,		"://img/hicolor_status_16x16_hb-pm-check.png" },
    { PAYMODE_CASH,			"://img/hicolor_status_16x16_hb-pm-cash.png" },
    { PAYMODE_XFER,			"://img/hicolor_status_16x16_hb-pm-transfer.png" },
    { PAYMODE_DCARD,		"://img/hicolor_status_16x16_hb-pm-dcard.png" },
    { PAYMODE_REPEATPMT,	"://img/hicolor_status_16x16_hb-pm-standingorder.png" },
    { PAYMODE_EPAYMENT,		"://img/hicolor_status_16x16_hb-pm-epayment.png" },
    { PAYMODE_DEPOSIT,		"://img/hicolor_status_16x16_hb-pm-deposit.png" },
    //TRANSLATORS: Financial institution fee
    { PAYMODE_FEE,			"://img/hicolor_status_16x16_hb-pm-fifee.png" },
    { PAYMODE_DIRECTDEBIT,	"://img/hicolor_status_16x16_hb-pm-directdebit.png" },
};

QMap<int, QString> HomeWallet::CYA_TXN_PAYMODE_NAMES =
{
    { PAYMODE_NONE,			"(none)" },
    { PAYMODE_CCARD,		"Credit card" },
    { PAYMODE_CHECK,		"Check" },
    { PAYMODE_CASH,			"Cash" },
    { PAYMODE_XFER,			"Bank Transfer" },
    { PAYMODE_DCARD,		"Debit card" },
    { PAYMODE_REPEATPMT,	"Standing order" },
    { PAYMODE_EPAYMENT,		"Electronic payment" },
    { PAYMODE_DEPOSIT,		"Deposit" },
    //TRANSLATORS: Financial institution fee
    { PAYMODE_FEE,			"FI fee" },
    { PAYMODE_DIRECTDEBIT,	"Direct Debit" },
};

QMap<int, QString> HomeWallet::CYA_ACC_TYPE =
{
    { ACC_TYPE_NONE,		"(no type)" },
    { ACC_TYPE_BANK,		"Bank"	},
    { ACC_TYPE_CASH,		"Cash"	},
    { ACC_TYPE_ASSET,		"Asset"	},
    { ACC_TYPE_CREDITCARD,	"Credit card" },
    { ACC_TYPE_LIABILITY,	"Liability" },
    { ACC_TYPE_CHECKING, 	"Checking" },
    { ACC_TYPE_SAVINGS, 	"Savings" },

//	{ ACC_TYPE_MUTUALFUND, 	"Mutual Fund" },
//	{ ACC_TYPE_INCOME, 		"Income" },
//	{ ACC_TYPE_EXPENSE, 	"Expense" },
//	{ ACC_TYPE_EQUITY, 		"Equity" },

};

QMap<int, QString> HomeWallet::CYA_FLT_RANGE = {
    {  FLT_RANGE_LAST_DAY       , "Yesterday" },
    {  FLT_RANGE_THIS_DAY       , "Today" },
    {  FLT_RANGE_NEXT_DAY       , "Tomorrow" },

    {  FLT_RANGE_LAST_WEEK      , "Last Week" },
    {  FLT_RANGE_THIS_WEEK      , "This Week" },
    {  FLT_RANGE_NEXT_WEEK      , "Next Week" },

    {  FLT_RANGE_LAST_FORTNIGHT , "Last Fortnight" },
    {  FLT_RANGE_THIS_FORTNIGHT , "This Fortnight" },
    {  FLT_RANGE_NEXT_FORTNIGHT , "Next Fortnight" },

};


QMap<int, QString> HomeWallet::CYA_FLT_RANGE_DWF = {
    {  FLT_RANGE_LAST_DAY       , "Yesterday" },
    {  FLT_RANGE_THIS_DAY       , "Today" },
    {  FLT_RANGE_NEXT_DAY       , "Tomorrow" },

    {  FLT_RANGE_LAST_WEEK      , "Last Week" },
    {  FLT_RANGE_THIS_WEEK      , "This Week" },
    {  FLT_RANGE_NEXT_WEEK      , "Next Week" },

    {  FLT_RANGE_LAST_FORTNIGHT , "Last Fortnight" },
    {  FLT_RANGE_THIS_FORTNIGHT , "This Fortnight" },
    {  FLT_RANGE_NEXT_FORTNIGHT , "Next Fortnight" },

    {  FLT_RANGE_LAST_MONTH     , "Last Month" },
    {  FLT_RANGE_THIS_MONTH     , "This Month" },
    {  FLT_RANGE_NEXT_MONTH     , "Next Month" },

    {  FLT_RANGE_LAST_QUARTER   , "Last Quarter" },
    {  FLT_RANGE_THIS_QUARTER   , "This Quarter" },
    {  FLT_RANGE_NEXT_QUARTER   , "Next Quarter" },

    {  FLT_RANGE_LAST_YEAR      , "Last Year" },
    {  FLT_RANGE_THIS_YEAR      , "This Year" },
    {  FLT_RANGE_NEXT_YEAR      , "Next Year" },

    {  FLT_RANGE_LAST_30DAYS    , "Last 30 Days" },
    {  FLT_RANGE_LAST_60DAYS    , "Last 60 Days" },
    {  FLT_RANGE_LAST_90DAYS    , "Last 90 Days" },

    {  FLT_RANGE_LAST_12MONTHS  , "Last 12 Months" },
    {  FLT_RANGE_MISC_30DAYS    , "30 Days Around" },
    {  FLT_RANGE_MISC_ALLDATE   , "All Date" },
};


QMap<int, QString> HomeWallet::CYA_FLT_RANGE_MQY = {
    {  FLT_RANGE_LAST_MONTH     , "Last Month" },
    {  FLT_RANGE_THIS_MONTH     , "This Month" },
    {  FLT_RANGE_NEXT_MONTH     , "Next Month" },

    {  FLT_RANGE_LAST_QUARTER   , "Last Quarter" },
    {  FLT_RANGE_THIS_QUARTER   , "This Quarter" },
    {  FLT_RANGE_NEXT_QUARTER   , "Next Quarter" },

    {  FLT_RANGE_LAST_YEAR      , "Last Year" },
    {  FLT_RANGE_THIS_YEAR      , "This Year" },
    {  FLT_RANGE_NEXT_YEAR      , "Next Year" },

	{  HBTK_IS_SEPARATOR, "" },
};


QMap<int, QString> HomeWallet::CYA_FLT_RANGE_NORMAL = {
    {  FLT_RANGE_LAST_30DAYS    , "Last 30 Days" },
    {  FLT_RANGE_LAST_60DAYS    , "Last 60 Days" },
    {  FLT_RANGE_LAST_90DAYS    , "Last 90 Days" },

    {  FLT_RANGE_LAST_12MONTHS  , "Last 12 Months" },
    {  FLT_RANGE_MISC_30DAYS    , "30 Days Around" },
    {  FLT_RANGE_MISC_ALLDATE   , "All Date" },
};

QMap<int, QString> HomeWallet::CYA_FLT_RANGE_BUDGET = {
    {  FLT_RANGE_LAST_12MONTHS  , "Last 12 Months" },
    {  FLT_RANGE_LAST_6MONTHS   , "Last 6 Months" },
    {  FLT_RANGE_MISC_ALLDATE   , "All Date" },
};


QMap<int, QString> HomeWallet::CYA_FLT_RANGE_CUSTOM = {
	//5.7 added back
    {  FLT_RANGE_MISC_CUSTOM   , "Custom" },
};


QMap<int, QString> HomeWallet::CYA_FLT_SCHEDULED = {
    { FLT_SCHEDULED_THISMONTH,		"This month" },
    { FLT_SCHEDULED_NEXTMONTH,		"Next month" },
	{ HBTK_IS_SEPARATOR, "" },
    { FLT_SCHEDULED_NEXT30DAYS,		"Next 30 days" },
    { FLT_SCHEDULED_NEXT60DAYS,		"Next 60 days" },
    { FLT_SCHEDULED_NEXT90DAYS,		"Next 90 days" },
	{ HBTK_IS_SEPARATOR, "" },
    { FLT_SCHEDULED_MAXPOSTDATE,	"Maximum Post Date" },
	{ HBTK_IS_SEPARATOR, "" },
    { FLT_SCHEDULED_ALLDATE,		"All" },
};


/*QMap<int, QString> CYA_TXN_STATUS =  
{
    { TXN_STATUS_NONE,			"None") },
    { TXN_STATUS_CLEARED, 		"Cleared") },
    { TXN_STATUS_RECONCILED, 	"Reconciled") },
    { TXN_STATUS_REMIND, 		"Remind") },
    { TXN_STATUS_VOID, 			"Void") },
	{ 0, NULL }
};*/


//this is a test
QMap<int,  QString > HomeWallet::CYA_TXN_STATUS =
{
    { TXN_STATUS_NONE,			"None" },
    { TXN_STATUS_CLEARED, 		"Cleared" },
    { TXN_STATUS_RECONCILED, 	"Reconciled" },
    { TXN_STATUS_REMIND, 		"Remind" },
    { TXN_STATUS_VOID, 			"Void" },
};
QMap<int, QString > HomeWallet::CYA_TXN_IMG =
{
    { TXN_STATUS_NONE,			"" },
    { TXN_STATUS_CLEARED, 		ICONNAME_HB_OPE_CLEARED},
    { TXN_STATUS_RECONCILED, 	ICONNAME_HB_OPE_RECONCILED },
    { TXN_STATUS_REMIND, 		ICONNAME_HB_OPE_REMIND },
    { TXN_STATUS_VOID, 			ICONNAME_HB_OPE_VOID},
};



QStringList HomeWallet::CYA_ARC_UNIT =  {
    "Day",
    "Week",
    "Month",
    "Year",
};


QStringList HomeWallet::RA_ARC_WEEKEND =  {
    "Possible",
    "Before",
    "After",
	//added 5.6
    "Skip",
};


QStringList HomeWallet::CYA_KIND =  {
    "Exp. & Inc.",
    "Expense",
    "Income",
};


QStringList HomeWallet::CYA_FLT_TYPE =  {
    "Any Type",
	"",
    "Expense",
    "Income",
    "Transfer",
};


QStringList HomeWallet::CYA_FLT_STATUS =  {
    "Any Status",
	"",
    "Cleared",
    "Uncleared",
    "Reconciled",
    "Unreconciled",
	"",
    "Uncategorized",
};


/*QStringList HomeWallet::OLD_CYA_FLT_RANGE =  {
    "This month",
    "Last month",
    "This quarter",
    "Last quarter",
    "This year",
    "Last year",
	"",
    "Last 30 days",
    "Last 60 days",
    "Last 90 days",
    "Last 12 months",
	"",
    "Other...",
	"",
    "All date",
};*/


QStringList HomeWallet::CYA_WEEKDAY = 
{
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat",
    "Sun",
};


/* = = = = = = = = = = = = = = = = = = = = */


//in prefs.c only
// in repbudget
//widget
QStringList HomeWallet::CYA_MONTHS = 
{
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December",
};


//hb_report.c rep_time.c ui_budget
QStringList HomeWallet::CYA_ABMONTHS = 
{
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec",
};


