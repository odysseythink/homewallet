#ifndef TIME_CHART_WIDGET_H
#define TIME_CHART_WIDGET_H

#include <QWidget>
#include <QGroupBox>

namespace Ui {
class TimeChartWidget;
}

class TimeChartWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit TimeChartWidget(QWidget *parent = nullptr);
    ~TimeChartWidget();

private:
    Ui::TimeChartWidget *ui;
};

#endif // TIME_CHART_WIDGET_H
