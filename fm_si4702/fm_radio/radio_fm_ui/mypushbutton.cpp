#include "mypushbutton.h"

MyPushButton::MyPushButton(QWidget *parent) :
    QPushButton(parent)
{
    // initialize the image path
    this->imagePath = ":/new/prefix1/image/start/1.png";
    this->uImagePath = "";

    this->isButtonEnabled = true; // the button is enabled
	
    // default setting;
    startxStep = 5;
    startyStep = 5;
    widthStep = 10;
    heightStep = 10;
}

/*
 * paint event, paint background image
*/
void MyPushButton::paintEvent(QPaintEvent *)
{
    QPainter paint(this); // create a painter

    if (true == this->isButtonEnabled)
    {
        paint.drawPixmap(0, 0, width(), height(), QPixmap(imagePath));
    }
    else
    {
        paint.drawPixmap(0, 0, width(), height(), QPixmap(uImagePath));
    }
}

/*
 * set the image path
*/
void MyPushButton::setImage(const QString &str, const QString &ustr)
{
    // save the image path
    this->imagePath = str;
    this->uImagePath = ustr;
    this->update();         // update the window
	
}

/*
 * mouse press event
 * when mouse presses, the window will become a little bigger
*/
void MyPushButton::mousePressEvent(QMouseEvent *)
{
    if (false == this->isButtonEnabled)
    {
        return;
    }

    this->setGeometry(x()-startxStep, y()-startyStep, width()+widthStep, height()+heightStep);
    emit pressed();
}

/*
 * mouse release event
 * when mouse releases, the window will regain the same
*/
void MyPushButton::mouseReleaseEvent(QMouseEvent *)
{
    if (false == this->isButtonEnabled)
    {
        return;
    }

    this->setGeometry(x()+startxStep, y()+startyStep, width()-widthStep, height()-heightStep);
    emit released();
}

/*
 * set button eable or not
*/
void MyPushButton::setButtonEnable(const bool temp)
{
    this->isButtonEnabled = temp;
    this->setEnabled(temp);
    this->update();
}

/*
* set change step
*/
void MyPushButton::setChangeStep(const int x, const int y, const int w, const int h)
{
    startxStep = x;
    startyStep = y;
    widthStep = w;
    heightStep = h;
}
