#include "bedpart.h"
#include<QHeaderView>
#include <QDebug>
#include <QFont>
#include <QItemSelectionModel>
#include <QPainter>
#include <QBrush>
#include<Qcolor>
bedpart::bedpart( QWidget *parent) : QWidget(parent)
{

    /* get current needlebed object */
    bedDialog = dynamic_cast<needlebed *>(parent);

    //初始化温度变量
    for(uchar i = 1;i<=48;i++)
    {
        bedDialog->bedTemp[i] =0;
    }
    //初始化针床状显示
    for(uchar i = 0; i < 30;i++)
        bedDialog->traybuf[i]=0;

    this->histogram = new HistogramView(this);

    QStringList header;  //QString类型的List容器
    header<<"通道号"<<"温度值";//

    this->list = new   QTableView(this);
    //  （17）设置表格内容不能编辑
    // this->list->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置表格内容不可编辑
    this->list->setFixedWidth(330);
    this->list->horizontalHeader()->setSectionResizeMode(QHeaderView::  Interactive);
    this->list->verticalHeader()->setSectionResizeMode(QHeaderView:: Stretch);
    this->list->verticalHeader()->setVisible(false);
    this->list->horizontalHeader()->setVisible(false);

    //this->list->horizontalHeader()->setWindowTitle("温度值");
    this->t_model = new  QStandardItemModel(this);

    QBrush  brush;
    //  brush.setStyle(Qt::SolidPattern);

    for(int i = 0;i < 49;i++)
    {

        if(i == 0)

            t_model->setItem(i,0,new QStandardItem("通道号"));

        else
        {
            QStandardItem* item = 0;
            item = new QStandardItem();
            t_model->setItem(i,0,item);
            t_model->setData(t_model->index(i, 0, QModelIndex()),i);
        }

    }
    for(uchar i =0;i<49;i++)
    {

        if(i == 0)
            t_model->setItem(i,1,new QStandardItem("温度值"));
        //  t_model->setData(t_model->index(0, 1, QModelIndex()),"温度值");

        else
        {
            QStandardItem* item = 0;
            item = new QStandardItem();
            t_model->setItem(i,1,item);
            t_model->setData(t_model->index(i, 1, QModelIndex()),0);
        }

    }
    //    for(uchar i =9;i<49;i++)
    //    {
    //            for(uchar j=0;j<2;j++)
    //            t_model->item(i,j)->setBackground(Qt:: cyan);   //setbackground().setColor(;
    //    }
    //    for(uchar i = 1;i<9;i++)
    //    {
    //             for(uchar j=0;j<2;j++)
    //            t_model->item(i,j)->setBackground(Qt::yellow);//magenta
    //    }
    this->histogram->setModel( this->t_model);
    this->list->setModel(this->t_model);
    QItemSelectionModel *selectionModel=new QItemSelectionModel(t_model);
    this->list->setSelectionModel(selectionModel);
    this->histogram->setSelectionModel(selectionModel);

    connect(selectionModel,SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this->list,SLOT(selectionChanged(QItemSelection,QItemSelection)));
    connect(selectionModel,SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this->histogram,SLOT(selectionChanged(QItemSelection,QItemSelection)));

    this->model = new  QStandardItemModel(this);
    this->m_list = new   QTableView (this);
    this->m_list->setModel(model);  //设置模型
    this->m_list->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    this->m_list->verticalHeader()->setVisible(false);
    this->m_list->horizontalHeader()->setVisible(false);//左边序列号不可见
    this->m_list->horizontalHeader()->setSectionResizeMode(QHeaderView::  Interactive);
    this->m_list->verticalHeader()->setSectionResizeMode(QHeaderView:: Stretch);
    QStringList   traysta;
    traysta<<"托盘信息:"<<"初始化"<<"就绪"<<"运动"<<"工作"<<"异常"<<"等待";
    QStringList   negpresMsg;
    negpresMsg<<"负压信息:"<<"高负压阀"<<"低负压阀"<<"破真空阀"<<"保压状态"<<"节压阀"<<"负压值:"<<"泄漏率：";
    QStringList   fireFlags;
    fireFlags<<"消防标志:"<<"多点温度"<<"烟雾+温度"<<"双烟雾"<<"预警温度";
    QStringList   senseStatus;
    senseStatus<<"感应状态:"<<"无托盘无压接"<<"有托盘无压接"<<"有托盘机构到位"<<"有托盘机构到位有压接"<<"货前叉对射"<<"极耳出位"<<"档板出位"
              <<"负极行程超限1"<<"负极行程超限2"<<"正极行程超限1"<<"正极行程超限2"<<"原点感应1"<<"原点感应2"<<"急停按钮"
             <<"针床门状态"<<"消防门气缸原位"<<"消防门气缸出位";
    QStringList  bedFaults;
    bedFaults<<"异常/故障:"<<"托盘位置"<<"烟雾"<<"正压低"<<"急停按下"<<"抽负压超时"<<"高温"<<"485通信中断"<<"温度板通信中断"
            <<"气缸位置冲突"<<"气缸运动超时"<<"按钮卡死"<<"正向行程超程"<<"负向行程超程"
           <<"回零超时"<<"开机回零超时"<<"前对射";
    QStringList  paraconfigs;
    paraconfigs<<"参数配置:"<<"针床类型"<<"负压值"<<"步进量"<<"消防温度"<<"预警温度"<<"保压时间"<<"高负压阀"
              <<"低负压阀"<<"破真空阀"<<"工装取电";

    for(uchar i = 0; i < 7; i++)
    {
        model->setItem(0,i,new QStandardItem(traysta.at(i)));
        model->item(0,i)->setTextAlignment(Qt::AlignVCenter);
        model->item(0,i)->setEditable(false);
    }
    for(uchar i = 0; i < 8; i++)
    {
        model->setItem(1,i,new QStandardItem( negpresMsg.at(i)));
        model->item(1,i)->setTextAlignment(Qt::  AlignVCenter);
        model->item(1,i)->setEditable(false);

    }
    for(uchar i = 0; i < 5; i++)
    {
        model->setItem(2,i,new QStandardItem(fireFlags.at(i)));
        model->item(2,i)->setTextAlignment(Qt::  AlignVCenter);
        model->item(2,i)->setEditable(false);
    }
    for(uchar i = 0; i < senseStatus.count(); i++)
    {
        if(i<=8)
        {
            model->setItem(3,i,new QStandardItem( senseStatus.at(i)));
            model->item(3,i)->setTextAlignment(Qt::  AlignVCenter);
            model->item(3,i)->setEditable(false);

        }
        else if((i>8)&&(i<=16))
        {
            model->setItem(4,i-8,new QStandardItem( senseStatus.at(i)));
            model->item(4,i-8)->setTextAlignment(Qt::  AlignVCenter);
            model->item(4,i-8)->setEditable(false);
        }
        else
        {
            model->setItem(5,i-16,new QStandardItem( senseStatus.at(i)));
            model->item(5,i-16)->setTextAlignment(Qt::  AlignVCenter);
            model->item(5,i-16)->setEditable(false);
        }

    }
    for(uchar i = 0; i < bedFaults.count(); i++)
    {
        if(i<=8)
        {
            model->setItem(6,i,new QStandardItem( bedFaults.at(i)));
            model->item(6,i)->setTextAlignment(Qt::  AlignVCenter);
            model->item(6,i)->setEditable(false);
        }
        else
        {
            model->setItem(7,i-8,new QStandardItem( bedFaults.at(i)));
            model->item(7,i-8)->setEditable(false);
            model->item(7,i-8)->setTextAlignment(Qt::  AlignVCenter);
        }
    }
    for(uchar i = 0; i < paraconfigs.count(); i++)
    {
        if(i>9)
        {
            model->setItem(9,0,new QStandardItem("写入:"));
            model->item(9,0)->setEditable(false);
            model->item(9,0)->setTextAlignment(Qt::  AlignVCenter);

            model->setItem(10,0,new QStandardItem("读出:"));
            model->item(10,0)->setTextAlignment(Qt::  AlignVCenter);
            model->item(10,0)->setEditable(false);

        }
        else
        {
            model->setItem(8,i,new QStandardItem( paraconfigs.at(i)));
            //设置字符位置
            model->item(8,i)->setTextAlignment(Qt::  AlignVCenter);
            model->item(8,i)->setEditable(false);
        }

    }
    for(uchar i =1;i<7;i++)
    {
        model->setItem(10,i,new QStandardItem());
        model->item(10,i)->setEditable(false);
    }


    //x是指定删除哪一行
    // model->removeRow(x);
    //删除所有行
    //  model->removeRows(0,model->rowCount());


    this->negAirComb = new combdelegate(this->m_list); //代理com控件
    this->negAirValuesp = new combdelegate(this->m_list); //代理spin控件
    this->motorSpin      = new combdelegate(this->m_list);  //电机的步数的代理
    this->keepNegAirTime  = new combdelegate(this->m_list); //负压保压时间的代理
    this->highNegAirCntl  = new combdelegate(this->m_list); //高负压控制的代理
    this->lowNegAirCntl   = new combdelegate (this->m_list); //低负压控制的代理
    this->breakVacuum     = new combdelegate(this->m_list);  //破真空控制的代理
    this->m_list->setItemDelegateForColumn(1,this->negAirComb);
    this->m_list->setItemDelegateForColumn(2, this->negAirValuesp );
    this->m_list->setItemDelegateForColumn(3,this->motorSpin);
    this->m_list->setItemDelegateForColumn(6,this->keepNegAirTime);
    this->m_list->setItemDelegateForColumn(7,this->highNegAirCntl);
    this->m_list->setItemDelegateForColumn(8,this->lowNegAirCntl);
    this->m_list->setItemDelegateForColumn(9,this->breakVacuum);


    this->m_layout  =   new    QGridLayout(this);
    this->m_layout->setMargin(1);
    this->m_layout->setSpacing(2);
    this->m_layout->addWidget(this->list,0,2,65,1);
    this->m_layout->addWidget(this->m_list,0,5,29,80);
    this->m_layout->addWidget(this->histogram,31,5,33,80);


    this->m_layout->setSizeConstraint(QLayout:: SetMaximumSize);

    this->setLayout( this->m_layout);
    m_timer  =new  QTimer(this);
    connect(this->t_model, SIGNAL(dataChanged(const  QModelIndex,const QModelIndex,const QVector<int> )), histogram,
            SLOT(dataChanged(const  QModelIndex,const  QModelIndex,const QVector<int> )));

    connect(this->model, SIGNAL(dataChanged(const  QModelIndex,const QModelIndex,const QVector<int> )), this,
            SLOT(dataChangedSlot(const  QModelIndex,const  QModelIndex,const QVector<int> )));

    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeoutEventMethod()));

    this->m_timer->start(500);

}



bedpart::~bedpart()
{


}



//数据改变函数
void bedpart::dataChangedSlot(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    QString  data;
    data =  model->data(topLeft).toString();
 //   qDebug() << model->data(topLeft).toString()<< endl;
  //  qDebug()<<topLeft.column()<<topLeft.row();
  //  ;
    switch (topLeft.column()) {
    case 1:
        if(data == "方型")
        {
           qDebug() <<" fangxing"<< endl;
        }
        else if(data == "软包")
        {
           qDebug() <<" ruanbao"<< endl;
        }
        else if(data == "圆柱")
        {
             qDebug() <<" yuanzhu"<< endl;
        }
        break;
    case 2:     //负压值
        bedDialog->needlebedPara[0]= model->data(topLeft).toUInt();
        break;
    case 3:    //电机步数
        bedDialog->needlebedPara[1]=model->data(topLeft).toUInt();
        break;
    case 4:    //保压时间
        bedDialog->needlebedPara[2] = model->data(topLeft).toUInt();
        break;
    case 5:     //高负压控制
        if(data == "打开")
          bedDialog->needlebedPara[3] = 0x01  ;
        else
            ;
        break;
    case 6:     //低负压控制
        if(data == "打开")
            ;
        else
            ;
        break;
    case 7:   //破真空控制
        if(data == "打开")
            ;
        else
            ;
        break;
    default:
        break;
    }
    //  bedDialog->needlebedPara[] = data.toUShort();

}




void bedpart::timeoutEventMethod()
{


    uchar datal= bedDialog->traybuf[0];  //托盘状态
    uchar datac= bedDialog->traybuf[6];//传感器低8位
    uchar datad=  bedDialog->traybuf[7];//传感器高8位
    uchar faultl= bedDialog->traybuf[1];  //异常码低8位
    uchar faultm= bedDialog->traybuf[2];  //异常码高8位
    for(int i = 1;i <=(bedDialog->locTemps);i++)
        t_model->setData(t_model->index(i, 1, QModelIndex()),(float)bedDialog->bedTemp[i]/10 ); //
    if(datal == 0x00)
        model->item(0,1)->setBackground(Qt::green);
    else
        model->item(0,1)->setBackground(Qt::color0);
    if(datal == 0x01)
        model->item(0,2)->setBackground(Qt::green);
    else
        model->item(0,2)->setBackground(Qt::color0);
    if(datal == 0x02)
        model->item(0,3)->setBackground(Qt::green);
    else
        model->item(0,3)->setBackground(Qt::color0);
    if(datal == 0x03)
        model->item(0,4)->setBackground(Qt::green);
    else
        model->item(0,4)->setBackground(Qt::color0);
    if(datal == 0x04)
        model->item(0,5)->setBackground(Qt::green);
    else
        model->item(0,5)->setBackground(Qt::color0);
    if(datal == 0x05)
        model->item(0,6)->setBackground(Qt::green);
    else
        model->item(0,6)->setBackground(Qt::color0);
    //传感器低8位
    for(uchar i = 0;i<8;i++)
    {
        if( (datac &(0x01<<i)) == (0x01<<i))
            model->item(3,(1+i))->setBackground(Qt::green);
        else
            model->item(3,(1+i))->setBackground(Qt::color0);
    }

    //传感器高8位
    for(uchar i =0;i<8;i++)
    {
        if((datad&(0x01<<i)) ==(0x01<<i) )
            model->item(4,(1+i))->setBackground(Qt::green);
        else
            model->item(4,(1+i))->setBackground(Qt::color0);
    }
    //异常码低8位
    for(uchar i =0;i<8;i++)
    {
        if((faultl&(0x01<<i)) ==(0x01<<i) )
            model->item(6,(1+i))->setBackground(Qt::red);
        else
            model->item(6,(1+i))->setBackground(Qt::color0);
    }
    //异常码高8位
    for(uchar i =0;i<8;i++)
    {
        if((faultm&(0x01<<i)) ==(0x01<<i) )
            model->item(7,(1+i))->setBackground(Qt::red);
        else
            model->item(7,(1+i))->setBackground(Qt::color0);
    }
    //步进值显示
    model->setData(model->index(10,3,QModelIndex()), bedDialog->needlebedPara[2]);


}








