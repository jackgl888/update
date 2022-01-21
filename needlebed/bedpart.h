#ifndef BEDPART_H
#define BEDPART_H
#include<QLineEdit>
#include<QPushButton>
#include<QLabel>
#include <QWidget>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QGridLayout>
#include  <QComboBox>
#include <QWidget>
#include<QStackedWidget>
#include<QSplitter>
#include <QTableView>
#include <QStandardItemModel>
#include"custom/combdelegate.h"
#include<QTimer>
#include <needlebed/histogramview.h>
#include "single/single.h"
#include"communication/serverthread.h"
#include "communication/myserial.h"
#include "needlebed.h"

class needlebed;
class  combdelegate;

#include <QMutex>
class bedpart : public QWidget
{
    Q_OBJECT
public:
    explicit bedpart(  QWidget *parent = nullptr);
    ~bedpart();
    void trayInit(const QString & ip);       //初始托盘
signals:

private:


    QTableView         *list;
    QTableView         *m_list;
    QGridLayout        * m_layout;
    QHBoxLayout        *windowLayout;
    QStandardItemModel *model ;
    QStandardItemModel * t_model;
    combdelegate       * negAirComb;
    combdelegate       *  negAirValuesp;
    combdelegate       * motorSpin;
    combdelegate        * keepNegAirTime;
   combdelegate         * highNegAirCntl;
   combdelegate          *lowNegAirCntl;
   combdelegate          *breakVacuum;
    HistogramView      *histogram;
     QTimer             *m_timer;
    needlebed           *bedDialog;

    QMutex              mutex;


public slots:

    void dataChangedSlot(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    void timeoutEventMethod();


};



#endif // BEDPART_H
