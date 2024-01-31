#include "transaction_mgr_dialog.h"
#include "ui_transaction_mgr_dialog.h"
#include "hw-preferences.h"

TransactionMgrDialog::TransactionMgrDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionMgrDialog)
{
    ui->setupUi(this);
    if(Preferences::Instance()->m_iTransactionModel != nullptr){
        ui->dataView->setModel(Preferences::Instance()->m_iTransactionModel);
        Preferences::Instance()->m_iTransactionModel->select();
    }
}

TransactionMgrDialog::~TransactionMgrDialog()
{
    delete ui;
}
