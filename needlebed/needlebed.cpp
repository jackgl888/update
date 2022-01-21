#include "needlebed.h"

needlebed::needlebed(QWidget *parent) : QWidget(parent)
{

    //    this->m_layout = new  QGridLayout();
    //    this->m_layout->setMargin(5);
    //    this->m_layout->setSpacing(10);
   //    this->excelInit();
    //初始化数据库位
//    this->model  = new   QSqlQueryModel (this);

//    Single::m_Instance()->initDatabase(SQL_CREATE_TEMPRETURE );

//    this->model->setQuery("select  * from bedTemp");  //绑定数据
  //  this->slotInsertData();

    this->traybuf[30]={0};
    this->lock = 0;
    QStringList  list;
    list<<"115200"<<"57600"<<"38400"<<"19200"<<"9600";
    this->boundrate  = new  QComboBox(this);
    this->boundrate->addItems(list);
    this->boundratechs = new   QLabel(tr("波特率:"),this);
    this->start  = new  QPushButton(tr("针床压合"));
    this->stop  = new   QPushButton(tr("针床弹开"));
    this->conBed = new   QPushButton(tr("断开"));
    this->emgbtn = new   QPushButton(tr("急停"));
    this->clearBtn = new QPushButton(tr("清除"));
    this->parasetBtn  = new QPushButton(tr("参数设置"));
    this->parareadBtn  = new QPushButton(tr("参数读取"));
    this->negAirCntl  = new QPushButton(tr("负压控制"));
    this->negAirCheck  = new QPushButton(tr("负压检测"));
    this->sqlEnable    = new  QPushButton(tr("sql失能"),this);
    this->delSql     =  new  QPushButton(tr("删除数据"),this);
    this->serialname  =  new   QComboBox(this);
    this->comname = new   QLabel(tr("串口号:"),this);
    this->locationNum   = new  QComboBox(this);
    this->locationLab    = new QLabel(tr("库位号"),this);
    this->versionRead = new QLineEdit(this);
    this->versionRead->setReadOnly(true);
    this->versionLab =  new QLabel(tr("版本:"),this);
    list.clear();
    list<<"库位一"<<"库位二"<<"库位三"<<"库位四";
    this->locationNum->addItems(list);
    list.clear();
    list<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<"COM6"<<"COM7"<<"COM8"<<
          "COM9"<<"COM10"<<"COM11"<<"COM12"<<"COM13"<<"COM14"<<"COM15"<<"COM16"
       <<"COM17"<<"COM18";
    this->serialname->addItems(list);
    this->splitterPart = new   QSplitter(Qt::Horizontal,this);
    this->splitterPart->setOpaqueResize(true);
    this->stack = new    QStackedWidget( splitterPart);
    this->m_bedpart = new bedpart(this);

    this->stack->addWidget(this->m_bedpart );

    QHBoxLayout *BtnLayout = new QHBoxLayout;

    BtnLayout->setSpacing(6);
    BtnLayout->addWidget(this->versionLab);
    BtnLayout->addWidget(this->versionRead);
    BtnLayout->addWidget(this->locationLab);
    BtnLayout->addWidget(this->locationNum);
    BtnLayout->addWidget( this->comname  );
    BtnLayout->addWidget( this->serialname);
    BtnLayout->addWidget( this->boundratechs);
    BtnLayout->addWidget( this->boundrate );
    BtnLayout->addWidget(this->parasetBtn);
    BtnLayout->addWidget(this->parareadBtn);
    BtnLayout->addWidget(this->negAirCheck);
    BtnLayout->addWidget(this->sqlEnable);
    BtnLayout->addWidget(this->delSql);
    BtnLayout->addWidget(this->negAirCntl);
    BtnLayout->addWidget(this->start);
    BtnLayout->addWidget(this->stop);
    BtnLayout->addWidget(this->emgbtn);
    BtnLayout->addWidget(this->clearBtn);
    BtnLayout->addWidget(this->conBed);

    //  this->m_layout->addWidget(splitterPart);
    QVBoxLayout *RightLayout = new  QVBoxLayout(this);
 //   RightLayout->setMargin(10);
  //  RightLayout->setSpacing(6);
    RightLayout->addWidget(splitterPart);
    RightLayout->addLayout(BtnLayout);
    splitterPart->setMaximumSize( splitterPart->maximumSize());
    splitterPart->setMinimumSize(splitterPart->minimumSize());
    splitterPart->show();
    //this->m_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
    this->m_timer  = new QTimer(this);
    //  this->m_timer->start(200);
    connect(this->m_timer,SIGNAL(timeout()),this,SLOT(timeoutEventMethod()));//定时器中断
    connect( this->conBed,SIGNAL(clicked()),this,SLOT(on_opencloseserial_clicked()));  //联机
    connect( this->start,SIGNAL(clicked()),this,SLOT(on_start_clicked()));   //启动
    connect( this->stop,SIGNAL(clicked()),this,SLOT(on_stop_clicked()));   //停止
    connect( this->emgbtn,SIGNAL(clicked()),this,SLOT(on_emgbtn_clicked()));   //急停
    connect(this->clearBtn,SIGNAL(clicked()),this,SLOT(slotClear()));       //清除异常
    connect(this->parasetBtn,SIGNAL(clicked()),this,SLOT(slotParaSet()));  //参数设置
    connect(this->parareadBtn,SIGNAL(clicked()),this,SLOT(slotParaRead())); //参数读取
    connect(this->negAirCheck,SIGNAL(clicked()),this,SLOT(slotNegAirCheck()));//气密性检测
    connect(this->negAirCntl,SIGNAL(clicked()),this,SLOT(slotNegAirCntl()));  //负压控制
    connect(this->sqlEnable,SIGNAL(clicked()),this,SLOT(slotInserCntl())); // 数据插入控制
    connect(this->delSql,SIGNAL(clicked()),this,SLOT(slotDeleteSql()));   //数据删除

}


needlebed::~needlebed()
{

}


//初始化表格
needlebed::excelInit()
{
        this->xlsx=   new    QXlsx::Document("book3.xlsx",this);  //打开一个boot3的文件
        QXlsx::Format  format3;   //建立样式
        format3.setFontColor(QColor(Qt::red));
        format3.setPatternBackgroundColor(QColor(152,251,152));//北京颜色
        format3.setFontSize(10); //设置字体大小
        format3.setHorizontalAlignment(QXlsx::Format::AlignHCenter);//设置对齐方式
        format3.setBorderStyle(QXlsx::Format::BorderDashDotDot);//边框样式
        xlsx->write("A2","hello QQ",format3);
        xlsx->write(5,1,222);
        xlsx->saveAs("E:/testpro/book.xlsx");

}


void needlebed::timeoutEventMethod()
{
    static uchar times;

    static  uchar  time,flag;
    static  uint    time_t=0;


    times++;
    //采样与联机心跳
    if(this->lock == 0x00)
    {
        if(times ==5)
        {
            times = 0;
            startSend(this->serialname->currentText(),0,'A',0,0);
        }
        else
            startSend(this->serialname->currentText(),0,'S',0,0);
    }

#if 0

    if(!this->xlsx->selectSheet("iii"))
        this->xlsx->addSheet("iii");

    for(uchar i= 0;i<TEMPNUM ;i++)
    {

        if(i == 0)
        {
            this->xlsx->write(time_t+1,i+1,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: "));
        }
        else
        {
            if(this->bedTemp[i-1]==0)
            {
                this->xlsx->write(time_t+1,i+1,65535);
            }
            else
                this->xlsx->write(time_t+1,i+1,this->bedTemp[i-1]);
        }
    }
    time_t++;

    if(time_t ==  MAXLINE )
        time_t = 0;

    this->xlsx->saveAs("E:/testpro/book.xlsx");

#endif

}

void needlebed::on_opencloseserial_clicked()
{
    static uchar i =0;

    if(i%2 == 1)
    {

        this->conBed->setText("断开");
        this->m_timer->stop();
        emit  stopSerial(this->serialname->currentText());

    }
    else
    {

        this->conBed->setText("联机");
        this->m_timer->start(430);
        Single::m_Instance()->serialPara.clear();
        Single::m_Instance()->serialPara.append(this->boundrate->currentText());
        Single::m_Instance()->serialPara.append(this->serialname->currentText());
        this->trayInit(this->serialname->currentText(),4,0);//初始化托盘
        this->m_thread  =new serverThread(0,0x01,0,0);
        Single::m_Instance()->comBuf[1]=this->serialname->currentText();   //将当前串口放入分配位
        //串口发送
        connect(this, SIGNAL(startSend(QString,int,uchar,ushort,ushort)),
                m_thread, SLOT( sendDataSlot(QString,int,uchar,ushort,ushort)),Qt::UniqueConnection);
        //停止串口
        connect(this,SIGNAL(stopSerial(const QString &)),m_thread,SLOT(stopSerialSlot(const QString  &)),Qt::UniqueConnection);
        //线程退出
        connect(m_thread,SIGNAL(finished()),m_thread,SLOT(deleteLater()),Qt::UniqueConnection);
        //更新串口状态
        connect(m_thread,SIGNAL(updateUiSerial(const QString &)),this,SLOT(disconnectSerial(const QString &)),Qt::UniqueConnection);

        connect(m_thread,SIGNAL( bedUpdate(uchar,uchar,QString)),this,
                SLOT(slotUpdateCmdData(uchar,uchar,QString)));  //界面更新来自串口的数据

        //ui传递总的数据到工作线程
        connect(this,SIGNAL(sig(QVariant)),m_thread,SLOT(slotGetData(QVariant)));

        //接收工作线程传过来的数据
        connect(m_thread,SIGNAL(sigtu(QVariant)),this,SLOT(slotUiUpdate(QVariant)));

        //接收工作线程传过来的发送数据包
        connect(m_thread,SIGNAL(sigSdData(QVariant)),this,SLOT(slotGetSdData(QVariant)));

        this->m_thread->start();
        this->startTouch= 0x01;

    }
    i++;
    if(i == 100)
        i= 0;
}

//启动
void needlebed::on_start_clicked()
{
    uchar traynum;
    this->lock = 0x01;

    if(this->locationNum->currentText() == "库位一")
        traynum = 0;
    else if(this->locationNum->currentText() == "库位二")
        traynum = 1;
    else if(this->locationNum->currentText() == "库位三")
        traynum = 2;
    else if(this->locationNum->currentText() == "库位四")
        traynum = 3;


    emit startSend(this->serialname->currentText(),traynum,'L',0,this->needlebedPara[7]);  //
}

//停止
void needlebed::on_stop_clicked()
{
    uchar traynum;
    this->lock = 0x01;

    if(this->locationNum->currentText() == "库位一")
        traynum = 0;
    else if(this->locationNum->currentText() == "库位二")
        traynum = 1;
    else if(this->locationNum->currentText() == "库位三")
        traynum = 2;
    else if(this->locationNum->currentText() == "库位四")
        traynum = 3;
    emit startSend(this->serialname->currentText()  ,traynum,'P',0,0);  //
}



//急停
void needlebed::on_emgbtn_clicked()
{

    uchar traynum;
    this->lock = 0x01;

    if(this->locationNum->currentText() == "库位一")
        traynum = 0;
    else if(this->locationNum->currentText() == "库位二")
        traynum = 1;
    else if(this->locationNum->currentText() == "库位三")
        traynum = 2;
    else if(this->locationNum->currentText() == "库位四")
        traynum = 3;
    emit startSend(this->serialname->currentText(),traynum,'R',0,0);  //
}




//清除
void needlebed::slotClear()
{
    uchar traynum;
    this->lock = 0x01;

    if(this->locationNum->currentText() == "库位一")
        traynum = 0;
    else if(this->locationNum->currentText() == "库位二")
        traynum = 1;
    else if(this->locationNum->currentText() == "库位三")
        traynum = 2;
    else if(this->locationNum->currentText() == "库位四")
        traynum = 3;
    emit startSend(this->serialname->currentText(),traynum,'Z',0,0);  //
}


//参数设置
void needlebed::slotParaSet()
{
    uchar traynum;
    this->lock = 0x01;

    if(this->locationNum->currentText() == "库位一")
        traynum = 0;
    else if(this->locationNum->currentText() == "库位二")
        traynum = 1;
    else if(this->locationNum->currentText() == "库位三")
        traynum = 2;
    else if(this->locationNum->currentText() == "库位四")
        traynum = 3;
    emit startSend(this->serialname->currentText(),traynum,'M',0,0);
}

//参数读取
void needlebed::slotParaRead()
{
    uchar traynum;
    this->lock = 0x01;

    if(this->locationNum->currentText() == "库位一")
        traynum = 0;
    else if(this->locationNum->currentText() == "库位二")
        traynum = 1;
    else if(this->locationNum->currentText() == "库位三")
        traynum = 2;
    else if(this->locationNum->currentText() == "库位四")
        traynum = 3;
    emit startSend(this->serialname->currentText(),traynum,'U',0,0);

}

//负压控制
void needlebed::slotNegAirCntl()
{
    uchar traynum,buf[10];

    this->lock = 0x01;

    if(this->locationNum->currentText() == "库位一")
        traynum = 0;
    else if(this->locationNum->currentText() == "库位二")
        traynum = 1;
    else if(this->locationNum->currentText() == "库位三")
        traynum = 2;
    else if(this->locationNum->currentText() == "库位四")
        traynum = 3;

    buf[0]=this->needlebedPara[3];
    buf[1]=this->needlebedPara[4];
    buf[2]=this->needlebedPara[5];

    emit startSend(this->serialname->currentText(),traynum,'X',0,0);

}

//负压检测
void needlebed::slotNegAirCheck()
{
    uchar traynum,buf[10];
    this->lock = 0x01;

    if(this->locationNum->currentText() == "库位一")
        traynum = 0;
    else if(this->locationNum->currentText() == "库位二")
        traynum = 1;
    else if(this->locationNum->currentText() == "库位三")
        traynum = 2;
    else if(this->locationNum->currentText() == "库位四")
        traynum = 3;
    buf[0]=this->needlebedPara[0];
    buf[1]=this->needlebedPara[0]>>8;
    buf[2]=this->needlebedPara[2];
    buf[3]=this->needlebedPara[2]>>8;

    emit startSend(this->serialname->currentText(),traynum,0x11,0,0);

}


//库位初始化
void needlebed::trayInit(const QString &ip, const uchar traynum, const uchar flag)
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






void needlebed::slotUpdateCmdData(const uchar cmd, const uchar traynum,const QString  &str)
{
    QString  text;

    switch (cmd) {

    case 'A':
        this->versionRead->setText( str);
        break;

    case 'S':
        break;

    case 'L':
        text =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
        qDebug()<<text+" LLLL";
        emit  startSwitch();
        break;

    default:
        break;
    }
    // this->lock = 0x00;
    //  emit  updateMap(cmd,traynum,data);

}



void needlebed::slotBedStart()
{
    uchar traynum,buf[10];
    this->lock = 0x01;

    if(this->locationNum->currentText() == "库位一")
        traynum = 0;
    else if(this->locationNum->currentText() == "库位二")
        traynum = 1;
    else if(this->locationNum->currentText() == "库位三")
        traynum = 2;
    else if(this->locationNum->currentText() == "库位四")
        traynum = 3;
    buf[0]=this->needlebedPara[0];
    buf[1]=this->needlebedPara[0]>>8;
    buf[2]=this->needlebedPara[2];
    buf[3]=this->needlebedPara[2]>>8;

    emit startSend(this->serialname->currentText(),traynum,'L',0,0);


}


//接收工作线程传过的数据
void needlebed::slotUiUpdate(QVariant variant)
{

    QVariant    m_variant;
     bedPara m_para;
  //   memset( m_para.bedTemp,0,80);

     m_para = variant.value<bedPara>();

    this->lenData = m_para.dataLen;
    memcpy(this->traybuf,m_para.bedSensor,10);
    //
    for(uchar i = 0;i<100;i++)
    this->bedTemp[i] = m_para.bedTemp[i];
   // memcpy(this->bedTemp,m_para.bedTemp,100);  //接温度
    memcpy(this->sdrcBuf,m_para.bufData,200);

    this->negpressValue = m_para.negAir;  //接负压
   if(this->sqlInsertFlag == 1)
     this->slotInsertData();  //更新温度数据
 //   m_variant.setValue(m_para);
//    emit  sig(m_variant);

}








void needlebed::slotInsertData()
{
    int row ;
  //  this->model->setQuery("select *from bedTemp");
 //   row  = this->model->rowCount();
   //  for(uchar i  = 0;i<TEMPNUM;i++)
    //  this->bedTemp[i]= 777;
    // db.transaction();
    Single::m_Instance()->insertData(SQL_INSERT_TEMPRETURE,bedTemp,row,TEMPNUM);
    // Single::m_Instance()->updateData(SQL_CREATE_TEMPRETURE);
    //Single::m_Instance()->insertData( SQL_INSERT_TEMPRETURE ,this->bedTemp,row,41);


}


//数据更新控制
void needlebed::slotInserCntl()
{
    static uchar i =0;

    if(i%2 == 1)
    {
           this->sqlInsertFlag = 1;
           this->sqlEnable->setText("sql使能");
    }
    else
     {
          this->sqlInsertFlag = 0;
        this->sqlEnable->setText("sql失能");
    }

   i++;
   if(i == 10)
       i = 0 ;

}


void needlebed::slotDeleteSql()
{
    Single::m_Instance()->deleteData("delete  from bedTemp");
}








void needlebed::disconnectSerial(const QString &ip)
{

}
