#include "main_win.h"
#include "ui_main_win.h"
#include "home_wallet.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QScreen>
#include "account_mgr_dialog.h"
#include "category_mgr_dialog.h"
#include "payee_mgr_dialog.h"
#include "transaction_add_dialog.h"
#include "transaction_mgr_dialog.h"

MainWin::MainWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWin)
{
    ui->setupUi(this);
    __InitToolsbar();
    setWindowTitle(QString("home wallet %1").arg(Preferences::Instance()->XHWFilepath()));
    auto rec = screen()->geometry();
    setGeometry((rec.width()-800)/2, (rec.height()-600)/2, 800, 600);
}

MainWin::~MainWin()
{
    delete ui;
}

void MainWin::__InitToolsbar()
{
    ui->m_iToolBar->addAction(ui->m_iNewAction);
    ui->m_iToolBar->addAction(ui->m_iOpenAction);
    ui->m_iToolBar->addWidget(ui->m_iOpenRecentMenu);
    ui->m_iToolBar->addAction(ui->m_iSaveAction);
    ui->m_iToolBar->addSeparator();
    ui->m_iToolBar->addAction(ui->m_iMgrAccountsAction);
    ui->m_iToolBar->addAction(ui->m_iMgrPayeesAction);
    ui->m_iToolBar->addAction(ui->m_iMgrCategoriesAction);
    ui->m_iToolBar->addAction(ui->m_iMgrScheduledTemplateAction);
    ui->m_iToolBar->addAction(ui->m_iMgrBudgetAction);
    ui->m_iToolBar->addAction(ui->m_iMgrAssignmentsAction);
    ui->m_iToolBar->addSeparator();
    ui->m_iToolBar->addAction(ui->m_iShowTransAction);
    ui->m_iToolBar->addAction(ui->m_iAddTransAction);
    ui->m_iToolBar->addSeparator();
    ui->m_iToolBar->addAction(ui->m_iReportsStatisticsAction);
    ui->m_iToolBar->addAction(ui->m_iReportsTrendTimeAction);
    ui->m_iToolBar->addAction(ui->m_iReportsBalanceAction);
    ui->m_iToolBar->addAction(ui->m_iReportsBudgetAction);
    ui->m_iToolBar->addAction(ui->m_iReportsVehicleCostAction);
}


/* hbfile action functions -------------------- */
void MainWin::__action_new()
{
    QDir dir(HomeWallet::Instance()->app_get_datas_dir());
    if(!dir.exists()){
        dir.mkpath(HomeWallet::Instance()->app_get_datas_dir());
    }
    QString defaultfilename = QDir(HomeWallet::Instance()->app_get_datas_dir()).absoluteFilePath("untitled.xhw");

    QString filename = QFileDialog::getSaveFileName(this, "wallet filename", defaultfilename, "*.xhw");
    if (filename != ""){
        HomeWallet::Instance()->backup_current_file();
        QFile::remove(Preferences::Instance()->XHWFilepath());
        HomeWallet::Instance()->file_ensure_xhb(filename);
        IHWFileIO::new_xml(Preferences::Instance()->XHWFilepath());
        setWindowTitle(QString("home wallet %1").arg(Preferences::Instance()->XHWFilepath()));
    }
}

void MainWin::__action_mgr()
{
    QAction* pAction = qobject_cast<QAction*>(sender());
    if (pAction == ui->m_iMgrAccountsAction){
        AccountMgrDialog* pDialog = new AccountMgrDialog(this);
        pDialog->exec();
        ui->m_iAccountSumWidget->Update();
        delete pDialog;
    } else if (pAction == ui->m_iMgrPayeesAction){
        PayeeMgrDialog* pDialog = new PayeeMgrDialog(this);
        pDialog->exec();
        delete pDialog;
    } else if (pAction == ui->m_iMgrCategoriesAction){
        CategoryMgrDialog* pDialog = new CategoryMgrDialog(this);
        pDialog->exec();
        delete pDialog;
    } else if (pAction == ui->m_iAddTransAction){
        auto kacc = ui->m_iAccountSumWidget->GetCurrentAccount();
        TransactionAddDialog* pDialog = new TransactionAddDialog(kacc, this);
        connect(pDialog, SIGNAL(sigTransactionChanged()), ui->m_iAccountSumWidget, SLOT(Update()));
        pDialog->exec();
        delete pDialog;
    } else if (pAction == ui->m_iShowTransAction){
        TransactionMgrDialog* pDialog = new TransactionMgrDialog(this);
        connect(pDialog, SIGNAL(sigTransactionChanged()), ui->m_iAccountSumWidget, SLOT(Update()));
        pDialog->exec();
        delete pDialog;
    }
}

/*
** request the user to save last change
*/
bool MainWin::__savechanges()
{
    bool retval = true;

    if(HomeWallet::Instance()->changes_count)
    {
        QMessageBox::StandardButton ret = QMessageBox::question(this, "Save changes to the file before closing?", QString("If you don't save, changes will be permanently lost.\nNumber of changes: %1.").arg(HomeWallet::Instance()->changes_count));

        if(ret == QMessageBox::No) {
            retval = false;
        } else {
            qDebug(" + should quick save %s\n", Preferences::Instance()->XHWFilepath().toStdString().c_str());
            //todo: should migrate this
            //#1720377 also backup
            HomeWallet::Instance()->file_ensure_xhb("");
            HomeWallet::Instance()->backup_current_file();
            homebank_save_xml(Preferences::Instance()->XHWFilepath());
        }
    }

    return retval;
}

