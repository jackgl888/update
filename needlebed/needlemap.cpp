#include "needlemap.h"

NeedleMap::NeedleMap(QWidget *parent) : QMainWindow(parent)
{


   // mapNeedleBed =  dynamic_cast<needlebed* >(parent);
    

    this-> scene = new QGraphicsScene(this);
    this-> scene->setSceneRect(-300,-300,600,600);
    this->button = new QPushButton(tr("开始"),this);

    this->initScene();                         	//初始化场景
    this->createActions();                    	//创建主窗体的所有动作
    this-> createMenus();
    this->m_timer  =new  QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeoutEventMethod()));
    connect(this->tray1,SIGNAL(triggered()),this,SLOT (reflashTray1()));
    connect(this->tray2,SIGNAL(triggered()),this,SLOT (reflashTray2()));
    connect(this->tray3,SIGNAL(triggered()),this,SLOT (reflashTray3()));
    connect(this->tray4,SIGNAL(triggered()),this,SLOT (reflashTray4()));

   // connect(this->setFrokPs,SIGNAL(triggered()),this,SLOT(slotParaSet()));

     this->m_timer->start(500);
    this->view = new QGraphicsView(this);
    this->view->setScene(scene);
    this->view->setMinimumSize(600,600);
    this->view->show();

    this->setCentralWidget(this->view);
    // this->resize(850,750);
    this->setWindowTitle(tr("Graphics Items"));


}

NeedleMap::~NeedleMap()
{

}

void NeedleMap::initScene()
{

    QFont font("Times",16);
    this->trayName1 = new QGraphicsTextItem("托盘一");
    this->trayName1->setFont(font);
    this->trayName1->setDefaultTextColor(QColor(qrand()%256,qrand()%256,qrand()%256));
    this->listItem.append( this->trayName1);

    this->trayName2 = new QGraphicsTextItem("托盘二");
    this->trayName2->setFont(font);
    this->trayName2->setDefaultTextColor(QColor(qrand()%256,qrand()%256,qrand()%256));
    this->listItem.append( this->trayName2);

    this->trayName3 = new QGraphicsTextItem("托盘三");
    this->trayName3->setFont(font);
    this->trayName3->setDefaultTextColor(QColor(qrand()%256,qrand()%256,qrand()%256));
    this->listItem.append( this->trayName3);

    this->trayName4 = new QGraphicsTextItem("托盘四");
    this->trayName4->setFont(font);
    this->trayName4->setDefaultTextColor(QColor(qrand()%256,qrand()%256,qrand()%256));
    this->listItem.append( this->trayName4);

    this->C_item = new QGraphicsRectItem(QRectF(0,0,1200,700));

    this->T_item = new QGraphicsRectItem(QRectF(0,0,800,350));
    this->f_smog = new  QGraphicsEllipseItem(0,0,70,70);
    this->b_smog = new  QGraphicsEllipseItem(0,0,70,70);
    this->f_tray = new  QGraphicsEllipseItem(0,0,30,30);
    this->b_tray = new  QGraphicsEllipseItem(0,0,30,30);
    this->backMotorCheckLeft = new QGraphicsRectItem(QRectF(0,0,30,20));
    this->originMotorCheckLeft = new QGraphicsRectItem(QRectF(0,0,30,20));
    this->forwardMotorCheckLeft = new QGraphicsRectItem(QRectF(0,0,30,20));
    this->backMotorCheckRight = new QGraphicsRectItem(QRectF(0,0,30,20));
    this->originMotorCheckRight= new QGraphicsRectItem(QRectF(0,0,30,20));
    this->forwardMotorCheckRight= new QGraphicsRectItem(QRectF(0,0,30,20));
    this->frontSensor= new QGraphicsRectItem(QRectF(0,0,30,20));

    this->forwardCylinderRight = new QGraphicsRectItem(QRectF(0,0,30,20));
    this->backCylinderRight= new QGraphicsRectItem(QRectF(0,0,30,20));

#if 0
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(qrand()%256,qrand()%256,qrand()%256));
    item->setPen(pen);
    item->setBrush(QColor(qrand()%256,qrand()%256,qrand()%256));
    item->setFlag(QGraphicsItem::ItemIsMovable);
#endif
    scene->addItem(this->trayName1);
    this->scene->addItem(this->C_item);
    this->scene->addItem(this->T_item);  //托盘1
    this->scene->addItem(this->f_smog );  //前烟感2
    this->scene->addItem(this->b_smog ); //后烟感3
    this->scene->addItem(this->f_tray);  //托盘
    this->scene->addItem(this->b_tray);
    this->scene->addItem(  this->backMotorCheckLeft);
    this->scene->addItem(  this->originMotorCheckLeft);
    this->scene->addItem(  this->forwardMotorCheckLeft);
    this->scene->addItem(  this->backMotorCheckRight);
    this->scene->addItem(  this->originMotorCheckRight);
    this->scene->addItem(  this->forwardMotorCheckRight);

    this->scene->addItem(  this->forwardCylinderRight );
    this->scene->addItem(   this->backCylinderRight );

    this->trayName1->setPos(0,0);
    this->C_item->setPos(-600,-350);
    this->T_item->setPos(-400,-175);
    this->f_smog->setPos(-500,-35);
    this->b_smog->setPos(430,-35);
    f_tray->setPos(-360,-140);
    b_tray->setPos(330,110);

    this->backMotorCheckLeft->setPos(-615,-320);
    this->originMotorCheckLeft->setPos(-615,-280);
    this->forwardMotorCheckLeft->setPos(-615,-100);
    this->backMotorCheckRight->setPos(-615,300);
    this->originMotorCheckRight->setPos(-615,260);
    this->forwardMotorCheckRight->setPos(-615,80);

    this->forwardCylinderRight->setPos(-350,-260);
    this->backCylinderRight->setPos(350,-260);
    this->frontSensor->setPos(-615,-10);

}


//创建所有动作
void NeedleMap::createActions()
{
    this->tray1 = new QAction(tr("库位一"),this);
    this->tray2 = new QAction(tr("库位二"),this);
    this->tray3 = new QAction(tr("库位三"),this);
    this->tray4 = new QAction(tr("库位四"),this);
  //  this->setFrokPs = new QAction(tr("工装设置"),this);
}


//创建所有菜单栏
void NeedleMap::createMenus()
{
    this->trayChoose= menuBar()->addMenu(tr("库位选择"));
    this->trayChoose->addAction(this->tray1);
    this->trayChoose->addAction(this->tray2);
    this->trayChoose->addAction(this->tray3);
    this->trayChoose->addAction(this->tray4);
   // this->paraSet  = menuBar()->addMenu(tr("参数设置"));
    //  this->paraSet->addAction(this->setFrokPs);
}





void NeedleMap::timeoutEventMethod()
{
   #if 1

        if( this->trayNum == 0x01)
        {
            if(this->traybuf[0]==0)
                this->trayName1->setPlainText("初始化状态");
            else  if(this->traybuf[0]==0x05)
                this->trayName1->setPlainText("等待状态");
            else  if(this->traybuf[0]==0x01)
                this->trayName1->setPlainText("就绪状态");
            else  if(this->traybuf[0]==0x04)
                this->trayName1->setPlainText("异常状态");
            else  if(this->traybuf[0]==0x02)
                this->trayName1->setPlainText("运动状态");
            else
                this->trayName1->setPlainText("工作状态");

        }
        else   if( this->trayNum ==0x02)
        {

            if(this->traybuf[0]==0)
                this->trayName2->setPlainText("初始化状态");
            else  if(this->traybuf[0]==0x05)
                this->trayName2->setPlainText("等待状态");
            else  if(this->traybuf[0]==0x01)
                this->trayName2->setPlainText("就绪状态");
            else  if(this->traybuf[0]==0x04)
                this->trayName2->setPlainText("异常状态");
            else  if(this->traybuf[0]==0x02)
                this->trayName2->setPlainText("运动状态");
            else
                this->trayName2->setPlainText("工作状态");
        }
        else   if( this->trayNum == 0x03)
        {

            if(this->traybuf[0]==0)
                this->trayName3->setPlainText("初始化状态");
            else  if(this->traybuf[0]==0x05)
                this->trayName3->setPlainText("等待状态");
            else  if(this->traybuf[0]==0x01)
                this->trayName3->setPlainText("就绪状态");
            else  if(this->traybuf[0]==0x04)
                this->trayName3->setPlainText("异常状态");
            else  if(this->traybuf[0]==0x02)
                this->trayName3->setPlainText("运动状态");
            else
                this->trayName3->setPlainText("工作状态");
        }
        else if( this->trayNum == 0x04)
        {

            if(this->traybuf[0]==0)
                this->trayName4->setPlainText("初始化状态");
            else  if(this->traybuf[0]==0x05)
                this->trayName4->setPlainText("等待状态");
            else  if(this->traybuf[0]==0x01)
                this->trayName4->setPlainText("就绪状态");
            else  if(this->traybuf[0]==0x04)
                this->trayName4->setPlainText("异常状态");
            else  if(this->traybuf[0]==0x02)
                this->trayName4->setPlainText("运动状态");
            else
                this->trayName4->setPlainText("工作状态");
        }



#endif
    //传感器图形化显示
    uchar data = this->traybuf[6];
    if(data&0x04)
    {
        this->f_tray->setBrush(Qt::green);
        this->b_tray->setBrush(Qt::green);
        this->backCylinderRight->setBrush(Qt::green);
        this->forwardCylinderRight->setBrush(Qt::green);
    }
    else if(data&0x02)
    {
        this->f_tray->setBrush(Qt::green);
        this->b_tray->setBrush(Qt::green);
        this->backCylinderRight->setBrush(Qt:: color0);
        this->forwardCylinderRight->setBrush(Qt:: color0);
    }
    else
    {
        if(data&0x20)
            this->backCylinderRight->setBrush(Qt::green);
        if(data&0x40)
            this->forwardMotorCheckLeft->setBrush(Qt::green);
        else
        {
            this->f_tray->setBrush(Qt:: color0);
            this->b_tray->setBrush(Qt:: color0);
            this->backCylinderRight->setBrush(Qt:: color0);
            this->forwardCylinderRight->setBrush(Qt:: color0);
        }

    }
    if(data&0x10)
        this->frontSensor->setBrush(Qt::green);
    else
        this->frontSensor->setBrush(Qt:: color0);
    if(data&0x80)
        this->backMotorCheckLeft->setBrush(Qt::red);//左边负极行程超过
    else
        this->backMotorCheckLeft->setBrush(Qt:: color0);

    uchar datam = this->traybuf[7];
    if(datam&0x01)
    this->backMotorCheckRight->setBrush(Qt::red);//右边负极行程超过
    else
      this->backMotorCheckRight->setBrush(Qt::color0);
    if(datam&0x02)
     this->forwardMotorCheckLeft->setBrush(Qt::red);  //左边正极感应
    else
       this->forwardMotorCheckLeft->setBrush(Qt::color0);
    if(datam&0x04)
     this->forwardMotorCheckRight->setBrush(Qt::red);  //右边正极限感应
     else
      this->forwardMotorCheckRight->setBrush(Qt::color0);
    if(datam&0x08)
     this->originMotorCheckLeft->setBrush(Qt::green);   //左边 原点
    else
     this->originMotorCheckLeft->setBrush(Qt::color0);
    if(datam&0x10)
     this->originMotorCheckRight->setBrush(Qt::green); //右边原点
    else
      this->originMotorCheckRight->setBrush(Qt::color0);

}


void NeedleMap::reflashTray1()
{
     this->trayNum = 0x01 ;
}

void NeedleMap::reflashTray2()
{
        this->trayNum = 0x02 ;
}

void NeedleMap::reflashTray3()
{
        this->trayNum = 0x03 ;
}

void NeedleMap::reflashTray4()
{

       this->trayNum = 0x04 ;
#if 0
    for(uchar i=0;i<this->listItem.count();i++)
    {
        if(scene->items().contains(listItem.at(i)))
        {
            scene->removeItem(listItem.at(i));
        }
    }

    Singel::m_Instance()->trayflag = 0x03;
    this->scene->addItem(this->trayName4);
    this->trayName1->setPos(0,0);
    //清空针床显示
    for(uchar i = 0; i<30;i++)
    Singel::m_Instance()->traybufft[i]=0;
      //清空温度显示
     for(uchar i = 1;i<=48;i++)
     Singel::m_Instance()->bedTempfh[i] =0;

#endif
}


//参数配置
void NeedleMap::slotParaSet()
{

    //  this-> m_paraset = new paraset(this);
}

//更新map
void NeedleMap::updateMapSlot(const uchar cmd, const uchar traynum, const uchar *data)
{
    QString   text;
    switch (cmd) {
    case 'A':

        break;
    case 'S':
        for(uchar i=0;i<8;i++)
            this->traybuf[i]=*(data+i); //得到传感器数据
        this->locTemps = *(data+18);
        for(uchar i= 0; i< this->locTemps ;i++)
        {
            this->bedTemp[i] =  (*(data+19+2*i))|(*(data+20+2*i)<<8);  //库位温度低

        }
        this->bedChannels= (*(data+20+2* this->locTemps))|(*(data+21+2* this->locTemps)<<8);  //提取通道温度
        for(uchar i =0;i< this->bedChannels;i++)
        {
            this->bedTemp[ this->locTemps+i] =(*(data+22+2*i+2*  this->locTemps))|
                    (*(data+23+2*i+2*  this->locTemps)<<8);
        }
        this->negpressValue = (*(data+25+2*this->bedChannels+2*this->locTemps))|
                (*(data+26+2*this->bedChannels+2*this->locTemps)<<8);//负压值
        break;
    case 'L':
        text =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
          qDebug()<<text+" LLLL";
        break;
    default:
        break;
    }
}

void NeedleMap::slotUiUpdate(QVariant variant)
{

    bedPara m_para = variant.value<bedPara>();
    memcpy(this->traybuf,m_para.bedSensor,10);
    memcpy(this->bedTemp,m_para.bedTemp,100);
    this->negpressValue=m_para.negAir;

}


