#ifndef ADJUSTTEMP_H
#define ADJUSTTEMP_H

#include <QMainWindow>
#include <QMenuBar>
#include  <QTableView>
#include <QStandardItemModel>
#include<QHeaderView>
#include  <QDockWidget>
#include <QTextBrowser>
#include  <QTreeWidget>
#include <QTreeWidgetItem>
#include   "commnunicationset.h"
#include   "adjustsetdialog.h"
#include "communication/serverthread.h"
#include  <QTreeView>
class adjustTemp : public QMainWindow
{
    Q_OBJECT
public:
    explicit adjustTemp(QWidget *parent = nullptr);
    ~adjustTemp();

    QTreeWidgetItem *  AddTreeRoot(QString name,QString desc);
    QTreeWidgetItem *  AddTreeNode(QTreeWidgetItem *parent,QString name,QString desc);
    
    void   createActions();                    	//创建主窗体的所有动作
    void   createMenus();                   //创建主窗体的菜单栏
    void   tempViewInit();
    void   msgBrowserInit();
    void   tempTreeInit();    //节点树
    void   trayWidgetInit(const QString & ip,const uchar traynum);   //创建树形节点
    void   tempNodeModleInit(QString &ip, uchar  trayNum);  //节点modle初始化
signals:
    void  stopSerial(const QString &ip );
    void  startSend(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value);
    void  updateMap( const uchar cmd,const uchar traynum,const uchar *data);  //发送信号更新map
    void  sig(QVariant para);
public slots:

private  slots:
    void communicationSetupSlot();  //通信配置
    void calibrationSetupSlot();  //校准配置
    void openCalibrationCodeSlot();  //get code
    void  getCommunicationSetSlot(QStringList &list);   //获得通信配置
    void getCalibrationSetSlot(QList<int >list);
    void   connectTargetSlot();              //联机
    void  slotUiUpdate(QVariant variant);   //接收工作线程传过来的数据
    void  tempTreeRowClicked(QModelIndex  index);// 节点点击响应

private:

 
    QAction  *adjustCodeOpen;
    QAction  *adjustCodeSave;
    QAction  *adjustCodeSaveAs;
    QAction *adjustCodeQuit;
    QAction  *comSetup;   //通信配置
    QAction  *adjustSetup;   //校准配置
    QAction  *adjustTarget;   //联机
    QAction  *codeClear;   //清零校准
    QAction  *adjustStart;   //开始校准
    QMenu *adjustCodeFile;   //文件
    QMenu *adjustCodeEdit;   //编辑
    QMenu *adjustCodeView;   //视图
    QMenu *adjustCodeCnct;   //联机
    QMenu *adjustCodeSetup;   //配置
    QMenu *adjustCodeFunc;   //功能
    QMenu *adjustCodeWin;   //窗口
    QMenu *adjustCodeHelp;   //帮助
    QTableView      *tempView;   //显示所有通道温度
    QStandardItemModel   *tempModle;  //数据模模型
    QDockWidget  *browserWidget;   //消息打印
    QDockWidget  *tempTreeDock;   //温度节点树
    QTextBrowser  *msgBrowser;
    QTreeView   *tempTree; //节点树
    QStandardItemModel    *tempNodeModle;   //温度板NODE数据模型
    serverThread  *m_thread;
    QList<int >adjustSetList;  //校准配置   从站地址  从站总数  总通道数  理论温度值
    uchar startSp;
    QList<Traymsg *>traymsgList;//托盘信息管理集合

};

#endif // ADJUSTTEMP_H
