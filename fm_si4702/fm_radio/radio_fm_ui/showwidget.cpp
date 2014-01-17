#include "showwidget.h"

ShowWidget::ShowWidget(QWidget *parent) :
    QWidget(parent)
{
    mainLayout = new QHBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    for(int WidgetIndex = 0;WidgetIndex < VOLUME_COUNT ; WidgetIndex++)
    {
        PYVolume* pyv = new PYVolume(this);
        m_PYVVec.push_back(pyv);
        mainLayout->addWidget(pyv);
    }

    setLayout(mainLayout);

    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(TIMER_INTERVAL);
    connect(m_updateTimer,SIGNAL(timeout()),this,SLOT(UpdateValue()));
    m_updateTimer->start();
}

ShowWidget::~ShowWidget()
{
    delete this->mainLayout;
    delete this->m_updateTimer;
}

void ShowWidget::UpdateValue()
{
    for(int WidgetIndex = 0;WidgetIndex < VOLUME_COUNT;WidgetIndex++)
    {
        qApp->processEvents();
        m_PYVVec.at(WidgetIndex)->setValue(rand()%10);
    }
}
