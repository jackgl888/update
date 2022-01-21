#include "commnunicationset.h"

commnunicationSet::commnunicationSet(QWidget *parent):QDialog(parent)
{


    this->save  = new QPushButton("保存",this);

    QLabel      *comname = new QLabel("串口名",this);
    QLabel      *boundratechs = new QLabel("波特率",this);
  boundrate = new QComboBox(this);
     serialname  = new  QComboBox(this);
    QStringList  list;
    list<<"115200"<<"57600"<<"38400"<<"19200"<<"9600";
    boundrate->addItems(list);
    list.clear();
    list<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<"COM6"<<"COM7"<<"COM8"<<
          "COM9"<<"COM10"<<"COM11"<<"COM12"<<"COM13"<<"COM14"<<"COM15"<<"COM16";
    serialname->addItems(list);
    QHBoxLayout     *comnameLayout = new QHBoxLayout();
    comnameLayout->addWidget(comname);
    comnameLayout->addWidget(serialname);
    QHBoxLayout     * boundrateLayout = new QHBoxLayout();
    boundrateLayout ->addWidget(boundratechs);
    boundrateLayout ->addWidget(boundrate );

    QVBoxLayout  *serialLayout = new QVBoxLayout ();
    serialLayout ->addLayout(comnameLayout);
    serialLayout ->addLayout( boundrateLayout);
    QGroupBox  * serialBox = new  QGroupBox(this);
    serialBox->setLayout(serialLayout);

      serialBox->setTitle("串口");
    QLabel  *netport = new QLabel("端口号",this);   //端口号
   m_port = new QSpinBox(this);
    m_port->setMaximum(10000);
    m_port->setValue(6000);
    QLabel  *ipaddrlb= new QLabel("本机IP");  //
   ipBox = new QComboBox(this);

    QHBoxLayout  *netportLayout = new QHBoxLayout();
    netportLayout->addWidget(netport);
    netportLayout->addWidget(m_port);
    QHBoxLayout  *ipLayout  =new QHBoxLayout();
    ipLayout->addWidget(ipaddrlb);
    ipLayout->addWidget(ipBox);
    QVBoxLayout  * netLayout = new QVBoxLayout();
    netLayout->addLayout(netportLayout);
    netLayout->addLayout(ipLayout);

    QGroupBox    *netBox= new QGroupBox(this);
    netBox->setTitle("网口");
    netBox->setLayout(netLayout);

    this->cfgType  = new QComboBox(this);
    list.clear();
    list<<"串口配置"<<"网口配置";
    cfgType->addItems(list);
    QHBoxLayout * btnLayout = new QHBoxLayout();
    btnLayout->addWidget(cfgType);
    btnLayout->addStretch();
    btnLayout->addWidget(this->save);
    QVBoxLayout * comLayout = new QVBoxLayout(this);
    comLayout->addWidget(serialBox);
    comLayout->addWidget(netBox);
    comLayout->addLayout(btnLayout);
    getLocalHostIp();
   this->setWindowTitle("Configuration network port or serial port");
    this->resize(QSize(550,300));


    connect(this->save,SIGNAL(clicked()),this,SLOT(btnslot())); //把配置信息返回去
     this->show();
}

commnunicationSet::~commnunicationSet()
{

}

void commnunicationSet::btnslot()
{
    QStringList  list;
    list.clear();
    if(cfgType->currentText()=="串口配置")
    {
           list.append(serialname->currentText());
           list.append(boundrate->currentText());
           emit  sendBackComCfg(list);
           this->close();
    }
    else
    {
        ;
    }
}
void commnunicationSet::getLocalHostIp()
{

    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    foreach(QHostAddress address,info.addresses())

    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)

            Single::m_Instance()->insertIplist(address.toString());

    }

      ipBox->addItems(Single::m_Instance()->getIplist());

}
