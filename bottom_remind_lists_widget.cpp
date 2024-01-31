#include "bottom_remind_lists_widget.h"
#include "ui_bottom_remind_lists_widget.h"

BottomRemindListsWidget::BottomRemindListsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BottomRemindListsWidget)
{
    ui->setupUi(this);
}

BottomRemindListsWidget::~BottomRemindListsWidget()
{
    delete ui;
}
