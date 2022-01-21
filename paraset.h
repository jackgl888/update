#ifndef PARASET_H
#define PARASET_H
#include <QDialog>
#include <QComboBox>
#include  <QHBoxLayout>
#include  <QLabel>
#include  <QTimer>
#include  "single/single.h"
class paraset : public QDialog
{
public:
   explicit paraset(QWidget *parent = 0);
    ~paraset();
private:

    QLabel *frokLable;
    QComboBox  * frokbox;
    QHBoxLayout  *froklayout;
    QTimer  *m_timer;
 private   slots:
    void timeoutMethod();

};

#endif // PARASET_H
