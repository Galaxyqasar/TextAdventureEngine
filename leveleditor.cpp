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
    
    action1 = new ActionWidget(this);
    action2 = new ActionWidget(this);
    action3 = new ActionWidget(this);
    
    actionLayout = new QVBoxLayout();
    actionLayout->addWidget(action1);
    actionLayout->addWidget(action2);
    actionLayout->addWidget(action3);
    
    questionEdit = new QLineEdit(this);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(timeLayout);
    mainLayout->addWidget(music);
    mainLayout->addWidget(background);
    mainLayout->addWidget(addLineButton);
    mainLayout->addItem(spacer);
    mainLayout->addWidget(questionEdit);
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
    
    QJsonArray lines;
    for(LineWidget *e : this->lines){
        lines.append(e->toJsonObject());
    }
    object.insert("lines", lines);
    object.insert("question", QJsonValue(questionEdit->text()));
    
    QJsonArray actions;
    actions.append(action1->toJsonObject());
    actions.append(action2->toJsonObject());
    actions.append(action3->toJsonObject());
    object.insert("actions", actions);
    
    return object;
}

void SceneEditor::addLine()
{
    LineWidget *newLine = new LineWidget(this);
    newLine->show();
    lines.append(newLine);
    mainLayout->insertWidget(mainLayout->count()-3, newLine);
    connect(newLine, SIGNAL(remove()), this, SLOT(removeLine()));
}

void SceneEditor::removeLine()
{
    LineWidget *widget = qobject_cast<LineWidget*>(sender());
    lines.removeAll(widget);
    widget->hide();
    mainLayout->removeWidget(widget);
    widget->deleteLater();
}

void SceneEditor::setIndex(int index)
{
    this->index = index;
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
    addTab(scenes.last(), "Index: " + QString::number(scenes.last()->getIndex()));
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

void LevelEditor::updateTitle(QString title)
{
    setTabText(currentIndex(), title);
}
