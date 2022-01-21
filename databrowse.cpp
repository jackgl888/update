#include "databrowse.h"

dataBrowse::dataBrowse(QWidget *parent) : QWidget(parent)
{


    this->clearScreen   = new QPushButton(tr("清除显示"),this);
    this->saveData   = new  QPushButton(tr("保存数据"),this);
    this->communication   = new  QTextEdit(this);
    this->communication->setReadOnly(true);

    this->typeLab  = new  QLabel(tr("设备选择:"),this);
    this->machineType = new QComboBox(this);
    QStringList  list;
    list <<"针床"<<"OCV切换"<<"安捷伦"<<"温度采样板"<<"温度校准工装";
    this->machineType->addItems(list);
    QGroupBox   *functionBox = new  QGroupBox(this);
    QHBoxLayout  *machine = new QHBoxLayout();
    this->mainlayout = new QVBoxLayout(this);
    machine->addStretch(200);
    machine->addWidget(this->typeLab);
    machine->addWidget(this->machineType);
    machine->addWidget(this->clearScreen);
    machine->addWidget(this->saveData);
    functionBox->setLayout(machine);
    this->mainlayout->addWidget(this->communication);
    this->mainlayout->addWidget(functionBox);

    connect(this->clearScreen,SIGNAL(clicked()),this,SLOT(slotClearScreen())); //清除数据
    connect(this->saveData,SIGNAL(clicked()),this,SLOT(slotSaveData())); //保存数据
    connect(this->machineType,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotChooseMachine())); //选择显示


}





//显示发送与接收的字节
void dataBrowse::slotUpdateCmdData(QVariant variant)
{

    QString    strData,text ;

   bedPara m_para = variant.value<bedPara>();

   memcpy(this->data,m_para.bufData,200);
     this->bufLen = m_para.dataLen;

   for(uchar i=0;i<this->bufLen;i++)
   {

     //  strData .append(QString::number(*(data+i),16).toUpper());


       strData .append(QString("%1").arg((*(data+i))&0xFF,2,16,QLatin1Char('0')).toUpper());

       strData.append(" ");
   }
   text = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ")+strData+"\r\n";
   this->communication->append(text );

}


//
void dataBrowse::slotClearScreen()
{
    this->communication->clear();
}

void dataBrowse::slotSaveData()
{

    QFile  myfile("C:/Users/Administrator/Desktop/runMsg.txt");//创建一个输出文件的文档
    if (myfile.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text))//注意WriteOnly是往文本中写入的时候用，ReadOnly是在读文本中内容的时候用，Truncate表示将原来文件中的内容清空
    {
        // ui->runnigMsg->setPlainText("OK");
        //读取之前setPlainText的内容，或直接输出字符串内容QObject::tr()
        QTextStream out(&myfile);

        out<<this->communication->toPlainText();   //  ->acceptRichText()<<endl;

        QMessageBox* pInfor =new QMessageBox(this);
        pInfor->setText("信息导出成功！");
        pInfor->setWindowTitle("");
        pInfor->show();

    }
}

void dataBrowse::slotChooseMachine()
{
    if(this->machineType->currentText() == "针床")
    {
        Single::m_Instance()->sendReceiveBs = 0x01;
    }
    else if(this->machineType->currentText() == "OCV切换")
    {
          Single::m_Instance()->sendReceiveBs = 0x03;
    }
    else if(this->machineType->currentText() == "安捷伦")
    {
          Single::m_Instance()->sendReceiveBs = 0x04;
    }
    else if(this->machineType->currentText() == "温度采样板")
    {

    }else
    {

    }
}




