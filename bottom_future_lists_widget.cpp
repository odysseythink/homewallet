#include "bottom_future_lists_widget.h"
#include "ui_bottom_future_lists_widget.h"

BottomFutureListsWidget::BottomFutureListsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BottomFutureListsWidget)
{
    ui->setupUi(this);
}

BottomFutureListsWidget::~BottomFutureListsWidget()
{
    delete ui;
}
