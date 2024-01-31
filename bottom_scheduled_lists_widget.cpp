#include "bottom_scheduled_lists_widget.h"
#include "ui_bottom_scheduled_lists_widget.h"

BottomScheduledListsWidget::BottomScheduledListsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BottomScheduledListsWidget)
{
    ui->setupUi(this);
}

BottomScheduledListsWidget::~BottomScheduledListsWidget()
{
    delete ui;
}
