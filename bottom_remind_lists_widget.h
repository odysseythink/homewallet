#ifndef BOTTOM_REMIND_LISTS_WIDGET_H
#define BOTTOM_REMIND_LISTS_WIDGET_H

#include <QWidget>

namespace Ui {
class BottomRemindListsWidget;
}

class BottomRemindListsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BottomRemindListsWidget(QWidget *parent = nullptr);
    ~BottomRemindListsWidget();

private:
    Ui::BottomRemindListsWidget *ui;
};

#endif // BOTTOM_REMIND_LISTS_WIDGET_H
