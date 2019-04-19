#include "textdisplay.h"

TextDisplay::TextDisplay(QWidget *parent) : QLabel(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setAlignment(Qt::AlignCenter);
}

void TextDisplay::updateText(QString text, QFont font)
{
    setText(text);
    setFont(font);
}
