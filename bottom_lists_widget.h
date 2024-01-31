#ifndef BOTTOM_LISTS_WIDGET_H
#define BOTTOM_LISTS_WIDGET_H

#include <QWidget>
#include <QGroupBox>

namespace Ui {
class BottomListsWidget;
}

class BottomListsWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit BottomListsWidget(QWidget *parent = nullptr);
    ~BottomListsWidget();

private slots:
    void on_scheduledBtn_clicked();

    void on_futureBtn_clicked();

    void on_remindBtn_clicked();

private:
    Ui::BottomListsWidget *ui;
};

#endif // BOTTOM_LISTS_WIDGET_H
