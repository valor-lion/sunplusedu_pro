#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QApplication>
#include "pyvolume.h"

#define VOLUME_COUNT 15
#define TIMER_INTERVAL 500

class ShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowWidget(QWidget *parent = 0);
    ~ShowWidget();


private:
    QVector<PYVolume*> m_PYVVec;
    QHBoxLayout* mainLayout;
    QTimer* m_updateTimer;


private slots:
    void UpdateValue();
};

#endif // TESTWIDGET_H
