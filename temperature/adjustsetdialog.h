#ifndef ADJUSTSETDIALOG_H
#define ADJUSTSETDIALOG_H
#include <QDialog>
#include <QObject>
#include <QGroupBox>
#include  <QLabel>
#include <QComboBox>
#include  <QHBoxLayout>
#include <QVBoxLayout>
#include  <QPushButton>
#include <QSpinBox>
class adjustSetDialog : public QDialog
{
    Q_OBJECT
public:
    explicit adjustSetDialog(QWidget *parent = 0);
        ~adjustSetDialog();
signals:
      void   sigCalibrationSet(QList<int> );
public slots:

private  slots:
        void btnSlot();
private:

    QPushButton  * save   ;
    QLabel   * slaveAddrLab ; //从站地址
    QSpinBox  *slaveAddr;
    QLabel    *slaveNumLab; //从站总数
    QSpinBox  *slaveNum;
    QLabel   * tpclChannelsLab; //温控通道数
    QSpinBox  *tpclChannels;  //温控通道数

    QSpinBox *  tempAdjustBox;

     QSpinBox   *allChannels;
     QLabel   *allChannelsLab;
};

#endif // ADJUSTSETDIALOG_H
