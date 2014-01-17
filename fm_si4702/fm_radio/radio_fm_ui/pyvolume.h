#ifndef PYVOLUME_H
#define PYVOLUME_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QPaintEvent>

#define PYV_LEFT_SPACE 5
#define PYV_TOP_SPACE 3
#define PYV_TIMER_INTERVAL 500
#define PYV_BAR_COUNT 10
#define PYV_RECT_RADIUS 3
#define PYV_OPACITY 0.4

#define PYV_BAR_DEFAULT_START_COLOR QColor(167,167,167)
#define PYV_BAR_DEFAULT_END_COLOR QColor(45,45,45)
#define PYV_BG_COLOR Qt::black

#define PYV_BAR_START_COLOR QColor(85,255,0)
#define PYV_BAR_MIDDLE_COLOR QColor(249,216,47)
#define PYV_BAR_END_COLOR QColor(249,107,24)


class PYVolume : public QWidget
{
    Q_OBJECT
public:
    explicit PYVolume(QWidget *parent = 0);

public:
    /// public interfaces
    /// the default maximum is 10
    void setValue(int value);

    int value() const;

protected:
    void paintEvent(QPaintEvent *);

    QSize sizeHint() const
    {
        return QSize(80,200);
    }


private:
    /// painting functions
    void drawBg(QPainter* painter);

    void draw(QPainter* painter);


private:
    /// utility functions
    void initVariables();

    /// variables
    int m_nValue;

    QTimer* m_decreTimer;

private slots:
    void DoDecrement();
};

#endif // PYVOLUME_H
