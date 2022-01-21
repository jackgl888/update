#include "customdialog.h"
#include <QPainter>
#include <QPixmap>

customDialog::customDialog(QWidget *parent):QDialog(parent)
{

}

customDialog::~customDialog()
{

}



void customDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
     this->hide();
}

void customDialog::paintEvent(QPaintEvent *event)
{

   QPainter  painter(this);
   painter.setRenderHint(QPainter::Antialiasing);
   QPixmap  img(":/pic/T.png");

   painter.drawPixmap(0,0,img.width(),img.height(),img);

   this->setFixedSize(img.width(),img.height());
}
