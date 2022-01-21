#ifndef MYANIMATIONBTN_H
#define MYANIMATIONBTN_H

#include <QWidget>
#include "customdialog.h"
#include <QPropertyAnimation>
#include  <QMouseEvent>
#include <QPainter>
#include <QDebug>
class QPropertyAnimation;

class myAnimationBtn : public QWidget
{
    Q_OBJECT
public:
    explicit myAnimationBtn(QWidget *parent = nullptr);
    ~myAnimationBtn();

    void enterImageChanged(QVariant value);
    void leaveImageChanged(QVariant value);

    void  setImagePath(QString path);
    void  setText(QString text){this->text = text;}

signals:

protected:
    void   enterEvent(QEvent *event);
     void  leaveEvent(QEvent *event);
     void  paintEvent(QPaintEvent *event);
      void mousePressEvent(QMouseEvent *event);

public slots:

private:
       int imageWidth;
       int imageHeight;

       int originalWidth;
       int originalHeight;

       int newWidth;
       int newHeight;

       QString imagePath;
       QString text;

       QPropertyAnimation* enterAnimation;
       QPropertyAnimation* leaveAnimation;

       customDialog* dialog;
};

#endif // MYANIMATIONBTN_H
