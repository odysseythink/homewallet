#ifndef ACCOUNTS_WIDGET_H
#define ACCOUNTS_WIDGET_H

#include <QWidget>
#include <QGroupBox>

namespace Ui {
class AccountSumWidget;
}

class AccountSumWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit AccountSumWidget(QWidget *parent = nullptr);
    ~AccountSumWidget();
    QString GetCurrentAccount();

public slots:
    void Update();

private:
    void __Update();

private:
    Ui::AccountSumWidget *ui;
};

#endif // ACCOUNTS_WIDGET_H
