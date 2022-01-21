#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H
#include <QDialog>

class customDialog :public QDialog
{
    Q_OBJECT
public:
   explicit  customDialog(QWidget *parent = NULL);
          ~customDialog();

protected:

private slots:
      void  mouseDoubleClickEvent(QMouseEvent *event);
      void paintEvent(QPaintEvent  *event);
private:

};

#endif // CUSTOMDIALOG_H
