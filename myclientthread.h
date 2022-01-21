#ifndef MYCLIENTTHREAD_H
#define MYCLIENTTHREAD_H
#include <QThread>
#include <Qtimer>
#include "communication/myclient.h"
#include <QObject>
#include "communication/mysocket.h"
class Socket;
class   Myclient;
class  MySocket;
class Myclientthread : public QThread
{
    Q_OBJECT
public:
    explicit Myclientthread(int sockDesc, QObject *parent = nullptr);
    ~Myclientthread();

signals:

public slots:
private:

       void run(void);
    MySocket* Ajlensocket;

        QTimer  *Ajlentimer;

      int m_sockDesc;
};

#endif // MYCLIENTTHREAD_H
