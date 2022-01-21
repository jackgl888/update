#ifndef NEEDLEBED_H
#define NEEDLEBED_H
#include"needlebed/bedpart.h"
#include<QLineEdit>
#include<QPushButton>
#include<QLabel>
#include <QWidget>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QGridLayout>
#include  <QComboBox>
#include <QWidget>
#include<QStackedWidget>
#include <QListWidget>
#include<QSplitter>
#include "motor/motor.h"
#include"needlemap.h"
#include "traymsg.h"
#include"custom/combdelegate.h"
#include "xlsxdocument.h"
#include  "xlsxformat.h"
#include  "xlsxcellrange.h"
#include "xlsxchart.h"
#include <QSqlDatabase>
#include <QSql>
#include<QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "single/single.h"
class   bedpart;
//class    NeedleMap;
class  dataBrowse;

#define   TRAYNUM       4
#define   MAXLINE        40000000
#define   TEMPNUM         49
class needlebed : public QWidget
{
    Q_OBJECT
public:
    explicit needlebed(QWidget *parent = nullptr);
    ~needlebed();


       excelInit();
private:
    uchar  traybuf[30];  //库位传感器
    QComboBox       *locationNum;  //库位号
    QTimer             *m_timer;
    QStackedWidget  *stack;
     uchar   startTouch;
    bedpart  *  m_bedpart;
    uchar       lock;   //用于采样命令与其它命令同步
    QSplitter    *  splitterPart;
   // QPushButton        *  serialcntl; //连机
    QComboBox          *   boundrate; //波特率
    QLabel           *   boundratechs ; //标签
    QComboBox      *  serialname;  //串口名
    QLabel        *comname;
    QGridLayout   * m_layout;
    serverThread  *m_thread;

    QLabel    *locationLab; //
    QLabel   *versionLab;
    QLineEdit  *versionRead;

    QPushButton        * start;
    QPushButton        * stop;
    QPushButton        * conBed;
    QPushButton        * emgbtn;
    QPushButton        * clearBtn;
    QPushButton        * parasetBtn;  //参数设置
    QPushButton        *parareadBtn;  //参数读取
    QPushButton         *negAirCntl;  //负压控制
    QPushButton         *negAirCheck; //气密性检测
    QPushButton         *sqlEnable;    //数据开始采样
    QPushButton         *delSql;       //删除表
    friend  class   bedpart;  //定义友元类
     QSqlQueryModel   *model;

    QXlsx ::Document  * xlsx;  //建个表
    float   bedTemp[100];  //库位温度包
    uchar   version;  //库位版本号
    ushort   negpressValue;  //负压值
    ushort  bedChannels; //针床总的通道数
    uchar   locTemps;  //库位温度数
    ushort   datalen;  //数据长度
    ushort  needlebedPara[20];//针床各配置参数
    QList<Traymsg *>traymsgList;//托盘信息管理集合

    uchar sdrcBuf[200];  //收发数据
     uchar  lenData;  //数据长度
     uchar  sqlInsertFlag;  //
signals:
    void stopSerial(const QString &ip );
    void startSend(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value);
    void updateMap( const uchar cmd,const uchar traynum,const uchar *data);  //发送信号更新map
    void startSwitch();                         //可以切换
    void  sig(QVariant);


public slots:
    void timeoutEventMethod(void);           //定时超时方法
    void on_opencloseserial_clicked();       //打开和关闭串口
    void disconnectSerial(const QString &ip);//串口掉线显示处理
    void on_start_clicked();   //启动
    void on_stop_clicked();   //停止
    void on_emgbtn_clicked();   //急停
  //  void timerCntl();  //定时器控制
    void slotClear();//清除异常
    void slotParaSet();// 参数设置
   void  slotParaRead(); //参数读取
   void   slotNegAirCntl(); //负压控制
   void   slotNegAirCheck(); //负压检测
   void trayInit(const QString & ip,const uchar traynum,const uchar flag);//初始化托盘
   void slotUpdateCmdData(const uchar cmd, const uchar traynum,const QString  &str);//更新界面数据
   void   slotBedStart();
   void  slotUiUpdate(QVariant variant);   //接收工作线程传过来的数据
  void  slotInsertData();  //温度数据插入
   void slotInserCntl();  //数据更新控制
   void slotDeleteSql();  //删除表

};



struct  mapPara
{
        uchar bedSensor[10];  //传感器
        float bedTemp[100];  //库位温度
        float negAir;
};

Q_DECLARE_METATYPE(mapPara)

#endif // NEEDLEBED_H
