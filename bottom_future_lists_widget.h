#ifndef BOTTOM_FUTURE_LISTS_WIDGET_H
#define BOTTOM_FUTURE_LISTS_WIDGET_H

#include <QWidget>

namespace Ui {
class BottomFutureListsWidget;
}

class BottomFutureListsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BottomFutureListsWidget(QWidget *parent = nullptr);
    ~BottomFutureListsWidget();

private:
    Ui::BottomFutureListsWidget *ui;
};

#endif // BOTTOM_FUTURE_LISTS_WIDGET_H
