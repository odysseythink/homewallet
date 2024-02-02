#include "help_welcome_dialog.h"
#include "ui_help_welcome_dialog.h"
#include "hw-preferences.h"

HelpWelcomeDialog::HelpWelcomeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpWelcomeDialog)
{
    ui->setupUi(this);
    Preferences::Instance()->set_showwelcome(ui->m_iShowCheckBox->isChecked());
}

HelpWelcomeDialog::~HelpWelcomeDialog()
{
    delete ui;
}

void HelpWelcomeDialog::__OnWelcomsBtnClicked()
{
    QPushButton* pBtn = qobject_cast<QPushButton*>(sender());
    if (pBtn != nullptr){
        if (pBtn == ui->m_iOpenLastBtn) {
            setResult(WELCOME_OPENLAST);
        } else if(pBtn == ui->m_iCreateNewBtn) {
            setResult(WELCOME_CREATENEW);
        } else if(pBtn == ui->m_iOpenExistingBtn) {
            setResult(WELCOME_OPENEXISTING);
        } else if(pBtn == ui->m_iOpenExampleBtn) {
            setResult(WELCOME_OPENSAMPLE);
        } else if(pBtn == ui->m_iReadManualBtn) {
            setResult(WELCOME_READMANUAL);
        } else if(pBtn == ui->m_iConfigPrefBtn) {
            setResult(WELCOME_CONFIGPREF);
        }
    } else {
        QCheckBox* pCheckBox = qobject_cast<QCheckBox*>(sender());
        if (pCheckBox == ui->m_iShowCheckBox) {
            Preferences::Instance()->set_showwelcome(ui->m_iShowCheckBox->isChecked());
        }
    }
}
