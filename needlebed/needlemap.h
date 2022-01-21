#ifndef NEEDLEMAP_H
#define NEEDLEMAP_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QMenuBar>
#include<QTimer>
#include "single/single.h"
#include <QPushButton>
#include <QDialog>
#include  "paraset.h"
#include <QLineEdit>
#include <QComboBox>
#include "needlebed.h"
#include  <QHBoxLayout>
class  needlebed;

class NeedleMap : public QMainWindow
{
    Q_OBJECT
public:
    explicit NeedleMap(QWidget *parent = nullptr);
    ~NeedleMap();
    void   initScene();            			//初始化场景
    void   createActions();                    	//创建主窗体的所有动作
    void   createMenus();                       	//创建主窗体的菜单栏

signals:

public slots:
    void timeoutEventMethod();    //定时器刷新界面
    void reflashTray1();
    void reflashTray2();
    void reflashTray3();
    void reflashTray4();
    void slotParaSet();
    void updateMapSlot( const uchar cmd,const uchar traynum,const uchar *data);
    void  slotUiUpdate(QVariant variant);   //接收工作线程传过来的数据
private:

    uchar  traybuf[30];  //库位传感器
    QTimer * m_timer;
    QPushButton  *button;
    QAction  *tray1;
    QAction  *tray2;
    QAction  *tray3;
    QAction  *tray4;
    uchar  trayNum;
    //  QAction  *setFrokPs;
    QMenu *trayChoose;
    //   QMenu *paraSet;  //参数设置
    QGraphicsScene *scene;
    QGraphicsView *view ;
    QGraphicsTextItem *trayName1;
    QGraphicsTextItem *trayName2;
    QGraphicsTextItem *trayName3;
    QGraphicsTextItem *trayName4;
    QGraphicsRectItem *C_item ;   //总库位平面
    QGraphicsRectItem *T_item ;     //托盘平面
    QGraphicsRectItem *frontSensor ;     //前对射检测
    QGraphicsEllipseItem  *f_smog;    //前烟感
    QGraphicsEllipseItem  *b_smog;    //后烟感
    QGraphicsEllipseItem  *f_tray;   //前托盘微动
    QGraphicsEllipseItem  *b_tray; //后托盘微动
    QGraphicsRectItem *backMotorCheckLeft ;  //电机负向运动超程检测左
    QGraphicsRectItem *originMotorCheckLeft ;  //原点负向运动超程检测左
    QGraphicsRectItem *forwardMotorCheckLeft ;  //正向负向运动超程检测左
    QGraphicsRectItem *backMotorCheckRight ;  //电机负向运动超程检测右
    QGraphicsRectItem *originMotorCheckRight;  //原点负向运动超程检测右
    QGraphicsRectItem *forwardMotorCheckRight;  //正向负向运动超程检测右
    QGraphicsRectItem *backCylinderRight;  //后气缸检测右
    QGraphicsRectItem *forwardCylinderRight;  //前气缸检测检测右

    QList<QGraphicsItem*> listItem ;
    ushort   bedTemp[100];  //库位温度包
    ushort   negpressValue;  //负压值
    uchar   locTemps;  //库位温度数
    ushort  bedChannels; //针床总的通道数

    //  paraset   *m_paraset;

};




#endif // NEEDLEMAP_H
