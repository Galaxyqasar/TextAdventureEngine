#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class TextDisplay : public QLabel
{
    Q_OBJECT
public:
    explicit TextDisplay(QWidget *parent = nullptr);
    void updateText(QString text, QFont font);
signals:
    
public slots:
};

#endif // TEXTDISPLAY_H
