#include "paraset.h"


paraset::paraset(QWidget *parent):QDialog(parent)
{
    QStringList  list;
    list<<"是"<<"否";
     this->m_timer  = new QTimer(this);
    this->frokLable  = new QLabel(tr("工装取电:"));
    this->frokbox     = new QComboBox(this);
    this->frokbox->addItems( list);
    this->froklayout = new QHBoxLayout(this);
    this->froklayout->addWidget(this->frokLable);
    this->froklayout->addWidget(this->frokbox);
    this->setModal(true);
    this->resize(100,100);
   this->show();
    connect(this->m_timer,SIGNAL(timeout()),this,SLOT(timeoutMethod()));
}

paraset::~paraset()
{

}


void paraset::timeoutMethod()
{
    ;
}
