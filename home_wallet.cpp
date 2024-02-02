#include "home_wallet.h"
#include "config.h"
#include <QCoreApplication>
#include <QDir>


HomeWallet* HomeWallet::m_iInstance = nullptr;

bool HomeWallet::setup()
{
    qDebug("starting......");

    qDebug("icon theme setup\n");
//    homewallet_icon_theme_setup();

#ifdef Q_OS_WIN
//    homewallet_setup_theme_extensions();
#endif
    qDebug("app date get julian\n");
//    homewallet_app_date_get_julian();


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
    qDebug("preference setdefault\n");
}

HomeWallet::~HomeWallet()
{
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

