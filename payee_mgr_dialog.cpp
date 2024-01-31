#include "payee_mgr_dialog.h"
#include "ui_payee_mgr_dialog.h"
#include "ui_payee_edit_dialog.h"
#include "hw-preferences.h"
#include "home_wallet.h"
#include <QMap>
#include <QInputDialog>
#include <QMessageBox>


PayeeMgrDialog::PayeeMgrDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PayeeMgrDialog)
{
    ui->setupUi(this);
    if(Preferences::Instance()->m_iPayeeModel != nullptr){
        ui->payeeTableView->setModel(Preferences::Instance()->m_iPayeeModel);
    }
    __Update();
}

PayeeMgrDialog::~PayeeMgrDialog()
{
    delete ui;
}

void PayeeMgrDialog::__Update()
{
    if(Preferences::Instance()->m_iPayeeModel != nullptr){
        qDebug("update ------");
        Preferences::Instance()->m_iPayeeModel->select();
    }
}

void PayeeMgrDialog::on_addBtn_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("payee"),
                                         tr("payee name:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty()) {
        qDebug("------text=%s", text.toStdString().c_str());
        Preferences::Instance()->m_iPayeeModel->insertRow(0);
        Preferences::Instance()->m_iPayeeModel->setData(Preferences::Instance()->m_iPayeeModel->index(0,0), QUuid::createUuid().toString().remove("{").remove("}"));
        Preferences::Instance()->m_iPayeeModel->setData(Preferences::Instance()->m_iPayeeModel->index(0,2), text);
        Preferences::Instance()->m_iPayeeModel->submit();
//        Account* pAcc = new Account(text);
//        Account::add_to_local(pAcc);
//        delete pAcc;
        __Update();

    }
}

void PayeeMgrDialog::on_delBtn_clicked()
{
    QList<QModelIndex> list =  ui->payeeTableView->selectionModel()->selectedIndexes();
    if (list.size() > 0){
        ui->payeeTableView->setEnabled(true);
        auto name = ui->payeeTableView->model()->index(list[0].row(), 2).data().toString();
        qDebug("-------name=%s", name.toStdString().c_str());
        QString msg = QString("<h3>Are you sure you want to permanently delete '%1'?</h3><p>If you delete an payee, it will be permanently lost.</p>").arg(name);
        if (QMessageBox::question(this, "delete payee", msg) == QMessageBox::Yes){
            if(Preferences::Instance()->m_iPayeeModel != nullptr){
                Preferences::Instance()->m_iPayeeModel->removeRow(list[0].row());
                __Update();
            }
        }
    }
}

void PayeeMgrDialog::on_editBtn_clicked()
{
    QModelIndex idx = ui->payeeTableView->currentIndex();
    if (idx.isValid()){
//        QMessageBox::warning(this, "edit payee", "edit payee failed, because no payee selected");
//        return;
        PayeeEditDialog* pDialog = new PayeeEditDialog(idx, Preferences::Instance()->m_iPayeeModel, this);
        pDialog->exec();
        delete  pDialog;
    }
}

PayeeEditDialog::PayeeEditDialog(QModelIndex idx, QSqlTableModel *model, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::PayeeEditDialog)
{
    m_Idx = idx;
    m_Model = model;
    ui->setupUi(this);
    if (model != nullptr && m_Idx.isValid() && !model->data(idx).isNull() && model->data(idx).isValid()){
        ui->nameEdit->setText(model->data(model->index(idx.row(), 2)).toString());
        ui->notesEdit->setPlainText(model->data(model->index(idx.row(), 5)).toString());
        QMap<int, QString>::iterator tmpIter = HomeWallet::CYA_TXN_PAYMODE_ICONS.begin();
        while(tmpIter != HomeWallet::CYA_TXN_PAYMODE_ICONS.end()){
            ui->paymentCBox->addItem(QIcon(tmpIter.value()), HomeWallet::CYA_TXN_PAYMODE_NAMES[tmpIter.key()], tmpIter.key());
            if (model->data(model->index(idx.row(), 4)).toInt() == tmpIter.key()){
                ui->paymentCBox->setCurrentText(HomeWallet::CYA_TXN_PAYMODE_NAMES[tmpIter.key()]);
            }
            ++tmpIter;
        }
        QMap<QString, QString> cats = Category::get_all_fullnames(TXN_TYPE_EXPENSE);
        QMap<QString, QString>::iterator catIter = cats.begin();
        while(catIter != cats.end()){
            ui->categoryCBox->addItem(catIter.value(), catIter.key());
            if (model->data(model->index(idx.row(), 3)).toString() == catIter.key()){
                ui->categoryCBox->setCurrentText(catIter.value());
            }
            ++catIter;
        }
        cats = Category::get_all_fullnames(TXN_TYPE_INCOME);
        catIter = cats.begin();
        while(catIter != cats.end()){
            ui->categoryCBox->addItem(catIter.value(), catIter.key());
            if (model->data(model->index(idx.row(), 3)).toString() == catIter.key()){
                ui->categoryCBox->setCurrentText(catIter.value());
            }
            ++catIter;
        }
    }
}

PayeeEditDialog::~PayeeEditDialog()
{

}

void PayeeEditDialog::__OnAccept()
{
    if (m_Model != nullptr && m_Idx.isValid() && !m_Model->data(m_Idx).isNull() && m_Model->data(m_Idx).isValid()){
        m_Model->setData(m_Model->index(m_Idx.row(), 2), ui->nameEdit->text());
        m_Model->setData(m_Model->index(m_Idx.row(), 5), ui->notesEdit->placeholderText());
        m_Model->setData(m_Model->index(m_Idx.row(), 4), ui->paymentCBox->currentData().toInt());
        m_Model->setData(m_Model->index(m_Idx.row(), 3), ui->categoryCBox->currentData().toString());
        m_Model->submit();
    }
}
