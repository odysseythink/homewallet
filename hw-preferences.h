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

class AppDir {
public:
    AppDir();
    ~AppDir();
    void build_package_paths ();
    void check_app_dir();
    QString app_get_config_dir(){return config_dir;}
    QString app_get_images_dir(){return images_dir;}
    QString app_get_pixmaps_dir(){return pixmaps_dir;}
    QString app_get_locale_dir(){return locale_dir;}
    QString app_get_help_dir(){return help_dir;}
    QString app_get_datas_dir(){return datas_dir;}

private:
    QString config_dir ;
    QString images_dir ;
    QString pixmaps_dir;
    QString locale_dir ;
    QString help_dir   ;
    QString datas_dir  ;
};


class Preferences : public QSettings, public AppDir
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
    bool load();
    void outputLog(const QString &type, const char* file, const char* func, int line, const QString &msg);
    ~Preferences();
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
            qDebug("sql=%s", sql.toStdString().c_str());
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
            qDebug("sql=%s", sql.toStdString().c_str());
            query.exec(sql);
            if (query.next()) {
                pIOObject->LoadFromSqlQuery(query);
                pItem;
            }
        }
        delete pItem;
        return nullptr;
    }
    void file_ensure_xhb(QString filename);
    void backup_current_file();
    bool showwelcome(){
        qDebug(" -> ** General");
        if (!contains("General")){
            qCritical("no General group in preference");
            return true;
        }
        QVariantMap groupsetting = value("General").toMap();
        if(!groupsetting.contains("ShowWelcome")) return true;
        return groupsetting.value("ShowWelcome").toBool();
    }
    void set_showwelcome(bool val){
        QVariantMap groupsetting;
        if (contains("General")){
            qWarning("no General group in preference");
            groupsetting = value("General").toMap();
        }
        groupsetting["ShowWelcome"] = val;
        setValue("General", groupsetting);
    }
    bool includeremind(){
        qDebug(" -> ** General");
        if (!contains("General")){
            qCritical("no General group in preference");
            return true;
        }
        QVariantMap groupsetting = value("General").toMap();
        if(!groupsetting.contains("IncludeRemind")) return true;
        return groupsetting.value("IncludeRemind").toBool();
    }
    void set_includeremind(bool val){
        QVariantMap groupsetting;
        if (contains("General")){
            qWarning("no General group in preference");
            groupsetting = value("General").toMap();
        }
        groupsetting["IncludeRemind"] = val;
        setValue("General", groupsetting);
    }
    bool BakIsAutomatic(){
        qDebug(" -> ** General");
        if (!contains("General")){
            qCritical("no General group in preference");
            return true;
        }
        QVariantMap groupsetting = value("General").toMap();
        if(!groupsetting.contains("BakIsAutomatic")) return true;
        return groupsetting.value("BakIsAutomatic").toBool();
    }
    void SetBakIsAutomatic(bool val){
        QVariantMap groupsetting;
        if (contains("General")){
            qWarning("no General group in preference");
            groupsetting = value("General").toMap();
        }
        groupsetting["BakIsAutomatic"] = val;
        setValue("General", groupsetting);
    }

    int BakMaxNumCopies(){
        qDebug(" -> ** General");
        if (!contains("General")){
            qCritical("no General group in preference");
            return true;
        }
        QVariantMap groupsetting = value("General").toMap();
        if(!groupsetting.contains("BakMaxNumCopies")) return true;
        return groupsetting.value("BakMaxNumCopies").toBool();
    }
    void SetBakIsAutomatic(int val){
        QVariantMap groupsetting;
        if (contains("General")){
            qWarning("no General group in preference");
            groupsetting = value("General").toMap();
        }
        groupsetting["BakMaxNumCopies"] = val;
        setValue("General", groupsetting);
    }
    QString BackupPath(){
        qDebug(" -> ** General");
        if (!contains("General")){
            qCritical("no General group in preference");
            return QDir::homePath();
        }
        QVariantMap groupsetting = value("General").toMap();
        if(!groupsetting.contains("BackupPath")) return QDir::homePath();
        return groupsetting.value("BackupPath").toString();
    }
    void SetBackupPath(QString val){
        QVariantMap groupsetting;
        if (contains("General")){
            qWarning("no General group in preference");
            groupsetting = value("General").toMap();
        }
        groupsetting["BackupPath"] = val;
        setValue("General", groupsetting);
    }
private:
    static void __init_wingeometry(struct WinGeometry *wg, int l, int t, int w, int h);
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
    void __removeOldConnection();
    bool __file_delete_existing(const QString filepath);
    bool __file_copy(const QString srcfile, const QString dstfile);

public:
    QSqlTableModel* m_iAccountsModel;
    QSqlTableModel* m_iPayeeModel;
    QSqlTableModel* m_iTransactionModel;
    QString connectionname;
    QString currency;
    unsigned int changes_count;

private:
    explicit Preferences();
    static Preferences* m_iInstance;
    QSqlDatabase m_DB;

    QMutex          m_mutex;
    QString m_LogPath;
    QDateTime m_LastLogTime;
    QFile* m_iLogFile;
    QTextStream* m_iLogStream;
};

void LogHandler(QtMsgType type, const QMessageLogContext &info, const QString &msg);

#endif
