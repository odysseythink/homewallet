#include "time_chart_widget.h"
#include "ui_time_chart_widget.h"

TimeChartWidget::TimeChartWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::TimeChartWidget)
{
    ui->setupUi(this);
}

TimeChartWidget::~TimeChartWidget()
{
    delete ui;
}
