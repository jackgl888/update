#ifndef MYSERIAL_H
#define MYSERIAL_H
#include<QtSerialPort/qserialport.h>
#include<QtSerialPort/qserialportinfo.h>
#include<QTimer>
#include<QMutexLocker>
#include<QTreeWidgetItem>
#include "single/single.h"
#include "tempreture.h"
#include "struct.h"
#define   RX_BUFF_LEN    1000
#define   MAX_SEND_SIZE   1024
#define   SERIAL_NUM    10

#define  WAIT_HEAD    0
#define  WAIT_LEN  0x01
#define  WAIT_MSG  0x02

#define   MAX_STOREROOM_MODULE   52
#define   MAX_TEMP_MODULE         4

class Myserial : public QSerialPort
{
    Q_OBJECT
public:
    explicit Myserial(QObject *parent = Q_NULLPTR);
    ~Myserial();
    
    void Init_serial(void);

signals:
    
    void  serialDiscnct(const QString &ip);
    void  startRead(void);  //读取电压
    void  updateBedUi(const uchar cmd,const uchar traynum,const QString  &str);  //更新bed  ui
    void  sig(QVariant);  //线程传数据给UI
    void  sigTempBack(QVariant);  //返回温度

private  slots:
    void  recvData(void);
    void  sendData(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value);
    void  startNextSend(const QString &ip,const uchar cmd);
    void  partlySendData(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value);
    void  modbusSendData(const QString  &ip, const  int traynum,const   uchar  cmd,ushort num,ushort value);//modbus发送
    void  ocvSwitchSendData(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value);//ocv切换发送数据
    void  ocvActionSendData(const QString  &ip, const  int traynum,const   uchar  cmd);//ocv动作发送数据
    void  needlebedSendData(const QString  &ip, const  int traynum,const   uchar  cmd);//针床数据发送
    void  frockSendData(const QString  &ip, const  int traynum,const   uchar  cmd);//拔吸嘴工装数据发送
    void tempSendData(const QString  &ip, const  int traynum,const   uchar  cmd); //温度工装数据发送
    void ohmmeterSendData(const QString  &ip, const  int traynum,const   uchar  cmd);//读取值值
    void pressFrockSendData(const QString  &ip, const  int traynum,const   uchar  cmd);//压力工装数据发送
    void  timeoutMethod(void);
    void  stopSerialSlot(const QString &ip);
    void  recvDataMethod(const QString &ip, const uchar *data);  //协议解析
    void  recvModbusData(const QString &ip, const uchar *data); //modus温度数据处理
    void  recvOcvSwitchData(const QString &ip, const uchar *data);   //Ocv数据处理
    void  recvOcvActionData(const QString &ip, const uchar *data);   //OCV动作
    void  recvNeedlebedData(const QString &ip, const uchar *data);//针床数据处理
    void  recvFrockData(const QString &ip, const uchar *data);//拔吸嘴工装数据处理
    void recvTempData(const QString &ip, const uchar *data); //温度工装数据处理
    void recvOhmmeterData(const QString &ip, const uchar *data);//阻值表
    void recvPressData(const QString &ip, const uchar *data); //压力表
    void  slotSendMMIDataTimeOut(void);//超时重发函数数
    void  startSwitchSlot(void);
    void  slotGetData(QVariant variant);
    void sendAdjustCode( uchar traynum,float * value);   //下发修调码
    void sendClearCodeSlot(uchar traynum);   //下发清零修调码

private:

    QStringList  list;
    QMutex   mutex,mutex2;   //创建一个互斥量
    uchar  sending_buf[1200];//当前发送包
    uchar serialRecv[500];  //接收数据缓存
    QTimer  *serialTimer; //
    QTimer  *resendTimer; //监视数据发送
    uchar    sendCnt[10]; //命令互斥时计时使用
    uchar    resendCnt[10]; //串口重发送计数 大于3次，发送失败
    uchar    cmdSending[10]; //
    uchar    traynumSinding; //
    const  uchar    *resendData;
    uint recv_cnt;		  //当前通讯口第几个字符   三个通讯口串口1 2 3
    uchar head_cnt;
    uint length;
    uchar wait;
    uchar  modbusType;   //modbus协议类型    1 欧姆龙   0  三菱
    ushort  tpChannels;  //通道总数
    uchar  startSample;  //采样开始标志
    uchar  tempSlaves;  //温度采样板从站个数
    uchar  startTempAdjust;  //校准开关
    uchar  adjustTempTraynum;   //要校准的温度板
    ushort   standedTempValue;   //基准值
    float  adjustTemp[100];  //用于校准
       float  adjustCode[100];  //用于校准
};


#endif // Myserial_H
