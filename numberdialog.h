#ifndef NUMBERDIALOG_H
#define NUMBERDIALOG_H

#include <QDialog>

namespace Ui {
class NumberDialog;
}

class NumberDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NumberDialog(QWidget *parent = nullptr);
    ~NumberDialog();
    int getValue();
    
private slots:
    void on_buttonBox_accepted();    
    void on_buttonBox_rejected();
    
private:
    Ui::NumberDialog *ui;
};

#endif // NUMBERDIALIG_H
