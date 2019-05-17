#ifndef LEVEL_H
#define LEVEL_H

#include <QObject>
#include <QString>
#include <QFont>
#include <QImage>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

typedef struct Line{
    QString text;
    QFont font;
    float duration;
} Line;

typedef struct Action{
    QString text;
    int nextIndex;
    QMap<QString, int> scores;
} Action;

class Level : public QObject
{
    Q_OBJECT
public:
    explicit Level(QObject *parent = nullptr);
    Level(Level &other);
    Level(QJsonObject level);
    ~Level();
    void operator=(Level other);
    
    static QList<Level *> loadFile(QString file);
    
    
    int index;
    QString time;
    QString question;
    QFont qFont;
    QString sound;
    QImage background;
    QList<Line> lines;
    QList<Action> actions;
    
signals:
    
public slots:
    
private:
};

#endif // LEVEL_H
