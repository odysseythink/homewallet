#include "home_wallet.h"
#include "config.h"
#include <QCoreApplication>
#include <QDir>


HomeWallet* HomeWallet::m_iInstance = nullptr;

bool HomeWallet::setup()
{
    qDebug("starting......");

    qDebug("icon theme setup\n");
//    homebank_icon_theme_setup();

#ifdef Q_OS_WIN
//    homebank_setup_theme_extensions();
#endif
    qDebug("app date get julian\n");
//    homebank_app_date_get_julian();


    #if MYDEBUG == 1

    g_print(" - user_name: %s\n", g_get_user_name ());
    g_print(" - real_name: %s\n", g_get_real_name ());
    g_print(" - user_cache_dir: %s\n", g_get_user_cache_dir());
    g_print(" - user_data_dir: %s\n", g_get_user_data_dir ());
    g_print(" - user_config_dir: %s\n", g_get_user_config_dir ());
    //g_print(" - system_data_dirs: %s\n", g_get_system_data_dirs ());
    //g_print(" - system_config_dirs: %s\n", g_get_system_config_dirs ());

    g_print(" - home_dir: %s\n", g_get_home_dir ());
    g_print(" - tmp_dir: %s\n", g_get_tmp_dir ());
    g_print(" - current_dir: %s\n", g_get_current_dir ());

    #endif

    return true;
}

HomeWallet::HomeWallet(QObject *parent)
    : QObject(parent)
{
    config_dir   = "";
    images_dir   = "";
    pixmaps_dir  = "";
    locale_dir   = "";
    help_dir     = "";
    datas_dir    = "";
    __build_package_paths();
    // check homedir for .homebank dir
    __check_app_dir();

    qDebug("preference setdefault\n");
}

HomeWallet::~HomeWallet()
{
    __build_package_paths();
}


/* build package paths at runtime */
void HomeWallet::__build_package_paths ()
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
 * check/create user home directory for .homebank (HB_DATA_PATH) directory
 */
void HomeWallet::__check_app_dir()
{
    QString homedir;
    QString configdir;
    QDir dir;

    qDebug("\n[homebank] check_app_dir\n");
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

    /* check for XDG .config/homebank */
    configdir = app_get_config_dir();
    qDebug(" - config_dir is: '%s'\n", configdir.toStdString().c_str());
    if(dir.exists(configdir))
    {
        /* just update folder security */
        qDebug(" - chmod 0700\n");
        first_run = false;
        qDebug(" - chmod 0700 finished\n");
    }
    else
    {
        /* create the config dir */
        qDebug(" - create config_dir\n");
        dir.mkpath(configdir);

        /* any old homedir configuration out there ? */
        homedir = QDir::homePath() + "/.homebank";
        qDebug(" - homedir is: '%s'\n", homedir.toStdString().c_str());
        if(dir.exists(homedir))
        {
            bool f1, f2;
            /* we must do the migration properly */
            qDebug(" - migrate old 2 files\n");
            f1 = __check_app_dir_migrate_file(homedir, config_dir, "preferences");
            f2 = __check_app_dir_migrate_file(homedir, config_dir, "lastopenedfiles");
            if(f1 && f2)
            {
                qDebug(" - removing old dir\n");
                dir.rmdir(homedir);
            }
        }
        first_run = true;
    }

}



bool HomeWallet::__check_app_dir_migrate_file(QString srcdir, QString dstdir, QString filename)
{
    QString srcpath;
    QString dstpath;
    //GError *error = NULL;
    bool retval = false;

    qDebug("check_app_dir_migrate_file");

    srcpath = srcdir + "/" + filename;
    dstpath = dstdir + "/" + filename;

    if (QFile::copy(srcpath, dstpath))
    {
        QFile::remove(srcpath);
        //g_print("sould delete %s\n", srcpath);
        retval = true;
    }

    return retval;
}

bool HomeWallet::__file_delete_existing(const QString filepath)
{
    bool retval = false;

    qDebug("\n[homebank] file delete existing\n");

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

bool HomeWallet::__file_copy(const QString srcfile, const QString dstfile)
{
    //GError *error = NULL;
    bool retval = false;

    qDebug("\n[homebank] file copy\n");

    if (QFile::copy(srcfile, dstfile))
    {
        retval = true;
    }

    qDebug(" - copied '%s' => '%s' :: %d\n", srcfile.toStdString().c_str(), dstfile.toStdString().c_str(), retval);
    return retval;
}

/*
** ensure the filename ends with '.xhb'
*/
void HomeWallet::file_ensure_xhb(QString filename)
{
    qDebug("\n[homebank] file_ensure_xhb\n");
    filename = (filename == "") ? Preferences::Instance()->XHWFilepath() : filename;

    qDebug(" in filepath: '%s'\n", Preferences::Instance()->XHWFilepath().toStdString().c_str());

    if(filename.endsWith(WALLET_FILE_SUFFIX) == false ) {
        QString newfilename = hb_filename_new_with_extension(filename, "xhb");
        Preferences::Instance()->SetXHWFilepath((newfilename != "") ? newfilename : "");
    } else {
        Preferences::Instance()->SetXHWFilepath((filename != "") ? filename : "");
    }

    qDebug(" out filepath: '%s'\n", Preferences::Instance()->XHWFilepath().toStdString().c_str());
}

void HomeWallet::backup_current_file()
{
    QString bakfilename;
    QStringList array;
    int i;

    qDebug("\n[homebank] backup_current_file\n");

    //do normal linux backup file
    qDebug(" normal backup with ~\n");
    bakfilename = hb_filename_new_with_extension (Preferences::Instance()->XHWFilepath(), "xhb~");
    HomeWallet::Instance()->__file_delete_existing(bakfilename);
    //#512046 copy file not to broke potential links
    //retval = g_rename(pathname, newname);
    HomeWallet::Instance()->__file_copy (Preferences::Instance()->XHWFilepath(), bakfilename);

    //do safe backup according to user preferences
    qDebug(" user pref backup\n");
    if( Preferences::Instance()->bak_is_automatic == true )
    {
        bakfilename = hb_filename_new_for_backup(Preferences::Instance()->XHWFilepath());
        if(QFileInfo(bakfilename).exists() == false )
        {
            HomeWallet::Instance()->__file_copy (Preferences::Instance()->XHWFilepath(), bakfilename);
        }

        //delete any offscale backup
        qDebug(" clean old backup\n");
        array = hb_filename_backup_list(Preferences::Instance()->XHWFilepath());

        qDebug(" found %d match\n", array.size());

        //#1847645
        //gchar *dirname = g_path_get_dirname(HomeWallet::Instance()->xhb_filepath);
        QString dirname = Preferences::Instance()->path_hbbak;

        for(i=0;i<array.size();i++)
        {
            QString offscalefilename = array[i];

            qDebug(" %d : '%s'\n", i, offscalefilename.toStdString().c_str());
            if( i >= Preferences::Instance()->bak_max_num_copies )
            {
                QString bakdelfilepath =  QDir(dirname).absoluteFilePath(offscalefilename);
                qDebug(" - should delete '%s'\n", bakdelfilepath.toStdString().c_str());

                HomeWallet::Instance()->__file_delete_existing(bakdelfilepath);

            }
        }
    }

}
