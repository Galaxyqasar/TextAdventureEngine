#include <QTabBar>

#include "engine.h"
#include "ui_engine.h"

Engine::Engine(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Engine)
{
    ui->setupUi(this);
    patchUi();
    textTimer = new QTimer();
    gameTimer = new QTimer();
    textTimer->setSingleShot(true);
    gameTimer->setInterval(100);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(gameTick()));
    connect(textTimer, SIGNAL(timeout()), this, SLOT(showNextText()));
    
    music = new QMediaPlayer();
}

Engine::~Engine()
{
    delete ui;
    for(Level *e : levels)
        delete e;
}

void Engine::gameTick()
{
    if(current->actions.length() == 1){
        if(current->actions.last().nextIndex == -1)
            gameOver();
    }
    int progress = ui->progressBar->value();
    if(progress > 0){
        progress--;
        ui->progressBar->setValue(progress);
    }
    else
        gameOver();
}

void Engine::gameOver()
{
    gameTimer->stop();
    ui->tabWidget->setCurrentIndex(gameOverScreen);
    music->stop();
}
void Engine::loadLevels(QString filename)
{
    QList<Level*> levels = Level::loadFile(filename);
    this->levels.append(levels);
}

void Engine::loadLevels(QStringList files)
{
    for(QString e : files){
        loadLevels(e);
    }
}

void Engine::updateLevel(int index)
{
    gameTimer->stop();
    if(index >= 0)
        position = current->actions[index].nextIndex;
    if(position == -1){
        gameOver();
        return;
    }
    bool found = false;
    for(Level *e : levels){
        if(e->index == position){
            current = e;
            found = true;
            break;
        }
    }
    if(!found){
        qDebug()<<"an error occured";
        qApp->quit();
        return;
    }
    ui->imageDisplay->setPixmap(QPixmap::fromImage(current->background));
    music->stop();
    music->setMedia(QUrl(current->sound));
    music->play();
    currentText = 0;
    showNextText();
    ui->progressBar->setValue(100);
    ui->progressBar->setVisible(false);
    actionDisplay->setActions(current->actions);
    actionDisplay->setVisible(false);
}

void Engine::showNextText()
{
    if(currentText == current->lines.length()){
        showQuestion();
        gameTimer->start();
    }
    else{
        QString text = current->lines[currentText].text;
        //text.replace("<br>", "\n");
        QFont font = current->lines[currentText].font;
        textDisplay->updateText(text, font);
        int duration = int(current->lines[currentText].duration*1000);
        textTimer->start(duration);
    }
    currentText++;
}

void Engine::showQuestion()
{
    ui->progressBar->setVisible(true);
    actionDisplay->setVisible(true);
    ui->progressBar->setValue(100);
    textDisplay->updateText(current->question, current->qFont);
}

void Engine::patchUi()
{
    mainLayout = new QVBoxLayout(ui->imageDisplay);
    editor = new LevelEditor(ui->levelEditorTab);
    textDisplay = new TextDisplay(ui->imageDisplay);
    numberDialog = new NumberDialog(this);
    mainLayout->addWidget(textDisplay);
    ui->levelEditorLayout->insertWidget(0,editor);
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setCurrentIndex(mainMenu);
    ui->statusBar->hide();
    ui->mainToolBar->hide();
    
    actionDisplay = new ActionDisplay(ui->mainGame, {"action1","action2","action3"});
    connect(actionDisplay, &ActionDisplay::actionPressed, this, [&](int index){
        updateLevel(index);
    });
    
    ui->mainGame->layout()->addWidget(actionDisplay);
    QSizePolicy spRetain = ui->progressBar->sizePolicy();
    spRetain.setRetainSizeWhenHidden(true);
    ui->progressBar->setSizePolicy(spRetain);
}

void Engine::on_pauseButton_clicked()
{
    if(!paused){
        ui->tabWidget->setCurrentIndex(pauseMenu);
        ui->pauseButton->setText(tr("Fortfahren"));
        gameTimer->stop();
    }
    else{
        ui->tabWidget->setCurrentIndex(mainGame);
        ui->pauseButton->setText(tr("Pause"));
        gameTimer->start();
    }
    paused = !paused;
}

void Engine::on_startButton_clicked()
{
    QDir current = QDir::current();
    QString path = current.absolutePath();
    QDirIterator iter(path, QStringList()<<"*.json", QDir::Files);
    QStringList files;
    while(iter.hasNext())
        files.append(iter.next());
    qDebug()<<files;
    
    loadLevels(files);
    paused = false;
    position = 0;
    ui->tabWidget->setCurrentIndex(mainGame);
    ui->progressBar->setValue(100);
    updateLevel();
}

void Engine::on_quitButton_clicked()
{
    qApp->quit();
}

void Engine::on_mainMenuButton_clicked()
{
    music->stop();
    gameTimer->stop();
    textTimer->stop();
    ui->tabWidget->setCurrentIndex(mainMenu);
}

void Engine::on_restartButton_clicked()
{
    on_startButton_clicked();
}

void Engine::on_openLevelEditorButton_clicked()
{
    ui->tabWidget->setCurrentIndex(levelEditor);
}

void Engine::on_newSceneButton_clicked()
{
    if(numberDialog->exec()){
        int index = numberDialog->getValue();
        editor->newScene(index);
    }
}

void Engine::on_backToMainMenu_clicked()
{
    ui->tabWidget->setCurrentIndex(mainMenu);
}

void Engine::on_SaveLevelButton_clicked()
{
    editor->save();
}

void Engine::on_loadLevelButton_clicked()
{
    editor->load();
}

ActionDisplay::ActionDisplay(QWidget *parent, QStringList buttonNames) : QWidget (parent)
{
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);

    for(QString e : buttonNames){
        actions.append(new QPushButton(this));
        actions.last()->setText(e);
        actions.last()->show();
        mainLayout->addWidget(actions.last());
        connect(actions.last(), &QPushButton::clicked, this, [&](){
            QPushButton *send = qobject_cast<QPushButton*>(sender());
            emit actionPressed(actions.indexOf(send));
        });
    }
    QSizePolicy spRetain = sizePolicy();
    spRetain.setRetainSizeWhenHidden(true);
    setSizePolicy(spRetain);
}

ActionDisplay::~ActionDisplay()
{
    for(QPushButton *e : actions)
        e->deleteLater();
}

void ActionDisplay::setActions(QStringList buttonNames)
{
    for(QPushButton *e : actions)
        e->deleteLater();
    actions.clear();
    
    for(QString e : buttonNames){
        actions.append(new QPushButton(this));
        actions.last()->setText(e);
        mainLayout->addWidget(actions.last());
        connect(actions.last(), &QPushButton::clicked, this, [&](){
            QPushButton *send = qobject_cast<QPushButton*>(sender());
            emit actionPressed(actions.indexOf(send));
        });
        QSizePolicy spRetain = actions.last()->sizePolicy();
        spRetain.setRetainSizeWhenHidden(true);
        actions.last()->setSizePolicy(spRetain);
    }
}

void ActionDisplay::setActions(QList<Action> actions){
    QStringList buttonNames;
    for(Action e : actions){
        buttonNames.append(e.text);
    }
    setActions(buttonNames);
}

void Engine::on_actionSave_triggered()
{
    editor->quickSave();
}

void Engine::on_actionOpen_triggered()
{
    editor->load();
}
