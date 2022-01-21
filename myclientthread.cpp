#include "myclientthread.h"

Myclientthread::Myclientthread(int sockDesc,QObject *parent) : QThread(parent),
    m_sockDesc(sockDesc)
{

}



Myclientthread::~Myclientthread()
{

}



void Myclientthread:: run(void)
{
       // this->Ajlentimer   =  new QTimer();
      //  this->Ajlentimer->moveToThread(this);
       //this->Ajlensocket   =new  Myclient( m_sockDesc);
//    if (! this->Ajlensocket ->setSocketDescriptor( m_sockDesc)) {
//        return ;
//    }
      this->Ajlensocket  = new MySocket( m_sockDesc);
//     if (! this->Ajlensocket ->setSocketDescriptor( m_sockDesc)) {
//         return ;
//     }

     this->Ajlensocket->connectToHost("192.168.0.10",5024);
     this->Ajlensocket->moveToThread(this);
     this->exec();
}

