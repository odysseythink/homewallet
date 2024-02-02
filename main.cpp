#include "main_win.h"
#include "hw-preferences.h"
#include "home_wallet.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(LogHandler);
    qDebug() << QString("%1-v%2 starting......").arg(PROGNAME).arg(HW_VERSION);
    MainWin w;
    w.show();

    auto nret = a.exec();
    delete Preferences::Instance();
    return nret;
}
