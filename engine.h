#ifndef ENGINE_H
#define ENGINE_H

#include <QMainWindow>
#include <QSound>
#include <QWidget>
#include <QTimer>

#include "level.h"
#include "leveleditor.h"
#include "numberdialog.h"

namespace Ui {
class Engine;
}

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
    
private slots:
    void gameTick();
    void gameOver();
    void loadLevels(QStringList files);
    void updateLevel();
    
    void on_pauseButton_clicked();    
    void on_startButton_clicked();    
    void on_quitButton_clicked();    
    void on_mainMenuButton_clicked();    
    void on_action1_clicked();    
    void on_action2_clicked();    
    void on_action3_clicked();    
    void on_restartButton_clicked();
    
    void on_openLevelEditorButton_clicked();
    
    void on_newSceneButton_clicked();
    
    void on_backToMainMenu_clicked();
    
    void on_SaveLevelButton_clicked();
    
    void on_loadLevelButton_clicked();
    
private:
    Ui::Engine *ui;
    LevelEditor *editor;
    NumberDialog *numberDialog;
    
    bool paused = false;
    int position;
    QTimer *gameTimer;
    QList<Level*> levels;
    Level *current;
    QSound *music;
};

#endif // ENGINE_H
