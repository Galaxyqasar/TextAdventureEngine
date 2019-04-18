#include <QTabBar>

#include "engine.h"
#include "ui_engine.h"

Engine::Engine(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Engine)
{
    ui->setupUi(this);
    editor = new LevelEditor(ui->levelEditorTab);
    numberDialog = new NumberDialog(this);
    ui->levelEditorLayout->insertWidget(0,editor);
    ui->tabWidget->tabBar()->hide();
    ui->tabWidget->setCurrentIndex(mainMenu);
    ui->statusBar->hide();
    ui->menuBar->hide();
    ui->mainToolBar->hide();
    gameTimer = new QTimer();
    gameTimer->setInterval(100);
    connect(gameTimer, SIGNAL(timeout()), this, SLOT(gameTick()));
    loadLevels({"./level1.json"});
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

void Engine::loadLevels(QStringList files)
{
    for(QString e : files){
        QList<Level*> file = Level::loadFile(e);
        levels.append(file);
    }
}

void Engine::updateLevel()
{
    if(levels.length() <= position){
        qDebug()<<"an error occured";
        qApp->quit();
        return;
    }
    current = levels[position];
    qDebug()<<current->background;
    ui->imageDisplay->setPixmap(QPixmap::fromImage(current->background));
    music = new QSound(current->sound);
    music->setLoops(QSound::Infinite);
    music->play();
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
    paused = false;
    position = 0;
    ui->tabWidget->setCurrentIndex(mainGame);
    ui->progressBar->setValue(100);
    updateLevel();
    gameTimer->start();
}

void Engine::on_quitButton_clicked()
{
    qApp->quit();
}

void Engine::on_mainMenuButton_clicked()
{
    ui->tabWidget->setCurrentIndex(mainMenu);
}

void Engine::on_action1_clicked()
{
    
}

void Engine::on_action2_clicked()
{
    
}

void Engine::on_action3_clicked()
{
    
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
