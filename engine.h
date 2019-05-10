#ifndef ENGINE_H
#define ENGINE_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QBoxLayout>
#include <QWidget>
#include <QTimer>
#include <QDir>
#include <QDirIterator>

#include "level.h"
#include "leveleditor.h"
#include "numberdialog.h"
#include "textdisplay.h"

namespace Ui {
class Engine;
}

class ActionDisplay : public QWidget{
    Q_OBJECT
public:
    explicit ActionDisplay(QWidget *parent = nullptr, QStringList buttonNames = {});
    ~ActionDisplay();
    void setActions(QStringList buttonNames);
    void setActions(QList<Action> actions);
    
signals:
    void actionPressed(int index);
    
private:
    QList<QPushButton*> actions;
    QHBoxLayout *mainLayout;
};

class Engine : public QMainWindow
{
    Q_OBJECT
    enum Tabs{
        mainMenu = 0,
        pauseMenu,
        mainGame,
        gameOverScreen,
        levelEditor
    };
    
public:
    explicit Engine(QWidget *parent = nullptr);
    ~Engine();
    
    void loadLevels(QString filename);
    void loadLevels(QStringList files);
    
private slots:
    void gameTick();
    void showNextText();
    
    void on_pauseButton_clicked();    
    void on_startButton_clicked();    
    void on_quitButton_clicked();    
    void on_mainMenuButton_clicked();
    void on_restartButton_clicked();    
    void on_openLevelEditorButton_clicked();    
    void on_newSceneButton_clicked();    
    void on_backToMainMenu_clicked();    
    void on_SaveLevelButton_clicked();    
    void on_loadLevelButton_clicked();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

private:
    void gameOver();
    void updateLevel(int index = -1);
    void showQuestion();
    void patchUi();
    
    
    Ui::Engine *ui;
    LevelEditor *editor;
    TextDisplay *textDisplay;
    NumberDialog *numberDialog;
    ActionDisplay *actionDisplay;
    
    QVBoxLayout *mainLayout;
    
    bool paused = false;
    int position, currentText;
    QTimer *gameTimer, *textTimer;
    QList<Level*> levels;
    Level *current;
    QMediaPlayer *music;
};

#endif // ENGINE_H
