#include "transaction_add_dialog.h"
#include "ui_transaction_add_dialog.h"
#include <QDateTime>
#include "hw-account.h"
#include "home_wallet.h"
#include <QMessageBox>

TransactionAddDialog::TransactionAddDialog(QString kacc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionAddDialog)
{
    ui->setupUi(this);
    __Update();
    m_Transaction.id = QUuid::createUuid().toString().remove("{").remove("}");
    m_Transaction.account = kacc;
}

TransactionAddDialog::~TransactionAddDialog()
{
    delete ui;
}

void TransactionAddDialog::__Update()
{
    if (ui->expenseRBtn->isChecked()){
        ui->amountEdit->setMinimum(-2147483647.00);
        ui->amountEdit->setMaximum(0);
    }else if (ui->incomeRBtn->isChecked()){
        ui->amountEdit->setMinimum(0);
        ui->amountEdit->setMaximum(2147483647.00);
    }else if (ui->transferRBtn->isChecked()){
        ui->amountEdit->setMinimum(-2147483647.00);
        ui->amountEdit->setMaximum(0);
    }

    ui->createdEdit->setDateTime(QDateTime::currentDateTime());
    ui->fromAccountCBox->clear();
    auto list = Account::get_all_by_type();
    QList<QSharedPointer<Account> >::iterator iter = list.begin();
    while(iter != list.end()){
        ui->fromAccountCBox->addItem(iter->get()->name, iter->get()->id);
        if (iter->get()->id == m_Transaction.account){
            ui->fromAccountCBox->setCurrentText(iter->get()->name);
        }
        ++iter;
    }
    ui->fromAccountCBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    ui->paymentAndToAccountCBox->clear();
    if(ui->expenseRBtn->isChecked() || ui->incomeRBtn->isChecked()){
        QMap<int, QString>::iterator tmpIter = HomeWallet::CYA_TXN_PAYMODE_ICONS.begin();
        while(tmpIter != HomeWallet::CYA_TXN_PAYMODE_ICONS.end()){
            ui->paymentAndToAccountCBox->addItem(QIcon(tmpIter.value()), HomeWallet::CYA_TXN_PAYMODE_NAMES[tmpIter.key()], tmpIter.key());
            ++tmpIter;
        }
    } else {
        QList<QSharedPointer<Account> >::iterator iter = list.begin();
        while(iter != list.end()){
            ui->paymentAndToAccountCBox->addItem(iter->get()->name, iter->get()->id);
            ++iter;
        }
    }
    ui->paymentAndToAccountCBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    if (ui->transferRBtn->isChecked()) ui->paymentAndToAccountCBox->setMaximumWidth(ui->fromAccountCBox->minimumSizeHint().width());
    else ui->paymentAndToAccountCBox->setMaximumWidth(ui->paymentAndToAccountCBox->minimumSizeHint().width());

    ui->categoryCBox->clear();
    QMap<QString, QString> cats;
    if (ui->expenseRBtn->isChecked())cats = Category::get_all_fullnames(TXN_TYPE_EXPENSE);
    else if (ui->incomeRBtn->isChecked())cats = Category::get_all_fullnames(TXN_TYPE_INCOME);
    else if (ui->transferRBtn->isChecked())cats = Category::get_all_fullnames(TXN_TYPE_TRANSFER);
    ui->categoryCBox->addItem("");
    QMap<QString, QString>::iterator catIter = cats.begin();
    while(catIter != cats.end()){
        ui->categoryCBox->addItem(catIter.value(), catIter.key());
        ++catIter;
    }
    ui->categoryCBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    __UpdateData();
}

void TransactionAddDialog::__UpdateData()
{
    if(ui->expenseRBtn->isChecked())m_Transaction.type = TXN_TYPE_EXPENSE;
    else if(ui->incomeRBtn->isChecked())m_Transaction.type = TXN_TYPE_INCOME;
    else if(ui->transferRBtn->isChecked())m_Transaction.type = TXN_TYPE_TRANSFER;
}

void TransactionAddDialog::on_expenseRBtn_clicked()
{
    ui->accountAndFromLbl->setText("Account:");
    ui->paymentAndToLbl->setText("Payment:");
    __Update();
}

void TransactionAddDialog::on_incomeRBtn_clicked()
{
    ui->accountAndFromLbl->setText("Account:");
    ui->paymentAndToLbl->setText("Payment:");
    __Update();
}

void TransactionAddDialog::on_transferRBtn_clicked()
{
    ui->accountAndFromLbl->setText("From:");
    ui->paymentAndToLbl->setText("To:");
    __Update();
}

void TransactionAddDialog::on_buttonBox_accepted()
{
    if (ui->fromAccountCBox->count() <= 0){
        QMessageBox::warning(this, "add transaction", "add transaction failed because no account provided");
        return;
    }
    if (ui->amountEdit->value() == 0.0){
        QMessageBox::warning(this, "add transaction", "add transaction failed because zero amount");
        return;
    }
    m_Transaction.created_at = ui->createdEdit->dateTime();
    if (ui->transferRBtn->isChecked()) m_Transaction.xfer_amount = ui->amountEdit->value();
    else m_Transaction.amount = ui->amountEdit->value();
    m_Transaction.account = ui->fromAccountCBox->currentData().toString();
    if (ui->transferRBtn->isChecked()) m_Transaction.xfer_account = ui->paymentAndToAccountCBox->currentData().toString();
    else m_Transaction.paymode = ui->paymentAndToAccountCBox->currentData().toInt();
    qDebug("-------paymode=%d", m_Transaction.paymode);
    if (ui->statusNoneRBtn->isChecked()) m_Transaction.status = TXN_STATUS_NONE;
    else if (ui->statusClearedRBtn->isChecked()) m_Transaction.status = TXN_STATUS_CLEARED;
    else if (ui->statusReconciledRBtn->isChecked()) m_Transaction.status = TXN_STATUS_RECONCILED;
    else if (ui->statusRemindRBtn->isChecked()) m_Transaction.status = TXN_STATUS_REMIND;
    else if (ui->statusVoidRBtn->isChecked()) m_Transaction.status = TXN_STATUS_VOID;
    else m_Transaction.status = TXN_STATUS_NONE;
    // payee ?
    m_Transaction.category = ui->categoryCBox->currentData().toString();
    m_Transaction.memo = ui->memoEdit->text();
    m_Transaction.memo = ui->infoEdit->text();
    if (m_Transaction.add_to_local()){
        Account::compute_balances(&m_Transaction);
        emit sigTransactionChanged();
    }
}
