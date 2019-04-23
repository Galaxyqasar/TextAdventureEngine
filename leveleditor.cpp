#include "leveleditor.h"
#include <QDebug>

SceneEditor::SceneEditor(int index, QWidget *parent) : QWidget(parent){
    this->index = index;
    numberDialog = new NumberDialog(this);
    changeIndexButton = new QPushButton(this);
    timeEdit = new QTimeEdit(this);
    dateEdit = new QDateEdit(this);
    
    changeIndexButton->setText(tr("Index Ändern"));
    connect(changeIndexButton, &QPushButton::clicked, this, [&](){
        if(numberDialog->exec()){
            setIndex(numberDialog->getValue());
        }
    });
    
    timeLayout = new QHBoxLayout();
    timeLayout->addWidget(changeIndexButton);
    timeLayout->addWidget(timeEdit);
    timeLayout->addWidget(dateEdit);
    
    music = new RessourceWidget(tr("Musik"), this);
    background = new RessourceWidget(tr("Hintergrund"), this);
    
    addLineButton = new QPushButton(this);
    addLineButton->setText(tr("Neue Zeile"));
    addLineButton->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
    connect(addLineButton, SIGNAL(clicked()), this, SLOT(addLine()));
    
    spacer = new QSpacerItem(20,40,QSizePolicy::Preferred, QSizePolicy::Expanding);
    
    addActionBtn = new QPushButton(this);
    addActionBtn->setText("Neue Action");
    connect(addActionBtn, &QPushButton::clicked, this, [&](){
        addAction();
    });
    
    actionLayout = new QVBoxLayout();
    for(int i = 0; i < 3; i++)
        addAction();
    
    actionLayout->addWidget(addActionBtn);
    
    questionLayout = new QHBoxLayout();
    changeqFont = new QPushButton(this);
    questionEdit = new QLineEdit(this);
    questionLayout->addWidget(questionEdit);
    questionLayout->addWidget(changeqFont);
    changeqFont->setText("Schriftart");
    connect(changeqFont, &QPushButton::clicked, this, [&](){
        bool ok;
        qFont = QFontDialog::getFont(&ok, qFont, this, tr("Schriftart auswählen"));
        if(!ok)
            qFont = QFont("Helvetica [Cronyx]", 10);
    });

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(timeLayout);
    mainLayout->addWidget(music);
    mainLayout->addWidget(background);
    mainLayout->addWidget(addLineButton);
    mainLayout->addItem(spacer);
    mainLayout->addLayout(questionLayout);
    mainLayout->addLayout(actionLayout);
}

QJsonObject SceneEditor::toJsonObjcet()
{
    QJsonObject object;
    object.insert("index", QJsonValue(index));
    object.insert("time", QJsonValue(timeEdit->time().toString()));
    object.insert("date", QJsonValue(dateEdit->date().toString()));
    object.insert("music", QJsonValue(music->getPath()));
    object.insert("background", QJsonValue(background->getPath()));
    object.insert("qFont", QJsonValue(qFont.toString()));
    object.insert("question", QJsonValue(questionEdit->text()));
    
    QJsonArray lines;
    for(LineWidget *e : this->lines){
        lines.append(e->toJsonObject());
    }
    object.insert("lines", lines);    
    
    QJsonArray actions;
    for(ActionWidget *e : this->actions)
        actions.append(e->toJsonObject());
    object.insert("actions", actions);
    
    return object;
}

void SceneEditor::setJsonObject(QJsonObject object)
{
    index = object.value("index").toInt();
    setIndex(index);
    timeEdit->setTime(QTime::fromString(object.value("time").toString()));
    dateEdit->setDate(QDate::fromString(object.value("date").toString()));
    music->setPath(object.value("music").toString());
    background->setPath(object.value("background").toString());
    questionEdit->setText(object.value("question").toString());
    qFont = QFont(object.value("qFont").toString());
    QJsonArray lines = object.value("lines").toArray();
    QJsonArray actions = object.value("actions").toArray();
    
    for(QJsonValueRef e : lines){
        addLine();
        this->lines.last()->setJsonObject(e.toObject());
    }
    for(QJsonValueRef e : actions){
        addAction();
        this->actions.last()->setJsonObject(e.toObject());
    }
}

void SceneEditor::addLine()
{
    LineWidget *newLine = new LineWidget(this);
    newLine->show();
    lines.append(newLine);
    mainLayout->insertWidget(mainLayout->count()-3, newLine);
    connect(newLine, SIGNAL(remove()), this, SLOT(removeLine()));
}

void SceneEditor::addAction()
{
    ActionWidget * newAction = new ActionWidget(this);
    newAction->show();
    actions.append(newAction);
    actionLayout->insertWidget(actionLayout->count()-1, newAction);
    connect(newAction, SIGNAL(remove()), this, SLOT(removeAction()));
}

void SceneEditor::removeLine()
{
    LineWidget *widget = qobject_cast<LineWidget*>(sender());
    lines.removeAll(widget);
    widget->hide();
    mainLayout->removeWidget(widget);
    widget->deleteLater();
}

void SceneEditor::removeAction()
{
    ActionWidget *widget = qobject_cast<ActionWidget*>(sender());
    actions.removeAll(widget);
    widget->hide();
    actionLayout->removeWidget(widget);
    widget->deleteLater();
}

void SceneEditor::setIndex(int index)
{
    this->index = index;
    qDebug()<<this->index;
    emit changeTitle("Index: " + QString::number(index));
}

int SceneEditor::getIndex()
{
    return index;
}

LevelEditor::LevelEditor(QWidget *parent, int index) : QTabWidget(parent){
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setTabsClosable(true);
    connect(this, &QTabWidget::tabCloseRequested, this, [&](int index){
        SceneEditor *current = scenes[index];
        scenes.removeAt(index);
        current->deleteLater();
    });
    newScene(index);
}

void LevelEditor::newScene(int index)
{
    scenes.append(new SceneEditor(index, this));
    connect(scenes.last(), SIGNAL(changeTitle(QString)), this, SLOT(updateTitle(QString)));
    insertTab(this->count()+1, scenes.last(), "Index: " + QString::number(scenes.last()->getIndex()));
}

void LevelEditor::save()
{
    QString path = QFileDialog::getSaveFileName(this);
    if(path.isEmpty())
        return;
    QFile f(path);
    QJsonDocument doc;
    QJsonObject root;
    QJsonArray levels;
    
    for(SceneEditor *e : scenes){
        QJsonObject current = e->toJsonObjcet();
        levels.append(current);
    }
    
    root.insert("levels", levels);
    doc.setObject(root);
    
    f.open(QIODevice::WriteOnly);
    f.write(doc.toJson());
    f.flush();
    f.close();
}

void LevelEditor::load()
{
    for(SceneEditor *e : scenes){
        scenes.removeAll(e);
        e->deleteLater();
    }
    QString path = QFileDialog::getOpenFileName(this);
    if(path.isEmpty())
        return;
    QFile f(path);
    f.open(QIODevice::ReadOnly);
    QByteArray json = f.readAll();
    f.close();
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject root = doc.object();
    QJsonArray levels = root.value("levels").toArray();
    for(QJsonValueRef e : levels){
        newScene();
        scenes.last()->setJsonObject(e.toObject());
    }
}

void LevelEditor::updateTitle(QString title)
{
    SceneEditor *scene = qobject_cast<SceneEditor*>(sender());
    int sceneIndex = scenes.indexOf(scene)+1;
    qDebug()<<"updated title"<<title<<sceneIndex;
    setTabText(sceneIndex, title);
}
