#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>
#include <QEvent>
#include <QString>

/*
* when press or release, the window will change(become bigger or smaller)
*/
class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(QWidget *parent = 0);
    void setImage(const QString &str = "", const QString &ustr = "");   // set the image
    void setButtonEnable(const bool);    // set button enabled or not
    void setChangeStep(const int x, const int y, const int w, const int h); // set change step

protected:
    void paintEvent(QPaintEvent *); 		// paint background picture
    void mousePressEvent(QMouseEvent *);    // mouse press event
    void mouseReleaseEvent(QMouseEvent *);  // mouse release event

private:
    QString imagePath, uImagePath;  // save the image path
    bool isButtonEnabled;           // the button is enabled or not
    int startxStep, startyStep, widthStep, heightStep;

signals:

public slots:

};

#endif // MYPUSHBUTTON_H
