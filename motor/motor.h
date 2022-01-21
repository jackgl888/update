#ifndef MOTOR_H
#define MOTOR_H
#include<QLineEdit>
#include<QPushButton>
#include <QComboBox>
#include<QLabel>
#include <QWidget>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QGridLayout>
#include <QTimer>
#include"single/single.h""
#include "communication/serverthread.h"
#include "needlebed/traymsg.h"
#include  <QTextBrowser>
#include "custom/myanimationbtn.h"
class motor : public QWidget
{
    Q_OBJECT
public:
    explicit motor(QWidget *parent = nullptr);
    ~motor();
signals:
    void stopSerial(const QString &ip );
    void startSend(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value);
    void sig(QVariant);
private  slots:
    void  moveForwardSlot(void); //前进
    void on_opencloseserial_clicked();//打开和关闭串口
    void disconnectSerial(const QString &ip);//串口掉线显示处理
    void timeoutEventMethod(void); //定时超时方法
    void on_senddata_clicked(void);   //停止
    void on_betback_clicked(void);   //后退
    void trayInit(const QString & ip,const uchar traynum,const uchar flag);//初始化托盘
    void slotUpdateCmdData(const uchar cmd, const uchar traynum,const QString  &str);//更新界面数据
    void  slotUiUpdate(QVariant variant);   //接收工作线程传过来的数据
private:

    QLabel   *boundratechs;
    QComboBox   *   boundrate;
    QComboBox   *  serialname;
    QPushButton  *  serialcntl;
    serverThread   *  m_thread;
    QVBoxLayout  *   m_vlout;
    QHBoxLayout  *   m_hlout;
    QSpinBox   *stepsin;
    QLineEdit   *stepsout;
    QSpinBox   *speedin;
    QLineEdit   *speedout;
    QTimer  *   m_timer;
    QPushButton *forward ;
    QPushButton *back ;
    QPushButton *stop ;
    QLabel   *comname;
    QLabel   *result;
    QLineEdit  *spresult;
    QLabel   *Oringe;//工装回零
    QLineEdit   *OringePoint;
    uchar  motorbuf[30];
    QList<Traymsg *>traymsgList;//托盘信息管理集合
    uchar sdrcBuf[200];  //收发数据
    QTextBrowser *m_browser;
    uchar lock;    //用于485总线上的命令的同步

    myAnimationBtn * myabtn;
};

#endif // MOTOR_H
