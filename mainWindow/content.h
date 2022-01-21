#ifndef CONTENT_H
#define CONTENT_H
#include<QStackedWidget>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include <QDialog>
#include<QFrame>
#include<QWidget>
#include "ocvneedlebed/ocvneedle.h"
#include "tempreture.h"
#include  "needlebed/needlebed.h"
#include"needlebed/needlemap.h"
#include <QTimer>
#include  <QComboBox>
#include "databrowse.h"
#include "temperature/adjusttemp.h"

//class  dataBrowse;

class content : public QFrame
{
    Q_OBJECT

public:
    content(QWidget *parent = 0);
    ~content();
    QStackedWidget  *stack;

private  slots:


private:


    ocvneedle  * m_ocvneedle;

    tempreture *m_temp;
    needlebed * m_bed;

    NeedleMap  * m_map;
    dataBrowse  *m_browse;
    adjustTemp *m_adjustTemp;
};

#endif // CONTENT_H
