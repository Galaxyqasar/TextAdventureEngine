#include "linedialog.h"
#include "ui_linedialog.h"

LineDialog::LineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LineDialog)
{
    ui->setupUi(this);
}

LineDialog::~LineDialog()
{
    delete ui;
}

QString LineDialog::getText()
{
    return ui->lineEdit->text();
}
