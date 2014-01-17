#include "mylist.h"

MyList::MyList(QWidget *parent) :
    QLabel(parent)
{
    this->setWinColor();
    this->setAutoFillBackground(true);
}

void MyList::mousePressEvent(QMouseEvent *)
{
    palette.setColor(QPalette::Window, this->pressedColor);
    this->setPalette(this->palette);

    emit this->pressed();
}

void MyList::mouseReleaseEvent(QMouseEvent *)
{
    palette.setColor(QPalette::Window, this->releasedColor);
    this->setPalette(this->palette);

    emit this->released();
}

void MyList::setWinColor(QColor releasedColor, QColor pressedColor)
{
    this->releasedColor = releasedColor;
    this->pressedColor = pressedColor;

    palette.setColor(QPalette::Window, this->releasedColor);
    this->setPalette(this->palette);
}
