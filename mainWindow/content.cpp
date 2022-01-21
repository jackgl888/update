#include "content.h"

content::content(QWidget *parent)
    : QFrame(parent)
{
    this->stack  = new QStackedWidget(this);
    this->stack->setFrameStyle(QFrame::Panel|QFrame::Raised|QFrame::  Sunken);


    this->m_ocvneedle   = new   ocvneedle(this);
    this->m_temp   = new  tempreture(this);
    this->m_map =new  NeedleMap(this);
    this->m_bed     =new  needlebed(this);
    this->m_browse  = new dataBrowse(this);
    this->m_adjustTemp  = new adjustTemp(this);

    this->stack->addWidget( this->m_bed);
    this->stack->addWidget(  this->m_temp );

    this->stack->addWidget(this->m_ocvneedle);
    this->stack->addWidget(this->m_map);
    this->stack->addWidget(this->m_browse);
    this->stack->addWidget(this->m_adjustTemp);


    QVBoxLayout *RightLayout = new  QVBoxLayout(this);

    RightLayout->addWidget(this->stack);



    connect(this->m_bed,SIGNAL(sig(QVariant)),this->m_map,SLOT(slotUiUpdate(QVariant)));//传到map
        #if 1

    connect(this->m_bed,SIGNAL(sig(QVariant)),     //显示针床收发的字符串
            this->m_browse,SLOT(slotUpdateCmdData(QVariant)));

    connect(this->m_temp,SIGNAL(sig(QVariant)),this->m_browse,
            SLOT(slotUpdateCmdData(QVariant)));//显示温度接收的数据

#endif
    //线程同步
    connect(this->m_bed,SIGNAL(startSwitch()),this->m_ocvneedle,SLOT(slotStartSwitch()));//针床压接，OCV开始切换
    connect(this->m_ocvneedle,SIGNAL(bedStart()),this->m_bed,SLOT(slotBedStart()));//切换完成，开始压接


}




content::~content()
{

}









