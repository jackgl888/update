#ifndef TRAYMSG_H
#define TRAYMSG_H
#include <QTreeWidgetItem>

#include <QObject>

class Single;

class Traymsg : public QObject
{
    Q_OBJECT
public:
    explicit Traymsg(QString  tip,  uchar tnum,  QTreeWidgetItem * titem,  QObject *parent = nullptr);
    ~Traymsg();

    //读托盘状态
    QString  getIp(void);
    QString  getMsg(void);
    uchar    getTraynum(void);
    bool     getSdflag(void);
    uchar    getRsdnum(void);
    int      getSdnum(void);
    uint     getProgvalue(void);
    bool     getStaChoose(void);
    bool     getStaHiden(void);
    bool     getItemchange(void);
    bool     getParentSta(void);
    QTreeWidgetItem *  getItem(void);
    //写托盘状态
    void     writeMsg(QString  tmsg);
    void     writeSdflag(bool tsdflag );
    void     writeRsdnum(uchar trsdnum);
    void     writeSdnum(   int tsdnum);
    void     writeProgvalue(uint tprogvalue);
    void     writeStaChoose(bool tstachoose);
    void     writeItemchange(bool  ischange);
    void     writeStaHiden(bool  hide);
    void     writePrentSta(bool  check);

signals:

private:
    QString  ip;       //  父节点名
    uchar    traynum;  //托盘号
    bool    sdflag;  //托盘当前发送状态  1发送中  0发送完成
    int      sdnum;   //包发送数量   升级成功或者升级数据发送失败值为-1  联机与运行重发送失败为-1
    uchar    rsdnum;   //重发次数
    QString  msg;    //托盘运行信息
    uint     progvalue;  //托盘升级进度条值
    bool     staChoose;  //ui选中状态
    bool     hiden;     //1.隐藏  0非隐藏
    bool     itemchange;   //是否变化
    bool     parentItemUchecked;  //隐藏主节点  1隐藏  0不隐藏
    QTreeWidgetItem * item;  //主节点



public slots:


};

#endif // TRAYMSG_H
