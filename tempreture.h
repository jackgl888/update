#ifndef TEMPRETURE_H
#define TEMPRETURE_H
#include <QPushButton>
#include  <QTextBrowser>
#include   <QGridLayout>
#include <QVBoxLayout>
#include <QWidget>
#include<QLineEdit>
#include <QComboBox>
#include <QLabel>
#include  <QGroupBox>
#include  <QTableView>
#include <QStandardItemModel>
#include "communication/serverthread.h"
#include  <QSpinBox>
#include "xlsxdocument.h"
#include  "xlsxformat.h"
#include  "xlsxcellrange.h"
#include "xlsxchart.h"
#include <QFileDialog>
#include "communication/server.h"
#include "needlebed/traymsg.h"
#include <QSqlDatabase>
#include <QSql>
#include<QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include"struct.h"
#define  temppath    "./log/book9/9.xlsx"

class   Server;
class serverThread;

#define   TRAYNUM    4
#define    CHANNELS  52

class tempreture : public QWidget
{
    Q_OBJECT
public:
    explicit tempreture(QWidget *parent = nullptr);
    ~tempreture();
      void  excelInit(void);  //表格初始化
   void initUiTemp(void);
       void  creatDB(void);
private:
    QPushButton   * touchFrock;
    QComboBox   *   boundrate;
    QComboBox   *   serialname;
    QComboBox   *   modeTye;
    QSpinBox   *  allChannels;
    QLabel      *  allChannelsLab;
    QLabel      *  modeLable;
    QPushButton    * guilingPress ;
    QLabel      *comname;
    QLabel      *boundratechs;
    QLabel   *tempLable;
    QSpinBox   *setTemp;  //设置温度值
    QSpinBox    *channel;  //加热通道号
    uchar        startSp;    //开始采样
    QLabel       *channeLabel;
    QPushButton   *startHeat;  //启动恒温
    QPushButton    *stopHeat;  //停止恒温
    QPushButton   *exportData;  //导出数据
    //  QTextBrowser  * tempmsg;
    QVBoxLayout   *  serialLayout;
    QVBoxLayout   *  tempcntlLayout;
    QHBoxLayout   *  bottomLayout;
    QGridLayout    *  mainlayout;
    QTableView      *tempList;   //显示所有通道温度
    QStandardItemModel   *tempModle;  //数据模模型

 //  QSqlTableModel   *model;
    QSqlQueryModel   *model;
    // QPushButton    *serialCntl;  //
    QTimer   *tempTimer;
    serverThread   *m_thread;
    QTextBrowser    *tempBrowser; //消息框
    QPushButton   *  msgClearBtn;  //消息清除
    QPushButton    *codeClearBtn;  //补偿码清零
    QLabel       *setValue;
    QLineEdit      *valueEdit;
    QLabel   * masterVersion;
    QLineEdit  *masterVersionEdit;
    QLabel   *slaveVersion;
    QLineEdit  *slaveVersionEdit;
    QGridLayout  * mslayout;
    QPushButton   *onlineBtn;  //modbus联机
    QPushButton    *sanlingOnlineBtn;  //三菱联机
    QPushButton   *tempCntOnlinelBtn;  // 温控联机
    QComboBox    *tempCntlSerialName; //
    QLabel       *tempCntlSerialNamelab;
    QComboBox   *   tempCntlBoundrate;  //温控波特率
    QLabel   * tempCntlBoundrateLab;
    QLabel   * tpclChannelsLab; //温控通道数
    QSpinBox  *tpclChannels;  //温控通道数
    QLabel   * slaveAddrLab; //从站地址
    QSpinBox  *slaveAddr;
    QLabel    *slaveNumLab; //从站总数
    QSpinBox  *slaveNum;
    QXlsx ::Document  * xlsx;  //建个表
    Server   *m_server;        //创建服务端
    friend  class Server;   //申明友元类
    QLabel  *netport;   //端口号
    QComboBox  *m_port;
    QLabel  *ipaddrlb;  //对方ip
    QLineEdit  *ipaddr;  //
    QLabel   *m_iplb;
    QLineEdit  *m_ip; //本机ip
    QPushButton  * sample;  //采样开始
    ushort    tempStart[10];  //温度处理载入参数
    float    temp[600];  //温度校准工装采样温度
    QString   mesg;   //温度信息打印
    QList<Traymsg *>traymsgList;//托盘信息管理集合
    QLabel   *TempLab;
    QLineEdit  *TempVersion; //版本号
    uchar synStart; //1  开始同步  0 结束同步

    QSqlQuery  *query;

    QComboBox   *calculate;//计算方法
    QLabel  *calculateLab;
    QLabel  *orderWayLab;
    QComboBox   *orderWay;
    QLabel  *conditionLab;  //查询方式
    QComboBox   *conditionBox;
    QPushButton   * startInsert;  //开始插入
    QPushButton   *selectBtn;   //开始查询
    QPushButton   * deleteBtn;   //删除
    QPushButton   * orderBtn;    //排序
    QPushButton   * calculateBtn;    //计算
    QLabel     *MinStart;
    QLabel      *MaxEnd;
    QSpinBox   *valueMin;  //删除开始位置
    QSpinBox   *valueMax; //删除结束位置
    QGroupBox  *sqlBox;
    QVBoxLayout  * topLayout;  //
    QVBoxLayout  * btnBottomLayout;
    QVBoxLayout  *sqlmainlayout;
    QList<QMap<QString, QVariant> > valuesList;
    char tempAdjustCode [CHANNELS];    //温度修调码
    QSpinBox  *tempAdjustBox  ; //校准理论值

signals:
    void  stopSerial(const QString &ip );
    void  startSend(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value);
    void  updateMap( const uchar cmd,const uchar traynum,const uchar *data);  //发送信号更新map
    void  sig(QVariant para);

public slots:

    void timeoutMethod(void);
    void omronProtocolInit(void);//欧姆龙采样协议
    void  sanlingProtocolInit(void);  //三菱modus协议
    void disconnectSerial(const QString &ip);//串口掉线显示处理
    void trayInit(const QString & ip,const uchar traynum,const uchar flag);//初始化托盘
    void  slotStartHeat(void);
    void  slotStopHeat(void);
    void  slotExportData(void); //导出数据
    void  guilingPressslot(void);//压力归零
    void  touchFrockslot();  //工装联机
    void   sampleSlot(void); //
    void   tempCntlInit();//温度工装串口控制
    void  slotUpdateCmdData(const uchar cmd,const uchar traynum,const QString &str);//更新界面数据
    void  slotUiUpdate(QVariant variant);   //接收工作线程传过来的数据
    void   slotTestSql(void);   //
    void  slotInsertData(void);   //插入数据
    void  slotDeleteData(void);   //删除 数据
    void  slotSelectData(void);   //查询数据
    void   slotOrderData(void);  //排序数据
    void  slotCalculateData(void);  //计算数据
    void  tempAdjustSlot(void);     //温度修凋
    void  msgClearSlot(void);  //消息清除
     void codeClearSlot(void);   //清零修调码

};



#endif // TEMPRETURE_H
