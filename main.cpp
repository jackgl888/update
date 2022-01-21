#include "mainWindow/c_widget.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{

 //   QString  path  =  QCoreApplication::applicationDirPath() + "//databasefile.db";
   //  qDebug()<<"MAIINthread:"<<QThread::currentThreadId();

    QApplication a(argc, argv);
    QFont font("AR PL KaitiM GB",9);
    a.setFont(font);
 //   Single::m_Instance()->openDatabase( path); //打开数据库
    C_widget w ;
    w.show();
    return a.exec();

}



/******************更新说明*************************

V1.0.1：
更新说明：
1、将温度部分托盘初始化时的工装地址初始化为253；增加工裝溫度采樣下發

V1.0.2：
2020.7.30  更新说明：
1. 温度部分增加支持omron的modbus_rtu协议，可以读取温度显示；
2，针床部分优化温度的显示，增加负压控制与气密性检测。
3.

V1.0.3
2020.9.24
更新说明：
1.增加串口多线程，更改多线程串口接收处理，程序已经做备份，并在备份程序上进行编写

V1.04
2020.11.27
更新说明：
1.修改数据库的数据的插入方法，防止漏插数据。


***************************************************/


































