#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QString>
#include <QDebug>

namespace Ui {
    class ListWidget;
}

class ListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ListWidget(unsigned size = 0, QWidget *parent = 0);
    ~ListWidget();
    void clearChannelInfo();
    void addOneChannelInfo(const QString str, const unsigned int freq);
    int addMultChannelInfo(QString *str, unsigned int *freq, const int num);
    void initInfo();
    void updateButton();
    void updateList();

private:
    Ui::ListWidget *ui;
    QString *channelInfo;
    unsigned int infoSize;
    unsigned int infoAct;

    int pageIndex;
    int pageSize;
    int pageNum;

private slots:
    void on_pushButtonDown_released();
    void on_pushButtonGoback_released();
    void on_pushButtonUp_released();
    void dealPressList();

signals:
    void gobackSignal();
    void channelNumSignal(int);
};

#endif // LISTWIDGET_H
