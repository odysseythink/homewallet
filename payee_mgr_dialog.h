#ifndef MGR_PAYEE_DIALOG_H
#define MGR_PAYEE_DIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlRecord>


namespace Ui {
class PayeeMgrDialog;
class PayeeEditDialog;
}

class PayeeEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PayeeEditDialog(QModelIndex idx, QSqlTableModel* model, QWidget *parent = nullptr);
    ~PayeeEditDialog();

private slots:
    void __OnAccept();

private:


private:
    Ui::PayeeEditDialog *ui;
    QSqlTableModel *m_Model;
    QModelIndex m_Idx;
};

class PayeeMgrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PayeeMgrDialog(QWidget *parent = nullptr);
    ~PayeeMgrDialog();

private slots:
    void on_addBtn_clicked();

    void on_delBtn_clicked();

    void on_editBtn_clicked();

private:
    void __Update();

private:
    Ui::PayeeMgrDialog *ui;
};

#endif // MGR_PAYEE_DIALOG_H
