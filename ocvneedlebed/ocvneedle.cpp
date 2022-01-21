#include "ocvneedle.h"
#include <synchapi.h>
ocvneedle::ocvneedle(QWidget *parent) : QWidget(parent)
{

    this->excelInit(); //初始化excell表格样式
    this-> scene = new QGraphicsScene(this);
    this-> scene->setSceneRect(-300,-300,600,600);
    this->view = new QGraphicsView(this);
    this->view->setScene(scene);
    this->view->setMinimumSize(600,600);
    this->view->show();

    for(uchar i = 0;i<80;i++)
    {
        this->channels[i] = new   QGraphicsEllipseItem(0,0,70,70);
        this->scene->addItem(  this->channels[i] );
        if(i<10)
        {
            this->channels[i]->setPos(280,350-i*85);
        }
        if((i>=10)&&(i<20))
        {
            this->channels[i]->setPos(190,350-(i-10)*85);
        }
        if((i>=20)&&(i<30))
        {
            this->channels[i]->setPos(100,350-(i-20)*85);
        }
        if((i>=30)&&(i<40))
        {
            this->channels[i]->setPos(10,350-(i-30)*85);

        }
        if((i>=40)&&(i<50))
        {
            this->channels[i]->setPos(-80,350-(i-40)*85);

        }
        if((i>=50)&&(i<60))
        {
            this->channels[i]->setPos(-170,350-(i-50)*85);

        }
        if((i>=60)&&(i<70))
        {
            this->channels[i]->setPos(-260,350-(i-60)*85);

        }
        if((i>=70)&&(i<80))
        {
            this->channels[i]->setPos(-350,350-(i-70)*85);

        }
        this->channels[i]->setBrush(QColor(255,255,255));

    }

    this->ctrlFrame  = new QFrame (this);

    QStringList  list;
    list<<"115200"<<"57600"<<"38400"<<"19200"<<"9600";
    this->boundrate  = new  QComboBox(this);
    this->boundrate->addItems(list);
    this->boundratechs = new   QLabel(tr("波特率:"),this);

    list.clear();
    list<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<"COM6"<<"COM7"<<"COM8"<<
          "COM9"<<"COM10"<<"COM11"<<"COM12"<<"COM13"<<"COM14"<<"COM15"<<"COM16";

    this->button =   new   QPushButton(tr("单个切换"),this);
    this->contSwitch  = new  QPushButton(tr("连续切换"),this);
    this->stopSwitch   = new  QPushButton(tr("停止切换"),this);
    this->comBtn  = new   QPushButton(tr("脱机"),this);
    this->serialname  =  new   QComboBox(this);
    this->serialname->addItems(list);
    this->comname = new   QLabel(tr("串口号:"),this);

    list.clear();
    list<<"电池电压"<<"电池内阻"<<"负极壳体电压"<<"正极壳体电压";
    this->mode  =  new QLabel(tr("模式:"),this);
    this->modein  = new QComboBox(this);
    this->modein->addItems(list);
    list.clear();
    list<<"2*8"<<"2*12"<<"3*8";
    this->packMode = new QComboBox(this);
    this->packMode->addItems(list);
    this->packModeLb  = new QLabel(tr("装盘模式:"),this);

    this->channelname  = new QLabel(tr("通道号:"),this);
    this->channel      = new QSpinBox(this);
    this->channel->setMaximum(256);

    this->switchTime   = new   QLabel(tr("切换间隔(ms):"),this);
    this->m_box  = new QSpinBox(this);  //输入切换时间 间隔
    this->m_box->setMaximum(5000);
    this->msgout = new QPushButton(tr("数据导出"),this);    //测量数据导出
    //测量结果
    this->battryVoltage   =new  QLabel(tr("电池电压:"),this);
    this->batVoltage =  new  QLineEdit(this);
    this->batVoltage->setReadOnly(true);

    this->battryResistence  =new  QLabel(tr("交流内阻:"),this);
    this->batResistence   = new QLineEdit(this);
    this->batResistence->setReadOnly(true);
    this->negVoltage  =  new QLabel(tr("负极壳体:"),this);
    this->negvolt     = new  QLineEdit(this);
    this->negvolt->setReadOnly(true);

    this->posVoltage  = new QLabel(tr("正极壳体:"),this);
    this->posvolt   = new   QLineEdit(this);
    this->posvolt->setReadOnly(true);

    this->bvToplever = new QLabel(tr("电池电压上限:"),this);
    this->bvtopEdit  = new QLineEdit(this);
    this->bvtopEdit->setReadOnly(true);

    this->bvDownlever  = new QLabel(tr("电池电压下限:"),this);
    this->bvdownEdit  =  new QLineEdit(this);
    this->brToplever  = new QLabel(tr("内阻上限："),this);
    this->brtopEdit  =  new QLineEdit(this);
    this->brDownlever  = new QLabel(tr("内阻下限："),this);
    this->brdownEdit  = new QLineEdit(this);
    this->svToplever   = new QLabel(tr("壳体电压上限:"),this);
    this->svtopEdit   = new QLineEdit(this);
    this->svDownlever   = new QLabel(tr("壳体电压下限:"),this);
    this->svdownEdit    = new QLineEdit(this);
    this->allChannelLb  = new QLabel(tr("通道总数:"),this);
    this->allChanneledit  =new QSpinBox(this);

    QVBoxLayout  *pklayout = new   QVBoxLayout ;   //装盘模式
    pklayout ->addWidget(this->packModeLb);
    pklayout ->addWidget(this->packMode);
    QGroupBox * packbox = new     QGroupBox;
    packbox->setLayout(pklayout);

    QVBoxLayout  *chlayout = new   QVBoxLayout ;   //通道
    chlayout ->addWidget(this->channelname);
    chlayout ->addWidget(this->channel);
    QGroupBox * channelbox = new     QGroupBox;
    channelbox->setLayout(chlayout);

    QVBoxLayout  *modelayout = new   QVBoxLayout ;   //模式
    modelayout->addWidget( this->mode);
    modelayout->addWidget( this->modein);
    QGroupBox * modelbox = new     QGroupBox;
    modelbox->setLayout(modelayout);

    QVBoxLayout  *comlayout = new   QVBoxLayout ;   //串口名
    comlayout->addWidget(comname);
    comlayout->addWidget(this->serialname);
    QGroupBox * combox = new     QGroupBox;
    combox->setLayout(comlayout);

    QVBoxLayout  *boundratelayout = new   QVBoxLayout ;   //串口名
    boundratelayout->addWidget(boundratechs);
    boundratelayout->addWidget(this->boundrate);
    QGroupBox * boundratebox = new     QGroupBox;
    boundratebox->setLayout(boundratelayout);

    QVBoxLayout  *timelayout = new   QVBoxLayout ;   //切换时间
    timelayout->addWidget(  this->switchTime );
    timelayout->addWidget(     this->m_box );
    QGroupBox * timebox = new     QGroupBox;
    timebox->setLayout(timelayout);

    QVBoxLayout  *ctrllayout = new   QVBoxLayout ;   //控制开关
    ctrllayout  ->setMargin(15);
    ctrllayout  ->setSpacing(20);

    ctrllayout->addWidget(this->comBtn);   //联机
    ctrllayout->addWidget(this->button);
    ctrllayout->addWidget(this->contSwitch);
    ctrllayout->addWidget(this->stopSwitch);

    QGroupBox *   ctrlbox = new   QGroupBox;
    ctrlbox->setLayout(  ctrllayout);


    QHBoxLayout  *bvlayout = new    QHBoxLayout() ;   //电池电压
    bvlayout ->addWidget(this->battryVoltage);
    bvlayout ->addWidget(this->batVoltage);
    QGroupBox * bvbox = new     QGroupBox;
    bvbox->setLayout( bvlayout);

    QHBoxLayout   *brlayout = new     QHBoxLayout()  ;   //电池内阻
    brlayout ->addWidget(this->battryResistence);
    brlayout ->addWidget(this->batResistence);
    QGroupBox * brbox = new     QGroupBox;
    brbox->setLayout( brlayout);

    QHBoxLayout   *neglayout = new     QHBoxLayout()  ;   //负极壳体
    neglayout ->addWidget(this->negVoltage);
    neglayout ->addWidget(this->negvolt);
    QGroupBox * negbox = new     QGroupBox();
    negbox->setLayout( neglayout);

    QGroupBox * posbox = new     QGroupBox();
    QHBoxLayout  *poslayout = new    QHBoxLayout() ;   //正极壳体
    poslayout ->addWidget(this->posVoltage);
    poslayout ->addWidget(this->posvolt);
    posbox->setLayout ( poslayout);

    QGroupBox  *numbox  =new QGroupBox(this);
    QHBoxLayout  *numlayout = new  QHBoxLayout();
    numlayout->addWidget(this->allChannelLb);
    numlayout->addWidget(this->allChanneledit);
    numbox->setLayout(numlayout);


    QGroupBox * leverbox = new     QGroupBox();
    QVBoxLayout  *leverlayout = new   QVBoxLayout() ;   //限定范围布局
    leverlayout->setMargin(20);
    leverlayout->setSpacing(25);
    leverlayout->addWidget(this->bvToplever);
    leverlayout->addWidget(this->bvtopEdit);
    leverlayout->addWidget(this->bvDownlever);
    leverlayout->addWidget(this->bvdownEdit);
    leverlayout->addWidget(this->brToplever);
    leverlayout->addWidget(this->brtopEdit);
    leverlayout->addWidget(this->brDownlever);
    leverlayout->addWidget(this->brdownEdit);
    leverlayout->addWidget(this->svToplever);
    leverlayout->addWidget(this->svtopEdit);
    leverlayout->addWidget(this->svDownlever);
    leverlayout->addWidget(this->svdownEdit);
    leverbox->setLayout(leverlayout);

    this->framelayout  =   new    QVBoxLayout();
    this->framelayout ->setMargin(1);
    this->framelayout ->setSpacing(1);
    this->framelayout->addWidget(packbox);
    this->framelayout->addWidget( channelbox);
    this->framelayout->addWidget(  modelbox);
    this->framelayout->addWidget( combox);
    this->framelayout->addWidget( boundratebox);
    this->framelayout->addWidget(  timebox);
    this->framelayout->addWidget( ctrlbox);
    this->framelayout->addWidget(numbox);

    this->resultlayout = new   QVBoxLayout();
    this->resultlayout->setMargin(1);
    this->resultlayout->setSpacing(1);
    this->resultlayout->addWidget(bvbox);
    this->resultlayout->addWidget(brbox);
    this->resultlayout->addWidget(negbox);
    this->resultlayout->addWidget( posbox);
    this->resultlayout->addWidget(leverbox);
    this->resultlayout->addWidget(this->msgout);//数据导出

    this->leftlayout   = new   QHBoxLayout;
    this->leftlayout->setMargin(1);
    this->leftlayout->setSpacing(1);
    this->leftlayout->addLayout(this->framelayout);
    this->leftlayout->addLayout(this->resultlayout);
    this->ctrlFrame->setLayout( this->leftlayout);

    this->mainlayout   = new QHBoxLayout(this);
    this->mainlayout->setMargin(2);
    this->mainlayout ->setSpacing(2);
    this->mainlayout->addWidget(  this->ctrlFrame);
    this->mainlayout->addWidget(  this->view);

    m_timer  =new  QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeoutEventMethod()));
    connect( this->comBtn,SIGNAL(clicked()),this,SLOT(on_opencloseserial_clicked()));  //串口控制
    connect( this->button,SIGNAL(clicked()),this,SLOT(on_senddata_clicked()));   //切换
    connect( this->contSwitch,SIGNAL(clicked()),this,SLOT(continus_senddata_clicked()));   //连续切换
    connect( this->stopSwitch,SIGNAL(clicked()),this,SLOT(stopSwitchMethod()));   //停止切换
    connect(this->msgout,SIGNAL(clicked()),this,SLOT(slotMsgOut())); //测量数据导出

    //
}




ocvneedle::~ocvneedle()
{

}


//场景初始化
void ocvneedle::initScene()
{

}

void ocvneedle::on_opencloseserial_clicked()
{
    static uchar i =0;

    if(i%2 == 1)
    {
        this->m_timer->stop();
        this->comBtn->setText("脱机");
        emit  stopSerial(this->serialname->currentText());

    }
    else
    {
        this->m_timer->start(1000);
        this->comBtn->setText("联机");
        Single::m_Instance()->serialPara.clear();
        Single::m_Instance()->serialPara.append(this->boundrate->currentText());
        Single::m_Instance()->serialPara.append(this->serialname->currentText());

        this->trayInit(this->serialname->currentText(),0,0x01);//初始化托盘
        Single::m_Instance()->socketFlag = 0;//安捷伦接入
        this->m_thread  =new serverThread(0,0x01,0x01,0x01);//客户端线程
        Single::m_Instance()->comBuf[3]=this->serialname->currentText();   //将当前串口放入分配位

        //串口发送
        connect(this, SIGNAL(startSend(QString,int,uchar,ushort,ushort)),
                m_thread, SLOT( sendDataSlot(QString,int,uchar,ushort,ushort)),Qt::  QueuedConnection);
        //停止串口
        connect(this,SIGNAL(stopSerial(const QString &)),m_thread,SLOT(stopSerialSlot(const QString  &)),Qt::  QueuedConnection);
        //线程退出
        connect(m_thread,SIGNAL(finished()),m_thread,SLOT(deleteLater()),Qt::  QueuedConnection);
        //更新串口状态
        connect(m_thread,SIGNAL(updateUiSerial(const QString &)),this,SLOT(disconnectSerial(const QString &)),Qt::  QueuedConnection);

        connect(m_thread,SIGNAL( bedUpdate(uchar,uchar,QString)),this,
                SLOT(slotUpdateCmdData(uchar,uchar,QString)));  //界面更新来自串口的数据
        //读取安捷伦数据
        connect(this,SIGNAL(startRead()),m_thread,SLOT(slotReadData()),Qt::  QueuedConnection);

        //接收完成安捷伦数据开始切换
        connect(m_thread,SIGNAL(startSwitch()),this,SLOT(slotStartSwitch()),Qt::  QueuedConnection);

        this->m_thread->start();
        this->startTouch  = 0x01;

    }
    i++;
    if(i == 100)
        i= 0;
}


void ocvneedle::disconnectSerial(const QString &ip)
{
    for(uchar i =0;i< this->serialname->count();i++)
    {
        if(this->serialname->itemText(i)== ip)

            this->serialname->removeItem(i);
    }
}



void ocvneedle::timeoutEventMethod()
{
    uchar mode;
    //   this->channels[channel]->setBrush(QColor(qrand()%256,qrand()%256,qrand()%256));
    uchar  channel =  this->total_sendbuf[6]-1;
    if(this->startTouch== 0x01)
    {
        startSend(this->serialname->currentText(),0x01,'A',0,0);
    }
    if(  this->total_sendbuf[9]== 'O')
    {
        this->total_sendbuf[9] = 0;

        if(this->total_sendbuf[6] == 0)
        {
            for(uchar i=0;i<80;i++)
                this->channels[i]->setBrush(QColor(255,255,255));
        }
        else
            this->channels[channel]->setBrush(QColor(0,255,64));

    }
    if(this->packMode->currentText()=="2*12")
        mode = 0x10;
    else if(this->packMode->currentText()=="2*8")
        mode = 0x20;
    else if(this->packMode->currentText()=="3*8")
        mode = 0x30;

    if(this->total_sendbuf[3]!=0x01 )
    {
        ushort num = this->channel->text().toUShort();
        this->total_sendbuf[0]= num;
        this->total_sendbuf[1]= num>>8;  //通道号
    }

    if(this->modein->currentText()=="电池电压")
        this->total_sendbuf[2]= 0x00|mode;
    else if(this->modein->currentText()=="电池内阻")
        this->total_sendbuf[2]= 0x01|mode;
    else  if(this->modein->currentText()=="负极壳体电压")
        this->total_sendbuf[2]= 0x02|mode;
    else  if(this->modein->currentText()=="正极壳体电压")
        this->total_sendbuf[2]= 0x03|mode;


    // this->batVoltage->setText(this->AjlValue);

}


void ocvneedle::on_senddata_clicked()
{
    // this->button->setStyleSheet("color:green");

    this->startTouch = 0; //停止发送联机

    emit startSend(this->serialname->currentText(),0,'C',this->channel->value(),0);  //

}


void ocvneedle::trayInit(const QString &ip, const uchar traynum, const uchar flag)
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



void ocvneedle::continus_senddata_clicked()
{
 static uchar i= 0;
    i++;
    if(i%2 == 1)
    {


        emit startSend(this->serialname->currentText(),0,'C',1,0);  //
        this->contSwitch->setStyleSheet("color:green");
    }
    else
    {

        emit startSend(this->serialname->currentText(),0,'C',0,0);  //
        this->contSwitch->setStyleSheet("color:black");

    }
  if(i==10)
      i = 0;

}


//停止切换
void ocvneedle::stopSwitchMethod()
{


    emit startSend(this->serialname->currentText(),0,'C',0,0);

}





//this->xlsx=   new    QXlsx::Document("book3.xlsx",this);  //打开一个boot3的文件
// QXlsx::Format  format3;   //建立样式
//format3.setFontColor(QColor(Qt::red));
//format3.setPatternBackgroundColor(QColor(152,251,152));//北京颜色
//format3.setFontSize(10); //设置字体大小
//format3.setHorizontalAlignment(QXlsx::Format::AlignHCenter);//设置对齐方式
//format3.setBorderStyle(QXlsx::Format::BorderDashDotDot);//边框样式
//xlsx->write("A2","hello QQ",format3);
//xlsx->write(5,1,222);
// xlsx->saveAs("book3.xlsx");

//测量数据导出
void ocvneedle::slotMsgOut()
{
    QString  msg;
    msg =  QFileDialog  :: getSaveFileName(this,"保存");
    if(!this->xlsx->selectSheet("uu"))
        this->xlsx->addSheet("uu");


}
//初始化excell操作
void ocvneedle::excelInit()
{
    this->xlsx  = new QXlsx::Document("book.xlsx",this);
    QXlsx::Format   format1;  // 建立样式
    format1.setFontColor(  QColor(Qt::red));

}

void ocvneedle::slotUpdateCmdData(const uchar cmd, const uchar traynum, const QString &str)
{

    static uchar ctime =0;
    ushort  pertime;
    QString  text;
    if(cmd == 'C')
    {
          this->startTouch  = 0x01;

         emit startRead();          //读取电压与内阻数据

        if(this->total_sendbuf[3] == 0x01)//ocv连续切换
        {

            if(  ctime< this->allChanneledit->text().toUShort())
            {
                ctime++;
                pertime =  this->m_box->text().toUShort();


                emit startRead();          //读取电压与内阻数据
                //Sleep( pertime );      //延时读取数据

                //  this->sendData(ip,0,'C');
                //  this->startSend(this->serialname->currentText(),cmd, traynum, 'C',data);//继续切换

            }
            else
            {
                //                Single::m_Instance()->ctime = 1;
                //                pertime = Single::m_Instance()->total_sendbuf[4]|(Single::m_Instance()->total_sendbuf[5]<<8);
                //                Sleep( pertime );
                //
                //                this->sendData(ip,0,'C'); //继续切换
            }
        }

    }
    else if(cmd == 'A')
    {
        text =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
        qDebug()<<text+" AAAA";
    }

}


//
void ocvneedle::slotStartSwitch()
{
       Sleep(100);
    qDebug()<<"start  LLLL";


}






