#ifndef __MAIN_WIN_H__
#define __MAIN_WIN_H__

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWin; }
QT_END_NAMESPACE

class MainWin : public QMainWindow
{
    Q_OBJECT

public:
    MainWin(QWidget *parent = nullptr);
    ~MainWin();

private:
    void __InitToolsbar();
    bool __savechanges();

private slots:
    void __action_new();
    void __action_mgr();

private:
    Ui::MainWin *ui;
};
#endif // __MAIN_WIN_H__
