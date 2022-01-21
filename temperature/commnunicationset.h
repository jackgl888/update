#ifndef COMNNUNICATIONSET_H
#define COMMNUNICATIONSET_H
#include <QObject>
#include <QGroupBox>
#include <QDialog>
#include  <QLabel>
#include <QComboBox>
#include  <QHBoxLayout>
#include <QVBoxLayout>
#include  <QPushButton>
#include <QSpinBox>
#include <QHostAddress>
#include  "single/single.h"
#include <QHostInfo>
class commnunicationSet :public QDialog 
{
    
    Q_OBJECT
public:
    explicit    commnunicationSet(QWidget *parent = 0);
    ~commnunicationSet();
    void getLocalHostIp();
private  slots:
    void  btnslot();

signals:
      void sendBackComCfg(QStringList &);
private:
    QComboBox   *   boundrate ;
    QComboBox   *   serialname  ;
    QComboBox  *cfgType;
    QSpinBox  *m_port;
    QPushButton *save;
    QComboBox *ipBox ;

};

#endif // COMNUNICATIONSET_H
