#ifndef MYSOCKET_H
#define MYSOCKET_H
//#include <QtNetwork>
#include <QTcpSocket>
#include<QMutexLocker>
#include<QTreeWidgetItem>
#include "serverthread.h"
#include <QHostAddress>
#include <QObject>
#include <QTimer>
#include"single/single.h"

class QTimer;
#define  SOCKET_NUM  10
class MySocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MySocket(int sockDesc, uchar flag, QObject *parent = Q_NULLPTR);
    ~MySocket();



signals:

    void  timerStart(int secs);
    void  startSwitch(void);
    void  serialDiscnct(void);    //
    void  updateBedUi(const uchar cmd,const uchar traynum,const uchar *data);  //更新bed  ui

private slots:
    void recvData(void);
    void sendData(const QString  &ip, const  int traynum,const   uchar  cmd,const uchar *data);
    void startNextSend(const QString &ip,const uchar cmd);
    void partlySendData(const QString  &ip, const  int traynum,const   uchar  cmd,const uchar *data);
    void recvDataMethod(const QString &ip, const uchar *data);
    void slotConnected();
    void slotDisconnected();
    void timeoutMethod(void);
    void slotReadData(void);  //读取电压值
    void tempSendData(const QString  &ip, const  int traynum,const   uchar  cmd,const uchar *data); //温度工装数据发送
    void ohmmeterSendData(const QString  &ip, const  int traynum,const   uchar  cmd,const uchar *data);//读取值值
    void pressFrockSendData(const QString  &ip, const  int traynum,const   uchar  cmd,const uchar *data);//压力工装数据发送

private:

    int m_sockDesc;
    QMutex   mutex,mutex2;   //鍒涘缓涓€涓簰鏂ラ噺
    uchar  sending_buf[1200];//褰撳墠鍙戦€佸寘
    uchar recvbuf[1000];
    QTimer  * m_timer;
    uchar    sendCnt; //命令互斥时计时使用
    uchar    resendCnt; //串口重发送计数 大于3次，发送失败
    uchar    cmdSending; //
    uchar    traynumSinding; //
    const  uchar    *resendData;
    QTimer  *resendTimer; //监视数据发送
};

#endif // MYSOCKET_H
