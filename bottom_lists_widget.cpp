#include "bottom_lists_widget.h"
#include "ui_bottom_lists_widget.h"

BottomListsWidget::BottomListsWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::BottomListsWidget)
{
    ui->setupUi(this);
}

BottomListsWidget::~BottomListsWidget()
{
    delete ui;
}

void BottomListsWidget::on_scheduledBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void BottomListsWidget::on_futureBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void BottomListsWidget::on_remindBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
