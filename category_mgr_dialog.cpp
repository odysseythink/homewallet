#include "category_mgr_dialog.h"
#include "ui_category_mgr_dialog.h"
#include "ui_category_edit_dialog.h"
#include <QInputDialog>
#include <QDir>
#include <QSqlQueryModel>
#include <QMessageBox>
#include "home_wallet.h"
#include "hw-preferences.h"
#include "hw-category.h"

CategoryMgrDialog::CategoryMgrDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoryMgrDialog)
{
    ui->setupUi(this);
    m_ExpenseCategoryModel.setHorizontalHeaderLabels(QStringList("Category"));
    m_IncomeCategoryModel.setHorizontalHeaderLabels(QStringList("Category"));
    __Update();
}

CategoryMgrDialog::~CategoryMgrDialog()
{
    delete ui;
}

void CategoryMgrDialog::__Update()
{
    QString type = "";
    QList<QSharedPointer<Category> > list;
    QStandardItemModel* pModel = nullptr;
    if (ui->expenseRBtn->isChecked()){
        pModel = &m_ExpenseCategoryModel;
        type = TXN_TYPE_EXPENSE;

    } else {
        pModel = &m_IncomeCategoryModel;
        type = TXN_TYPE_INCOME;
    }
    Category::get_all_fullnames(type);
    while(pModel->rowCount() > 0){
        auto tmp = pModel->takeRow(0);
        tmp.clear();
    }

    list = Category::get_all_by_type(type);
    QList<QStandardItem*> items = Category::ParseCategoriesToItems("", list);
    if (items.size() > 0){
        for(int iLoop = 0; iLoop < items.size(); iLoop++){
            pModel->appendRow(items[iLoop]);
        }
    }
    ui->categoryTreeView->setModel(pModel);
}


void CategoryMgrDialog::__DeleteCategory(QStandardItem *item)
{
    if(item == nullptr){
        return;
    }
    while (item->rowCount() > 0){
        auto subitems = item->takeRow(0);
        if (subitems.size() >  0){
            __DeleteCategory(subitems[0]);
            Category::del_from_local(subitems[0]->data().toMap()["id"].toString());
        }
    }
    Category::del_from_local(item->data().toMap()["id"].toString());
}

void CategoryMgrDialog::__UpdateTypeCategory(QStandardItem *item, const QString& type)
{
    if(item == nullptr){
        return;
    }
    while (item->rowCount() > 0){
        auto subitems = item->takeRow(0);
        if (subitems.size() >  0){
            __UpdateTypeCategory(subitems[0], type);
            Category::update_type_to_local(subitems[0]->data().toMap()["id"].toString(), type);
        }
    }
    Category::update_type_to_local(item->data().toMap()["id"].toString(), type);
}


void CategoryMgrDialog::on_addBtn_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("add category"),
                                         tr("category name:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty()){
        Category* pCategory = new Category();
        pCategory->id = QUuid::createUuid().toString().remove("{").remove("}");
        QStandardItemModel* pModel = nullptr;
        if (ui->expenseRBtn->isChecked()){
            pModel = &m_ExpenseCategoryModel;
            pCategory->type = TXN_TYPE_EXPENSE;

        } else {
            pModel = &m_IncomeCategoryModel;
            pCategory->type = TXN_TYPE_INCOME;
        }
        pCategory->name = text;
        QModelIndex idx = ui->categoryTreeView->currentIndex();
        if(!idx.isValid()){
            qDebug("no current index");
            pCategory->parent = "";
        } else {
            auto item = pModel->itemFromIndex(idx);
            if (item == nullptr){
                delete pCategory;
                QMessageBox::warning(this, "add category", "add category failed, because can't find selected item");
                return;
            }
            qDebug("current =%s", item->text().toStdString().c_str());
            pCategory->parent = item->data().toMap()["id"].toString();
        }
        Category::add_to_local(pCategory);
        delete pCategory;
    }
    __Update();
}

void CategoryMgrDialog::on_delBtn_clicked()
{
    QStandardItemModel* pModel = nullptr;
    if (ui->expenseRBtn->isChecked()){
        pModel = &m_ExpenseCategoryModel;

    } else {
        pModel = &m_IncomeCategoryModel;
    }
    QModelIndex idx = ui->categoryTreeView->currentIndex();
    if(idx.isValid()){
        auto item = pModel->itemFromIndex(idx);
        if(item != nullptr){
            QString msg = "Are you sure you want to permanently delete '";
            if (item->rowCount() > 0){
                msg += item->text() + "' and all it's sub category?";
            } else {
                msg += item->text() + "'?";
            }
            if (QMessageBox::question(this, "delete category", msg) == QMessageBox::Yes){
                __DeleteCategory(item);
                __Update();
            }
        }
    }
}

void CategoryMgrDialog::__OnCategoryTypeChanged()
{
    qDebug("__OnCategoryTypeChanged");
    __Update();
}

void CategoryMgrDialog::on_editBtn_clicked()
{
    QStandardItemModel* pModel = nullptr;
    if (ui->expenseRBtn->isChecked()){
        pModel = &m_ExpenseCategoryModel;

    } else {
        pModel = &m_IncomeCategoryModel;
    }
    QModelIndex idx = ui->categoryTreeView->currentIndex();
    if(idx.isValid()){
        auto item = pModel->itemFromIndex(idx);
        if(item != nullptr){
            CategoryEditDialog* pEditDialog = new CategoryEditDialog(item->text(), item->data().toMap()["type"].toString(), this);
            if (pEditDialog->exec() == QDialog::Accepted){
                if (item->text() != pEditDialog->name()){
                    Category* pCategory = new Category();
                    pCategory->id = item->data().toMap()["id"].toString();
                    pCategory->parent = item->data().toMap()["parent"].toString();
                    pCategory->flags = item->data().toMap()["flags"].toInt();
                    pCategory->name = pEditDialog->name();
                    pCategory->type = item->data().toMap()["type"].toString();
                    Category::add_to_local(pCategory);
                    delete pCategory;
                }
            }
            delete pEditDialog;
            __Update();
        }
    }
}


CategoryEditDialog::CategoryEditDialog(const QString& name, const QString& type, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::CategoryEditDialog)
{
      ui->setupUi(this);
    ui->nameEdit->setText(name);
}

CategoryEditDialog::~CategoryEditDialog()
{

}

QString CategoryEditDialog::name()
{
    return ui->nameEdit->text();
}
