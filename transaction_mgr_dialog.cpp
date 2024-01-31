#include "transaction_mgr_dialog.h"
#include "ui_transaction_mgr_dialog.h"

TransactionMgrDialog::TransactionMgrDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionMgrDialog)
{
    ui->setupUi(this);
}

TransactionMgrDialog::~TransactionMgrDialog()
{
    delete ui;
}
