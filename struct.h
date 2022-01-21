#ifndef STRUCT_H
#define STRUCT_H

#include<QMetaType>


struct tempPara
{
    ushort  all_chanels ;
    uchar    slaveNum;   //从站总数
    uchar    slaveId;
    uchar   protocol;   //协议类型   1  欧姆龙   0  三菱
    float    tempValue[600];  //所有通道温度值

};

struct   valuetp
{
          uchar version;
          uchar  tpId;
          uchar  adjustFlag;
          float adjustCode[200];
          float  tpValues[600];  //温度值
};

struct  bedPara
{
        uchar bedSensor[10];  //传感器
        float bedTemp[100];  //库位温度
        float negAir;
        uchar bufData[200];//数据接收与发送缓存

        ushort  dataLen;  //数据长度
};

Q_DECLARE_METATYPE(bedPara)
Q_DECLARE_METATYPE(valuetp)

Q_DECLARE_METATYPE(tempPara)


#endif // STRUCT_H
