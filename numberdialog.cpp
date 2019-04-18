#include "numberdialog.h"
#include "ui_numberdialog.h"

NumberDialog::NumberDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NumberDialog)
{
    ui->setupUi(this);
    ui->spinBox->setMaximum(INT_MAX);
    ui->spinBox->setMinimum(INT_MIN);
}

NumberDialog::~NumberDialog()
{
    delete ui;
}

void NumberDialog::on_buttonBox_accepted()
{
    accept();
}

void NumberDialog::on_buttonBox_rejected()
{
    reject();
}

int NumberDialog::getValue()
{
    return ui->spinBox->value();
}
