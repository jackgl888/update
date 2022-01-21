#include "traymsg.h"



Traymsg::Traymsg(QString  tip,  uchar tnum,QTreeWidgetItem * titem, QObject *parent) : QObject(parent)
{

    this->ip = tip;
    this->traynum =tnum;
    this->sdnum = 0;
    this->sdflag = false;
    this->staChoose = false;
    this->hiden = true;
    this->progvalue = 0;
    this->rsdnum = 0;
    this->parentItemUchecked = false;
    this->item = titem;

}



Traymsg::~Traymsg()
{

}





QString Traymsg::getIp()
{
    return  this->ip;
}

QString Traymsg::getMsg()
{
    return  this->msg;
}

uchar Traymsg::getTraynum()
{
    return  this->traynum;
}

bool Traymsg::getSdflag()
{
    return  this->sdflag;
}

uchar Traymsg::getRsdnum()
{
    return  this->rsdnum;
}

int Traymsg::getSdnum()
{
    return  this->sdnum;
}

uint Traymsg::getProgvalue()
{
    return  this->progvalue;
}

bool Traymsg::getStaChoose()
{
    return  this->staChoose;
}


bool Traymsg::getStaHiden()
{
    return  this->hiden;
}

bool Traymsg::getItemchange()
{
    return  this->itemchange;
}

bool Traymsg::getParentSta()
{
    return   this->parentItemUchecked;
}

QTreeWidgetItem *Traymsg::getItem()
{
   return  this->item;
}



void Traymsg::writeMsg(QString tmsg)
{
    this->msg =tmsg;
}



void Traymsg::writeSdflag(bool tsdflag)
{
     this->sdflag = tsdflag;
}

void Traymsg::writeRsdnum(uchar trsdnum)
{
      this->rsdnum  = trsdnum;
}

void Traymsg::writeSdnum(int tsdnum)
{
    this->sdnum = tsdnum;
}

void Traymsg::writeProgvalue(uint tprogvalue)
{
    this->progvalue  =tprogvalue;
}

void Traymsg::writeStaChoose(bool tstachoose)
{
    this->staChoose  =tstachoose;
}

void Traymsg::writeItemchange(bool ischange)
{
    this->itemchange  = ischange;
}

void Traymsg::writeStaHiden(bool hide)
{
    this->hiden = hide;
}

void Traymsg::writePrentSta(bool check)
{
    this->parentItemUchecked   =check;
}

