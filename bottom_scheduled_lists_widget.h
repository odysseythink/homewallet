#ifndef BOTTOM_SCHEDULED_LISTS_WIDGET_H
#define BOTTOM_SCHEDULED_LISTS_WIDGET_H

#include <QWidget>

namespace Ui {
class BottomScheduledListsWidget;
}

class BottomScheduledListsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BottomScheduledListsWidget(QWidget *parent = nullptr);
    ~BottomScheduledListsWidget();

private:
    Ui::BottomScheduledListsWidget *ui;
};

#endif // BOTTOM_SCHEDULED_LISTS_WIDGET_H
