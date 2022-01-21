#include "serverthread.h"
#include<QDebug>






serverThread::serverThread(int sockDesc,uchar serial,uchar socket,uchar netPortKind, QObject *parent) :
    QThread(parent),
    m_sockDesc(sockDesc)

{

    this->serilPort = serial;
    this->socketPort = socket;
    this->netKind = netPortKind;             //服务端与客户端的区分
}


serverThread::~serverThread()
{
    // qDebug()<<"XGID:"<<this->currentThreadId();
#if  0
    delete   m_socket;

    if(Single::m_Instance()->AjlCnected== 1)
        delete  Ajlensocket;
    else
    {
        m_serial->clear();
        m_serial->close();
        m_serial->deleteLater();
    }
#endif

}


void serverThread::run(void)
{

    if(this->serilPort == 0x01) //串口线程
    {

        m_serial = new Myserial();
        m_serial->moveToThread(this);

        //串口发送数据
        connect(this, SIGNAL(sendData(QString,int,uchar,ushort,ushort)),
                m_serial, SLOT(sendData(QString,int,uchar,ushort,ushort)),Qt::  QueuedConnection);

        //线程退出时先退出串口
        connect(m_serial,SIGNAL(serialDiscnct(const QString &)),this,SLOT(serialDisconnect(const QString &)),
                Qt::DirectConnection);
        //停止串口
        connect(this,SIGNAL(stopSerial(const QString &)),m_serial,SLOT( stopSerialSlot(const QString & ))
                ,Qt::  QueuedConnection);

        //串口发送数据到界面
        connect(m_serial,SIGNAL(updateBedUi(uchar,uchar,QString)),this,
                SLOT( bedCmdDataSlot(uchar,uchar,QString)),Qt::QueuedConnection);

        //ui传数据到工作线程
        connect(this,SIGNAL(sigto(QVariant)),m_serial,SLOT(slotGetData(QVariant)),Qt::  QueuedConnection);

        //数据传给ui
        connect(m_serial,SIGNAL(sig(QVariant)),this,SLOT(dataSendUi(QVariant)),Qt::  QueuedConnection);

        //温度数据传给ui
        connect(m_serial,SIGNAL(sigTempBack(QVariant)),this,SLOT(slotTempBack(QVariant)));

    }
#if 0
    if( this->socketPort == 0x01 ) //网口线程
    {

        if(this->netKind ==0x01 )  //作为客户端时注册方法
        {
            if(Single::m_Instance()->socketFlag == 0x00) //安捷伦
            {

                this->m_socket  = new MySocket( m_sockDesc,0x01);
                //  //     if (! this->Ajlensocket ->setSocketDescriptor( m_sockDesc)) {
                //  //         return ;
                //  //     }

                this->m_socket->connectToHost("192.168.0.10",5024);
                this->m_socket->moveToThread(this);

                connect(this->m_socket, &MySocket::disconnected, this, &serverThread::disconnectToHost,Qt::QueuedConnection);
                //网口发送数据
                connect(this, SIGNAL(sendData(const QString&, const int,const uchar,const uchar *)),
                        this->m_socket, SLOT(sendData(const QString&, const int,const uchar,const uchar *)),Qt:: QueuedConnection);

                connect(this,SIGNAL(readData()),m_socket,SLOT(slotReadData()),Qt::QueuedConnection);
                connect(m_socket,SIGNAL(startSwitch()),this,SLOT(startSwitchSlot()),Qt::QueuedConnection);  //读完数又开始切换

            }

        }
        else  //作服务器时接入的客户端分类注册信号与槽
        {
            this-> m_socket = new MySocket( m_sockDesc,0);
            //        if (!m_socket->setSocketDescriptor( m_sockDesc)) {
            //            return ;
            //        }
            this->ports.append(m_sockDesc);
            this->m_socket->moveToThread(this);

            //  QString  ip =m_socket->peerAddress().toString().remove(0,7);
            //  ipList.append(ip);
            //            if( ip == ) //温度工装
            //            {

            //            }
            //            else if(ip == ) //压力
            //           {

            //            }

            connect(this->m_socket, &MySocket::disconnected, this,
                    &serverThread::disconnectToHost,Qt::QueuedConnection);
            //网口发送数据
            connect(this, SIGNAL(sendData(const QString&, const int,const uchar,const uchar *)),
                    this->m_socket, SLOT(sendData(const QString&, const int,const uchar,const uchar *)),
                    Qt:: QueuedConnection);
        }

    }

#endif
    this->exec();

}





void serverThread:: sendDataSlot(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value)
{
    emit  sendData(ip, traynum,cmd,num,value) ;
}


//串口掉线处理
void serverThread::serialDisconnect(const QString & ip)
{

    Single::m_Instance()->serialPara.clear();  //清除当前升级串口的所有参数
    Single::m_Instance()->portNamelist.removeOne(ip);
    //for(ushort i=0;i<UPDATENUM ;i++)
    // Single::m_Instance()->messageRefresh(ip,i,NULL,0,true,true,true);
    emit  updateUiSerial(ip);
    this->quit();
    this->wait();
}


void serverThread:: bedCmdDataSlot(const uchar cmd,const uchar traynum,const QString &str)
{
    emit  bedUpdate(cmd,traynum,str);
}


void serverThread::dataSendUi(QVariant para)
{
    emit sigtu(para);
}



void serverThread::slotReadData()
{
    emit  readData();
}

void serverThread::startSwitchSlot()
{
    emit  startSwitch();
}


//传送数据到工作线程
void serverThread::slotGetData(QVariant para)
{
    emit sigto(para);
}



//返回温度
void serverThread::slotTempBack(QVariant para)
{
     emit  sigTempBack(para);
}




void serverThread::disconnectToHost(void)
{
    QString  ip;
    //    for(int i=0;i<ports.length();i++)
    //    {
    //        if(ports.at(i) == m_sockDesc)
    //        {

    //            ip =ipList.at(i);
    //            ipList.removeAt(i);
    //            ports.removeAt(i);
    //            break;
    //        }
    //    }
    //  for(ushort i=0;i<UPDATENUM ;i++)
    //  Single::m_Instance()->messageRefresh(ip,i,NULL,0,true,true,true);

    //Ajlensocket->disconnectFromHost();
    emit disconnectTCP( ip);
    this->quit();
    this->wait();
}









void serverThread::stopSerialSlot(const QString &ip)
{
    emit  stopSerial(ip);
}







