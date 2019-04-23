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
    if(index >= 0)
        position = current->actions[index].nextIndex;
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
    current = levels[position];
    ui->action1->setText(current->actions[0].text);
    ui->action2->setText(current->actions[1].text);
    ui->action3->setText(current->actions[2].text);
    ui->imageDisplay->setPixmap(QPixmap::fromImage(current->background));
    music->stop();
    music->setMedia(QUrl(current->sound));
    music->play();
    currentText = 0;
    showNextText();
    ui->progressBar->setValue(100);
    ui->progressBar->setVisible(false);
    ui->action1->setVisible(false);
    ui->action2->setVisible(false);
    ui->action3->setVisible(false);
}

void Engine::showNextText()
{
    if(currentText == current->lines.length()){
        showQuestion();
        gameTimer->start();
    }
    else{
        QString text = current->lines[currentText].text;
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
    ui->action1->setVisible(true);
    ui->action2->setVisible(true);
    ui->action3->setVisible(true);
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
    ui->menuBar->hide();
    ui->mainToolBar->hide();
    
    QSizePolicy spRetain = ui->progressBar->sizePolicy();
    spRetain.setRetainSizeWhenHidden(true);
    ui->progressBar->setSizePolicy(spRetain);
    
    spRetain = ui->action1->sizePolicy();
    spRetain.setRetainSizeWhenHidden(true);
    ui->action1->setSizePolicy(spRetain);
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
    
    loadLevels({"./level1.json","test.json"});
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

void Engine::on_action1_clicked()
{
    updateLevel(0);
}

void Engine::on_action2_clicked()
{
    updateLevel(1);
}

void Engine::on_action3_clicked()
{
    updateLevel(2);
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
