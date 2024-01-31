#ifndef MGR_CATEGORIES_DIALOG_H
#define MGR_CATEGORIES_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "hw-category.h"

namespace Ui {
class CategoryMgrDialog;
class CategoryEditDialog;
}

class CategoryEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryEditDialog(const QString& name, const QString& type, QWidget *parent = nullptr);
    ~CategoryEditDialog();
    QString name();

private:


private slots:


private:
    Ui::CategoryEditDialog *ui;
};


class CategoryMgrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryMgrDialog(QWidget *parent = nullptr);
    ~CategoryMgrDialog();

private:
    void __Update();
    void __DeleteCategory(QStandardItem* item);
    void __UpdateTypeCategory(QStandardItem* item, const QString& type);

private slots:
    void on_addBtn_clicked();
    void on_delBtn_clicked();
    void __OnCategoryTypeChanged();
    void on_editBtn_clicked();

private:
    Ui::CategoryMgrDialog *ui;
    QStandardItemModel m_ExpenseCategoryModel;
    QStandardItemModel m_IncomeCategoryModel;
};

#endif // MGR_CATEGORIES_DIALOG_H
