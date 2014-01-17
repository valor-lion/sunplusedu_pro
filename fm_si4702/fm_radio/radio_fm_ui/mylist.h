#ifndef MYLIST_H
#define MYLIST_H

#include <QLabel>
#include <QEvent>
#include <QPalette>
#include <QColor>
#include <QString>

class MyList : public QLabel
{
    Q_OBJECT
public:
    explicit MyList(QWidget *parent = 0);
    void setWinColor(QColor releasedColor = QColor(199, 199, 149),
                    QColor pressedColor = QColor(255, 170, 127));

protected:
    void mousePressEvent(QMouseEvent *);    // mouse press event
    void mouseReleaseEvent(QMouseEvent *);  // mouse release event

signals:
    void pressed();
    void released();

private:
    QColor releasedColor;
    QColor pressedColor;
    QPalette palette;

public slots:

};

#endif // MYLIST_H
