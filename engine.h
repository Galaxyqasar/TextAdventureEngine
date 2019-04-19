#ifndef ENGINE_H
#define ENGINE_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QBoxLayout>
#include <QWidget>
#include <QTimer>

#include "level.h"
#include "leveleditor.h"
#include "numberdialog.h"
#include "textdisplay.h"

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
    void updateLevel(int index = -1);
    void showNextText();
    void showQuestion();
    
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
    TextDisplay *textDisplay;
    NumberDialog *numberDialog;
    
    QVBoxLayout *mainLayout;
    
    bool paused = false;
    int position, currentText;
    QTimer *gameTimer, *textTimer;
    QList<Level*> levels;
    Level *current;
    QMediaPlayer *music;
};

#endif // ENGINE_H
