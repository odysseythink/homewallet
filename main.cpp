#include "main_win.h"
#include "hw-preferences.h"
#include "home_wallet.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HomeWallet::Instance();
    Preferences::Instance();
    MainWin w;
    w.show();

    return a.exec();
}
