#include "adjusttemp.h"

adjustTemp::adjustTemp(QWidget *parent) : QMainWindow(parent)
{

    createActions();                    	//创建主窗体的所有动作
    createMenus();                   //创建主窗体的菜单栏
    this->tempViewInit();
    this->setCentralWidget(this->tempView);
    msgBrowserInit();//消息窗口

    tempTreeInit();       //view初始化
}


adjustTemp::~adjustTemp()
{

}

#if 0
QTreeWidgetItem *adjustTemp::AddTreeRoot(QString name, QString desc)
{
    QTreeWidgetItem * item=new QTreeWidgetItem(this->tempTree);
    this->tempTree->addTopLevelItem(item);
    item->setText(0, name );
    item->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt:: ItemIsSelectable);
    item->setCheckState(0, Qt::Unchecked);
    return item;
}


QTreeWidgetItem *adjustTemp::AddTreeNode(QTreeWidgetItem *parent, QString name, QString desc)
{
    QTreeWidgetItem * item=new QTreeWidgetItem(parent);
    parent->addChild(item);
    item->setText(0,name);
    item->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt:: ItemIsSelectable);
    item->setCheckState(0, Qt::Unchecked);
    return item;
}

#endif

void adjustTemp::createActions()
{
    this->adjustCodeOpen = new QAction("Open",this);
    this->adjustCodeSave= new QAction("Save",this);
    this->adjustCodeSaveAs= new QAction("Save As",this);
    this->adjustCodeQuit= new QAction("Quit",this);
    this->comSetup  = new QAction("Communication",this);
    this->adjustSetup = new QAction("Calibration",this);
    this->adjustTarget = new QAction("Connect",this);
    this->codeClear  = new QAction("Code Clear",this);
    this->adjustStart  = new QAction("Adjust Start",this);
    
}

void adjustTemp::createMenus()
{
    // menuBar()->setFixedHeight(50);
    QFont    font("MS Shell Dlg 2",9);

    this->setFont(font);
    this->adjustCodeFile= menuBar()->addMenu(tr("File"));
    this->adjustCodeFile->setFont(font);
    //this->adjustCodeFile->setFixedSize(40,60);
    this->adjustCodeFile->addAction( this->adjustCodeOpen );
    this->adjustCodeFile->addAction(  this->adjustCodeSave);
    this->adjustCodeFile->addAction(this->adjustCodeSaveAs);
    this->adjustCodeFile->addAction( this->adjustCodeQuit);
    this->adjustCodeEdit  = menuBar()->addMenu(tr("Edit"));
    this->adjustCodeView  = menuBar()->addMenu(tr("View"));
    this->adjustCodeCnct  = menuBar()->addMenu(tr("Target"));
    this->adjustCodeCnct ->addAction(this->adjustTarget);  //联机
    this->adjustCodeCnct ->setFont(font);
    this->adjustCodeSetup = menuBar()->addMenu(tr("Setup"));
    this->adjustCodeSetup->addAction(this->comSetup);
    this->adjustCodeSetup->addAction(this->adjustSetup);
    this->adjustCodeSetup->setFont(font);
    this->adjustCodeFunc  = menuBar()->addMenu(tr("Function"));

    this->adjustCodeFunc->addAction(this->codeClear);
    this->adjustCodeFunc->addAction(this->adjustStart);
    this->adjustCodeFunc->setFont(font);
    this->adjustCodeWin = menuBar()->addMenu(tr("Window"));
    this->adjustCodeHelp = menuBar()->addMenu(tr("Help"));


    //通信配置
    connect(this->comSetup,SIGNAL(triggered()),this,SLOT(communicationSetupSlot()));
    connect(this->adjustSetup,SIGNAL(triggered()),this,SLOT(calibrationSetupSlot()));//校准配置
    connect(this->adjustCodeOpen,SIGNAL(triggered()),this,SLOT(openCalibrationCodeSlot()));  //GET校准码
    connect(this->adjustTarget,SIGNAL(triggered()),this,SLOT(connectTargetSlot())); //联机
    
}

void adjustTemp::tempViewInit()
{
    this->tempView   = new   QTableView(this);
    this->tempModle   =new   QStandardItemModel(this);
    this->tempView->setModel(    this->tempModle);    //绑定模型
    // this->tempView ->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置表格内容不可编辑
    this->tempView ->horizontalHeader()->setDefaultAlignment(Qt::  AlignBaseline);
    this->tempView ->horizontalHeader()->setSectionResizeMode(QHeaderView:: Stretch);
    this->tempView ->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->tempView->verticalHeader()->setVisible(true);
    this->tempView->horizontalHeader()->setVisible(true);
    this->tempView->setAlternatingRowColors(true);
    this->tempView->verticalHeader()->setStyleSheet("QHeaderView::section {background-color: rgb(255, 255, 255)}");
    this->tempView->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color: rgb(255, 255, 255)}");
    this->tempView->setGridStyle(Qt:: SolidLine);
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
}

void adjustTemp:: msgBrowserInit()
{
    this->browserWidget = new QDockWidget(this);
    browserWidget->setFeatures(QDockWidget::DockWidgetMovable);
    browserWidget->setAllowedAreas(Qt:: BottomDockWidgetArea);
    this->msgBrowser = new QTextBrowser(this);
    browserWidget->setWindowTitle("Message Browser");
    browserWidget->setWidget(msgBrowser);
    this->addDockWidget(Qt:: BottomDockWidgetArea,browserWidget);
}

void adjustTemp::tempTreeInit()
{
    this->tempTree  = new  QTreeView(this);
    //  this->tempTree->setRootIsDecorated(false);
    this->tempTree->setStyleSheet("QTreeWidget{background-color:rgb(208,236,251); color: black;}"\
                                  "QTreeWidget::item{height:25px}");

    this->tempTree->setSelectionMode(QAbstractItemView::    ContiguousSelection);
    //   this->tempTree->setStyleSheet("QTreeWidget::item{height:25px}");  //设置行宽,     我也找不到提供的接口  setStyleSheet很强大建议看
   // this->tempTree->setColumnCount(2);
    this->tempTree->setColumnWidth(0,250);
   connect(tempTree,SIGNAL(clicked(QModelIndex)),SLOT(tempTreeRowClicked(QModelIndex)));
   this->tempNodeModle = new QStandardItemModel(30,2,this);
   this->tempNodeModle->setHeaderData(0,Qt::Horizontal,"设备名称") ;
   this->tempNodeModle->setHeaderData(1,Qt::Horizontal,"版本号");
    QStringList  header;
    header <<"设备节点"<<"版本号";
  //  this->tempTree->set;
    this->tempTree->setAlternatingRowColors(true);
    this->tempTree->setModel(this->tempNodeModle);   //
    this->tempTreeDock= new QDockWidget(this);
    tempTreeDock->setFeatures(QDockWidget::DockWidgetMovable);
    tempTreeDock->setAllowedAreas(Qt::    LeftDockWidgetArea);
 //  tempTreeDock->
    tempTreeDock->setWindowTitle("Targets");
    tempTreeDock->setWidget(tempTree);
    this->addDockWidget(Qt::    LeftDockWidgetArea,  tempTreeDock);

}




//节点上树
void adjustTemp::trayWidgetInit(const QString &ip, const uchar traynum)
{

#if 0
    QTreeWidgetItem *itemRoot=NULL;
    QTreeWidgetItem *itemParent=NULL;
    itemRoot=AddTreeRoot(ip,0);//创建父节点
    itemRoot->setExpanded(true);
    itemRoot->setBackgroundColor(1,QColor(128,255,255));

    for(uchar k = 0;k<traynum;k++)
    {
        QString tr = "温度盒"+QString::number(k);
        itemParent= AddTreeNode(itemRoot,tr,0);
        itemParent->setExpanded(true);
        itemParent->setBackgroundColor(1,QColor(0,128,255));
    }
#endif

}


//
void adjustTemp::tempNodeModleInit(QString &ip, uchar  trayNum)
{
    QString tr;
     QStandardItem *rootItem = new QStandardItem(ip);
     rootItem->setIcon(QIcon(":/new/res/serial.png"));  //

     this->tempNodeModle->setItem(0,rootItem);

     for(uchar i = 0;i<trayNum;i++)
     {
         tr= QString("%1%2").arg("温度盒").arg(QString::number(i));
         QStandardItem *tempBorad  = new QStandardItem(tr);
         tempBorad->setIcon(QIcon(":/new/res/temp1.png"));
         rootItem->setChild(i,tempBorad);
     }

}



void adjustTemp::communicationSetupSlot()
{

    commnunicationSet  *communicationSet = new commnunicationSet(this);

    connect(communicationSet,SIGNAL(sendBackComCfg(QStringList&)),this,
            SLOT(getCommunicationSetSlot(QStringList&)));    //回传通信数据


}

void adjustTemp::calibrationSetupSlot()
{
    adjustSetDialog *calibration = new adjustSetDialog(this);
    //校准设置
    connect(calibration,SIGNAL(sigCalibrationSet(QList<int>)),this,SLOT(getCalibrationSetSlot(QList<int >)));
}

void adjustTemp::openCalibrationCodeSlot()
{

}


//
void adjustTemp::getCommunicationSetSlot(QStringList &list)
{
    

    Single::m_Instance()->serialPara.clear();
    Single::m_Instance()->serialPara=list;

    this->m_thread  =new serverThread(0,0x01,0,0);
    Single::m_Instance()->comBuf[0]= list.value(0);   //保存此端口


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


void adjustTemp::getCalibrationSetSlot(QList<int> list)
{
    adjustSetList = list;

    //节点上树
    this->tempNodeModleInit(Single::m_Instance()->comBuf[0],adjustSetList.value(2));

}

//发送联机
void adjustTemp::connectTargetSlot()
{

}


//显示数据
void adjustTemp::slotUiUpdate(QVariant variant)
{

}

void adjustTemp::tempTreeRowClicked(QModelIndex  index)
{
     uchar row,colum;
     row= index.row();
     colum = index.column();
}
