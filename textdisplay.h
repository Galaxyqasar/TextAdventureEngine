#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMap>
#include <QLayout>

class TextDisplay : public QLabel
{
    Q_OBJECT
public:
    explicit TextDisplay(QWidget *parent = nullptr);
    void updateText(QString text, QFont font);
    QString fontStyleSheet(QFont font);
    void updateScores(QMap<QString, int> scores);
    void showScores();
signals:
    
public slots:
private:
    QMap<QString, int> scores;
    QHBoxLayout *layout;
    QLabel *scoreDisplay;
    QWidget *spacer;
    
};

#endif // TEXTDISPLAY_H
