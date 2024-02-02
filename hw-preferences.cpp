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
void LogHandler(QtMsgType type, const QMessageLogContext &info, const QString &msg)
{
    QString lType;
    switch (type) {
    case QtDebugMsg:
        lType = "D";
        break;
    case QtInfoMsg:
        lType = "I";
        break;
    case QtWarningMsg:
        lType = "W";
        break;
    case QtCriticalMsg:
        lType = "E";
        break;
    case QtFatalMsg:
        lType = "F";
        break;
    default:
        break;
    }
    Preferences::Instance()->outputLog(lType, info.file, info.function, info.line, msg);
}


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

Preferences::Preferences()
    : QSettings("preferences.json" ,QSettings::registerFormat("json", readJsonFile, writeJsonFile))
{
    m_iLogFile = nullptr;
    m_iLogStream = nullptr;
    m_iAccountsModel = nullptr;
    m_iPayeeModel = nullptr;
    m_iTransactionModel = nullptr;
    m_LogPath = "logs";
    build_package_paths();
    // check homedir for .homewallet dir
    check_app_dir();

    sync();
    load();
    if (XHWFilepath() != ""){
        QFileInfo fi(XHWFilepath());
        if (!fi.exists()){
            SetXHWFilepath("");
        } else {
            addConnection("QSQLITE", XHWFilepath(), "","", "", 0);
        }
        currency = Currency::get_system_iso();
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
    if (m_iTransactionModel != nullptr){
        delete m_iTransactionModel;
        m_iTransactionModel = nullptr;
    }
    m_iTransactionModel = new QSqlTableModel(nullptr, db);
    m_iTransactionModel->setTable("t_transactions");

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
    if (m_iTransactionModel != nullptr){
        delete m_iTransactionModel;
        m_iTransactionModel = nullptr;
    }
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

    if(!groupsetting.contains("BackupPath")) groupsetting["BackupPath"] = QDir::homePath();
    if(!groupsetting.contains("ShowWelcome")) groupsetting["ShowWelcome"] = true;
    if(!groupsetting.contains("BakIsAutomatic")) groupsetting["BakIsAutomatic"] = true;
    if(!groupsetting.contains("BakMaxNumCopies")) groupsetting["BakMaxNumCopies"] = 5;
    if(!groupsetting.contains("IncludeRemind")) groupsetting["IncludeRemind"] = false;
}

void Preferences::__loadWindows(uint32_t &version)
{
    if (!contains("Windows")){
        qWarning("no Windows group in preference");
        return;
    }
    QVariantMap groupsetting = value("Windows").toMap();
    qDebug(" -> ** Windows");
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
}

void Preferences::__loadFormat(uint32_t &version)
{
    if (!contains("Format")){
        qWarning("no Format group in preference");
        return;
    }
    QVariantMap groupsetting = value("Format").toMap();
    qDebug(" -> ** Format");
}

void Preferences::__loadFilter(uint32_t &version)
{
    if (!contains("Filter")){
        qWarning("no Filter group in preference");
        return;
    }
    QVariantMap groupsetting = value("Filter").toMap();
    qDebug(" -> ** Filter");
}

void Preferences::__loadAPI(uint32_t &version)
{
    if (!contains("API")){
        qWarning("no API group in preference");
        return;
    }
    QVariantMap groupsetting = value("API").toMap();
    qDebug(" -> ** API");
}

void Preferences::__loadEuro(uint32_t &version)
{

}

void Preferences::__loadReport(uint32_t &version)
{
    if (!contains("Report")){
        qWarning("no Report group in preference");
        return;
    }
    QVariantMap groupsetting = value("Report").toMap();
    qDebug(" -> ** Report");
}

void Preferences::__loadExchange(uint32_t &version)
{
    if (!contains("Exchange")){
        qWarning("no Exchange group in preference");
        return;
    }
    QVariantMap groupsetting = value("Exchange").toMap();
    qDebug(" -> ** Exchange");
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
    QString group;
    uint32_t version = 0;
    bool loaded;
    //GError *error = NULL;

    qDebug("starting......");

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

    return retval;
}


/*
** ensure the filename ends with '.xhb'
*/
void Preferences::file_ensure_xhb(QString filename)
{
    qDebug("\n[homewallet] file_ensure_xhb\n");
    filename = (filename == "") ? XHWFilepath() : filename;

    qDebug(" in filepath: '%s'\n", XHWFilepath().toStdString().c_str());

    if(filename.endsWith(WALLET_FILE_SUFFIX) == false ) {
        QString newfilename = hb_filename_new_with_extension(filename, "xhb");
        SetXHWFilepath((newfilename != "") ? newfilename : "");
    } else {
        SetXHWFilepath((filename != "") ? filename : "");
    }

    qDebug(" out filepath: '%s'\n", XHWFilepath().toStdString().c_str());
}

void Preferences::backup_current_file()
{
    QString bakfilename;
    QStringList array;
    int i;

    qDebug("\n[homewallet] backup_current_file\n");

    //do normal linux backup file
    qDebug(" normal backup with ~\n");
    bakfilename = hb_filename_new_with_extension (XHWFilepath(), "xhb~");
    __file_delete_existing(bakfilename);
    //#512046 copy file not to broke potential links
    //retval = g_rename(pathname, newname);
    __file_copy (XHWFilepath(), bakfilename);

    //do safe backup according to user preferences
    qDebug(" user pref backup\n");
    if( BakIsAutomatic() == true )
    {
        bakfilename = hb_filename_new_for_backup(XHWFilepath());
        if(QFileInfo(bakfilename).exists() == false )
        {
            __file_copy (XHWFilepath(), bakfilename);
        }

        //delete any offscale backup
        qDebug(" clean old backup\n");
        array = hb_filename_backup_list(XHWFilepath());

        qDebug(" found %d match\n", array.size());

        QString dirname = BackupPath();

        for(i=0;i<array.size();i++)
        {
            QString offscalefilename = array[i];

            qDebug(" %d : '%s'\n", i, offscalefilename.toStdString().c_str());
            if( i >= BakMaxNumCopies() )
            {
                QString bakdelfilepath =  QDir(dirname).absoluteFilePath(offscalefilename);
                qDebug(" - should delete '%s'\n", bakdelfilepath.toStdString().c_str());

                __file_delete_existing(bakdelfilepath);

            }
        }
    }
}

bool Preferences::__file_delete_existing(const QString filepath)
{
    bool retval = false;

    qDebug("\n[homewallet] file delete existing\n");

    QFileInfo fi(filepath);
    if(fi.exists())
    {
        qDebug(" - deleting: '%s'\n", filepath.toStdString().c_str());
        QFile::remove(filepath);
        retval = true;
    }
    else
    {
        qDebug(" - cannot delete: '%s'\n", filepath.toStdString().c_str());
    }

    return retval;
}

bool Preferences::__file_copy(const QString srcfile, const QString dstfile)
{
    //GError *error = NULL;
    bool retval = false;

    qDebug("\n[homewallet] file copy\n");

    if (QFile::copy(srcfile, dstfile))
    {
        retval = true;
    }

    qDebug(" - copied '%s' => '%s' :: %d\n", srcfile.toStdString().c_str(), dstfile.toStdString().c_str(), retval);
    return retval;
}

void Preferences::outputLog(const QString &type, const char* file, const char* func, int line, const QString &msg)
{
    QMutexLocker locker(&m_mutex);
    QDateTime now = QDateTime::currentDateTime();
    if (m_LastLogTime.daysTo(now) > 0 || nullptr == m_iLogFile){
        if (m_iLogStream != nullptr){
            m_iLogStream->flush();
            delete m_iLogStream;
            m_iLogStream = nullptr;
        }
        if (m_iLogFile != nullptr){
            m_iLogFile->flush();
            m_iLogFile->close();
            delete m_iLogFile;
            m_iLogFile = nullptr;
        }
        QDir dir;
        if(!dir.exists(m_LogPath)) dir.mkpath(m_LogPath);
        QString name = QString("%1/%2.txt").arg(m_LogPath).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));

        m_iLogFile = new QFile(name);
        if(!m_iLogFile->open(QIODevice::WriteOnly | QIODevice::Append)){
            delete m_iLogFile;
            m_iLogFile = nullptr;
            return;
        }
        m_iLogStream = new QTextStream(m_iLogFile);
        m_iLogStream->setCodec("utf-8");
    }
    QString time = now.toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString str = QString("[%1][%2 %3:%5][%4] %6\n")
                      .arg(type).arg(file).arg(func).arg(time).arg(line).arg(msg);

    *m_iLogStream << str.toUtf8();
    printf("%s",str.toUtf8().toStdString().c_str());
}

Preferences::~Preferences()
{
    if (m_iLogStream != nullptr){
        m_iLogStream->flush();
        delete m_iLogStream;
        m_iLogStream = nullptr;
    }
    if (m_iLogFile != nullptr){
        m_iLogFile->flush();
        m_iLogFile->close();
        delete m_iLogFile;
        m_iLogFile = nullptr;
    }
}

Preferences* Preferences::m_iInstance = nullptr;


AppDir::AppDir()
{

}

AppDir::~AppDir()
{

}

/* build package paths at runtime */
void AppDir::build_package_paths ()
{
    qDebug("starting......");

    QString homedir = QDir::homePath();
#ifdef Q_OS_WIN
    QString prefix = QCoreApplication::applicationDirPath();
    locale_dir   = prefix+"/"+ "share"+"/"+ "locale";
    images_dir   = prefix+"/"+ "share"+"/"+ PACKAGE+"/"+ "images";
    pixmaps_dir  = prefix+"/"+ "share"+"/"+ PACKAGE+"/"+ "icons";
    help_dir     = prefix+"/"+ "share"+"/"+ PACKAGE+"/"+ "help";
    datas_dir    = prefix+"/"+ "share"+"/"+ PACKAGE+"/"+ "datas";
    #ifdef PORTABLE_APP
        qDebug(" - app is portable under windows");
        config_dir   = prefix+"/"+ "config";
    #else
        config_dir   = QDir(homedir).absoluteFilePath(QString(HB_DATA_PATH));
    #endif
#else
    locale_dir   = DATA_DIR+"/"+ "locale";
    images_dir   = SHARE_DIR+"/"+ "images";
    pixmaps_dir  = DATA_DIR+"/"+ PACKAGE+"/"+  "icons;
    help_dir     = DATA_DIR+"/"+ PACKAGE+"/"+  "help";
    datas_dir    = DATA_DIR+"/"+ PACKAGE+"/"+  "datas";
    config_dir   = g_get_user_config_dir(), HB_DATA_PATH;
#endif

    qDebug(" - config_dir : %s\n", config_dir.toStdString().c_str());
    qDebug(" - images_dir : %s\n", images_dir.toStdString().c_str());
    qDebug(" - pixmaps_dir: %s\n", pixmaps_dir.toStdString().c_str());
    qDebug(" - locale_dir : %s\n", locale_dir.toStdString().c_str());
    qDebug(" - help_dir   : %s\n", help_dir.toStdString().c_str());
    qDebug(" - datas_dir  : %s\n", datas_dir.toStdString().c_str());

}

/*
 * check/create user home directory for .homewallet (HB_DATA_PATH) directory
 */
void AppDir::check_app_dir()
{
    QString homedir;
    QString configdir;
    QDir dir;

    qDebug("\n[homewallet] check_app_dir\n");
    /* check if <userdir>/.config exist */

#ifndef Q_OS_WIN
    configdir = QDir::homePath();
    qDebug(" - check '%s' exists\n", configdir.toStdString().c_str());
    if(!dir.exists(configdir))
    {
        qDebug(" - creating dir");
        dir.mkpath(configdir);
    }
#endif

    /* check for XDG .config/homewallet */
    configdir = app_get_config_dir();
    qDebug(" - config_dir is: '%s'\n", configdir.toStdString().c_str());
    if(dir.exists(configdir))
    {
        /* just update folder security */
        qDebug(" - chmod 0700\n");
        qDebug(" - chmod 0700 finished\n");
    }
    else
    {
        /* create the config dir */
        qDebug(" - create config_dir\n");
        dir.mkpath(configdir);

        /* any old homedir configuration out there ? */
        homedir = QDir::homePath() + "/.homewallet";
        qDebug(" - homedir is: '%s'\n", homedir.toStdString().c_str());
    }
}

