#ifndef MGR_ACCOUNTS_DIALOG_H
#define MGR_ACCOUNTS_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QTableWidgetItem>
#include <QDataWidgetMapper>
#include <QSqlRecord>

namespace Ui {
class AccountMgrDialog;
}

class AccountMgrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AccountMgrDialog(QWidget *parent = nullptr);
    ~AccountMgrDialog();

private:
    void __Update();

private slots:
    void __OnAdd();
    void __OnDel();
    void __OnEdit();
    void __OnAccountClicked(const QModelIndex &index);
    void __OnBeforeUpdate(int row, QSqlRecord &record);

    void __OnDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    void __OnCurrencyChanged(const QString &text);

signals:
    void sigAccountChanged();

private:
    Ui::AccountMgrDialog *ui;
    QDataWidgetMapper m_Mapper;
};

#endif // MGR_ACCOUNTS_DIALOG_H
