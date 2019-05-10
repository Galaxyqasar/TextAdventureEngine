#include "textdisplay.h"
#include <QDebug>
TextDisplay::TextDisplay(QWidget *parent) : QLabel(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setAlignment(Qt::AlignCenter);
    setTextFormat(Qt::RichText);
}

void TextDisplay::updateText(QString text, QFont font)
{
    setText(text);
    //qDebug()<<text<<font;
    //setStyleSheet(fontStyleSheet(font));
    //qDebug()<<fontStyleSheet(font);
    setFont(font);
    qDebug()<<QLabel::font().pointSize()<<font<<font.pixelSize();
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
