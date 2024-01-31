#include "account_sum_widget.h"
#include "ui_account_sum_widget.h"
#include "home_wallet.h"
#include <QTreeWidgetItem>

AccountSumWidget::AccountSumWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::AccountSumWidget)
{
    ui->setupUi(this);
    __Update();
}

AccountSumWidget::~AccountSumWidget()
{
    delete ui;
}

QString AccountSumWidget::GetCurrentAccount()
{
    auto pItem = ui->treeWidget->currentItem();
    if (pItem != nullptr){
        if (pItem->childCount() > 0){
            return "";
        }
        return pItem->data(0, Qt::UserRole).toString();
    } else {
        return "";
    }
}

void AccountSumWidget::Update()
{
    __Update();
}

void AccountSumWidget::__Update()
{
    qDebug("AccountSumWidget::__Update");
    Account::compute_balances();
    ui->treeWidget->clear();
    QMap<int, QString>::iterator iter = HomeWallet::CYA_ACC_TYPE.begin();
    while(iter != HomeWallet::CYA_ACC_TYPE.end()){
        auto list = Account::get_all_by_type(iter.key());
        if (list.size() > 0){
            QStringList accTypelist = {iter.value()};
            double bal_recon = 0.;
            double bal_clear = 0.;
            double bal_today = 0.;
            double bal_future= 0.;
            QTreeWidgetItem* pItem = new QTreeWidgetItem(accTypelist);
            QList<QSharedPointer<Account> >::iterator accIter = list.begin();
            while(accIter != list.end()){
                qDebug("----------%s---%s--%d", iter.value().toStdString().c_str(), accIter->get()->name.toStdString().c_str(),accIter->get()->type);
                QStringList tmplist = {accIter->get()->name};
                bal_recon += accIter->get()->bal_recon;
                tmplist << QString::number(accIter->get()->bal_recon);
                bal_clear += accIter->get()->bal_clear;
                tmplist << QString::number(accIter->get()->bal_clear);
                bal_today += accIter->get()->bal_today;
                tmplist << QString::number(accIter->get()->bal_today);
                bal_future += accIter->get()->bal_future;
                tmplist << QString::number(accIter->get()->bal_future);
                QTreeWidgetItem* pSubItem = new QTreeWidgetItem(tmplist);
                pSubItem->setData(0, Qt::UserRole, accIter->get()->id);
                pItem->addChild(pSubItem);
                ++accIter;
            }
            pItem->setText(1, QString::number(bal_recon));
            pItem->setText(2, QString::number(bal_clear));
            pItem->setText(3, QString::number(bal_today));
            pItem->setText(4, QString::number(bal_future));
            ui->treeWidget->addTopLevelItem(pItem);
        }
        ++iter;
    }
}
