#ifndef DATABROWSE_H
#define DATABROWSE_H
#include <QTextBrowser>
#include  <QComboBox>
#include   <QLabel>
#include <QWidget>
#include  <QVBoxLayout>
#include  <QHBoxLayout>
#include <QGroupBox>
#include "needlebed/needlebed.h"
#include  <QMessageBox>
#include <QDateTime>
class dataBrowse : public QWidget
{
    Q_OBJECT
public:
    explicit dataBrowse(QWidget *parent = nullptr);



signals:

public slots:
          void slotUpdateCmdData(QVariant variant);//更新界面数据
          void  slotClearScreen();
          void  slotSaveData();
          void  slotChooseMachine();

private:
       QTextEdit  *communication;  //数据显示区
       QLabel  *typeLab;
       QComboBox  *machineType; //选择正在通信的设备
       QVBoxLayout  *mainlayout;
        QPushButton  *clearScreen;  //清除显示
        QPushButton  *saveData;//保存数据
        uchar  data[200];
        ushort  bufLen;

};

#endif // DATABROWSE_H
