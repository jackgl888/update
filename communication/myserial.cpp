#include "myserial.h"
#include <QDateTime>
#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QDebug>
#include <QDebug>
#include <synchapi.h>



Myserial::Myserial(QObject *parent):
    QSerialPort(parent)
{
    
    this->Init_serial();
    this->serialTimer = new QTimer(this);   //串口管理与定时发送定时器
    this->resendTimer =new QTimer(this);  //超时重发定时器
    connect(this,&Myserial:: readyRead, this, &Myserial::recvData,Qt::  QueuedConnection);//
    connect(serialTimer, SIGNAL(timeout()), this, SLOT(timeoutMethod()));
    connect(resendTimer,SIGNAL(timeout()),this,SLOT( slotSendMMIDataTimeOut()));  //超时重发
    this->serialTimer->start(500);

    for(uchar i = 0;i<10;i++)
    {
        this->sendCnt[i] = 0;
        this->resendCnt[i]=0;
        this->cmdSending[i]=0;
    }
    this->startTempAdjust = 0;
    this->startSample = 0;

}



Myserial::~Myserial()
{
    
}




void Myserial::recvDataMethod(const QString &ip, const uchar *data)
{


     //关闭定时器
    this->resendTimer->stop();


    if(this->portName() == Single::m_Instance()->comBuf[0] )  //modbus
    {
        this->recvModbusData(ip,data);
    }
    else if(this->portName() == Single::m_Instance()->comBuf[1]) //针床
    {
        this->recvNeedlebedData(ip,data);
    }
    else if(this->portName()== Single::m_Instance()->comBuf[2]) //OCV动作
    {
        this->recvOcvActionData(ip,data);
    }
    else if(this->portName() == Single::m_Instance()->comBuf[3]) //OCV切换
    {
        this->recvOcvSwitchData(ip,data);
    }
    else if(this->portName() == Single::m_Instance()->comBuf[4]) //内阻表
    {
        this->recvOhmmeterData(ip,data);
    }
    else if(this->portName() == Single::m_Instance()->comBuf[5]) //拔嘴工装
    {
        this->recvFrockData(ip,data);
    }
    else if(this->portName() == Single::m_Instance()->comBuf[6]) //温控工装
    {
        this->recvTempData(ip,data);
    }
    else if(this->portName() == Single::m_Instance()->comBuf[7]) //压力工装
    {
        this->recvPressData(ip,data);
    }


}


//解析modbus协议
void Myserial::recvModbusData(const QString &ip, const uchar *data)
{
    //  const  QString str = QString::fromUtf8(data.data(),data.length());
    static uchar  adjustResendTimes=0;  //校准重发次数
    uchar traynum=0 ;
    uchar cmd ,adjustResult;
    QString  str = NULL;
    traynum = *(data+6);  //
    cmd =     *(data+7);

    valuetp  m_para;
    QVariant    variant;
    m_para.tpId = traynum;   //当前设备地址
    this->resendCnt[traynum] = 0; //清零节点的重发次数

    switch (cmd) {

    case 'A':
        m_para.version =  *(data+8);   //版本号
        str = QString("%1.%2").arg("版本号").arg(QString::number(m_para.version));
        emit updateBedUi(cmd,traynum,str);
        break;
    case 'X':
            adjustResult= *(data+8);
            //失败重新发送三次
             if((adjustResult == 'E')&&( adjustResendTimes<=2))
             {

                 this->sendAdjustCode( traynum,m_para.adjustCode);
                  adjustResendTimes++;
                  return;  //完成当前发送
             }
             else if(adjustResult == 'O') //成功
             {
                   //
                   str = "校准成功！";
                   adjustResendTimes= 0;
                   emit updateBedUi('X',traynum,str);
             }
             else  //失败
             {
                 str = "校准失败！";
                  adjustResendTimes = 0;
                 emit updateBedUi('X',traynum,str);
             }


        break;
    case 'S':
        for(uchar i=0;i<this->tpChannels;i++)
        {
            m_para.tpValues[i]=(*(data+11+2*i)<<8)|*(data+10+2*i);
            this->adjustTemp[i]=m_para.tpValues[i];
        }
        variant.setValue(m_para);
        emit  sig(variant);
        break;
    default:
        break;
    }
#if 1
    Sleep(50);
    this->sendCnt[traynum]++;
    if(this->sendCnt[traynum]>2)
    {
        this->sendCnt[traynum] = 0;
        this->partlySendData(this->portName(),traynum,'A',0,0);//温度联机
    }
    else
    {
        traynum++;
        if(traynum>=this->tempSlaves)
            traynum =0;
        this->partlySendData(this->portName(),traynum,'S',0,0);//温度采样
    }
#endif
}

//OCV切换
void Myserial::recvOcvSwitchData(const QString &ip, const uchar *data)
{

    static uchar ctime=0;
    const uchar *str;
    ushort len,pertime;
    QString  tr, text;       //  const  QString str = QString::fromUtf8(data.data(),data.length());
    uchar traynum ;
    uchar cmd ;
    len = *(data + 4) +(*(data + 5)<<8) ;  //数据长度
    cmd = *(data+7);   //;  //命令字
    traynum = *(data+8);  //   托盘号
    //   Single::m_Instance()->total_sendbuf[6] =*(data+8);  //返回当前 切换的通道 号
    //  Single::m_Instance()->total_sendbuf[7] =*(data+9);
    //  Single::m_Instance()->total_sendbuf[9] =*(data+10);
    //  tr = QString::number( traynum);
    str=data+6;
    if(cmd != 'A')            //联机命令暂时不校验
    {
        if(false==Single::m_Instance()->net_packet_checksum(str,len))
            return;
    }
    //emit updateBedUi(cmd,traynum,data);
#if 0
    switch (cmd) {
    case 'A':

        text =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
        qDebug()<<text+" AAAA";

        text = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ")+ ip + "  托盘"+tr+"联机成功";
        Single::m_Instance()->messageRefresh(ip,traynum,text,0,true,false,false);
        break;

    case 'C':

        if(Single::m_Instance()->total_sendbuf[3] == 0x01)//ocv连续切换
        {

            if(  ctime< Single::m_Instance()->total_sendbuf[24])
            {
                ctime++;
                pertime = Single::m_Instance()->total_sendbuf[4]|(Single::m_Instance()->total_sendbuf[5]<<8);


                emit startRead();          //读取电压与内阻数据
                Sleep( pertime );      //延时读取数据

                //  this->sendData(ip,0,'C');
                this->partlySendData(ip,  ctime,'C');//继续切换


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
        else
            ;
        break;
    default:
        break;
    }
#endif
}



//OCV动作
void Myserial::recvOcvActionData(const QString &ip, const uchar *data)
{

}



//针床数据处理
void Myserial::recvNeedlebedData(const QString &ip, const uchar *data)
{

    const uchar *str;
    ushort len,lenBed,lenChannel;
    QString  tr, text;            //  const  QString str = QString::fromUtf8(data.data(),data.length());
    uchar traynum ,cmd, sensorBuf[10] ;

    QVariant    variant;
    bedPara  m_parameter;

    len = *(data + 4) +(*(data + 5)<<8) ;  //数据长度
    cmd = *(data+7);   //;  //命令字
    traynum = *(data+8);  //   托盘号
    tr = QString::number( traynum);
    str=data+6;
    if(cmd != 'A')            //联机命令暂时不校验
    {
        if(false==Single::m_Instance()->net_packet_checksum(str,len))
            return;
    }

    switch (cmd) {
    case 'A':

        //版本号显示
        text =  "1.0." +  QString::number( *(data+0)) ;
        emit updateBedUi(cmd,traynum,text);

        //        memset(m_parameter.bedSensor,0,10);
        //        memset(m_parameter.bedTemp,0,100);
        //        memcpy(m_parameter.bufData,data,len+6);  //将发送的数据同步到发送数据区显示
        //      m_parameter.dataLen = len + 6;
        //        variant.setValue(m_parameter);
        //        emit  sig(variant);
        break;
    case 'S':

        for(uchar i=0;i<8;i++)
            sensorBuf[i]=*(data+i); //得到传感器数据
        lenBed = *(data+18);
        for(uchar i= 0; i< lenBed ;i++)
        {
            m_parameter.bedTemp[i] =  (*(data+19+2*i))|(*(data+20+2*i)<<8);  //库位温度
        }
        lenChannel= (*(data+20+2* lenBed))|(*(data+21+2*lenBed)<<8);  //提取通道温度
        for(uchar i =0;i< lenChannel;i++)
        {
            m_parameter.bedTemp[ lenBed+i] =(*(data+22+2*i+2*  lenBed))|
                    (*(data+23+2*i+2*  lenBed)<<8);
        }
        m_parameter.negAir = (*(data+25+2*lenChannel+2*lenBed))|
                (*(data+26+2*lenChannel+2*lenBed)<<8);//负压值
        //将参数传给ui
        memcpy(m_parameter.bufData,data,len+6);  //将发送的数据同步到发送数据区显示
        m_parameter.dataLen = len + 6;
        variant.setValue(m_parameter);
        emit  sig(variant);

        break;
    case 'L':
        text =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
        qDebug()<<text+" LLLL";
        //   emit  startSwitch();
        break;
    default:
        break;
    }


}

//拔嘴工装数据处理
void Myserial::recvFrockData(const QString &ip, const uchar *data)
{

    ushort len;
    QString  tr,str,str1,str2;       //  const  QString str = QString::fromUtf8(data.data(),data.length());
    uchar traynum,cmd ;

    QVariant    variant;
    bedPara  m_para;

    len = *(data + 4) +(*(data + 5)<<8) ;  //数据长度
    cmd = *(data+7);   //;  //命令字
    traynum = *(data+8);  //   托盘
    tr = QString::number( traynum);

    switch (cmd) {

    case'S':


        if(*(data + 8) == 0x01 )  //有残留
        {
            str1 = "有吸嘴残留";

        }else
        {
            str1 = "有吸嘴残留";

        }
        if(*(data + 9) == 0x01 )  //回零位
        {
            str2 = "机构已回零位";

        }else
        {
            str2 = "机构已回零位";

        }
        str = str1 +" " +str2;
        emit updateBedUi(cmd,traynum,str);

        memcpy(m_para.bufData,data,len+6);  //将发送的数据同步到发送数据区显示
        m_para.dataLen = len + 6;
        variant.setValue(m_para);
        emit  sig(variant);
        break;

    case 'C':
        Sleep(100);
        //  this->sendData(ip,250,'S');
        memcpy(m_para.bufData,data,len+6);  //将发送的数据同步到发送数据区显示
        m_para.dataLen = len + 6;
        variant.setValue(m_para);
        emit  sig(variant);
        break;

    default:
        break;
    }

}

//温度工装数据处理
void Myserial::recvTempData(const QString &ip, const uchar *data)
{
    valuetp  m_para;
    const uchar *str;
    ushort len;
    QString  tr, text;       //  const  QString str = QString::fromUtf8(data.data(),data.length());
    uchar traynum ;
    uchar cmd ;
    len = *(data + 4) +(*(data + 5)<<8) ;  //数据长度
    cmd = *(data+7);   //;  //命令字
    traynum = *(data+8);  //   托盘号

    tr = QString::number( traynum);
    str=data+6;
    if(cmd != 'A')            //联机命令暂时不校验
    {
        if(false==Single::m_Instance()->net_packet_checksum(str,len))
            return;
    }

    cmd = *(data+7);   //;  //命令字
    traynum = *(data+6);  //   托盘号
    //emit  updateBedUi(cmd,traynum,data);
#if 1

    switch (cmd) {

    case 'A':
        m_para.version = *(data+11);
        return;
        break;
    case'S':

        for(uchar i=0;i<52;i++)  //
        {
            m_para.tpValues[i]=*(data+10+2*i)|(*(data+11+2*i)<<8);
        }

        break;
#if 0
    case 'L':

        text =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
        qDebug()<<text+" LLLL";
        if(*(data+9) == 'O')
            Single::m_Instance()->mesg = QString::number( Single::m_Instance()->tempStart[0])+"启动成功！";
        else
            Single::m_Instance()->mesg =QString::number( Single::m_Instance()->tempStart[0])+"启动失败！";

        break;
    case'P':
        if(*(data+9) == 'O')
            Single::m_Instance()->mesg =QString::number( Single::m_Instance()->tempStart[0])+ "停止成功！";
        else
            Single::m_Instance()->mesg =QString::number( Single::m_Instance()->tempStart[0])+"停止失败！";

        break;
#endif
    default:
        break;
    }
#endif
    QVariant    variant;
    variant.setValue(m_para);

    emit  sigTempBack(variant);
}


//读取内阻值
void Myserial::recvOhmmeterData(const QString &ip, const uchar *data)
{

}


//读取压力值
void Myserial::recvPressData(const QString &ip, const uchar *data)
{

}


//数据超时重发函数
void Myserial::slotSendMMIDataTimeOut()
{
    QString text,str;

    this->resendTimer->stop();
    this->resendCnt[this->traynumSinding]++;
    str =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
    if(this->resendCnt[this->traynumSinding]>3)
    {
        this->resendCnt[this->traynumSinding] = 0;
        //报该节点掉线并发送下一个节点采样
        text = QString("%1.%2.%3.%4").arg(str).arg("设备").arg(QString::number(this->traynumSinding)).arg("断通信");
        emit  updateBedUi(this->cmdSending[this->traynumSinding],this->traynumSinding,text);

        this->traynumSinding++;

        if(  this->traynumSinding<this->tempSlaves)
        {
            this->partlySendData(this->portName(), this->traynumSinding,'S',0,0);
        }
        else
        {
            this->traynumSinding = 0;
            this->partlySendData(this->portName(),  this->traynumSinding,'S',0,0);
        }
    }
    else
    {

        this->partlySendData(this->portName(),this->traynumSinding,this->cmdSending[this->traynumSinding],0,0);
    }

}



//
void Myserial::startSwitchSlot()
{
    Sleep(300);
    this->sendData(this->portName(),0,'C',0,0);
}


//取出界面传来的数据
void Myserial::slotGetData(QVariant  variant)
{
    tempPara m_para = variant.value<tempPara>();
    this->tempSlaves= m_para.slaveNum;
    this->tpChannels = m_para.all_chanels ;
    this->modbusType = m_para.protocol;
}






void  Myserial::Init_serial()
{
    //  qDebug()<<"INthread:"<<QThread::currentThreadId();
    QString  num = Single::m_Instance()->serialPara.at(0);
    
    //设置串口名
    this->setPortName( Single::m_Instance()->serialPara.at(1));
    //设置波特率
    this->setBaudRate(num.toInt());
    //设置数据位数
    this->setDataBits(QSerialPort::Data8);
    //设置奇偶校验
    this->setParity(QSerialPort::NoParity);
    //设置停止位
    this->setStopBits(QSerialPort::OneStop);
    //设置流控制
    this->setFlowControl(QSerialPort::NoFlowControl);
    this->open(QIODevice::ReadWrite);

    QCoreApplication::processEvents();//避免界面冻结
    
}




//串口接收数据
void Myserial::recvData(void)
{

    // qDebug()<<"INthread:"<<QThread::currentThreadId();

    QByteArray  str1;
    uchar str2[500]={0};
    uint    tmp=0;
    Sleep(5);
    str1 = this->readAll();


    memcpy(str2,str1.data(),str1.length()); //


    for(uchar i=0;i<str1.length();i++)
    {
        if(this->wait == WAIT_HEAD)    //接收
        {
            if(str2[i] == 0xEE)
            {
                if(this->head_cnt == this->recv_cnt)
                {
                    this->serialRecv[this->recv_cnt]=str2[i];
                    if(++head_cnt>=4)
                        this->wait = WAIT_LEN;
                }
                else
                    goto  err_process;
            }
            else
                goto   err_process;
        }
        else  if(this->wait== WAIT_LEN)
        {
            if(this->recv_cnt == 4)
            {
                this->serialRecv[this->recv_cnt]=str2[i];
                this->length = str2[i];
            }
            if(this->recv_cnt == 5)
            {
                this->serialRecv[this->recv_cnt]=str2[i];
                tmp= str2[i];        //
                tmp<<=8;
                this->length|=tmp;
                if((this->length +6)<=RX_BUFF_LEN)
                {
                    this->wait = WAIT_MSG;    //
                }
                else {
                    goto err_process;
                }
            }
        }
        else if(this->wait == WAIT_MSG)
        {
            this->serialRecv[this->recv_cnt]=str2[i];
            if(this->recv_cnt>= (this->length +6-1))
            {
                this->wait= WAIT_HEAD;
                this->recv_cnt =0;
                this->head_cnt =0;
                this->length = 0;
                this->recvDataMethod(this->portName(),this->serialRecv);
                return ;
            }
        }
        else
            goto   err_process;
        this->recv_cnt++;
        continue;
    }

    return;
err_process:
    wait = WAIT_HEAD;
    recv_cnt = 0;
    head_cnt= 0;
    length = 0;
    return;
}




void Myserial::sendData(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value)
{
    
    //  qDebug()<<"SDthread:"<<QThread::currentThreadId();
    //  QString  text = "ghtytuitowqe";
    //  QByteArray  data = text.toUtf8();
    //  this->write( data);
    
    QMutexLocker  Locker(&this->mutex);
    QString  peerAddr=  this->portName();
    
    if (ip == peerAddr) {                 //判定发送是否是目的客户端的socket

        this->partlySendData(ip, traynum, cmd,num,value);

    }
    //   qDebug()<<"SDthread:"<<QThread::currentThreadId();
    
}




//发送下一个节点数据
void Myserial::startNextSend(const QString &ip, const uchar cmd)
{
    


}



//下发修调码
void Myserial::sendAdjustCode( uchar traynum,float * value)
{

    ushort len ;

    for(uchar j =0;j<4;j++)
        sending_buf[j] = 0xFF;

        len = 6+2+ this->tpChannels*2+2+1+1;
    sending_buf[4]= len-6;
    sending_buf[5]= (len-6)>>8;   //
    sending_buf[6]=  traynum;    //
    sending_buf[7]= 'X';   //

    for(uchar i=0;i< this->tpChannels;i++)
    {
        if( *(value+i)>0)
            sending_buf[8+2*i]= 0;   //符号
        else
            sending_buf[8+2*i]= 1;   //符号
        sending_buf[9+2*i]=(uchar) abs(*(value+i)); // 修调值
    }
    sending_buf[len-4]= 0;  //流水
    sending_buf[len-3] = 0;   //流水
    sending_buf[len-2] = 0;   //加密
    sending_buf[len-1] = 0;   //和

    for(uchar i=6;i<len-1;i++)
        sending_buf[len-1]+=sending_buf[i];
    this->write(reinterpret_cast<const char*>(sending_buf),len);

    //加入重发
    this->traynumSinding=traynum;//保存当前正在发送的托盘 号
    this->cmdSending[this->traynumSinding] ='X'; //保存界面传过来的发送命令
    //this->resendData = data;
    //数据发送并使能200ms超时发送定时器
   // this->resendTimer->start(1000); //

}

void Myserial::sendClearCodeSlot(uchar traynum)
{
    ushort len ;

    for(uchar j =0;j<4;j++)
        sending_buf[j] = 0xFF;

        len = 6+2+ this->tpChannels*2+2+1+1;
    sending_buf[4]= len-6;
    sending_buf[5]= (len-6)>>8;   //
    sending_buf[6]=  traynum;    //
    sending_buf[7]= 'X';   //
    for(uchar i=0;i< this->tpChannels;i++)
    {

            sending_buf[8+2*i]= 0;   //符号

        sending_buf[9+2*i]=0; // 修调值
    }
    sending_buf[len-4]= 0;  //流水
    sending_buf[len-3] = 0;   //流水
    sending_buf[len-2] = 0;   //加密
    sending_buf[len-1] = 0;   //和

    for(uchar i=6;i<len-1;i++)
        sending_buf[len-1]+=sending_buf[i];
    this->write(reinterpret_cast<const char*>(sending_buf),len);

    //加入重发
    this->traynumSinding=traynum;//保存当前正在发送的托盘 号
    this->cmdSending[this->traynumSinding] ='X'; //保存界面传过来的发送命令
    //this->resendData = data;
    //数据发送并使能200ms超时发送定时器
    this->resendTimer->start(1000); //
}



//分包发送数据
void Myserial::partlySendData(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value)
{

    if(this->portName() == Single::m_Instance()->comBuf[0] )  //modbus
    {
        this->modbusSendData(ip,traynum,cmd,num,value);   //数据发送与重发送
    }
#if 0
    else if(this->portName() == Single::m_Instance()->comBuf[1]) //针床
    {
        this->needlebedSendData(ip,traynum,cmd);
    }
    else if(this->portName()== Single::m_Instance()->comBuf[2]) //OCV动作
    {
        this->ocvActionSendData(ip,traynum,cmd);
    }
    else if(this->portName() == Single::m_Instance()->comBuf[3]) //OCV切换
    {
        this->ocvSwitchSendData(ip,traynum,cmd,num,value);
    }
    else if(this->portName() == Single::m_Instance()->comBuf[4]) //内阻表
    {
        this->ohmmeterSendData(ip,traynum,cmd);
    }
    else if(this->portName() == Single::m_Instance()->comBuf[5]) //拔嘴工装
    {
        this->frockSendData(ip,traynum,cmd);
    }
    else if(this->portName() == Single::m_Instance()->comBuf[6]) //温控工装
    {
        this->tempSendData(ip,traynum,cmd);
    }
    else if(this->portName() == Single::m_Instance()->comBuf[7]) //压力工装
    {
        this->pressFrockSendData(ip,traynum,cmd);
    }
    else
        ;
#endif
    //  this->sendCnt = 0; //禁止总线上其它数据的发送(联机与采样)

    this->traynumSinding=traynum;//保存当前正在发送的托盘 号
    this->cmdSending[this->traynumSinding] = cmd; //保存界面传过来的发送命令
    //this->resendData = data;
    //数据发送并使能200ms超时发送定时器
    this->resendTimer->start(2000); //

}



//modbus协议发送
void Myserial::modbusSendData(const QString &ip, const int traynum, const uchar cmd,ushort num,ushort value)
{

    ushort crc= 0,len=0;
    bedPara  m_para;
    QVariant    variant;
    for(uchar j =0;j<4;j++)
        sending_buf[j] = 0xFF;

    switch (cmd) {

    case 'A' :
        len = 6;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= traynum;    //托盘号
        sending_buf[7]=  cmd;   //升级命令字
        sending_buf[8]=  0 ;    //流水
        sending_buf[9]= 0 ;   //流水
        sending_buf[10]=0x00;// 加密
        sending_buf[11]=0;
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf),12);

        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //   emit  sig(variant);
        break;


    case 'S':
        len = 6;
        sending_buf[4]= len;
        sending_buf[5]= len>>8;   //
        sending_buf[6]=  traynum;    //
        sending_buf[7]= cmd;   //
        sending_buf[8]= 0;   //流水号
        sending_buf[9]= 0; // 流水号
        sending_buf[10]= 0;  //加密
        sending_buf[11] = 0;
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf),12);
        memcpy(m_para.bufData,this->sending_buf,12);              //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //  emit  sig(variant);
        break;
    case 'X':            //修
             if(num == 1)      //下发修调码清零
             {
                   this->sendClearCodeSlot(traynum);
             }
             else
            {
                 this->adjustTempTraynum = traynum;
                 this->startTempAdjust = 1;
                 this->standedTempValue = value;
                 //计算修调值并下发
                 if((traynum == this->adjustTempTraynum)&&(this->startTempAdjust == 1))
                 {
                     this->startTempAdjust =0;
                     for(uchar i=0;i<this->tpChannels;i++)
                     {
                         if(this->adjustTemp[i]==0XFFFF)
                             this->adjustTemp[i]=0;
                         else
                             this->adjustCode[i]=this->standedTempValue-this->adjustTemp[i];

                     }
                     this->sendAdjustCode( traynum,adjustCode);

                 }

             }
        break;

    default:
        break;
    }

}
//OCV切换数据发送
void Myserial::ocvSwitchSendData(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value)
{

    ushort len;
    bedPara  m_para;
    QVariant    variant;
    for(uchar j =0;j<4;j++)
        sending_buf[j] = 0xFF;

    switch (cmd) {
    case 'A':                        //统一联机命令

        len = 6;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 0;    //托盘号
        sending_buf[7]=  cmd;   //升级命令字
        sending_buf[8]=  traynum ;    //流水
        sending_buf[9]= 0x01 ;
        sending_buf[10]=0x00;// 加密
        sending_buf[11]=0;
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf),12);

        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //  emit  sig(variant);
        break;
    case 'C':   //切换命令

        len = 6;
        sending_buf[4]= len ;   //长度
        sending_buf[5]= len>>8;   //长度
        sending_buf[6]= traynum ;    //托盘号
        sending_buf[7]= cmd;   //升级命令字

        sending_buf[8]= 0 ;   //切换模式
        //        if(*(data+0)==0x01)
        //        {
        //            sending_buf[9]=  1;    //通道
        //            sending_buf[10]=0 ;
        //        }
        //        else
        //        {

        sending_buf[9]=  (uchar)num;    //通道
        sending_buf[10]=  (uchar)(num >>8);
        //   }
#if 0
#endif
        sending_buf[11]=0;
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf),12);
        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //  emit  sig(variant);
        break;
    default:
        break;
    }

}
//OCV动作切换发送数据
void Myserial::ocvActionSendData(const QString &ip, const int traynum, const uchar cmd)
{

}
//针床数据发送
void Myserial::needlebedSendData(const QString &ip, const int traynum, const uchar cmd)
{

    ushort len;

    bedPara  m_para;
    QVariant    variant;
    for(uchar j =0;j<4;j++)
        sending_buf[j] = 0xFF;

    switch (cmd) {

    case 'A':                        //统一联机命令

        len = 6;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 0;    //托盘号
        sending_buf[7]=  cmd;   //升级命令字
        sending_buf[8]=  traynum ;    //流水
        sending_buf[9]= 0x01 ;
        sending_buf[10]=0x00;// 加密
        sending_buf[11]=0;
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf),12);
        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //   emit  sig(variant);
        break;


    case 'S':
        len = 6;
        sending_buf[4]= len ;   //长度
        sending_buf[5]= len>>8;   //长度
        sending_buf[6]=0;    //
        sending_buf[7]= cmd;   //采样命令字
        sending_buf[8]=  traynum ; //
        sending_buf[9]= 0 ;
        sending_buf[10]= 0 ;
        sending_buf[11]= 0 ; //校验和
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf),12);

        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //   emit  sig(variant);
        break;

        //气密性检测
    case 0x11:

        len = 8;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 0;
        sending_buf[7]= cmd;   //升级命令字
        sending_buf[8]= traynum;
        //        sending_buf[9]= Single::m_Instance()->needlebedPara[0];   //抽负压值
        //        sending_buf[10]= Single::m_Instance()->needlebedPara[0]>>8;
        //        sending_buf[11]= Single::m_Instance()->needlebedPara[2];
        //        sending_buf[12]= Single::m_Instance()->needlebedPara[2]>>8;   //保压时间
#if 0
        sending_buf[9]= *(data+0);   //抽负压值
        sending_buf[10]= *(data+1);
        sending_buf[11]= *(data+2);
        sending_buf[12]= *(data+3);   //保压时间
#endif
        sending_buf[13]=0;
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[13]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf), 14);
        memcpy(m_para.bufData,this->sending_buf,14);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 14;
        variant.setValue(m_para);
        //    emit  sig(variant);

        break;
    case 'M':
        //电机参数下发
        len = 8;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 0;
        sending_buf[7]= cmd;   //
        sending_buf[8]= traynum;
        sending_buf[9]= 0x01;      //参数个数
        sending_buf[10]= 0x01 ;   //参数序号    1： 电机步数
        //        sending_buf[11]= Single::m_Instance()->needlebedPara[1];// 步数低字节
        //        sending_buf[12]= Single::m_Instance()->needlebedPara[1]>>8;//步数高字节
#if 0
        sending_buf[11]= *(data+0);// 步数低字节
        sending_buf[12]= *(data+1);//步数高字节
#endif
        sending_buf[13] = 0;
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[13]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf), 14);
        memcpy(m_para.bufData,this->sending_buf,14);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 14;
        variant.setValue(m_para);
        //    emit  sig(variant);
        break;

    case 'X':     //负压控制

        len = 8;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 0;
        sending_buf[7]= cmd;   //升级命令字
        sending_buf[8]= traynum;
#if 0
        sending_buf[9]= *(data+0);
        sending_buf[10]= *(data+1);
        sending_buf[11]= *(data+2);
#endif
        sending_buf[12]= 0;
        sending_buf[13]=0;
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[13]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf), 14);
        memcpy(m_para.bufData,this->sending_buf,14);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 14;
        variant.setValue(m_para);
        //    emit  sig(variant);
        break;

    case 'U':                                         //读出设置参数
        len = 8;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 0;
        sending_buf[7]= cmd;   //升级命令字
        sending_buf[8]= traynum;
        sending_buf[9]= 0x0;
        sending_buf[10]= 0x0 ;
        sending_buf[11]= 0;
        sending_buf[12]= 0;
        sending_buf[13]=0;
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[13]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf), 14);
        memcpy(m_para.bufData,this->sending_buf,14);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 14;
        variant.setValue(m_para);
        //    emit  sig(variant);
        break;

    case 'L':

        len = 18;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 0;
        sending_buf[7]= cmd;   //升级命令字
        sending_buf[8]=  traynum;        //托盘号
        for(uchar i = 9;i<22;i++)
            sending_buf[i]= 0;
        //sending_buf[22]=Single::m_Instance()->needlebedPara[7];
        //  sending_buf[22]=*(data+0);
        sending_buf[23]=0;          //校验码
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[23]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf), 24);
        memcpy(m_para.bufData,this->sending_buf,24);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 24;
        variant.setValue(m_para);
        //  emit  sig(variant);
        break;
    case 'P':

        len = 6;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 0;
        sending_buf[7]= cmd;   //升级命令字
        sending_buf[8]=  traynum;          //校验码
        sending_buf[9]= 0;          //校验码
        sending_buf[10]= 0;          //校验码
        sending_buf[11]= 0;          //校验码
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf), 12);
        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //   emit  sig(variant);
        break;
    case 'Z':

        len = 6;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 0;
        sending_buf[7]= cmd;   //升级命令字
        sending_buf[8]=  traynum;          //校验码
        sending_buf[9]= 0;          //校验码
        sending_buf[10]= 0;          //校验码
        sending_buf[11]= 0;          //校验码
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf), 12);

        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //    emit  sig(variant);

        break;
    case 'R':

        len = 6;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 0;
        sending_buf[7]= cmd;   //升级命令字
        sending_buf[8]=  traynum;          //校验码
        sending_buf[9]= 0;          //校验码
        sending_buf[10]= 0;          //校验码
        sending_buf[11]= 0;          //校验码
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf), 12);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //   emit  sig(variant);
        break;
    default:
        break;
    }
}
//拔吸嘴 工装数据发送
void Myserial::frockSendData(const QString &ip, const int traynum, const uchar cmd)
{
    ushort len;

    bedPara  m_para;
    QVariant    variant;

    for(uchar j =0;j<4;j++)
        sending_buf[j] = 0xFF;

    switch (cmd) {
    case 'A':                        //统一联机命令

        len = 6;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 0;    //托盘号
        sending_buf[7]=  cmd;   //升级命令字
        sending_buf[8]=  traynum ;    //流水
        sending_buf[9]= 0x01 ;
        sending_buf[10]=0x00;// 加密
        sending_buf[11]=0;
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf),12);

        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //  emit  sig(variant);
        break;
    case 'S':

        len = 6;
        sending_buf[4]= len ;   //长度
        sending_buf[5]= len>>8;   //长度
        sending_buf[6]= traynum;    //工装地址
        sending_buf[7]= cmd;   //采样命令字
        sending_buf[8]= 0 ; //
        sending_buf[9]= 0 ;
        sending_buf[10]= 0 ;
        sending_buf[11]= 0 ; //校验和
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf),12);
        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //  emit  sig(variant);
        break;

    case 'M':
        //电机参数下发

        len = 14;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 0;
        sending_buf[7]= cmd;   //升级命令字
        sending_buf[8]= 0;    //托盘号
        sending_buf[9]=0x02;     //参数个数
        sending_buf[10]=0x00;     //步数序号
        //        sending_buf[11]=Single::m_Instance()->total_sendbuf[0];     //步数低字节
        //        sending_buf[12]=Single::m_Instance()->total_sendbuf[1];     //步数高字节
#if 0
        sending_buf[11]=*(data+0);     //步数低字节
        sending_buf[12]=*(data+1);     //步数高字节
        sending_buf[13]=0x01;     //频率序号
        sending_buf[14]=*(data+2);      //频率低字节
        sending_buf[15]=*(data+3);      //频率低字节
#endif
        sending_buf[16]= 0x00;      //流水
        sending_buf[17]= 0x00 ;
        sending_buf[18]=0x00;// 加密
        sending_buf[19]=0;
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[19]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf),20);

        memcpy(m_para.bufData,this->sending_buf,20);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 20;
        variant.setValue(m_para);
        //  emit  sig(variant);

        break;
    default:
        break;
    }
}
//温度工装数据发送
void Myserial::tempSendData(const QString &ip, const int traynum, const uchar cmd)
{
    ushort len;

    bedPara  m_para;
    QVariant    variant;
    for(uchar j =0;j<4;j++)
        sending_buf[j] = 0xFF;

    switch (cmd) {
    case 'A':                        //统一联机命令
#if 0
        len = 6;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 0;    //托盘号
        sending_buf[7]=  cmd;   //升级命令字
        sending_buf[8]=  traynum ;    //流水
        sending_buf[9]= 0x01 ;
        sending_buf[10]=0x00;// 加密
        sending_buf[11]=0;
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf),12);
        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        emit  sig(variant);
#endif

#if 1
        len = 6;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 1;    //地址
        sending_buf[7]=  cmd;   //升级命令字
        sending_buf[8]=  0 ;    //流水
        sending_buf[9]= 0 ;
        sending_buf[10]=0;// 加密
        sending_buf[11]=0;
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf),12);
        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //  emit  sig(variant);
#endif

        break;
    case 'S':

        len = 6;
        sending_buf[4]= len ;   //长度
        sending_buf[5]= len>>8;   //长度
        sending_buf[6]= 0x01;    //工装地址
        sending_buf[7]= cmd;   //采样命令字
        sending_buf[8]= 0 ;      //
        sending_buf[9]= 0 ;
        sending_buf[10]= 0 ;
        sending_buf[11]= 0 ; //校验和
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf),12);
        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //    emit  sig(variant);
        break;

    case 'L':

        len = 7;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]=traynum;  //工装地址
        sending_buf[7]= cmd;   //升级命令字
        //        sending_buf[8]=  Single::m_Instance()->tempStart[0];        //通道号
        //        sending_buf[9]=  Single::m_Instance()->tempStart[1];      //设置温度
        //        sending_buf[10]=  Single::m_Instance()->tempStart[1]>>8 ;
        //        sending_buf[11]=Single::m_Instance()->tempStart[2];

#if 0
        sending_buf[8]=  *(data+0);        //通道号
        sending_buf[9]=  *(data+1);        //设置温度
        sending_buf[10]=  *(data+2);
        sending_buf[11]=*(data+3);
#endif
        sending_buf[12]=0;          //校验码
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[12]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf), 13);
        memcpy(m_para.bufData,this->sending_buf,13);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 13;
        variant.setValue(m_para);
        //    emit  sig(variant);

        break;
    case 'P':


        len = 6;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= traynum;    //工装地址
        sending_buf[7]= cmd;   //升级命令字
        // sending_buf[8]= Single::m_Instance()->tempStart[0];
        //sending_buf[8]=*(data+0);
        sending_buf[9]= 0;          //校验码
        sending_buf[10]= 0;          //校验码
        sending_buf[11]= 0;          //校验码
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf), 12);
        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //  emit  sig(variant);
        break;
    default:
        break;
    }
}

//读取阻值
void Myserial::ohmmeterSendData(const QString &ip, const int traynum, const uchar cmd)
{

}



//压力工装发送数据
void Myserial::pressFrockSendData(const QString &ip, const int traynum, const uchar cmd)
{
    ushort len;
    bedPara  m_para;
    QVariant    variant;
    for(uchar j =0;j<4;j++)
        sending_buf[j] = 0xFF;
    switch (cmd) {
    case 'A':                        //统一联机命令

        len = 6;
        sending_buf[4]=len ;   //长度
        sending_buf[5]=len>>8;   //长度
        sending_buf[6]= 0;    //托盘号
        sending_buf[7]=  cmd;   //升级命令字
        sending_buf[8]=  traynum ;    //流水
        sending_buf[9]= 0x01 ;
        sending_buf[10]=0x00;// 加密
        sending_buf[11]=0;
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf),12);
        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        variant.setValue(m_para);
        //  emit  sig(variant);
        break;
    case 'S':

        len = 6;
        sending_buf[4]= len ;   //长度
        sending_buf[5]= len>>8;   //长度
        sending_buf[6]= traynum;    //工装地址
        sending_buf[7]= cmd;   //采样命令字
        sending_buf[8]= 0 ; //
        sending_buf[9]= 0 ;
        sending_buf[10]= 0 ;
        sending_buf[11]= 0 ; //校验和
        for(uchar i=6;i<len+6-1;i++)
            sending_buf[11]+=sending_buf[i];
        this->write(reinterpret_cast<const char*>(sending_buf),12);
        memcpy(m_para.bufData,this->sending_buf,12);  //将发送的数据同步到发送数据区显示
        memset(m_para.bedSensor,0,10);
        memset(m_para.bedTemp,0,100);
        m_para.dataLen = 12;
        return;
        break;

    default:
        break;
    }
}

//
void Myserial::timeoutMethod(void)
{

    // QMutexLocker Locker (&mutex3);

    QStringList   serialbuf;
    QString   text;
    //清空临时串口缓存
    serialbuf.clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        serialbuf.append(info.portName());
    
    for(int i=0;i<  Single::m_Instance()->portNamelist.count();i++)
    {
        //如果缓存中不包含
        if(!serialbuf.contains(Single::m_Instance()->portNamelist.at(i)))
        {
            if(Single::m_Instance()->serialPara.contains(Single::m_Instance()->portNamelist.at(i)) )
            {
                Single::m_Instance()->serialPara.clear();  //清除当前升级串口的所有参数
            }
            emit serialDiscnct(Single::m_Instance()->portNamelist.at(i));
            Single::m_Instance()->portNamelist.removeAt(i);
            this->close();
        }
    }
#if 0
    if(this->portName() == Single::m_Instance()->comBuf[1]) //温控
    {
        this->sendCnt++;
        if(this->sendCnt == 5)
        {
            this->sendCnt=0;
            this-> tempSendData(this->portName(),253,'A');//5秒发送一次联机
        }
        else if(  this->sendCnt<5)
            this-> tempSendData(this->portName(),253,'S');//温度采样
        //else
        ;
    }
#endif

    //定时发送数据
    if((this->portName() ==Single::m_Instance()->comBuf[0])&&(this->startSample == 0) )  //modbus
    {

        this->partlySendData(this->portName(),0,'S',0,0);
        this->startSample = 1;

    }
#if 0
    if(this->portName() == Single::m_Instance()->comBuf[1]) //针床
    {
        this->sendCnt++;
        if(this->sendCnt == 5)
        {
            this->sendCnt = 0;
            this->needlebedSendData(this->portName(),Single::m_Instance()->trayflag,'A'); //针床联机
        }
        else if(this->sendCnt<5)
            this->needlebedSendData(this->portName(),Single::m_Instance()->trayflag,'S'); //针床采样
    }
    if(this->portName() == Single::m_Instance()->comBuf[2])//OCV动作
    {

    }
    if(this->portName() ==Single::m_Instance()->comBuf[3] )//OCV切换板
    {
        this->sendCnt++;
        if(this->sendCnt == 5)
        {
            this->sendCnt = 0;
            this->ocvSwitchSendData(this->portName(),1,'A'); //
        }

    }
    if(this->portName() == Single::m_Instance()->comBuf[4])//内阻表
    {

    }
    if(this->portName() == Single::m_Instance()->comBuf[5])//拔吸嘴工装
    {

    }
    if(this->portName() == Single::m_Instance()->comBuf[6]) //温控
    {
        this->sendCnt++;
        if(this->sendCnt == 5)
        {
            this->sendCnt=0;
            this-> tempSendData(this->portName(),253,'A');//5秒发送一次联机
        }
        else if(  this->sendCnt<5)
            this-> tempSendData(this->portName(),253,'S');//温度采样
        else
            ;
    }
    if(this->portName() ==Single::m_Instance()->comBuf[7] )//压力
    {
        this->sendCnt++;
        if( this->sendCnt == 5)
        {
            this->sendCnt=0;
            this-> pressFrockSendData(this->portName(),240,'A');//5秒发送一次联机
        }
        else if( this->sendCnt<5)
            this-> pressFrockSendData(this->portName(),240,'S');//温度采样
        else
            ;
    }
#endif

}




//关闭串口
void Myserial::stopSerialSlot(const QString &ip)
{
    
    if(this->portName() == ip)
    {
        emit serialDiscnct(ip);  //销毁线程及串口对象
        this->close();

    }
    
}


