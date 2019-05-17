#include "level.h"

Level::Level(QObject *parent) : QObject(parent)
{
    
}

Level::Level(Level &other) : QObject(nullptr){
    this->background = other.background;
    this->actions = other.actions;
    this->index = other.index;
    this->lines = other.lines;
    this->sound = other.sound;
    this->time = other.time;
}

Level::Level(QJsonObject level) : QObject(nullptr)
{
    this->index = level.value("index").toInt();
    this->time = level.value("time").toString();
    this->sound = level.value("music").toString();
    this->question = level.value("question").toString();
    this->background = QImage(level.value("background").toString());
    this->qFont.fromString(level.value("qfont").toString());
    QJsonArray lines = level.value("lines").toArray();
    QJsonArray actions = level.value("actions").toArray();
    for(QJsonValueRef e : lines){
        QJsonObject current = e.toObject();
        QString text = current.value("text").toString();
        QFont font;
        font.fromString(current.value("font").toString());
        float duration = float(current.value("duration").toDouble());
        Line line = {text, font, duration};
        this->lines.append(line);
    }
    for(QJsonValueRef e : actions){
        QJsonObject current = e.toObject();
        QString text = current.value("text").toString();
        int nextIndex = current.value("nextIndex").toInt();
        QJsonObject scoresObject = current.value("scores").toObject();
        QMap<QString, int> scores;
        for(QString key : scoresObject.keys()){
            scores[key] = scoresObject.value(key).toInt();
        }
        Action action = {text, nextIndex, scores};
        this->actions.append(action);
    }
}

Level::~Level()
{
    ;
}

QList<Level*> Level::loadFile(QString file)
{
    QList<Level*> levels;
    QFile f(file);
    f.open(QIODevice::ReadOnly);
    QByteArray json = f.readAll();
    f.close();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if(error.error != QJsonParseError::NoError){
        qDebug()<<error.errorString();
        return {};
    }
    QJsonObject root = doc.object();
    QJsonArray levelArray = root.take("levels").toArray();
    for(QJsonValueRef e : levelArray){
        Level *current = new Level(e.toObject());
        levels.append(current);
    }
    return  levels;
}
