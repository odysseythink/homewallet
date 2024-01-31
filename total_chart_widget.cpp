#include "total_chart_widget.h"
#include "home_wallet.h"
#include <QMenu>
#include <QPainter>
#include <Qt>
#include <QPen>
#include <QList>

CustomSlice::CustomSlice(QString label, qreal value)
    : QPieSlice(label, value)
{
    connect(this, &CustomSlice::hovered, this, &CustomSlice::showHighlight);
}

QBrush CustomSlice::originalBrush()
{
    return m_originalBrush;
}

void CustomSlice::showHighlight(bool show)
{
    if (show) {
        QBrush brush = this->brush();
        m_originalBrush = brush;
        brush.setColor(brush.color().lighter());
        setBrush(brush);
    } else {
        setBrush(m_originalBrush);
    }
}

QList<int> rangeIdxs = {
    FLT_RANGE_LAST_DAY       ,
    FLT_RANGE_THIS_DAY       ,
    FLT_RANGE_NEXT_DAY       ,

    FLT_RANGE_LAST_WEEK      ,
    FLT_RANGE_THIS_WEEK      ,
    FLT_RANGE_NEXT_WEEK      ,

    FLT_RANGE_LAST_FORTNIGHT ,
    FLT_RANGE_THIS_FORTNIGHT ,
    FLT_RANGE_NEXT_FORTNIGHT ,

    FLT_RANGE_LAST_MONTH     ,
    FLT_RANGE_THIS_MONTH     ,
    FLT_RANGE_NEXT_MONTH     ,

    FLT_RANGE_LAST_QUARTER   ,
    FLT_RANGE_THIS_QUARTER   ,
    FLT_RANGE_NEXT_QUARTER   ,

    FLT_RANGE_LAST_YEAR      ,
    FLT_RANGE_THIS_YEAR      ,
    FLT_RANGE_NEXT_YEAR      ,

    FLT_RANGE_LAST_30DAYS    ,
    FLT_RANGE_LAST_60DAYS    ,
    FLT_RANGE_LAST_90DAYS    ,

    FLT_RANGE_LAST_12MONTHS  ,
    FLT_RANGE_MISC_30DAYS    ,
    FLT_RANGE_MISC_ALLDATE   ,
};

TotalChartWidget::TotalChartWidget(QWidget *parent) :
    QGroupBox(parent),m_iChartView(nullptr)
{
    QVBoxLayout* pMainLayout = new QVBoxLayout();

    // create chart
    QChart *chart = new QChart;
//    chart->setTitle("Piechart customization");
    chart->setAnimationOptions(QChart::AllAnimations);

    // create series
    m_iSeries = new QPieSeries();
    m_iSeries->setLabelsVisible();
    chart->addSeries(m_iSeries);
    // create chart view
    m_iChartView = new QChartView(chart);
    pMainLayout->addWidget(m_iChartView);

    QHBoxLayout* pBottomLayout = new QHBoxLayout();
    QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    pBottomLayout->addSpacerItem(spacer);
    pBottomLayout->addWidget(new QLabel("Range:"));
    m_iRangeCBox = new QComboBox();
    for (auto idx : rangeIdxs) {
        if (HomeWallet::CYA_FLT_RANGE_DWF.contains(idx)){
            m_iRangeCBox->addItem(HomeWallet::CYA_FLT_RANGE_DWF[idx], idx);
        }
    }
    pBottomLayout->addWidget(m_iRangeCBox);
    pBottomLayout->addWidget(new QLabel("Type:"));
    m_iTypeCBox = new QComboBox();
    m_iTypeCBox->addItem(tr("Category"));
    m_iTypeCBox->addItem(tr("Payee")   );
    m_iTypeCBox->addItem(tr("Account") );
    m_iTypeCBox->addItem(tr("Balance") );
    pBottomLayout->addWidget(m_iTypeCBox);
    pMainLayout->addLayout(pBottomLayout);
    setLayout(pMainLayout);
    __Update();
}

TotalChartWidget::~TotalChartWidget()
{
}

void TotalChartWidget::__Update()
{
    m_iSeries->clear();
    QMap<QString, QString> list = Category::get_all_fullnames("");
    QMap<QString, QString>::iterator iter = list.begin();
    while (iter != list.end()) {
        printf("[%s %s:%d]\n", __FILE__, __FUNCTION__, __LINE__);
        auto cat = Preferences::get_item<Category>(iter.key());
        printf("[%s %s:%d]\n", __FILE__, __FUNCTION__, __LINE__);
        if (cat != nullptr){

            delete cat;
        }
        ++iter;
    }
    *m_iSeries << new CustomSlice("Slice 1", 10.0);
    *m_iSeries << new CustomSlice("Slice 2", 20.0);
    *m_iSeries << new CustomSlice("Slice 3", 30.0);
    *m_iSeries << new CustomSlice("Slice 4", 40.0);
    *m_iSeries << new CustomSlice("Slice 5", 50.0);
}

