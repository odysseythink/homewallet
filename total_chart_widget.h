#ifndef TOTAL_CHART_WIDGET_H
#define TOTAL_CHART_WIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QDialog>
#include <QRect>
#include <QLabel>
#include <QRadioButton>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QComboBox>

class CustomSlice : public QPieSlice
{
    Q_OBJECT

public:
    CustomSlice(QString label, qreal value);

public:
    QBrush originalBrush();

public Q_SLOTS:
    void showHighlight(bool show);

private:
    QBrush m_originalBrush;
};

class TotalChartWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit TotalChartWidget(QWidget *parent = nullptr);
    ~TotalChartWidget();

private:
    void __Update();

private:
    QChartView* m_iChartView;
    QPieSeries* m_iSeries;
    QComboBox* m_iRangeCBox;
    QComboBox* m_iTypeCBox;
};




#endif // TOTAL_CHART_WIDGET_H
