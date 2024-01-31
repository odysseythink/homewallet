#ifndef TRANSACTION_MGR_DIALOG_H
#define TRANSACTION_MGR_DIALOG_H

#include <QDialog>

namespace Ui {
class TransactionMgrDialog;
}

class TransactionMgrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionMgrDialog(QWidget *parent = nullptr);
    ~TransactionMgrDialog();

private:
    Ui::TransactionMgrDialog *ui;
};

#endif // TRANSACTION_MGR_DIALOG_H
