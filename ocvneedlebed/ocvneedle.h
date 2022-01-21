#ifndef OCVNEEDLE_H
#define OCVNEEDLE_H
#include<QLabel>
#include<QLineEdit>
#include<QGridLayout>
#include <QPushButton>
#include <QWidget>
#include <QComboBox>
#include <QTimer>
#include  <single/single.h>
#include "communication/myserial.h"
#include "communication/serverthread.h"
#include "QSpinBox"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QFrame>
#include  <QHBoxLayout>
#include   <QVBoxLayout>
#include <QGroupBox>
#include "xlsxdocument.h"
#include  "xlsxformat.h"
#include  "xlsxcellrange.h"
#include "xlsxchart.h"
#include <QFileDialog>
#include  "needlebed/traymsg.h"



class ocvneedle : public QWidget
{
    Q_OBJECT
public:
    explicit ocvneedle(QWidget *parent = nullptr);
    ~ocvneedle();

    void  initScene(void);  //托盘初始化
private:
    float ValueVoltage[500]; //电压值
    float ValueRegister[500]; //电流值
    uchar  total_sendbuf[200];   //


    QPushButton  *  contSwitch;   //连续切换
    QPushButton  *  stopSwitch;  //  停止切换
    QComboBox   *   boundrate;
    QComboBox   *  serialname;
    QTimer       *m_timer;
    serverThread   *  m_thread;

    QLabel * channelname;
    QLabel  * mode;
    QComboBox  *modein;
    QSpinBox  * channel;
    QPushButton *comBtn; //联机与断开OCV切换 板
    QPushButton  *button;
    QLabel   *comname;
    QLabel   *boundratechs;
    QLabel  * switchTime;    //切换间隔
    QSpinBox              * m_box;
    QPushButton     *msgout;   //数据导出
    QFrame                *ctrlFrame;  //主布局
    QVBoxLayout           *framelayout;
    QVBoxLayout           *resultlayout;
    QHBoxLayout           *leftlayout;
    QHBoxLayout           *mainlayout;  //主布局
    QGraphicsScene        *scene;
    QGraphicsView         *view ;
    QGraphicsRectItem     *C_item ;   //总库位平面
    QGraphicsRectItem     *T_item ;     //托盘平面
    QGraphicsEllipseItem  *channels[100];   //通道数组
    QLabel     *battryVoltage;    //电池电压
    QLabel     *battryResistence;  //电池内阻
    QLabel     *negVoltage;        //负极壳体电压
    QLabel     *posVoltage;        //正极壳体电压
    QLineEdit  *batVoltage;   //电池电压
    QLineEdit  *batResistence;   //电池内阻
    QLineEdit  *negvolt;   //负极壳体
    QLineEdit  *posvolt;   //正极壳体
    QLabel     *bvToplever; //电压上限
    QLabel     *bvDownlever; //电池电压下限
    QLabel     *brToplever; //内阻上限
    QLabel     *brDownlever;//内阻下限
    QLabel     *svToplever;//壳体电压上限
    QLabel     *svDownlever;  //
    QLineEdit  *bvtopEdit;
    QLineEdit  *bvdownEdit;
    QLineEdit  *brtopEdit;
    QLineEdit  *brdownEdit;
    QLineEdit  *svtopEdit;
    QLineEdit  *svdownEdit;
    QLabel   *allChannelLb;
    QSpinBox  * allChanneledit;
    QComboBox   *packMode;//电池PACK模式
    QLabel     *packModeLb;
    QXlsx::Document   * xlsx;
    uchar  startTouch;
   QList<Traymsg *>traymsgList;//托盘信息管理集合
signals:
    void stopSerial(const QString &ip );
    void startSend(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort tpValue);
    void  bedStart();
    void  startRead();
private slots:
    void on_opencloseserial_clicked();//联机OCV切换板
    void disconnectSerial(const QString &ip);//串口掉线显示处理
    void timeoutEventMethod(void); //定时超时方法
    void on_senddata_clicked(void);   //发送
    void trayInit(const QString & ip,const uchar traynum,const uchar flag);//初始化托盘
    void continus_senddata_clicked(void);  //连续切换
    void stopSwitchMethod(void);//停止切换
    void   slotMsgOut(void);  //测量数据导出
    void   excelInit(void);  //表格初始化
     void  slotUpdateCmdData(const uchar cmd,const uchar traynum,const QString &str);//更新界面数据
    void   slotStartSwitch();

};

#endif // OCVNEEDLE_H
