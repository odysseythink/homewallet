#ifndef ADD_TRANSACTION_H
#define ADD_TRANSACTION_H

#include <QDialog>
#include "hw-transaction.h"

namespace Ui {
class TransactionAddWidget;
}

class TransactionAddWidget : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionAddWidget(QString kacc = "", QWidget *parent = nullptr);
    ~TransactionAddWidget();

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
    Ui::TransactionAddWidget *ui;
    Transaction m_Transaction;
};

#endif // ADD_TRANSACTION_H
