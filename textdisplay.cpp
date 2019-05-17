#include "textdisplay.h"
#include <QDebug>
TextDisplay::TextDisplay(QWidget *parent) : QLabel(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setAlignment(Qt::AlignCenter);
    setTextFormat(Qt::RichText);
    
    scoreDisplay = new QLabel(this);
    scoreDisplay->setAlignment(Qt::AlignTop);
    spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout = new QHBoxLayout(this);
    layout->addWidget(spacer);
    layout->addWidget(scoreDisplay);
}

void TextDisplay::updateText(QString text, QFont font)
{
    setText(text);
    //qDebug()<<text<<font;
    //setStyleSheet(fontStyleSheet(font));
    //qDebug()<<fontStyleSheet(font);
    setFont(font);
    showScores();
}

QString TextDisplay::fontStyleSheet(QFont font)
{
    const QString baseStr = QStringLiteral("font-family: \"") + font.family()
            + QStringLiteral("\";font-weight: ") + (font.bold() ? QStringLiteral("bold") : QStringLiteral("normal"))
            + QStringLiteral(";font-size: ") + QString::number(font.pointSize()) + "pt"
            + QStringLiteral(";font-style: ");
    switch(font.style()){
    case QFont::StyleItalic:
        return baseStr + QStringLiteral("italic");
    case QFont::StyleOblique:
        return baseStr + QStringLiteral("oblique");
    default:
        return baseStr + QStringLiteral("normal");
    }
}

void TextDisplay::updateScores(QMap<QString, int> scores)
{
    this->scores = scores;
    showScores();
}

void TextDisplay::showScores()
{
    QString text = "Scores:\n";
    for(QString key : scores.keys()){
        text.append(key + " : " + QString::number(scores[key]) + "\n");
    }
    qDebug()<<text;
    scoreDisplay->setText(text);
}
