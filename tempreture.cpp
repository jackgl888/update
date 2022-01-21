#include "tempreture.h"
#include<QHeaderView>
#include<QTimer>
#include "single/single.h"
#include <QDateTime>
#include <synchapi.h>
QSqlDatabase db;
tempreture::tempreture(QWidget *parent) : QWidget(parent)
{

    this->m_server   =new Server(this);
    this->m_server->listen(QHostAddress::Any,7500);  //监听端口及ipQHostAddress::Any
    //清零温度·
    for(ushort i = 0;i<   CHANNELS  ; i++)
    {
        this->temp[i] = 0;
        this->tempAdjustCode[i]=0;
    }

    this->synStart = 0;

    // 初始化保存温度的excel表
    this->excelInit();

    QStringList  list;
    list<<"115200"<<"57600"<<"38400"<<"19200"<<"9600";
    this->boundrate  = new  QComboBox(this);
    this->boundrate->addItems(list);
    list.clear();
    list<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<"COM6"<<"COM7"<<"COM8"<<
          "COM9"<<"COM10"<<"COM11"<<"COM12"<<"COM13"<<"COM14"<<"COM15"<<"COM16";
    this->comname = new QLabel(tr("串口："),this);
    this->serialname  = new QComboBox(this);   //串口名
    this->serialname->addItems(list);
    list.clear();
    list<<"温度"<<"修调码";
    this->modeTye = new QComboBox(this);
    this->modeTye->addItems(list);
    this->modeLable = new QLabel(tr("显示值："),this);
    this->boundratechs = new   QLabel(tr("波特率:"),this);

    // this->sanlingOnlineBtn = new QPushButton(tr("三菱脱机"),this);
    this->onlineBtn   =  new QPushButton(tr("脱机"),this);

    this->allChannels  = new QSpinBox(this);  //总通道数
    this->allChannelsLab  = new QLabel(tr("通道总数"),this);
    this->ipaddrlb   =  new  QLabel(tr("客户端IP："),this);
    this->ipaddr   = new   QLineEdit(this);
    this->m_iplb   =  new  QLabel(tr("本机IP"),this);
    this->m_ip    = new   QLineEdit(this);
    this->ipaddr->setReadOnly(true);  //只读
    this->touchFrock  = new  QPushButton(tr("联机"),this);  //工装联机
    this->TempLab  = new QLabel(tr("版本:"));
    this->TempVersion  = new QLineEdit(this);
    this->TempVersion->setReadOnly(true);

    QGroupBox  *netbox   = new QGroupBox(this);
    QHBoxLayout *iplayout  = new   QHBoxLayout();
    iplayout->addWidget(this->ipaddrlb);
    iplayout->addWidget(this->ipaddr);
    QHBoxLayout  *m_iplayout  = new QHBoxLayout();
    m_iplayout->addWidget(m_iplb);
    m_iplayout->addWidget(m_ip);


    this->slaveAddrLab  = new QLabel(tr("从站地址:"),this);
    this->slaveAddr  = new QSpinBox(this);
    this->slaveAddr->setMinimum(0);
    this->slaveAddr->setMaximum(245);
    this->slaveNumLab = new QLabel(tr("从站总数:"),this);
    this->slaveNum  = new QSpinBox(this);
    this->slaveNum->setMaximum(245);
    this->allChannels->setMaximum(256);
    //校准温度输入
    QLabel *adjustTempLab  = new QLabel(tr("仪器温度:"),this);
    this->tempAdjustBox   = new  QSpinBox(this);
    tempAdjustBox->setMinimum(0);
    tempAdjustBox->setMaximum(800);
    QHBoxLayout  *adjustLayout = new QHBoxLayout();
    adjustLayout->addWidget(adjustTempLab);
    adjustLayout->addWidget(tempAdjustBox);

    //校准button
    QPushButton  *adjustBtn  = new QPushButton(tr("校准"),this);
    this->codeClearBtn    = new QPushButton("清零校准",this);
    QHBoxLayout   *  adjustBtnLayout = new QHBoxLayout();

    adjustBtnLayout->addWidget(adjustBtn);
    adjustBtnLayout->addWidget( this->codeClearBtn );


    this->guilingPress  = new  QPushButton(tr("压力清零:"),this);
    this->sample  = new  QPushButton(tr("采样开始:"),this);
    QVBoxLayout  *netlayout  = new QVBoxLayout();
    netlayout->addLayout(iplayout);
    netlayout->addLayout(m_iplayout);
    netlayout->addWidget(this->guilingPress);  //压力归零
    netlayout->addWidget(this->touchFrock);
    netlayout->addWidget(this->sample);  //开始采样
    netbox->setFixedWidth(250);
    netbox->setLayout(netlayout);

    QHBoxLayout  * serialcom  = new  QHBoxLayout();
    QHBoxLayout  *sbound  =new QHBoxLayout();
    QHBoxLayout  *smode  =new QHBoxLayout();
    QHBoxLayout  *sprotocol = new QHBoxLayout();
    QHBoxLayout  *sslaveAddr = new QHBoxLayout();
    QHBoxLayout   *sslaveNum  = new  QHBoxLayout();
    QHBoxLayout   *sallchannels  = new QHBoxLayout();
    QHBoxLayout   *version = new  QHBoxLayout();

    version->addWidget(this->TempLab);
    version->addWidget(this->TempVersion);
    version->addWidget(this->onlineBtn);

    QGroupBox     * serialBox = new    QGroupBox ();
    serialcom->addWidget(this->comname);
    serialcom->addWidget(this->serialname);
    sbound->addWidget( this->boundratechs);
    sbound->addWidget(this->boundrate);
    smode->addWidget(this->modeLable);
    smode->addWidget(this->modeTye);

    sslaveAddr->addWidget(this->slaveAddrLab);
    sslaveAddr->addWidget(this->slaveAddr);
    sslaveNum->addWidget(this->slaveNumLab);//从站总数
    sslaveNum->addWidget(this->slaveNum);
    sallchannels->addWidget(this->allChannelsLab);  //通道总数
    sallchannels->addWidget(this->allChannels);
    this->serialLayout  =new QVBoxLayout();

    this->serialLayout->addLayout(serialcom);  //串口集合
    this->serialLayout->addLayout(sbound);
    this->serialLayout->addLayout(smode);//模式设置
    this->serialLayout->addLayout(sprotocol);  //协议选择
    this->serialLayout->addLayout(sslaveAddr);//从站地址选择
    this->serialLayout->addLayout(sslaveNum);  //从站总数
    this->serialLayout->addLayout(sallchannels);  //通道总数
    this->serialLayout->addLayout(version);  //版本号
    this->serialLayout->addLayout(adjustLayout);
    this->serialLayout->addLayout(adjustBtnLayout);
    // this->serialLayout->addWidget(this->onlineBtn);   //联机
    //  this->serialLayout->addWidget(this->sanlingOnlineBtn);

    serialBox->setFixedWidth(290);
    serialBox->setLayout(this->serialLayout);

    this->tempCntOnlinelBtn  = new QPushButton(tr("脱机"),this);  //温控联机
    this->tempCntlBoundrateLab   = new QLabel("波特率:");
    this->tempCntlBoundrate   = new QComboBox(this);
    this->tempCntlSerialName  = new QComboBox(this);
    this->tempCntlSerialNamelab = new QLabel(tr("串口号："),this);

    list.clear();
    list<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<"COM6"<<"COM7"<<"COM8"<<
          "COM9"<<"COM10"<<"COM11"<<"COM12"<<"COM13"<<"COM14"<<"COM15"<<"COM16";
    this->tempCntlSerialName ->addItems(list);
    list.clear();
    list<<"115200"<<"57600"<<"38400"<<"19200"<<"9600";
    this->tempCntlBoundrate->addItems(list);
    this->tpclChannels  = new   QSpinBox(this);
    this->tpclChannelsLab  = new   QLabel(tr("通道总数："),this);
    this->tpclChannels->setMaximum(256);
    this->tempLable  = new QLabel(tr("设置温度:"),this);
    this->setTemp    = new QSpinBox(this);
    this->channeLabel   = new QLabel(tr("通道号:"),this);
    this->channel        = new QSpinBox(this);
    this->startHeat      = new  QPushButton(tr("开始恒温"),this);
    this->stopHeat      =  new  QPushButton(tr("停止恒温"),this);
    this->exportData    = new   QPushButton(tr("导出数据"),this);
    this->setTemp->setMaximum(1000);    //最大加热到100度
    this->channel->setMaximum(256);     //最大通道数量为256
    QHBoxLayout  *stplayout  = new QHBoxLayout();
    stplayout->addWidget(this->tempLable);
    stplayout->addWidget(this->setTemp);

    QHBoxLayout  *chlayout  = new QHBoxLayout();
    chlayout->addWidget(this->channeLabel);
    chlayout->addWidget(this->channel);

    QHBoxLayout  *tempbandrate = new QHBoxLayout();
    tempbandrate->addWidget(this->tempCntlBoundrateLab);
    tempbandrate->addWidget(this->tempCntlBoundrate);

    QHBoxLayout *tempchannellayout   = new QHBoxLayout();
    tempchannellayout->addWidget(this->tpclChannelsLab);
    tempchannellayout->addWidget(this->tpclChannels);

    QHBoxLayout   *tempCntlSerialNamelayout = new QHBoxLayout();
    tempCntlSerialNamelayout->addWidget( this->tempCntlSerialNamelab );
    tempCntlSerialNamelayout->addWidget( this->tempCntlSerialName);

    this->tempcntlLayout =  new QVBoxLayout();

    this->tempcntlLayout->addLayout(tempCntlSerialNamelayout);
    this->tempcntlLayout->addLayout(tempbandrate);
    this->tempcntlLayout->addLayout(stplayout);
    this->tempcntlLayout->addLayout(chlayout);
    this->tempcntlLayout->addLayout(tempchannellayout );
    this->tempcntlLayout->addWidget(this->startHeat);
    this->tempcntlLayout->addWidget(this->stopHeat);
    this->tempcntlLayout->addWidget(this->exportData);
    this->tempcntlLayout->addWidget(this->tempCntOnlinelBtn);

    QGroupBox    *tempBox =new QGroupBox(this);
    tempBox->setFixedWidth(290);
    tempBox->setLayout(this->tempcntlLayout);

    this->tempBrowser  = new  QTextBrowser(this);  //信息窗口
    this->msgClearBtn  = new QPushButton("消息清除",this) ;  //消息清除

    QHBoxLayout  *clearBtnLayout  = new QHBoxLayout();
    clearBtnLayout->addStretch(10);
    clearBtnLayout->addWidget(msgClearBtn);
    QVBoxLayout   *browserLayout  = new QVBoxLayout();
    browserLayout->addWidget(tempBrowser);
    browserLayout->addLayout(clearBtnLayout);
#if 0

    //创建数据库
    // QSqlDatabase


    //    db = QSqlDatabase::addDatabase("QSQLITE");
    //    db.setDatabaseName("databasefile.db");

    //    if (!db.open())
    //    {
    //        return;
    //    }

    //   this->model  = new  QSqlTableModel(this,db);

    this->model  = new   QSqlQueryModel (this);  //数据库位绑定的模型

    //  model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    Single::m_Instance()->initDatabase(SQL_CREATE_TEMPRETURE);  //初始化数据库


    //  this->creatDB();

    //     QSqlQuery query;

    //    this->query->exec("create table cars (carid int primary key, name varchar(50), factoryid int, year int, foreign key(factoryid) references factory(id))");	//(c)
    //    this->query->exec(QObject::tr("insert into cars values(1,'奥迪A6',1,2005)"));
    //    this->query->exec(QObject::tr("insert into cars values(2, '捷达', 1, 1993)"));
    //    this->query->exec(QObject::tr("insert into cars values(3, '宝来', 1, 2000)"));
    //    this->query->exec(QObject::tr("insert into cars values(4, '毕加索',2, 1999)"));
    //    this->query->exec(QObject::tr("insert into cars values(5, '富康', 2, 2004)"));
    //    this->query->exec(QObject::tr("insert into cars values(6, '标致307',2, 2001)"));
    //    this->query->exec(QObject::tr("insert into cars values(7, '桑塔纳',3, 1995)"));
    //    this->query->exec(QObject::tr("insert into cars values(8, '帕萨特',3, 2000)"));


    //this->slotInsertData();
    model->setQuery("SELECT * FROM  bedtemp1");

    // model->setTable("te");
    //   model->select();

    //  int column = model->columnCount();	//获取列数
    //  int row = model->rowCount();		//获取行数


    //    for(uchar i = 0;i<7;i++)
    //        this->temp[i]=666;
    //    for(uchar j = 0;j<6;j++)
    //    {
    // model->insertRow(row+j) ;
    //        for(uchar i = 1;i<7;i++)
    //            model->setData(model->index(row+j,i),this->temp[i]);

    //    }

    //  model->setQuery("SELECT * FROM  temp WHERE temp1 >=300 ");
    // model->select();
    //   model->submit();  //提交修改
#endif
    this->tempList   = new   QTableView(this);
    this->tempModle   =new   QStandardItemModel(this);
    this->tempList->setModel(    this->tempModle);    //绑定模型
    this->tempList ->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置表格内容不可编辑
    this->tempList ->horizontalHeader()->setDefaultAlignment(Qt::  AlignBaseline);
    this->tempList ->horizontalHeader()->setSectionResizeMode(QHeaderView:: Stretch);
    this->tempList ->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->tempList->verticalHeader()->setVisible(true);
    this->tempList->horizontalHeader()->setVisible(true);
    this->tempList->setAlternatingRowColors(true);
    this->tempList->verticalHeader()->setStyleSheet("QHeaderView::section {background-color: rgb(255, 255, 255)}");
    this->tempList->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color: rgb(255, 255, 255)}");
    this->tempList->setGridStyle(Qt:: SolidLine);

    this->calculate  = new QComboBox(this);
    list.clear();
    list<<"平均值"<<"项数"<<"总和"<<"最大值"<<"最小值"<<"方差";
    this->calculateLab = new QLabel(tr("计算公式:"),this);
    QHBoxLayout  *calculateLayout = new  QHBoxLayout();
    calculateLayout->addWidget(this->calculateLab);
    calculateLayout->addWidget(this->calculate);

    this->calculate->addItems(list);
    this->conditionLab  = new QLabel("查询条件:");
    this->conditionBox  = new QComboBox(this);

    list.clear();
    list<<"索引"<<"值";
    this->conditionBox->addItems(list);
    this->orderWayLab  = new QLabel(tr("排序方式:"),this);
    this->orderWay     = new QComboBox(this);
    list.clear();
    list<<"升序"<<"降序";
    this->orderWay->addItems(list);
    this->startInsert   = new QPushButton(tr("插入"),this);
    this->selectBtn    = new QPushButton(tr("查询"),this);
    this->deleteBtn  = new  QPushButton(tr("删除"),this);
    this->orderBtn    =new  QPushButton(tr("排序"),this);
    this->calculateBtn  = new QPushButton(tr("计算"),this);
    this->MinStart = new QLabel(tr("起始:"),this);  //起始删除id
    this->MaxEnd   =  new QLabel(tr("结束:"),this);
    this->valueMax  = new  QSpinBox(this);
    this->valueMin   =  new  QSpinBox(this);

    QHBoxLayout   *orderLayout  = new QHBoxLayout();
    orderLayout ->addWidget(this->orderWayLab);
    orderLayout->addWidget(this->orderWay);
    //
    QHBoxLayout  *conditioLayout = new QHBoxLayout();
    conditioLayout->addWidget(this->conditionLab);
    conditioLayout->addWidget(this->conditionBox);
    //
    QHBoxLayout   *minLayout  = new QHBoxLayout();
    minLayout->addWidget(this->MinStart);
    minLayout->addWidget(this->valueMin);
    //
    QHBoxLayout  *maxLayout = new QHBoxLayout();
    maxLayout->addWidget(this->MaxEnd);
    maxLayout->addWidget(this->valueMax);
    QHBoxLayout *btnLayout_1 =  new QHBoxLayout();
    btnLayout_1->addWidget(this->startInsert);
    btnLayout_1->addWidget(this->selectBtn);
    QHBoxLayout   *btnLayout_2 = new  QHBoxLayout();
    btnLayout_2->addWidget(this->orderBtn);
    btnLayout_2->addWidget(this->deleteBtn);
    QHBoxLayout   *btnLayout_3 = new  QHBoxLayout();
    btnLayout_3->addWidget(this->calculateBtn);

    this->topLayout   =  new  QVBoxLayout();
    this->topLayout->addLayout(conditioLayout);
    this->topLayout->addLayout(minLayout);
    this->topLayout->addLayout(maxLayout);
    this->topLayout->addLayout(orderLayout);
    this->topLayout->addLayout(calculateLayout);

    this->btnBottomLayout  = new QVBoxLayout();
    this->btnBottomLayout->addLayout(btnLayout_1);
    this->btnBottomLayout->addLayout(btnLayout_2);
    this->btnBottomLayout->addLayout(btnLayout_3);
    this->sqlmainlayout  = new QVBoxLayout();
    this->sqlmainlayout->addLayout(this->topLayout);
    this->sqlmainlayout->addLayout(this->btnBottomLayout);

    this->sqlBox  =new QGroupBox(this);

    this->sqlBox->setLayout(this->sqlmainlayout);

#if 1
    QStandardItem   *item;
    for(int i = 0;i<9;i++)
    {
        item  = new QStandardItem ();
        this->tempModle->setItem(0,i,item);
        item  = new QStandardItem ();
        this->tempModle->setItem(1,i,item);
        item  = new QStandardItem ();
        this->tempModle->setItem(2,i,item);
        item  = new QStandardItem ();
        this->tempModle->setItem(3,i,item);
        item  = new QStandardItem ();
        this->tempModle->setItem(4,i,item);
        item  = new QStandardItem ();
        this->tempModle->setItem(5,i,item);
    }
#endif
    this->bottomLayout  =new QHBoxLayout();

     this->bottomLayout->addLayout(browserLayout);
    this->bottomLayout->addWidget( serialBox);

    this->bottomLayout->addWidget(netbox);
    this->bottomLayout->addWidget( tempBox);
    this->bottomLayout->addWidget(this->sqlBox);


    // this->m_layout->setSizeConstraint(QLayout::SetFixedSize);
    this->mainlayout  = new QGridLayout(this);
    this->mainlayout->addWidget(this->tempList,0,0,250,100);
    this->mainlayout->addLayout(this->bottomLayout,250,0,100, 100);

    this->tempTimer   = new  QTimer(this);

    connect(this->onlineBtn,SIGNAL(clicked()),this,SLOT(omronProtocolInit()));  //联机
    connect(this->tempTimer,SIGNAL(timeout()),this,SLOT(timeoutMethod()));  //
    connect(this->startHeat,SIGNAL(clicked()),this,SLOT(slotStartHeat()));
    connect(this->stopHeat,SIGNAL(clicked()),this,SLOT(slotStopHeat()));
    connect(this->exportData,SIGNAL(clicked()),this,SLOT(slotExportData()));  //导出数据
    connect(this->guilingPress,SIGNAL(clicked()),this,SLOT(guilingPressslot()));   //压力归零
    connect(this->touchFrock,SIGNAL(clicked()),this,SLOT(touchFrockslot()));  //工装联机
    connect(this->sample,SIGNAL(clicked()),this,SLOT(sampleSlot()));
    connect(this->tempCntOnlinelBtn,SIGNAL(clicked()),this,SLOT(tempCntlInit()));    //温控联机
    //  connect(this->sanlingOnlineBtn,SIGNAL(clicked()),this,SLOT(sanlingProtocolInit()));//初始化菱协议
    connect(this->deleteBtn,SIGNAL(clicked()),this,SLOT(slotDeleteData()));  //数据删除操作
    connect(this->selectBtn,SIGNAL(clicked()),this,SLOT(slotSelectData())); //数据查询处理
    connect(this->startInsert,SIGNAL(clicked()),this,SLOT(slotInsertData()));//数据插入处理
    connect(this->orderBtn,SIGNAL(clicked()),this,SLOT(slotOrderData())); //数据排序
    connect(this->calculateBtn,SIGNAL(clicked()),this,SLOT(slotCalculateData()));  //数据计算
    connect(adjustBtn,SIGNAL(clicked()),this,SLOT(tempAdjustSlot()));  //修调
    connect(this->msgClearBtn,SIGNAL(clicked()),this,SLOT(msgClearSlot()));  //消息清除
     connect(this->codeClearBtn,SIGNAL(clicked()),this,SLOT(codeClearSlot()));     //清零校准

}



tempreture::~tempreture()
{
    
}

void tempreture::initUiTemp()
{
    
}

void tempreture::creatDB()
{
#if 1
    this->query  = new QSqlQuery();


    this->query->exec("create table tekk (id int primary key, t1 int,"
                      "t2 int,t3 int,t4 int, t5 int,t6 int,t7 int,"
                      "t8 int,t9 int,t10 int, t11 int,t12 int,t13 int,"
                      "t14 int,t15 int,t16 int, t17 int,t18 int,t19 int ,"
                      "t20 int,t21 int,t22 int, t23 int,t24 int,t25 int,"
                      "t26 int,t27 int,t28 int, t29 int,t30 int,t31 int,"
                      "t32 int,t33 int,t34 int, t35 int,t36 int,t37 int,"
                      "t38 int,t39 int,t40 int)");

    //           this->query->exec("select * from te");
    //           QSqlRecord rec = this->query->record();
    //           qDebug() << QObject::tr("te numbres：" )<< rec.count();

    this->query->exec("delete from  tekk");
    db.transaction();
    this->query->prepare("insert into tekk values(?,?,?,?,?,?,?,?,?,"
                         "?,?,?,?,?,?,?,?,"
                         "?,?,?,?,?,?,?,?,"
                         "?,?,?,?,?,?,?,?,"
                         "?,?,?,?,?,?,?,?)");
    //    //(b)

    for(int i = 0; i<10;i++)
    {

        for(int j = 0;j<41;j++)
            this->query->bindValue(j,i);

        this->query->exec();

        //            if(!success)
        //            {
        //                QSqlError lastError=query->lastError();
        //                qDebug()<<lastError.driverText()<<QString(QObject::tr("插入失败"));
        //            }

    }
    db.commit();
    //  success=this->query->exec();
    //    if(!success)
    //    {
    //        QSqlError lastError=this->query->lastError();
    //        qDebug()<<lastError.driverText()<<QString(QObject::tr("wrong"));
    //    }
    //    this->query->exec(QObject::tr("insert into te values(1, 111, 111,111, 111,111, 111,111)"));
    //    this->query->exec(QObject::tr("insert into te values(2, 211, 211,211, 211,211, 211,211)"));
    //    this->query->exec(QObject::tr("insert into te values(3, 311, 311,311, 311,311, 311,311)"));

    //    this->query->exec(QObject::tr("insert into temp values(4, 111, 111,111, 111,111, 111,111)"));
    //    this->query->exec(QObject::tr("insert into temp values(5, 211, 211,211, 211,211, 211,211)"));
    //    this->query->exec(QObject::tr("insert into temp values(6, 311, 311,311, 311,311, 311,311)"));

    //    this->query->exec("create table cars (carid int primary key, name varchar(50), factoryid int, year int, foreign key(factoryid) references factory(id))");	//(c)
    //    this->query->exec(QObject::tr("insert into cars values(1,'奥迪A6',1,2005)"));
    //    this->query->exec(QObject::tr("insert into cars values(2, '捷达', 1, 1993)"));
    //    this->query->exec(QObject::tr("insert into cars values(3, '宝来', 1, 2000)"));
    //    this->query->exec(QObject::tr("insert into cars values(4, '毕加索',2, 1999)"));
    //    this->query->exec(QObject::tr("insert into cars values(5, '富康', 2, 2004)"));
    //    this->query->exec(QObject::tr("insert into cars values(6, '标致307',2, 2001)"));
    //    this->query->exec(QObject::tr("insert into cars values(7, '桑塔纳',3, 1995)"));
    //    this->query->exec(QObject::tr("insert into cars values(8, '帕萨特',3, 2000)"));


#endif
}






void tempreture::timeoutMethod()
{
    static  uchar  time,flag;
    static  uint    time_t=0;
    int traynum;
    uchar num=this->allChannels->text().toUShort();

#if 0
    //   if(time == 5)
    //    {
    //        time = 0;
    //        if(!this->xlsx->selectSheet("iii"))
    //            this->xlsx->addSheet("iii");
    //        for(uchar k = 0;k<32;k++)
    //        this->xlsx->write(k,1,2);
    //        time_t++;//一秒一行，一行32 列
    if(!this->xlsx->selectSheet("iii"))
        this->xlsx->addSheet("iii");
    //    for(uchar k = 0;k<32;k++)
    //    this->xlsx->write(k,1,2);
    for(uchar i= 0;i<35;i++)
    {

        if(i == 0)
        {
            this->xlsx->write(time_t+1,i+1,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: "));
        }

        else
        {
            if(this->temp[i-1]==0)
            {
                this->xlsx->write(time_t+1,i+1,65535);
            }
            else
                this->xlsx->write(time_t+1,i+1,this->temp[i-1]);
        }

        //  this->xlsx->write(i%10+1,i/10+1,888);
    }
    time_t++;
    if(time_t == 65530)
        time_t = 0;
    time=0;
    //  for(uchar k = 0;k<34;k++)
    //      this->temp[k]=0;
    this->xlsx->saveAs("./book.xlsx");

    //  }
    //   time++;
#endif

    if(this->startSp == 0x01)
    {
        traynum  = this->slaveAddr->text().toInt();
        // emit  startSend(this->serialname->currentText(),traynum,0x03,0,num);  //下发轮询指令
#if 1

#endif
    }
    if(this->synStart == 0)
    {
        tempPara  m_para;
        m_para.all_chanels  = this->allChannels->text().toUShort();
        m_para.slaveNum = this->slaveNum->text().toUShort();
        m_para.protocol =  0x01;   //欧姆龙协议
        QVariant    variant;
        variant.setValue(m_para);
        emit  sig(variant);

        this->synStart  = 1;
    }
}




void tempreture::omronProtocolInit(void)
{
    static uchar i =0;

    if(i%2 == 1)
    {
        this->onlineBtn->setText("脱机");
        this->startSp =0;
        emit  stopSerial(this->serialname->currentText());
        this->tempTimer->stop();
    }
    else
    {
        this->tempTimer->start(500);

        this->onlineBtn->setText("联机");
        Single::m_Instance()->serialPara.clear();
        Single::m_Instance()->serialPara.append(this->boundrate->currentText());
        Single::m_Instance()->serialPara.append(this->serialname->currentText());

        this->trayInit(this->serialname->currentText(),this->slaveNum->text().toUShort(),0);//初始化托盘
        this->m_thread  =new serverThread(0,0x01,0,0);
        Single::m_Instance()->comBuf[0]= this->serialname->currentText();   //将

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

        connect(m_thread,SIGNAL( bedUpdate(const uchar,const uchar,const QString &)),this,
                SLOT(slotUpdateCmdData(const uchar,const uchar,const QString &)));  //界面更新来自串口的数据

        //UI传递总的数据到工作线程
        connect(this,SIGNAL(sig(QVariant)),m_thread,SLOT(slotGetData(QVariant)));

        //接收工作线程传过来的数据
        connect(m_thread,SIGNAL(sigtu(QVariant)),this,SLOT(slotUiUpdate(QVariant)));

        this->m_thread->start();

        this->startSp = 0x01;  //开始采样

    }
    i++;
    if(i == 100)
        i= 0;
}


//三菱协议初始化
void tempreture::sanlingProtocolInit()
{
    static uchar i =0;

    if(i%2 == 1)
    {
        this->sanlingOnlineBtn->setText("三菱脱机");
        this->startSp =0;
        emit  stopSerial(this->serialname->currentText());
        this->tempTimer->stop();
    }
    else
    {

        this->tempTimer->start(200);
        this->sanlingOnlineBtn->setText("三菱联机");
        Single::m_Instance()->serialPara.clear();
        Single::m_Instance()->serialPara.append(this->boundrate->currentText());
        Single::m_Instance()->serialPara.append(this->serialname->currentText());

        this->trayInit(this->serialname->currentText(),this->slaveNum->text().toUShort(),0);//初始化托盘
        this->m_thread  =new serverThread(0,0x01,0,0);
        Single::m_Instance()->comBuf[8]= this->serialname->currentText();   //将当前串口放入分配位

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

        connect(m_thread,SIGNAL( bedUpdate(uchar,uchar,const QString &)),this,
                SLOT(slotUpdateCmdData(uchar,uchar,const QString & )));  //界面更新来自串口的数据

        //UI传递总的数据到工作线程
        connect(this,SIGNAL(sig(QVariant)),m_thread,SLOT(slotGetData(QVariant)));

        //接收工作线程传过来的数据
        connect(m_thread,SIGNAL(sigtu(QVariant)),this,SLOT(slotUiUpdate(QVariant)));

        this->m_thread->start();

        this->startSp = 0x01;  //开始采样

    }
    i++;
    if(i == 100)
        i= 0;
}




void tempreture::trayInit(const QString &ip,const uchar traynum,const uchar flag)
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

void tempreture::slotStartHeat()
{


#if 0
    emit startSend(this->serialname->currentText(),253,
                   'L',this->channel->text().toUShort(),this->setTemp->text().toUShort());  //温度校准工装地址
#endif
}

void tempreture::slotStopHeat()
{

    emit startSend(this->serialname->currentText(),253,'P',this->channel->text().toUShort(),0);

}


//表格初始化
void tempreture::excelInit()
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

    xlsx->saveAs("./book.xlsx");

}


//导出数据
void tempreture::slotExportData()
{
    QString   msg;
    msg =QFileDialog:: getSaveFileName(this,"保存");


    //    if(!this->xlsx->selectSheet("iii"))
    //        this->xlsx->addSheet("iii");
    //    for(uchar k = 0;k<32;k++)
    //    this->xlsx->write(k,1,2);
    //    for(uchar i= 0;i<50;i++)
    //    {
    //         this->xlsx->write(i%10+1,i/10+1,2);
    //      //  this->xlsx->write(i%10+1,i/10+1,888);
    //    }
    this->xlsx->saveAs(msg);
    this->tempBrowser->append("导出成功！");

}

void tempreture::guilingPressslot()
{

    emit  startSend(this->serialname->currentText(),0,'P',0,0);
}


void tempreture::touchFrockslot()
{

    emit  startSend(this->serialname->currentText(),0,'A',0,0);
}


void tempreture::sampleSlot()
{
    static  uchar i =0;

    if(i%2 == 0)
    {
        this->tempTimer->start(500);

        this->sample->setText("停止采样");
    }
    else
    {
        this->tempTimer->stop();

        this->sample->setText("开始采样");
    }
    i++;
    if(i == 10)
        i = 0;
}



//温度工装串口控制
void tempreture::tempCntlInit()
{
    static uchar i =0;

    if(i%2 == 1)
    {
        this->tempCntOnlinelBtn->setText("脱机");
        emit  stopSerial(this-> tempCntlSerialName->currentText());
        this->tempTimer->stop();
    }

    {
        this->tempTimer->start(1000);

        this->tempCntOnlinelBtn->setText("联机");
        Single::m_Instance()->serialPara.clear();
        Single::m_Instance()->serialPara.append(this->tempCntlBoundrate->currentText());
        Single::m_Instance()->serialPara.append(this->tempCntlSerialName->currentText());

        this->trayInit(this->tempCntlSerialName->currentText(),253,0x01);//初始化托盘
        this->m_thread  =new serverThread(0,0x01,0,0);
        Single::m_Instance()->comBuf[6]= this->tempCntlSerialName->currentText();   //将当前串口放入分配位

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

        //UI传递总的数据到工作线程
        //   connect(this,SIGNAL(sigtu(QVariant)),m_thread,SLOT(slotGetData(QVariant)));

        //接收工作线程传过来的温度数据
        connect(m_thread,SIGNAL(sigTempBack(QVariant)),this,SLOT(slotUiUpdate(QVariant)));

        this->m_thread->start();

    }
    i++;
    if(i == 100)
        i= 0;
}


//打印运行信息
void tempreture::slotUpdateCmdData(const uchar cmd, const uchar traynum,const QString  &str)
{
    //if(str.length())
    switch (cmd) {
    case 'A':
        if(str.contains("版本号"))
        {

            if(this->slaveAddr->text().toUShort()== traynum)
            {
                QStringList  list = str.split(".");
                this->TempVersion->setText(list.at(1));
            }

        }
        else
            this->tempBrowser->append(str);
        break;
    case 'X':
          this->tempBrowser->append(str);
        break;
    default:
        break;
    }


}


//
void tempreture::slotUiUpdate(QVariant variant)
{

    uchar id1,id2;

    valuetp m_para = variant.value<valuetp>();
    id1 = this->slaveAddr->text().toUShort();
    id2 = m_para.tpId;
    if (this->slaveAddr->text().toUShort()== m_para.tpId)  //显示当前选择的id的信息
    {

        memcpy(this->temp,m_para.tpValues,500);

        if(m_para.adjustFlag == 1)   //进行修调
        {
            for(ushort i = 0;i< this->allChannels->text().toUShort();i++)//
            {
                this->tempAdjustCode[i]=m_para.adjustCode[i];
            }
            m_para.adjustFlag = 0;
        }
        else
        {
            if(this->modeTye->currentText()=="修调码")
            {
                for(ushort i = 0;i< this->allChannels->text().toUShort();i++)//显示修调码
                    this->tempModle->setData(this->tempModle->index(i/9,i%9,QModelIndex()),this->tempAdjustCode[i]);
            }
            else {
                for(ushort i = 0;i< this->allChannels->text().toUShort();i++)//显示温度
                {
                    if(temp[i]==65535)
                     this->tempModle->item(i/9,i%9)->setBackground(Qt::yellow);
                    else
                     this->tempModle->item(i/9,i%9)->setBackground(Qt::color0);

                     this->tempModle->setData(this->tempModle->index(i/9,i%9,QModelIndex()),this->temp[i]);
                }

            }

        }

    }

}


void tempreture::slotTestSql()
{

    static  uchar i = 0;
    if(i%2 == 0)
    {
        model->setQuery("select *  from te ");

    }
    else
    {
        QString   value = QString::number(21);
        QString  word = "select *  from te  where  temp7 >"+value;

        model->setQuery("select *  from te  where  temp1 >value");
        model->setQuery("select *  from te  where  temp2 >value");
        model->setQuery("select *  from te  where  temp3 >value");
        model->setQuery("select *  from te  where  temp4 >value");
        model->setQuery("select *  from te  where  temp5 >value");
        model->setQuery("select *  from te  where  temp6 >value");
        model->setQuery(word);

    }
    i++;
    if(i == 10)
        i = 0;


}


//插入数据
void tempreture::slotInsertData()
{
    int row = this->model->rowCount();
    for(uchar i = 0;i<52;i++)
        this->temp[i] = 777;

    // db.transaction();
    Single::m_Instance()->insertData( SQL_INSERT_TEMPRETURE ,this->temp,row,52);


#if 0
    this->query->prepare("insert into tekk values(?,?,?,?,?,?,?,?,?,"
                         "?,?,?,?,?,?,?,?,"
                         "?,?,?,?,?,?,?,?,"
                         "?,?,?,?,?,?,?,?,"
                         "?,?,?,?,?,?,?,?)");
    for(uchar j = 0;j<10;j++)
    {
        for(int i = 0; i<41;i++)
        {
            if(i == 0)
                this->query->bindValue(i,j+row);
            else
                this->query->bindValue(i,this->temp[i]);

            bool success= this->query->exec();

            if(!success)
            {
                QSqlError lastError=query->lastError();
                qDebug()<<lastError.driverText()<<QString(QObject::tr("插入失败"));
            }
        }
    }
#endif
    //   db.commit();
    //  Single::m_Instance()->database.commit();
    // model->setQuery("SELECT * FROM  tekk");

}

void tempreture::slotDeleteData()
{
    QString   minValue = QString::number(this->valueMin->value());
    QString   maxValue = QString::number(this->valueMax->value());
    QString  word = "delete from tekk  where  id  >= "+minValue+" and id <="+maxValue;

    if((this->valueMax->value() == 0)&&(this->valueMin->value()==0))    //删除整个表
    {
        model->setQuery("delete  from tekk");
        model->setQuery("SELECT * FROM  tekk");
    }else     //指定删除某些行
    {
        model->setQuery(word);
        model->setQuery("SELECT * FROM  tekk");
        //   this->query->exec();
    }

}

void tempreture::slotSelectData()
{
    // db.transaction();
    QString  word ;
    if((this->valueMax->value() == 0)&&(this->valueMin->value()==0))    //显示整个表
    {

        word = "select *  from tekk ";

        model->setQuery(word);


    }
    else  //按条件显示表
    {
        if(this->conditionBox->currentText() == "索引")   //按索引
        {
            QString   minValue = QString::number(this->valueMin->value());
            QString   maxValue = QString::number(this->valueMax->value());
            QString  word = "select *  from tekk  where  id  between "+minValue+" and "+maxValue;
            //   Single::m_Instance()->selectData(word,this->valuesList);
            model->setQuery(word);

        }else  //按值
        {
            QString   minValue = QString::number(this->valueMin->value());
            QString   maxValue = QString::number(this->valueMax->value());
            QString  word1 = "select *  from tekk  where  t1 between "+minValue+" and "+maxValue;
            QString  word2 = "select *  from tekk  where  t2 between "+minValue+" and "+maxValue;
            QString  word3 = "select *  from tekk  where  t3 between "+minValue+" and "+maxValue;
            QString  word4 = "select *  from tekk  where  t4 between "+minValue+" and "+maxValue;
            QString  word5 = "select *  from tekk  where  t5 between "+minValue+" and "+maxValue;
            QString  word6 = "select *  from tekk  where  t6 between "+minValue+" and "+maxValue;
            QString  word7 = "select *  from tekk  where  t7 between "+minValue+" and "+maxValue;
            QString  word8 = "select *  from tekk  where  t8 between "+minValue+" and "+maxValue;
            QString  word9 = "select *  from tekk  where  t9 between "+minValue+" and "+maxValue;
            QString  word10 = "select *  from tekk  where  t10 between "+minValue+" and "+maxValue;
            QString  word11 = "select *  from tekk  where  t11 between "+minValue+" and "+maxValue;
            QString  word12 = "select *  from tekk  where  t12 between "+minValue+" and "+maxValue;
            QString  word13 = "select *  from tekk  where  t13 between "+minValue+" and "+maxValue;
            QString  word14 = "select *  from tekk  where  t14 between "+minValue+" and "+maxValue;
            QString  word15= "select *  from tekk  where  t15 between "+minValue+" and "+maxValue;
            QString  word16= "select *  from tekk  where  t16 between "+minValue+" and "+maxValue;
            QString  word17= "select *  from tekk  where  t17 between "+minValue+" and "+maxValue;
            QString  word18= "select *  from tekk  where  t18 between "+minValue+" and "+maxValue;
            QString  word19= "select *  from tekk  where  t19 between "+minValue+" and "+maxValue;
            QString  word20= "select *  from tekk  where  t20 between "+minValue+" and "+maxValue;
            QString  word21= "select *  from tekk  where  t21 between "+minValue+" and "+maxValue;
            QString  word22= "select *  from tekk  where  t22 between "+minValue+" and "+maxValue;
            QString  word23= "select *  from tekk  where  t23 between "+minValue+" and "+maxValue;
            QString  word24 = "select *  from tekk  where  t24 between "+minValue+" and "+maxValue;
            QString  word25 = "select *  from tekk  where  t25 between "+minValue+" and "+maxValue;
            QString  word26 = "select *  from tekk  where  t26 between "+minValue+" and "+maxValue;
            QString  word27 = "select *  from tekk  where  t27 between "+minValue+" and "+maxValue;
            QString  word28 = "select *  from tekk  where  t28 between "+minValue+" and "+maxValue;
            QString  word29 = "select *  from tekk  where  t29 between "+minValue+" and "+maxValue;
            QString  word30 = "select *  from tekk  where  t30 between "+minValue+" and "+maxValue;
            QString  word31 = "select *  from tekk  where  t31 between "+minValue+" and "+maxValue;
            QString  word32 = "select *  from tekk  where  t32 between "+minValue+" and "+maxValue;
            QString  word33= "select *  from tekk  where  t33 between "+minValue+" and "+maxValue;
            QString  word34= "select *  from tekk  where  t34 between "+minValue+" and "+maxValue;
            QString  word35= "select *  from tekk  where  t35 between "+minValue+" and "+maxValue;
            QString  word36= "select *  from tekk  where  t36 between "+minValue+" and "+maxValue;
            QString  word37= "select *  from tekk  where  t37 between "+minValue+" and "+maxValue;
            QString  word38= "select *  from tekk  where  t38 between "+minValue+" and "+maxValue;
            QString  word39= "select *  from tekk  where  t39 between "+minValue+" and "+maxValue;
            QString  word40= "select *  from tekk  where  t40 between "+minValue+" and "+maxValue;

            model->setQuery(word1);
            model->setQuery(word2);
            model->setQuery(word3);
            model->setQuery(word4);
            model->setQuery(word5);
            model->setQuery(word6);
            model->setQuery(word7);
            model->setQuery(word8);
            model->setQuery(word9);
            model->setQuery(word10);
            model->setQuery(word11);
            model->setQuery(word12);
            model->setQuery(word13);
            model->setQuery(word14);
            model->setQuery(word15);
            model->setQuery(word16);
            model->setQuery(word17);
            model->setQuery(word18);
            model->setQuery(word19);
            model->setQuery(word20);
            model->setQuery(word21);
            model->setQuery(word22);
            model->setQuery(word23);
            model->setQuery(word24);
            model->setQuery(word25);
            model->setQuery(word26);
            model->setQuery(word27);
            model->setQuery(word28);
            model->setQuery(word29);
            model->setQuery(word30);
            model->setQuery(word31);
            model->setQuery(word32);
            model->setQuery(word33);
            model->setQuery(word34);
            model->setQuery(word35);
            model->setQuery(word36);
            model->setQuery(word37);
            model->setQuery(word38);
            model->setQuery(word39);
            model->setQuery(word40);

        }

    }
    //  db.commit();
}


void tempreture::slotOrderData()
{
    QString   column ,text,word;
    column    = QString::number(this->valueMin->value());
    if(this->orderWay->currentText() == "升序")
        text = "ASC";
    else
        text = "DESC";
    if(this->valueMin->value() == 1)
    {
        word = "select *  from tekk  order by  t1 " + text ;
        model->setQuery(word);
    }
    else  if(this->valueMin->value() == 2)
    {
        word = "select *  from tekk  order by  temp2 " + text ;
        model->setQuery(word);
    }
    else  if(this->valueMin->value() == 3)
    {
        word = "select *  from tekk  order by  temp3 " + text ;
        model->setQuery(word);
    }
    else  if(this->valueMin->value() == 4)
    {
        word = "select *  from te  order by  temp4 " + text ;
        model->setQuery(word);
    }
    else  if(this->valueMin->value() == 5)
    {
        word = "select *  from te  order by  temp5 " + text ;
        model->setQuery(word);
    }
    else  if(this->valueMin->value() == 6)
    {
        word = "select *  from te  order by  temp6 " + text ;
        model->setQuery(word);
    }
    else  if(this->valueMin->value() == 7)
    {
        word = "select *  from te  order by  temp7 " + text ;
        model->setQuery(word);
    }
    else
        model->setQuery("select *  from tekk ");

}



//计算数据
void tempreture::slotCalculateData()
{
    QString  word,rowStr;
    int  rows;
    if(this->calculate->currentText() == "平均值")  //平均值
    {

        rows = this->model->rowCount();
        rows+=1;
        rowStr =QString::number(rows);
        word = "insert into  tekk   select "+  rowStr+", AVG(t1) ,AVG(t2),AVG(t3),AVG(t4),"
                                                      "AVG(t5),AVG(t6),AVG(t7), AVG(t8) ,AVG(t9),AVG(t10),AVG(t11),"
                                                      "AVG(t12),AVG(t13),AVG(t14),"
                                                      "AVG(t15),AVG(t16),AVG(t17), AVG(t18) ,AVG(t19),AVG(t20),AVG(t21),"
                                                      "AVG(t22),AVG(t23),AVG(t24),"
                                                      "AVG(t25),AVG(t26),AVG(t27), AVG(t28) ,AVG(t29),AVG(t30),AVG(t31),"
                                                      "AVG(t32),AVG(t33),AVG(t34), AVG(t35) ,AVG(t36),AVG(t37),AVG(t38),"
                                                      "AVG(t39),AVG(t40) from tekk";
        model->setQuery(word);
        model->setQuery("select *  from tekk ");


    }
    else if(this->calculate->currentText() == "项数") //项数
    {


        rows = this->model->rowCount();
        rows+=1;
        rowStr =QString::number(rows);
        word = "insert into  tekk   select "+  rowStr+", COUNT(t1) , COUNT(t2), COUNT(t3),COUNT(t4),"
                                                      "COUNT(t5),COUNT(t6),COUNT(t7), COUNT(t8) ,COUNT(t9),COUNT(t10),COUNT(t11),"
                                                      "COUNT(t12),COUNT(t13),COUNT(t14),"
                                                      "COUNT(t15),COUNT(t16),COUNT(t17), COUNT(t18) ,COUNT(t19),COUNT(t20),COUNT(t21),"
                                                      "COUNT(t22),COUNT(t23),COUNT(t24),"
                                                      "COUNT(t25),COUNT(t26),COUNT(t27), COUNT(t28) ,COUNT(t29),COUNT(t30),COUNT(t31),"
                                                      "COUNT(t32),COUNT(t33),COUNT(t34), COUNT(t35) ,COUNT(t36),COUNT(t37),COUNT(t38),"
                                                      "COUNT(t39),COUNT(t40) from tekk";

        model->setQuery(word);
        model->setQuery("select *  from tekk ");

    }
    else if(this->calculate->currentText() == "总和")  //总和
    {

        rows = this->model->rowCount();
        rows+=1;
        rowStr =QString::number(rows);
        word = "insert into  tekk   select "+  rowStr+", SUM(t1) , SUM(t2), SUM(t3),SUM(t4),"
                                                      "SUM(t5),SUM(t6),SUM(t7), SUM(t8) ,SUM(t9),SUM(t10),SUM(t11),"
                                                      "SUM(t12),SUM(t13),SUM(t14),"
                                                      "SUM(t15),SUM(t16),SUM(t17), SUM(t18) ,SUM(t19),SUM(t20),SUM(t21),"
                                                      "SUM(t22),SUM(t23),SUM(t24),"
                                                      "SUM(t25),SUM(t26),SUM(t27), SUM(t28) ,SUM(t29),SUM(t30),SUM(t31),"
                                                      "SUM(t32),SUM(t33),SUM(t34), SUM(t35) ,SUM(t36),SUM(t37),SUM(t38),"
                                                      "SUM(t39),SUM(t40) from tekk";

        model->setQuery(word);
        model->setQuery("select *  from tekk ");


    }
    else if(this->calculate->currentText() == "最大值")  //最大值
    {

        rows = this->model->rowCount();
        rows+=1;
        rowStr =QString::number(rows);
        word = "insert into  tekk   select "+  rowStr+", MAX(t1) , MAX(t2), MAX(t3),MAX(t4),"
                                                      "MAX(t5),MAX(t6),MAX(t7), MAX(t8) ,MAX(t9),MAX(t10),MAX(t11),"
                                                      "MAX(t12),MAX(t13),MAX(t14),"
                                                      "MAX(t15),MAX(t16),MAX(t17), MAX(t18) ,MAX(t19),MAX(t20),MAX(t21),"
                                                      "MAX(t22),MAX(t23),MAX(t24),"
                                                      "MAX(t25),MAX(t26),MAX(t27), MAX(t28) ,MAX(t29),MAX(t30),MAX(t31),"
                                                      "MAX(t32),MAX(t33),MAX(t34), MAX(t35) ,MAX(t36),MAX(t37),MAX(t38),"
                                                      "MAX(t39),MAX(t40) from tekk";

        model->setQuery(word);
        model->setQuery("select *  from tekk ");

    }
    else if(this->calculate->currentText() == "最小值")  //最小值
    {

        rows = this->model->rowCount();
        rows+=1;
        rowStr =QString::number(rows);
        word = "insert into  tekk   select "+  rowStr+", MIN(t1) , MIN(t2), MIN(t3),MIN(t4),"
                                                      "MIN(t5),MIN(t6),MIN(t7), MIN(t8) ,MIN(t9),MIN(t10),MIN(t11),"
                                                      "MIN(t12),MIN(t13),MIN(t14),"
                                                      "MIN(t15),MIN(t16),MIN(t17), MIN(t18) ,MIN(t19),MIN(t20),MIN(t21),"
                                                      "MIN(t22),MIN(t23),MIN(t24),"
                                                      "MIN(t25),MIN(t26),MIN(t27), MIN(t28) ,MIN(t29),MIN(t30),MIN(t31),"
                                                      "MIN(t32),MIN(t33),MIN(t34), MIN(t35) ,MIN(t36),MIN(t37),MIN(t38),"
                                                      "MIN(t39),MIN(t40) from tekk";

        model->setQuery(word);
        model->setQuery("select *  from tekk ");


    }
    else if(this->calculate->currentText() == "方差") //方差
    {

        rows = this->model->rowCount();
        rows+=1;
        rowStr =QString::number(rows);
        word = "insert into  tekk   select "+  rowStr+", VAR(t1) , VAR(t2), VAR(t3),VAR(t4),"
                                                      "VAR(t5),VAR(t6),VAR(t7), VAR(t8) ,VAR(t9),VAR(t10),VAR(t11),"
                                                      "VAR(t12),VAR(t13),VAR(t14),"
                                                      "VAR(t15),VAR(t16),VAR(t17), VAR(t18) ,VAR(t19),VAR(t20),VAR(t21),"
                                                      "VAR(t22),VAR(t23),VAR(t24),"
                                                      "VAR(t25),VAR(t26),VAR(t27), VAR(t28) ,VAR(t29),VAR(t30),VAR(t31),"
                                                      "VAR(t32),VAR(t33),VAR(t34), VAR(t35) ,VAR(t36),VAR(t37),VAR(t38),"
                                                      "VAR(t39),VAR(t40) from tekk";

        model->setQuery(word);
        model->setQuery("select *  from tekk ");


    }

}



//进行温度修调
void tempreture::tempAdjustSlot()
{
     emit this->startSend(this->serialname->currentText(),this->slaveAddr->text().toUShort(),
                          'X',0,this->tempAdjustBox->text().toUShort());
}


//消息清除
void tempreture::msgClearSlot()
{
    this->tempBrowser->clear();
}


//修调码清零
void tempreture::codeClearSlot()
{
    emit this->startSend(this->serialname->currentText(),this->slaveAddr->text().toUShort(),
                         'X',1,this->tempAdjustBox->text().toUShort());
}







void tempreture::disconnectSerial(const QString &ip)
{

}

//串口搜索






