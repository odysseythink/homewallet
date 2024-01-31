#ifndef HELP_WELCOME_DIALOG_H
#define HELP_WELCOME_DIALOG_H

#include <QDialog>

namespace Ui {
class HelpWelcomeDialog;
}

class HelpWelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpWelcomeDialog(QWidget *parent = nullptr);
    ~HelpWelcomeDialog();
    enum {
        WELCOME_OPENLAST = 2,
        WELCOME_CREATENEW,
        WELCOME_OPENEXISTING,
        WELCOME_OPENSAMPLE,
        WELCOME_READMANUAL,
        WELCOME_CONFIGPREF,
    };

private:
    void __OnWelcomsBtnClicked();

private:
    Ui::HelpWelcomeDialog *ui;
};

#endif // HELP_WELCOME_DIALOG_H
