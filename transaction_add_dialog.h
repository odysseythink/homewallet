#ifndef ADD_TRANSACTION_H
#define ADD_TRANSACTION_H

#include <QDialog>
#include "hw-transaction.h"

namespace Ui {
class TransactionAddDialog;
}

class TransactionAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionAddDialog(QString kacc = "", QWidget *parent = nullptr);
    ~TransactionAddDialog();

private:
    void __Update();
    void __UpdateData();

private slots:
    void on_expenseRBtn_clicked();

    void on_incomeRBtn_clicked();

    void on_transferRBtn_clicked();

    void on_buttonBox_accepted();

signals:
    void sigTransactionChanged();

private:
    Ui::TransactionAddDialog *ui;
    Transaction m_Transaction;
};

#endif // ADD_TRANSACTION_H
