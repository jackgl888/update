#include "motor.h"



motor::motor(QWidget *parent) : QWidget(parent)
{
   this->lock = 0;
    QLabel *m_stepsi = new QLabel("步进量写入:",this);
    QLabel *m_speedi = new QLabel("速  度写入：",this);

    QLabel *m_stepso = new QLabel("步进量读出:",this);
    QLabel *m_speedo = new QLabel("速  度读出：",this);

    this->myabtn  =new myAnimationBtn(this);

  //  this->myabtn->setImagePath(":/pic/tim.png");

    this->boundratechs = new   QLabel(tr("波特率:"),this);
    QStringList  list;
    list<<"115200"<<"57600"<<"38400"<<"19200"<<"9600";
    this->boundrate  = new  QComboBox(this);
    this->boundrate->addItems(list);
    this->m_browser  = new  QTextBrowser(this);

    this->serialname  =  new   QComboBox(this);
    this->comname = new   QLabel(tr("串口号:"),this);

    this->serialcntl = new   QPushButton(tr("脱机"),this);
    this->forward = new QPushButton(tr("电机前进"),this);  //电机前进
    this->back = new QPushButton(tr("电机后退"),this);  //电机后退
    this->stop = new QPushButton(tr("电机停止"),this);  //电机停止
    this->result  = new  QLabel(tr("吸嘴残留:"),this);
    this->spresult = new QLineEdit(this);
    this->Oringe  = new  QLabel(tr("回零状态:"),this);
    this->OringePoint   = new  QLineEdit(this);
    this->spresult->setReadOnly(true);
    this->OringePoint->setReadOnly(true);

    this->stepsin = new  QSpinBox(this);
    this->stepsout =new  QLineEdit(this);
    this->speedin = new  QSpinBox(this);
    this->speedout =new  QLineEdit(this);
    this->stepsout->setReadOnly(true);
    this->speedout->setReadOnly(true);

//   QLabel    xizuilb[4];
//   for()

    QGridLayout  *mainlayout = new  QGridLayout(this);
    mainlayout->setMargin(25);
    mainlayout->setSpacing(20);
    mainlayout->addWidget(m_stepsi,0,0,1,1);
    mainlayout->addWidget(serialcntl ,0,2,1,1);
    mainlayout->addWidget( this->stepsin,0,1,1,1);
    mainlayout->addWidget(m_speedi ,1,0,1,1);
    mainlayout->addWidget( this->speedin,1,1,1,1);
    mainlayout->addWidget(m_stepso ,2,0,1,1);
    mainlayout->addWidget(  this->stepsout,2,1,1,1);
    mainlayout->addWidget(m_speedo ,3,0,1,1);
    mainlayout->addWidget( this->speedout,3,1,1,1);
    mainlayout->addWidget( this->comname,4,0,1,1);
    mainlayout->addWidget( this->serialname ,4,1,1,1);
    mainlayout->addWidget( this->boundratechs,5,0,1,1);
    mainlayout->addWidget( this->boundrate ,5,1,1,1);
    mainlayout->addWidget(this->result,6,0,1,1);
    mainlayout->addWidget(this->spresult,6,1,1,1);
    mainlayout->addWidget(this->Oringe,7,0,1,1);
    mainlayout->addWidget(this->OringePoint,7,1,1,1);

    mainlayout->addWidget(forward  ,10,0,1,1);
    mainlayout->addWidget(back,11,0,1,1);
    mainlayout->addWidget(stop,13,2,1,1);
   mainlayout->addWidget( this->myabtn,15,2,10,10);

    mainlayout->addWidget(this->m_browser,15,10,15,40);

    mainlayout->setSizeConstraint(QLayout::  SetFixedSize);
    this->setLayout(mainlayout);

    m_timer  =new  QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeoutEventMethod()));
    connect( this->serialcntl,SIGNAL(clicked()),this,SLOT(on_opencloseserial_clicked()));  //联机
    connect( this->forward ,SIGNAL(clicked()),this,SLOT(on_senddata_clicked()));   //前进
    connect( this->back ,SIGNAL(clicked()),this,SLOT(on_betback_clicked()));   //后退

    //   connect( this->contSwitch,SIGNAL(clicked()),this,SLOT(continus_senddata_clicked()));   //连续切换
  //


}

motor::~motor()
{

}

void motor::on_opencloseserial_clicked()
{
    static uchar i =0;

    if(i%2 == 1)
    {
        this->serialcntl->setText("脱机");
        emit  stopSerial(this->serialname->currentText());
        this->m_timer->stop();
    }
    else
    {
         this->m_timer->start(200);
        this->serialcntl->setText("联机");
        Single::m_Instance()->serialPara.clear();
        Single::m_Instance()->serialPara.append(this->boundrate->currentText());
        Single::m_Instance()->serialPara.append(this->serialname->currentText());

        this->trayInit(this->serialname->currentText(),0,0x01);//初始化托盘
        this->m_thread  =new serverThread(0,0x01,0,0);

        //串口发送
        connect(this, SIGNAL(startSend(QString,int,uchar,ushort,ushort)),
                m_thread, SLOT( sendDataSlot(QString,int,uchar,ushort,ushort)),Qt::UniqueConnection);
        //停止串口
        connect(this,SIGNAL(stopSerial(const QString &)),m_thread,SLOT(stopSerialSlot(const QString  &)),Qt::UniqueConnection);
        //线程退出
        connect(m_thread,SIGNAL(finished()),m_thread,SLOT(deleteLater()),Qt::UniqueConnection);
        //更新串口状态
        connect(m_thread,SIGNAL(updateUiSerial(const QString &)),this,
                SLOT(disconnectSerial(const QString &)),Qt::UniqueConnection);

        connect(m_thread,SIGNAL( bedUpdate(uchar,uchar,QString)),this,
                SLOT(slotUpdateCmdData(uchar,uchar,QString)));  //界面更新来自串口的数据

        //接收工作线程传过来的数据
        connect(m_thread,SIGNAL(sigtu(QVariant)),this,SLOT(slotUiUpdate(QVariant)));

        this->m_thread->start();

    }
    i++;
    if(i == 100)
        i= 0;
}

void motor::disconnectSerial(const QString &ip)
{
    for(uchar i =0;i< this->serialname->count();i++)
    {
        if(this->serialname->itemText(i)== ip)

            this->serialname->removeItem(i);
    }
}

void motor::moveForwardSlot()
{


    emit startSend(this->serialname->currentText(),250,'C',0,1);  //250为拔吸嘴工装

}

void motor::timeoutEventMethod()
{
    ushort steps ;

    static uchar times;
    times++;

    steps =this->stepsin->value();
    //    seed = this->speedin->text().toUInt();

    //写入步数
    this->motorbuf[0] = steps;
    this->motorbuf[1] =  steps>>8;


    //采样与联机心跳
    if(this->lock == 0x00)
    {
        if(times ==5)
        {
            times = 0;
            startSend(this->serialname->currentText(),1,'A',0,0);
        }
        else
            startSend(this->serialname->currentText(),1,'S',0,0);
    }


}

void motor::on_senddata_clicked()
{

    emit startSend(this->serialname->currentText(),0,'C',0,1);  //

}

void motor::on_betback_clicked()
{

    emit startSend(this->serialname->currentText(),0,'C',0,0);  //

}

void motor::trayInit(const QString &ip, const uchar traynum, const uchar flag)
{
    if(flag == 0x01) //只创建一个托盘
    {
           Traymsg * traymsg = new  Traymsg(ip,traynum,NULL,this);
           traymsgList.append(traymsg);
    }
    else  //从零开始创建多个托盘
    {
        for(uchar i = 0;i< traynum ;i++)
        {
            Traymsg * traymsg = new  Traymsg(ip,i,NULL,this);
            traymsgList.append(traymsg);
        }
    }

}


void motor::slotUpdateCmdData(const uchar cmd, const uchar traynum, const QString &str)
{
    this->m_browser->setText(str);
}


void motor::slotUiUpdate(QVariant variant)
{
    QVariant    m_variant;
    bedPara m_para = variant.value<bedPara>();

    m_variant.setValue(m_para);
    if(Single::m_Instance()->sendReceiveBs == 0x05)   //拔吸嘴工装
    emit  sig(m_variant);

}




