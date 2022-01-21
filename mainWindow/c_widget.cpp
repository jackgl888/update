#include "c_widget.h"

C_widget::C_widget(QWidget *parent) : QWidget(parent)
{
     QFont    font;
     font.setFamily("MS Shell Dlg 2");
     this->setFont(font);


    this->splitterMain = new  QSplitter(Qt:: Horizontal, this);
    splitterMain->setOpaqueResize(false);
    this->list = new QListWidget(splitterMain);
    this->list->setMaximumWidth(120);
    this->list->setFrameStyle(QFrame::Panel|QFrame::Raised|QFrame::Sunken);
   // this->list->setMaximumSize(500,6000);

    this->list->setStyleSheet("QListWidget{border:1px solid gray; color:rgb(196,196,196);\
           background-color:rgb(71,71,71);margin:12px,0px,0px,0px;}" \
"QListWidget::Item:hover{background-color:rgb(140, 140, 140); color: black;}"  // 鼠标停放时的色彩
"QListWidget::Item:selected{background-color:rgb(102, 102, 102); border-style: inset; }"); //按下);/*上、下、左、右，间距*/

   list->setViewMode(QListView::IconMode);
    list->setResizeMode(QListView  ::Adjust);
      list->setIconSize(QSize(60,60));
  //   list->setGridSize(QSize(200,60));
    list->setSpacing(0);
   // list->setViewMode(QListWidget::IconMode);  //这样的形式也可以
   // list->setViewMode(QListView::ListMode);
    list->setFocusPolicy(Qt::NoFocus);
    list->setMovement(QListView::Static);  //不可移动
    list->insertItem(0,QObject::tr("针床"));
    list->insertItem(2,QObject::tr("电机"));
   list->insertItem(1,QObject::tr("温度"));
    list->insertItem(3,QObject::tr("OCV"));
    list->insertItem(4,QObject::tr("感应器"));
    list->insertItem(5,QObject::tr("通信"));
    list->insertItem(6,"温度");

    for(uchar i = 0;i<list->count();i++)
    {
       list->item(i)->setSizeHint(QSize(120,100));
       list->item(i)->setTextAlignment(Qt::AlignCenter);
       if(0==i)
       list->item(i)->setIcon(QIcon(":/new/res/needlebed.png"));
       if(2==i)
       list->item(i)->setIcon(QIcon(":/new/res/motor.png"));
       if(3==i)
       list->item(i)->setIcon(QIcon(":/new/res/ocv.png"));
       if(4==i)
       list->item(i)->setIcon(QIcon(":/new/res/sensor.png"));
       if(5==i)
       list->item(i)->setIcon(QIcon(":/new/res/communication.png"));
       if(6==i)
        list->item(i)->setIcon(QIcon(":/new/res/temp2.png"));
    }

    this->m_content =   new  content(splitterMain);

//    this->serialname  =  new   QComboBox(this);
//    this->comname = new   QLabel(tr("串口号:"),this);
//    this->comname->setFixedHeight(30);
//    this->serialname->setFixedSize(150,30);
//    QHBoxLayout *BtnLayout = new QHBoxLayout;

//    //this->serialCntl->setIcon(QIcon(":/pic/tim.png"));
//   // this->serialCntl->setFixedSize(50,50);

//    BtnLayout->addStretch(20);
//    BtnLayout->setSpacing(0);

//    BtnLayout->addWidget(this->comname);
//    BtnLayout->addWidget(this->serialname);


    this->m_timer  = new QTimer(this);
    this->m_timer->start(200);

    connect(this->m_timer,SIGNAL(timeout()),this,SLOT(timeoutMethod()));  //定时器溢出

    QObject::connect(list,SIGNAL(currentRowChanged(int)),m_content->stack,
                     SLOT(setCurrentIndex(int)));
    splitterMain->setWindowTitle(QObject::tr("调试软件"));
   // splitterMain->setMinimumSize(splitterMain->minimumSize());
  //  splitterMain->setMaximumSize(splitterMain->maximumSize());.

      QVBoxLayout  *m_layout  =  new  QVBoxLayout(this);
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    m_layout->addWidget(this->splitterMain);
    //m_layout->addLayout(BtnLayout);

    splitterMain->show();

}


C_widget::~C_widget()
{
    delete  list;
    delete  m_content;
}

#if 0

//定时器管理串口
void C_widget::timeoutMethod()
{
    //串口接入处理
    foreach (const  QSerialPortInfo  &info, QSerialPortInfo::availablePorts()) {
        QSerialPort  serial;
        //在给串口分配过过串口设备之后（也就是给QSerialPort的对象分配过特定端口口之后），
        //我们得先调用open打开该串口，在open成功返回之后，再进行该串口的波特率，停止位，校验位等的配置。
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            //将串口名中入缓存
            if(!Single::m_Instance()->portNamelist.contains(info.portName()))
            {
                Single::m_Instance()->portNamelist.append(info.portName());
                this->serialname->addItem(info.portName());
            }

            serial.close();
        }
    }
    //串口退出处理
    QStringList   serialbuf;
    //清空临时缓存
    foreach (const  QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        serialbuf.append(info.portName());
    }
    for(uchar i = 0;i<Single::m_Instance()->portNamelist.count();i++)
    {
        //如果缓存中不包含
        if(!serialbuf.contains( Single::m_Instance()->portNamelist.at(i)))
        {

//            if((this->serialCntl->text()=="串口打开")
//                    &&(this->serialname->currentText()== Single::m_Instance()->portNamelist.at(i)))
//            {
//                break;
//            }
//            else

            for(int j = 0;j<this->serialname->count();j++)
            {
                if(this->serialname->itemText(j) ==  Single::m_Instance()->portNamelist.at(i))
                {
                    this->serialname->removeItem(j);     //清除当前串口box显示
                }
            }
               Single::m_Instance()->portNamelist.removeAt(i);
        }
    }
}

#endif



