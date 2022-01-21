#include "adjustsetdialog.h"

adjustSetDialog::adjustSetDialog(QWidget *parent):QDialog(parent)
{



    this->slaveAddrLab  = new QLabel(tr("从站地址:"),this);
    this->slaveAddr  = new QSpinBox(this);
    this->slaveAddr->setMinimum(0);
    this->slaveAddr->setMaximum(245);
    this->slaveNumLab = new QLabel(tr("从站总数:"),this);
    this->slaveNum  = new QSpinBox(this);
    this->slaveNum->setMaximum(245);


    QHBoxLayout  *slaveAddrLayout = new QHBoxLayout();
    slaveAddrLayout->addWidget(slaveAddrLab);
    slaveAddrLayout->addWidget(slaveAddr);

    QHBoxLayout  *slaveNumLayout = new QHBoxLayout();
    slaveAddrLayout->addWidget(slaveNumLab);
    slaveAddrLayout->addWidget(slaveNum);


    this->allChannels  = new QSpinBox(this);  //总通道数
    this->allChannelsLab  = new QLabel(tr("通道总数"),this);
   this->allChannels->setMaximum(256);
    QHBoxLayout  *allChannelsLayout = new QHBoxLayout();
    slaveAddrLayout->addWidget(allChannelsLab);
    slaveAddrLayout->addWidget(allChannels);


    //校准温度输入
    QLabel *adjustTempLab  = new QLabel(tr("仪器温度:"),this);
    this->tempAdjustBox   = new  QSpinBox(this);
    tempAdjustBox->setMinimum(0);
    tempAdjustBox->setMaximum(800);
    QHBoxLayout  *adjustLayout = new QHBoxLayout();
    adjustLayout->addWidget(adjustTempLab);
    adjustLayout->addWidget(tempAdjustBox);


    save   = new QPushButton ("保存",this);
     //save->setFixedSize(70,50);
    QHBoxLayout  * btnLayout =  new QHBoxLayout();
   btnLayout->addStretch();
    btnLayout->addWidget(save);
    QVBoxLayout   *adjustSetLayout = new QVBoxLayout(this);
    adjustSetLayout->addLayout(slaveAddrLayout);
    adjustSetLayout->addLayout(slaveNumLayout);
    adjustSetLayout->addLayout(allChannelsLayout);
    adjustSetLayout->addLayout(adjustLayout);
    adjustSetLayout->addWidget(save);

    connect(this->save,SIGNAL(clicked()),this,SLOT(btnSlot()));
    this->setWindowTitle("Temperature calibration configuration");
    this->resize(QSize(200,100));
    this->show();
   
}

adjustSetDialog::~adjustSetDialog()
{
    
}

void adjustSetDialog::btnSlot()
{
    QList <int >setList;
    setList.append(slaveAddr->value());//从站地址
    setList.append(slaveNum->value());  //从站总数
    setList.append(allChannels->value()); //总通道数
    setList.append(tempAdjustBox->value());  //理论温度

    emit  sigCalibrationSet(setList);

    this->close();

}







