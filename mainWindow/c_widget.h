#ifndef C_WIDGET_H
#define C_WIDGET_H
#include<QSplitter>
#include <QListWidget>
#include<QGridLayout>
#include <QWidget>
#include "content.h"

namespace Ui {
class C_widget;
}
class C_widget : public QWidget
{
    Q_OBJECT
public:
    explicit C_widget(QWidget *parent = nullptr);
    ~C_widget();

private:


//   QComboBox      *  serialname;  //串口名
//    QLabel        *comname;



    QSplitter  *splitterMain;
    QListWidget *list;
    content  *m_content;

        QTimer   *m_timer;

signals:


public slots:
    //void timeoutMethod(void);  //定时器管理串口


};

#endif // C_WIDGET_H
