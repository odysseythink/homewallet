#include "account_mgr_dialog.h"
#include "ui_account_mgr_dialog.h"
#include "home_wallet.h"
#include <QAction>
#include <QInputDialog>
#include <QTableWidgetItem>
#include <QSqlQueryModel>

static QString qDBCaption(const QSqlDatabase &db)
{
    QString nm = db.driverName();
    nm.append(QLatin1Char(':'));
    if (!db.userName().isEmpty())
        nm.append(db.userName()).append(QLatin1Char('@'));
    nm.append(db.databaseName());
    return nm;
}

AccountMgrDialog::AccountMgrDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountMgrDialog)
{
    ui->setupUi(this);
    QMap<int, QString>::const_iterator iter = HomeWallet::CYA_ACC_TYPE.constBegin();
    while(iter != HomeWallet::CYA_ACC_TYPE.constEnd()){
        ui->m_iAccountTypeCBox->addItem(iter.value(), iter.key());
        ++iter;
    }
    ui->m_iAccountSearchEdit->addAction(new QAction(QIcon("://img/icon-search.png"), ""), QLineEdit::LeadingPosition);
    if(Preferences::Instance()->m_iAccountsModel != nullptr){
        ui->m_iAccountsTableView->setModel(Preferences::Instance()->m_iAccountsModel);
//        for(int iLoop = 3; iLoop < 19; iLoop++)
//            ui->m_iAccountsTableView->setColumnHidden(iLoop, true);
        m_Mapper.setModel(Preferences::Instance()->m_iAccountsModel);
        m_Mapper.addMapping(ui->m_iAccountTypeCBox, 2, "currentIndex");
        m_Mapper.addMapping(ui->m_iBankNameEdit, 7);
        m_Mapper.addMapping(ui->m_iNumberEdit, 6);
        m_Mapper.addMapping(ui->m_iInitialEdit, 8);
        m_Mapper.addMapping(ui->m_iWebsiteEdit, 13);
        m_Mapper.addMapping(ui->m_iNotesEdit, 14);
        m_Mapper.addMapping(ui->m_iIsClosedCBox, 19);
        m_Mapper.addMapping(ui->m_iExcludeFromAccountSumCBox, 20);
        m_Mapper.addMapping(ui->m_iOutflowInSumCBox, 21);
        m_Mapper.addMapping(ui->m_iExcludeFromBudgetCBox, 22);
        m_Mapper.addMapping(ui->m_iExcludeFromAnyReportsCBox, 23);
        m_Mapper.addMapping(ui->m_iMinimumEdit, 9);
        m_Mapper.addMapping(ui->m_iMaximumEdit, 10);
        m_Mapper.addMapping(ui->m_iCheque1Edit, 11);
        m_Mapper.addMapping(ui->m_iCheque2Edit, 12);

        m_Mapper.addMapping(ui->m_iCurrencyCBox, 5);

        connect(Preferences::Instance()->m_iAccountsModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(__OnDataChanged(const QModelIndex&, const QModelIndex&)));
    }
    ui->m_iCurrencyCBox->clear();

    auto list = Currency::get_all_currencies(true);
    QList<QSharedPointer<Currency> >::iterator curiter = list.begin();
    while(curiter != list.end()){
        ui->m_iCurrencyCBox->addItem(curiter->get()->iso_code,curiter->get()->name);
        ++curiter;
    }
    list.clear();
    ui->m_iCurrencyNameLbl->setText(ui->m_iCurrencyCBox->currentData().toString());
    __Update();
}

AccountMgrDialog::~AccountMgrDialog()
{
    delete ui;
}

void AccountMgrDialog::__Update()
{
//    m_AccountsTableModel.select();
//    while(ui->m_iAccountsTableWidget->rowCount()){
//        ui->m_iAccountsTableWidget->removeRow(0);
//    }
//    auto list = Account::get_all_accounts();
//    qDebug("-----------list.size()=%d", list.size());
//    QList<QSharedPointer<Account> >::iterator iter = list.begin();
//    while(iter != list.end()){
//        if(iter->get() != nullptr){
//            ui->m_iAccountsTableWidget->insertRow(ui->m_iAccountsTableWidget->rowCount());
//            ui->m_iAccountsTableWidget->setItem(ui->m_iAccountsTableWidget->rowCount()-1, 0, new QTableWidgetItem(iter->get()->id));
//            ui->m_iAccountsTableWidget->setItem(ui->m_iAccountsTableWidget->rowCount()-1, 1, new QTableWidgetItem(iter->get()->name));
//        }
//        ++iter;
//    }
//    list.clear();
//    QSqlQueryModel *model = new QSqlQueryModel(ui->m_iAccountsTableView);
//    QSqlDatabase db = Preferences::Instance()->currentDatabase();
//    QString caption = qDBCaption(db);
//    qDebug("----------caption=%s", caption.toStdString().c_str());
//    QSqlQuery query(db);
//    query.exec("select id, name, type from t_accounts;");
//    while(query.next()){
//        qDebug("----------id=%s", query.value(0).toString().toStdString().c_str());
//    }
//    model->setQuery(QSqlQuery("select id, name, type from t_accounts;", db));
//    ui->m_iAccountsTableView->setModel(model);
    if(Preferences::Instance()->m_iAccountsModel != nullptr){
        qDebug("update ------");
        Preferences::Instance()->m_iAccountsModel->select();
    }
    if(!ui->m_iAccountsTableView->currentIndex().isValid()){
        ui->m_iAccountSettingWidget->setEnabled(false);
        m_Mapper.setCurrentIndex(-1);
    } else {
        ui->m_iAccountSettingWidget->setEnabled(true);
        m_Mapper.setCurrentIndex(ui->m_iAccountsTableView->currentIndex().row());
    }
}

void AccountMgrDialog::__OnAdd()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("account"),
                                         tr("account name:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty()) {
        qDebug("------text=%s", text.toStdString().c_str());
        Preferences::Instance()->m_iAccountsModel->insertRow(0);
        Preferences::Instance()->m_iAccountsModel->setData(Preferences::Instance()->m_iAccountsModel->index(0,0), QUuid::createUuid().toString().remove("{").remove("}"));
        Preferences::Instance()->m_iAccountsModel->setData(Preferences::Instance()->m_iAccountsModel->index(0,1), text);
        Preferences::Instance()->m_iAccountsModel->setData(Preferences::Instance()->m_iAccountsModel->index(0,8), 0.);
        QString cur = Currency::get_system_iso();
        Preferences::Instance()->m_iAccountsModel->setData(Preferences::Instance()->m_iAccountsModel->index(0,5), cur);
        Preferences::Instance()->m_iAccountsModel->submit();
//        Account* pAcc = new Account(text);
//        Account::add_to_local(pAcc);
//        delete pAcc;
        __Update();

    }
}

void AccountMgrDialog::__OnDel()
{
    QList<QModelIndex> list =  ui->m_iAccountsTableView->selectionModel()->selectedIndexes();
    if (list.size() > 0){
        ui->m_iAccountSettingWidget->setEnabled(true);
        auto name = ui->m_iAccountsTableView->model()->index(list[0].row(), 1).data().toString();
        qDebug("-------name=%s", name.toStdString().c_str());
        QString msg = QString("<h3>Are you sure you want to permanently delete '%1'?</h3><p>If you delete an account, it will be permanently lost.</p>").arg(name);
        if (QMessageBox::question(this, "delete account", msg) == QMessageBox::Yes){
            if(Preferences::Instance()->m_iAccountsModel != nullptr){
                Preferences::Instance()->m_iAccountsModel->removeRow(list[0].row());
                __Update();
            }
        }
    }
}

void AccountMgrDialog::__OnEdit()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("account"),
                                         tr("account name:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty()) {
        qDebug("------text=%s", text.toStdString().c_str());
        Account* pAcc = new Account(text);
        if (pAcc != nullptr) pAcc->save_to_local();
        delete pAcc;
        __Update();
    }
}

void AccountMgrDialog::__OnAccountClicked(const QModelIndex &index)
{
    QList<QModelIndex> list =  ui->m_iAccountsTableView->selectionModel()->selectedIndexes();
    if (list.size() > 0){
        ui->m_iAccountSettingWidget->setEnabled(true);
        auto id = ui->m_iAccountsTableView->model()->index(index.row(), 0).data().toString();
        qDebug("-------id=%s", id.toStdString().c_str());
        if(Preferences::Instance()->m_iAccountsModel != nullptr){
            m_Mapper.setCurrentModelIndex(index);
        }
    }
}

void AccountMgrDialog::__OnBeforeUpdate(int row, QSqlRecord &record)
{
    qDebug("-+-+-+-+-+-+-+-+-+-+-+-+-+__OnBeforeUpdate");
}

void AccountMgrDialog::__OnDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    qDebug("-+-+-+-+-+-+-+-+-+-+-+-+-+");
    if(Preferences::Instance()->m_iAccountsModel != nullptr){
        Preferences::Instance()->m_iAccountsModel->submitAll();
    }
}

void AccountMgrDialog::__OnCurrencyChanged(const QString &text)
{
    ui->m_iCurrencyNameLbl->setText(ui->m_iCurrencyCBox->currentData().toString());
}
