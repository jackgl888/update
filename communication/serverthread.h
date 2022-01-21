#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H
//#include "server.h"
#include <QThread>
#include "mysocket.h"
#include <QTimer>
#include <QTreeWidgetItem>
#include<QMutexLocker>
#include "myserial.h"
#include<QDateTime>
class Socket;
class Myserial;
class  MySocket;

class serverThread : public QThread
{
    Q_OBJECT
public:
    serverThread(int sockDesc,uchar serial,uchar socket,uchar netPortKind,QObject *parent = Q_NULLPTR) ;
    ~serverThread();
  void run(void);


signals:

    void  disconnectTCP(const QString &ip);
    void  sendData(const QString  &ip, const  int traynum,const uchar cmd,ushort channel,ushort value);

    void  stopSerial(const QString &ip);
    void  updateUiSerial(const QString &ip);
    void  bedUpdate( const uchar cmd,const uchar traynum,const QString &str); //
    void  readData();
    void  startSwitch();
    void  sigto(QVariant para);
     void sigtu(QVariant para);
     void sigTempBack(QVariant para);

private slots:   


    void  stopSerialSlot(const QString &ip );
    void disconnectToHost(void);
    void sendDataSlot(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value);
    void serialDisconnect(const QString & ip);
    void  bedCmdDataSlot( const uchar cmd,const uchar traynum,const QString &str);  //数据传回界面
    void  dataSendUi(QVariant para);
    void  slotReadData();
    void  startSwitchSlot();
    void  slotGetData(QVariant para);
    void slotTempBack(QVariant para);  //温度返回

private:

    MySocket * m_socket;
    int m_sockDesc;
    QList <int >ports;
    QList <QString> ipList;
    Myserial  * m_serial;
    uchar  serilPort;  // 1:表示串口线程
    uchar   socketPort;// 1:表示网口线程
    uchar  netKind;  // 0：作服务端时开的线程     1：作客户端时开的线程

};

#endif // SERVERTHREAD_H
