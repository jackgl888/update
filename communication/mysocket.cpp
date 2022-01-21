#include "mysocket.h"


#include <synchapi.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QDebug>


MySocket::MySocket(int sockDesc, uchar flag,QObject *parent) :
    QTcpSocket(parent),
    m_sockDesc(sockDesc)
{

    this->resendTimer = new QTimer(this);
    connect(this, SIGNAL(readyRead()), this, SLOT(recvData()));
    connect(this->resendTimer,SIGNAL(timeout()),this,SLOT(timeoutMethod()));
    if(flag == 0x01)  //客户端
    {
        this->resendTimer  = new QTimer(this);
        connect(this,SIGNAL(connected()),this,SLOT(slotConnected()));
        connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));

    }

}

MySocket::~MySocket()
{

}


void MySocket::sendData(const QString  &ip, const  int traynum,const   uchar  cmd,const uchar *data)
{


    // QMutexLocker  Locker(&mutex);
    QString  peerAddr=  this->peerAddress().toString().remove(0,7);

    if (ip == peerAddr) {                 //判定发送是否是目的客户端的socket

        //emit   timerStart(200);

        this->partlySendData(ip, traynum, cmd,data);

    }
    //   qDebug()<<"SDthread:"<<QThread::currentThreadId();

}



void MySocket::recvData(void)
{

    // QMutexLocker  Locker(&mutex2);
    // qDebug()<<"REthread:"<<QThread::currentThreadId();
    QString ip = this->peerAddress().toString().remove(0, 7);

     emit startSwitch();
#if 0
    //   QByteArray data = readAll();

    QByteArray  str1;
    QString   str3,str4,str5;
    float  value;
    uchar str2[800];
    typedef enum { WAIT_HEAD, WAIT_LEN,WAIT_MSG } wait_t;
    static  uint recv_cnt;		  //当前通讯口第几个字符   三个通讯口串口1 2 3
    static  uchar head_cnt;
    static  uint length;
    static  wait_t wait;


    uint tmp;
    str1 =   this->readAll();

#if 1

    if(Single::m_Instance()->socketFlag == 0) //安捷伦
   {
        str3 =    str1 ;
        if(str3.contains("READ?"))
        {
            str4=  str3.remove( "READ?");
            str5=  str4.left(8);
            value =  str5.toFloat();
            value/=100;
          //  Single::m_Instance()->AjlValue = QString("%1").arg(value);
            //
            return;
        }
        else
        {
            str4=  str3.left(8);
            value =  str4.toFloat();
            value/=100;
          // Single::m_Instance()->AjlValue= QString("%1").arg(value);
            //  emit startSwitch();
            return;
        }

   }


#endif

    memset(str2,0,800);
    memcpy(str2,str1.data(),str1.length());
    for(ushort i=0;i<str1.length();i++)
    {

        if(wait == WAIT_HEAD)  //接收
        {
            if(str2[i] == 0xEE)
            {
                if(head_cnt == recv_cnt)
                {
                    this->recvbuf[recv_cnt]=str2[i];
                    if(++head_cnt>=4)
                        wait = WAIT_LEN;
                }
                else
                    goto  err_process;
            }
            else
                goto   err_process;
        }
        else  if(wait == WAIT_LEN)
        {
            if(recv_cnt == 4)
            {
                this->recvbuf[recv_cnt]=str2[i];
                length = str2[i];
            }
            if(recv_cnt == 5)
            {
                this->recvbuf[recv_cnt]=str2[i];
                tmp= str2[i];
                tmp<<=8;
                length|=tmp;
                if((length +6)<=RX_BUFF_LEN)
                {
                    wait = WAIT_MSG;
                }
                else {
                    goto err_process;
                }
            }

        }
        else if(wait == WAIT_MSG)
        {
            this->recvbuf[recv_cnt]=str2[i];
            if(recv_cnt>=(length+6-1))
            {
                wait = WAIT_HEAD;
                recv_cnt =0;
                head_cnt =0;
                length = 0;
                //接收一帧数处理
                this->recvDataMethod(ip,this->recvbuf);
                return ;
            }
        }
        else
            goto   err_process;

        recv_cnt++;
        continue;

err_process:
        wait = WAIT_HEAD;
        recv_cnt = 0;
        head_cnt = 0;
        length = 0;
        return;
    }
#endif
}

//分包发送数据
void MySocket:: partlySendData(const QString  &ip, const  int traynum,const   uchar  cmd,const uchar *data)
{
    ushort crc,len,num=0;
    static uchar secindex =16;  //扇区索引起始地址为16*addrsize + 0x0800 0000;
    QString text;
    QString trnum = QString::number(traynum);
    for(uchar j =0;j<4;j++)
        sending_buf[j] = 0xFF;

    if(Single::m_Instance()->netMethod == 0x01)     //软件作为客户端时
    {
        if(Single::m_Instance()->socketFlag == 0x01) //安捷伦
        {
             ;
        }
//        else if()
//        {
//             ;
//        }
    }
    else //软件作为服务器时
    {
         if(Single::m_Instance()->socketFlag == 0) //温度工装
         {
                ;
         }
//         else  if( )
//         {
//               ;
//         }
    }
    this->cmdSending = cmd; //保存界面传过来的发送命令
    this->traynumSinding=traynum;//保存当前正在发送的托盘 号
    this->resendData = data;
    //数据发送并使能200ms超时发送定时器
    this->resendTimer->start(1000); //

   #if 0
    switch (cmd) {

    case 'A':                        //统一联机命令
        for(uchar i =0;i<Single::m_Instance()->Traymsgset.count();i++)
        {
            if(Single::m_Instance()->Traymsgset.at(i)->getTraynum() == traynum)
            {
                Single::m_Instance()->Traymsgset.at(i)->writeSdflag(true);
                len = 7;
                sending_buf[4]=len ;   //长度
                sending_buf[5]=len>>8;   //长度
                sending_buf[6]= 0;    //托盘号
                sending_buf[7]=  cmd;   //升级命令字
                sending_buf[8]=  0x0A ;    //工装类型
                sending_buf[9]= 0x00 ;   //流水号
                sending_buf[10]= 0x00 ;   //流水号
                sending_buf[11]=0x00;// 加密
                sending_buf[12]=0;     //校验码
                for(uchar i=6;i<len+6-1;i++)
                    sending_buf[12]+=sending_buf[i];
                this->write(reinterpret_cast<const char*>(sending_buf),13);
            }
        }

    case 'P':                        //压力清零
        for(uchar i =0;i<Single::m_Instance()->Traymsgset.count();i++)
        {
            if(Single::m_Instance()->Traymsgset.at(i)->getTraynum() == traynum)
            {
                Single::m_Instance()->Traymsgset.at(i)->writeSdflag(true);

                len = 7;
                sending_buf[4]=len ;   //长度
                sending_buf[5]=len>>8;   //长度
                sending_buf[6]= 0;    //托盘号
                sending_buf[7]=  cmd;   //升级命令字
                sending_buf[8]=  0x0A ;    //工装类型
                sending_buf[9]= 0x00 ;   //流水号
                sending_buf[10]= 0x00 ;   //流水号
                sending_buf[11]=0x00;// 加密
                sending_buf[12]=0;     //校验码
                for(uchar i=6;i<len+6-1;i++)
                    sending_buf[12]+=sending_buf[i];
                this->write(reinterpret_cast<const char*>(sending_buf),13);
            }
        }
        break;
    case 'S':                        //压力采样
        for(uchar i =0;i<Single::m_Instance()->Traymsgset.count();i++)
        {
            if(Single::m_Instance()->Traymsgset.at(i)->getTraynum() == traynum)
            {
                Single::m_Instance()->Traymsgset.at(i)->writeSdflag(true);

                len = 5;
                sending_buf[4]=len ;   //长度
                sending_buf[5]=len>>8;   //长度
                sending_buf[6]= 0;    //托盘号
                sending_buf[7]=  cmd;   //升级命令字

                sending_buf[8]= 0x00 ;   //流水号
                sending_buf[9]=0x00;// 加密
                sending_buf[10]=0;     //校验码
                for(uchar i=6;i<len+6-1;i++)
                    sending_buf[10]+=sending_buf[i];
                this->write(reinterpret_cast<const char*>(sending_buf),11);
            }
        }
        break;
    default :
        break;
    }


#if 0
    if(cmd == 'w')
    {
        for(uchar i = 0;i<Single::m_Instance()->Traymsgset.count();i++)
        {
            if(Single::m_Instance()->Traymsgset.at(i)->getSdnum() != -2)
            {
                Single::m_Instance()->Traymsgset.at(i)->writeSdflag(true);
                len = 16-6;
                sending_buf[4]=len ;   //长度
                sending_buf[5]=len>>8;   //长度
                sending_buf[6]= traynum ;    //托盘号
                sending_buf[7]= cmd;   //升级命令字
                sending_buf[8]= Single::m_Instance()->select;      //设备类型  0：表示针床  1：表示温度控制板
                sending_buf[9]= Single::m_Instance()->Single::m_Instance()->upgradeNumMax ;      //  0：默认为4个 否则  1~255;
                for(char c = 0;c<4;c++)
                    sending_buf[10+c]=0;
                crc = Single::m_Instance()->CRC16_Modbus(&sending_buf[6], 14-6);
                sending_buf[14]=  crc& 0XFF;
                sending_buf[15] = (crc>> 8)& 0XFF;
                this->write(reinterpret_cast<const char*>(sending_buf),16);
                return;
            }

        }

    }
    for(uchar i = 0;i<Single::m_Instance()->Traymsgset.count();i++)
    {
        if((Single::m_Instance()->Traymsgset.at(i)->getIp()==ip)&&
                (Single::m_Instance()->Traymsgset.at(i)->getTraynum()==traynum)&&
                (Single::m_Instance()->Traymsgset.at(i)->getStaChoose()==true)&&
                (Single::m_Instance()->Traymsgset.at(i)->getStaHiden() ==false))
        {
            switch (cmd) {
            case 'x':
                Single::m_Instance()->Traymsgset.at(i)->writeSdflag(true);//置位发送标志
                num =  Single::m_Instance()->Traymsgset.at(i)->getSdnum();
                sending_buf[6]= traynum ;    //托盘号
                sending_buf[7]= cmd;   //升级命令字
                sending_buf[8]=0;     //预留位1
                if( Single::m_Instance()->select == 1)  //温度采板flash容量为1024，此为1
                {
                    sending_buf[9]= secindex+num;

                    sending_buf[10]=0;
                }

                else
                {
                    if(num%2==0)
                        sending_buf[9]= secindex+num/2;      //  扇区索引 secindex   secAddr = (uint32_t)FLASH_PAGE_SIZE * secIndex;
                    sending_buf[10]=num%2;    //扇区偏移  secOffset   温度采样板恒为 1 针床主控制板 0 ,//扇区地址
                }

                if(( Single::m_Instance()->lastbytes>0)&&(num ==  Single::m_Instance()->totalSdtimes -1))//最后一帧
                {
                    len = 5+  Single::m_Instance()->lastbytes  +2;  //从箱号到最后帧最后一个字节
                    sending_buf[4]=len ;   //长度
                    sending_buf[5]=len>>8;   //长度
                    memcpy( sending_buf+11,(Single::m_Instance()->total_sendbuf+(num* MAX_SEND_SIZE )),Single::m_Instance()->lastbytes);
                    crc = Single::m_Instance()->CRC16_Modbus(&sending_buf[6], 5+ Single::m_Instance()->lastbytes );
                    sending_buf[11+Single::m_Instance()->lastbytes]=  crc& 0XFF;
                    sending_buf[11+Single::m_Instance()->lastbytes+1] =  (crc>> 8)& 0XFF;
                    this->write(reinterpret_cast<const char*>(sending_buf),len+6);
                    text =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ")+ip + " 托盘"+ trnum+"正在升级...";
                    Single::m_Instance()->messageRefresh(ip,traynum,  text ,Single::m_Instance()->totalSdtimes,false,false,false);
                    return ;
                }
                else
                {
                    len = 5+ MAX_SEND_SIZE  +2;  //从箱号到最后帧最后一个字节
                    sending_buf[4]=len ;   //长度
                    sending_buf[5]=len>>8;   //长度
                    memcpy( sending_buf+11,(Single::m_Instance()->total_sendbuf+(num* MAX_SEND_SIZE )),MAX_SEND_SIZE );
                    crc =Single::m_Instance()->CRC16_Modbus(&sending_buf[6], 5+ MAX_SEND_SIZE );
                    sending_buf[11+MAX_SEND_SIZE ]= crc& 0XFF;
                    sending_buf[11+MAX_SEND_SIZE +1] = (crc>> 8)& 0XFF;
                    this->write(reinterpret_cast<const char*>(sending_buf), len+6);
                    text =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ")+ip + " 托盘"+ trnum+"正在升级...";
                    Single::m_Instance()->messageRefresh(ip, traynum, text ,num +1,false,false,false);
                    return;
                }
                break;
            case 'z':
                for(uchar i = 0;i<Single::m_Instance()->Traymsgset.count();i++)
                {
                    if((Single::m_Instance()->Traymsgset.at(i)->getIp()==ip)&&
                            (Single::m_Instance()->Traymsgset.at(i)->getTraynum()==traynum))
                    {
                        Single::m_Instance()->Traymsgset.at(i)->writeSdflag(true);
                        len = 4;
                        sending_buf[4]=len ;   //长度
                        sending_buf[5]=len>>8;   //长度
                        sending_buf[6]= traynum ;    //托盘号
                        sending_buf[7]= cmd;   //升级命令字
                        crc =Single::m_Instance()->CRC16_Modbus(&sending_buf[6], 2);
                        sending_buf[8]=crc& 0XFF;
                        sending_buf[9]=(crc>> 8)& 0XFF;
                        this->write(reinterpret_cast<const char*>(sending_buf), 10);
                        return;
                    }
                }
                break;
            default:
                break;
            }
        }
    }
#endif
#endif
}



//接收数据

void MySocket::recvDataMethod(const QString &ip, const uchar * data)
{

    this->resendTimer->stop();
    this->resendCnt =0;

    ushort len;
    QString  tr, text;       //  const  QString str = QString::fromUtf8(data.data(),data.length());
    len = *(data + 4) +(*(data + 5)<<8) ;  //数据长度
    //   if(Single::m_Instance()->CRC16_Modbus(data+6, len))
    //      return;
    if(!Single::m_Instance()->net_packet_checksum(data+6,len)) //和校验
        return ;
    uchar cmd =     *(data+7);   //str[6];
    uchar traynum = *(data+6);   //str[7]
    uchar runFlag = *(data+9);   //str[8]
    tr = QString::number(traynum );
    if(runFlag == 'E')
    {
        text =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ")+ip +
                " 托盘"+ tr +"升级写入失败...";

        return;
    }

    if(Single::m_Instance()->netMethod == 0x01)     //软件作为客户端时
    {
        if(Single::m_Instance()->socketFlag == 0x01) //安捷伦
        {
             ;
        }
//        else if()
//        {
//             ;
//        }
    }
    else //软件作为服务器时
    {
         if(Single::m_Instance()->socketFlag == 0) //温度工装
         {
                ;
         }
//         else  if()
//         {
//               ;
//         }
    }

    emit updateBedUi(cmd,traynum,data);


    #if 0
    switch (cmd) {
    case 'A':
        for(uchar i = 0;i<Single::m_Instance()->Traymsgset.count();i++)  // 联机回复
        {
            if((Single::m_Instance()->Traymsgset.at(i)->getIp()==ip)&&
                    (Single::m_Instance()->Traymsgset.at(i)->getTraynum()==traynum))
            {
                Single::m_Instance()->mesg = "联机成功！";
            }
        }
        break;
    case 'w':

        for(uchar i = 0;i<Single::m_Instance()->Traymsgset.count();i++)
        {
            if((Single::m_Instance()->Traymsgset.at(i)->getIp()==ip)&&
                    (Single::m_Instance()->Traymsgset.at(i)->getTraynum()==traynum))
            {

                Single::m_Instance()->Traymsgset.at(i)->writeSdflag(false); //清零发送标志
                //清零重发次数
                Single::m_Instance()->Traymsgset.at(i)->writeRsdnum(0);
                Single::m_Instance()->Traymsgset.at(i)->writeSdnum(-2);
                text = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ")+ ip + "  托盘"+tr+"联机成功";
                Single::m_Instance()->messageRefresh(ip,traynum,text,0,true,false,false);
                this->startNextSend(ip,'w');  //发送下一个节点
                return;
            }
        }
        break;
    case 'S':
        for(uchar i = 0;i<Single::m_Instance()->Traymsgset.count();i++)  // 压力采样
        {
            if((Single::m_Instance()->Traymsgset.at(i)->getIp()==ip)&&
                    (Single::m_Instance()->Traymsgset.at(i)->getTraynum()==traynum))
            {
                for(ushort i=0;i<256;i++)
                {
                    Single::m_Instance()->temp[i]=*(data+14+2*i)|(*(data+15+2*i)<<8);
                }
            }
        }
        break;
    case 'P':
        for(uchar i = 0;i<Single::m_Instance()->Traymsgset.count();i++)  // 压力归零
        {
            if((Single::m_Instance()->Traymsgset.at(i)->getIp()==ip)&&
                    (Single::m_Instance()->Traymsgset.at(i)->getTraynum()==traynum))
            {
                Single::m_Instance()->mesg = "压力归零完成！";
            }
        }
        break;


    case 'x':
#if 0
        for(uchar i = 0;i<Single::m_Instance()->Traymsgset.count();i++)
        {
            if((Single::m_Instance()->Traymsgset.at(i)->getIp()==ip)&&
                    (Single::m_Instance()->Traymsgset.at(i)->getTraynum()==traynum))
            {
                Single::m_Instance()->Traymsgset.at(i)->writeSdflag(false); //清零发送标志
                uint num= Single::m_Instance()->Traymsgset.at(i)->getSdnum()+1;
                Single::m_Instance()->Traymsgset.at(i)->writeSdnum(num);//发送次数加1
                if(num >=Single::m_Instance()->totalSdtimes)
                {
                    Single::m_Instance()->Traymsgset.at(i)->writeSdnum(-1); //成功写入-1
                    text = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ")+ip+ " "+ tr+"号托盘升级成功!";
                    Single::m_Instance()->messageRefresh(ip,traynum,text, Single::m_Instance()->totalSdtimes,false,false,false);      //更新界面
                    this->startNextSend(ip,'x');  //发送下一个节点
                }
                else
                    this->sendData(ip, traynum,'x');    //分包发送
                //清零重发次数
                Single::m_Instance()->Traymsgset.at(i)->writeRsdnum(0);
            }
        }

#endif
        break;
    case 'z':
        for(uchar i = 0;i<Single::m_Instance()->Traymsgset.count();i++)
        {
            if((Single::m_Instance()->Traymsgset.at(i)->getIp()==ip)&&
                    (Single::m_Instance()->Traymsgset.at(i)->getTraynum()==traynum))
            {
                Single::m_Instance()->Traymsgset.at(i)->writeSdflag(false); //清零发送标志
                Single::m_Instance()->Traymsgset.at(i)->writeSdnum(-2);//发送次数
                Single::m_Instance()->Traymsgset.at(i)->writeRsdnum(0); //清零重发次数
                if(runFlag == 'O')
                {
                    text = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ")+ ip + "  托盘"+tr+"运行成功！";
                    Single::m_Instance()->messageRefresh(ip,traynum,text,0,false,false,false);   //更新界面
                    this->startNextSend(ip,'z');
                    return;
                }
                else
                {
                    text = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ")+ ip + "  托盘"+tr+"运行失败！";
                    Single::m_Instance()->messageRefresh(ip,traynum,text,0,false,false,false); //更新界面
                    this->startNextSend(ip,'z');
                    return;
                }
            }
        }
        break;
    default:
        break;
    }
#endif
}

void MySocket::slotConnected()
{
    qDebug()<<"连接成功";
}

void MySocket::slotDisconnected()
{
    qDebug()<<"连接失败";
}


//定时超时处理
void MySocket::timeoutMethod(void)
{
    //  QString ip = this->peerAddress().toString().remove(0, 7);

}




//读取电压值
void MySocket::slotReadData()
{
    uchar buf[7] ={0x52, 0x45, 0x41, 0x44, 0x3F, 0x0D, 0x0A };
    this->write(reinterpret_cast<const char*>(buf),7);
}



void MySocket::tempSendData(const QString &ip, const int traynum, const uchar cmd, const uchar *data)
{

}

void MySocket::ohmmeterSendData(const QString &ip, const int traynum, const uchar cmd, const uchar *data)
{

}

void MySocket::pressFrockSendData(const QString &ip, const int traynum, const uchar cmd, const uchar *data)
{

}







//发送下一个节点数据
void MySocket::startNextSend(const QString &ip, const uchar cmd)
{
   #if 0
    for(uchar i = 0;i<Single::m_Instance()->Traymsgset.count();i++)
    {
        if(Single::m_Instance()->Traymsgset.at(i)->getIp()==ip)
        {

            if((Single::m_Instance()->Traymsgset.at(i)->getStaChoose()==true)&&
                    Single::m_Instance()->Traymsgset.at(i)->getStaHiden() ==false)
            {

                if(cmd =='x')
                {
                    if(Single::m_Instance()->Traymsgset.at(i)->getSdnum()!=-1)
                    {
                        Single::m_Instance()->Traymsgset.at(i)->writeSdnum(0);
                        Single::m_Instance()->messageRefresh(ip,Single::m_Instance()->Traymsgset.at(i)->getTraynum(),NULL,0,false,false,false);
                        this->sendData(ip,Single::m_Instance()->Traymsgset.at(i)->getTraynum(),cmd); //分包发送
                        return;
                    }

                }
                else if(cmd =='z')
                {
                    if(Single::m_Instance()->Traymsgset.at(i)->getSdnum()!=-2)
                    {
                        this->sendData(ip, Single::m_Instance()->Traymsgset.at(i)->getTraynum(),cmd);
                        return;
                    }

                }
                else
                    ;
            }
            if((cmd == 'w')&&(Single::m_Instance()->Traymsgset.at(i)->getSdnum()!=-2))
            {
                this->sendData(ip, Single::m_Instance()->Traymsgset.at(i)->getTraynum(),cmd);
                return;
            }
        }
    }
#endif

}


