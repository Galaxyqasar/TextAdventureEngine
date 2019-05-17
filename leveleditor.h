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
#include <QTextEdit>

#include "numberdialog.h"

class LineWidget : public QWidget{
    Q_OBJECT
public:
    explicit LineWidget(QWidget *parent = nullptr) : QWidget(parent){
        fontButton = new QPushButton(this);
        textEdit = new QLineEdit(this);
        durationSpinBox = new QDoubleSpinBox(this);
        removeLine = new QPushButton(this);
        removeLine->setIcon(QIcon(":/darkstyle/icon_close.png"));
        removeLine->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
        fontButton->setText(tr("Schriftart"));
        connect(removeLine, &QPushButton::clicked, this, [&](){
            emit remove();
        });
        connect(fontButton, &QPushButton::clicked, this, [&](){
            bool ok;
            font = QFontDialog::getFont(&ok, font, this, tr("Schriftart auswählen"));
            if(!ok)
                font = QFont("Helvetica [Cronyx]", 10);
            textEdit->setFont(font);
        });
        
        mainLayout = new QHBoxLayout(this);
        mainLayout->addWidget(textEdit);
        mainLayout->addWidget(fontButton);
        mainLayout->addWidget(durationSpinBox);
        mainLayout->addWidget(removeLine);
    }
    QFont getFont(){return font;}
    QString getText(){return textEdit->text();}
    double getDuration(){return durationSpinBox->value();}
    QJsonObject toJsonObject(){
        QJsonObject object;
        object.insert("text", getText());
        object.insert("font", getFont().toString());
        object.insert("duration", getDuration());
        return object;
    }
    void setJsonObject(QJsonObject object){
        textEdit->setText(object.value("text").toString());
        durationSpinBox->setValue(object.value("duration").toDouble());
        font.fromString(object.value("font").toString());
    }
    ~LineWidget(){
        delete mainLayout;
        delete textEdit;
        delete fontButton;
        delete durationSpinBox;
        delete removeLine;
    }
signals:
    void remove();
private:
    QHBoxLayout *mainLayout;
    QLineEdit *textEdit;
    QPushButton *fontButton;
    QDoubleSpinBox *durationSpinBox;
    QPushButton *removeLine;
    
    QFont font;
};

class ScoreWidget : public QFrame{
    Q_OBJECT
public:
    explicit ScoreWidget(QWidget *parent = nullptr, QString name = "", int v = 0) : QFrame(parent){
        nameDisplay = new QLabel(name, this);
        value = new QSpinBox(this);
        value->setValue(v);
        layout = new QHBoxLayout(this);
        layout->addWidget(nameDisplay);
        layout->addWidget(value);
    }
    QString getName(){
        return  nameDisplay->text();
    }
    int getValue(){
        return value->value();
    }
    ~ScoreWidget(){
        delete nameDisplay;
        delete value;
        delete layout;
    }
private:
    QLabel *nameDisplay;
    QSpinBox *value;
    QHBoxLayout *layout;
};

class ActionWidget : public QWidget{
    Q_OBJECT
public:
    explicit ActionWidget(QWidget *parent = nullptr) : QWidget(parent){
        lineEdit = new QLineEdit(this);
        nextIndexSpinBox = new QSpinBox(this);
        nextIndexSpinBox->setMaximum(INT_MAX);
        nextIndexSpinBox->setMinimum(INT_MIN);
        removeBtn = new QPushButton(this);
        removeBtn->setIcon(QIcon(":/darkstyle/icon_close.png"));
        removeBtn->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
        connect(removeBtn, &QPushButton::clicked, this, [&](){
            emit remove();
        });
        
        mainLayout = new QHBoxLayout(this);
        mainLayout->addWidget(nextIndexSpinBox);
        mainLayout->addWidget(lineEdit);
        mainLayout->addWidget(removeBtn);
    }
    QString getText(){return lineEdit->text();}
    int getNextIndex(){return nextIndexSpinBox->value();}
    QJsonObject toJsonObject(){
        QJsonObject object;
        object.insert("text", getText());
        object.insert("nextIndex", getNextIndex());
        QJsonObject scoresObject;
        for(ScoreWidget *w : scores){
            scoresObject.insert(w->getName(), w->getValue());
        }
        object.insert("scores", scoresObject);
        return object;
    }
    void setJsonObject(QJsonObject object){
        lineEdit->setText(object.value("text").toString());
        nextIndexSpinBox->setValue(object.value("nextIndex").toInt());
        QJsonObject scoresObject = object.value("scores").toObject();
        for(QString key : scoresObject.keys()){
            addScore(key, scoresObject.value(key).toInt());
        }
    }
    void addScore(QString score, int value = 0){
        scores.append(new ScoreWidget(this, score, value));
        mainLayout->insertWidget(mainLayout->count()-1, scores.last());
    }
    ~ActionWidget(){
        delete mainLayout;
        delete lineEdit;
        delete nextIndexSpinBox;
    }
signals:
    void remove();
private:
    QHBoxLayout *mainLayout;
    QLineEdit *lineEdit;
    QSpinBox *nextIndexSpinBox;
    QPushButton *removeBtn;
    QList<ScoreWidget*> scores;
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
    void addAction();
    void removeLine();
    void removeAction();
    void setIndex(int index);
    int getIndex();
    void addScore(QString score);
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
    QList<ActionWidget*> actions;
    ActionWidget *action1, *action2, *action3;
    QPushButton *addLineButton;
    QSpacerItem *spacer;
    QList<LineWidget*> lines;
    QLineEdit *questionEdit;
    QTimeEdit *timeEdit;
    QDateEdit *dateEdit;
    QPushButton *changeIndexButton;
    QPushButton *changeqFont;
    QPushButton *addActionBtn;
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
    void quickSave();
    void load();
    void updateScores(QStringList scores);
public slots:
    void updateTitle(QString title);
private:
    QString file;
    QStringList scores;
    QList<SceneEditor*> scenes; 
};

#endif // LEVELEDITOR_H
