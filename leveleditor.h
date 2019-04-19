#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpacerItem>
#include <QFontDialog>
#include <QTabWidget>
#include <QTimeEdit>
#include <QDateEdit>
#include <QFileDialog>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "numberdialog.h"

class LineWidget : public QWidget{
    Q_OBJECT
public:
    explicit LineWidget(QWidget *parent = nullptr) : QWidget(parent){
        fontButton = new QPushButton(this);
        lineEdit = new QLineEdit(this);
        durationSpinBox = new QDoubleSpinBox(this);
        removeLine = new QPushButton(this);
        removeLine->setText("X");
        removeLine->setMaximumWidth(20);
        fontButton->setText(tr("Schriftart"));
        connect(removeLine, &QPushButton::clicked, this, [&](){
            emit remove();
        });
        connect(fontButton, &QPushButton::clicked, this, [&](){
            bool ok;
            font = QFontDialog::getFont(&ok, font, this, tr("Schriftart auswählen"));
            if(!ok)
                font = QFont("Helvetica [Cronyx]", 10);
        });
        
        mainLayout = new QHBoxLayout(this);
        mainLayout->addWidget(lineEdit);
        mainLayout->addWidget(fontButton);
        mainLayout->addWidget(durationSpinBox);
        mainLayout->addWidget(removeLine);
    }
    QFont getFont(){return font;}
    QString getText(){return lineEdit->text();}
    double getDuration(){return durationSpinBox->value();}
    QJsonObject toJsonObject(){
        QJsonObject object;
        object.insert("text", getText());
        object.insert("font", getFont().toString());
        object.insert("duration", getDuration());
        return object;
    }
    void setJsonObject(QJsonObject object){
        lineEdit->setText(object.value("text").toString());
        durationSpinBox->setValue(object.value("duration").toDouble());
        font.fromString(object.value("font").toString());
    }
    ~LineWidget(){
        delete mainLayout;
        delete lineEdit;
        delete fontButton;
        delete durationSpinBox;
        delete removeLine;
    }
signals:
    void remove();
private:
    QHBoxLayout *mainLayout;
    QLineEdit *lineEdit;
    QPushButton *fontButton;
    QDoubleSpinBox *durationSpinBox;
    QPushButton *removeLine;
    
    QFont font;
};

class ActionWidget : public QWidget{
    Q_OBJECT
public:
    explicit ActionWidget(QWidget *parent = nullptr) : QWidget(parent){
        lineEdit = new QLineEdit(this);
        nextIndexSpinBox = new QSpinBox(this);
        nextIndexSpinBox->setMaximum(INT_MAX);
        nextIndexSpinBox->setMinimum(INT_MIN);
        
        mainLayout = new QHBoxLayout(this);
        mainLayout->addWidget(nextIndexSpinBox);
        mainLayout->addWidget(lineEdit);
    }
    QString getText(){return lineEdit->text();}
    int getNextIndex(){return nextIndexSpinBox->value();}
    QJsonObject toJsonObject(){
        QJsonObject object;
        object.insert("text", getText());
        object.insert("nextIndex", getNextIndex());
        return object;
    }
    void setJsonObject(QJsonObject object){
        lineEdit->setText(object.value("text").toString());
        nextIndexSpinBox->setValue(object.value("nextIndex").toInt());
    }
    ~ActionWidget(){
        delete mainLayout;
        delete lineEdit;
        delete nextIndexSpinBox;
    }
private:
    QHBoxLayout *mainLayout;
    QLineEdit *lineEdit;
    QSpinBox *nextIndexSpinBox;
};

class RessourceWidget : public QWidget{
    Q_OBJECT
public:
    explicit RessourceWidget(QString name, QWidget *parent = nullptr) : QWidget(parent){
        pathEdit = new QLineEdit(this);
        showFileDialog = new QPushButton(this);
        label = new QLabel(this);
        showFileDialog->setText(tr("Datei Öffnen"));
        label->setText(name);
        connect(showFileDialog, &QPushButton::clicked, this, [&](){
            QString path = QFileDialog::getOpenFileName(this);
            pathEdit->setText(path);
        });
        
        mainLayout = new QHBoxLayout(this);
        mainLayout->addWidget(label);
        mainLayout->addWidget(pathEdit);
        mainLayout->addWidget(showFileDialog);
    }
    QString getPath(){return pathEdit->text();}
    void setPath(QString path){pathEdit->setText(path);}
    ~RessourceWidget(){
        delete mainLayout;
        delete pathEdit;
        delete showFileDialog;
    }
private:
    QHBoxLayout *mainLayout;
    QLabel *label;
    QLineEdit *pathEdit;
    QPushButton *showFileDialog;
};

class SceneEditor : public QWidget{
    Q_OBJECT
public:
    explicit SceneEditor(int index = 0, QWidget *parent = nullptr);
    QJsonObject toJsonObjcet();
    void setJsonObject(QJsonObject object);
public slots:
    void addLine();
    void removeLine();
    void setIndex(int index);
    int getIndex();
signals:
    void changeTitle(QString title);
private:
    int index;
    QFont qFont;
    QHBoxLayout *timeLayout;
    QHBoxLayout *questionLayout;
    QVBoxLayout *resourceLayout;
    QVBoxLayout *mainLayout;
    QVBoxLayout *actionLayout;
    ActionWidget *action1, *action2, *action3;
    QPushButton *addLineButton;
    QSpacerItem *spacer;
    QList<LineWidget*> lines;
    QLineEdit *questionEdit;
    QTimeEdit *timeEdit;
    QDateEdit *dateEdit;
    QPushButton *changeIndexButton;
    QPushButton *changeqFont;
    RessourceWidget *music, *background;
    NumberDialog *numberDialog;
};

class LevelEditor : public QTabWidget
{
    Q_OBJECT
public:
    explicit LevelEditor(QWidget *parent = nullptr, int index = 0);
    void newScene(int index = 0);
    void save();
    void load();
public slots:
    void updateTitle(QString title);
private:
    QList<SceneEditor*> scenes; 
};

#endif // LEVELEDITOR_H
