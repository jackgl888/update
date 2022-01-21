#ifndef COMBDELEGATE_H
#define COMBDELEGATE_H
#include <QItemDelegate>
#include <QAbstractItemModel>
#include <QComboBox>
#include "QSpinBox"
#include<QStandardItemModel>
#include "needlebed/needlebed.h"




class combdelegate : public QItemDelegate
{
   public:
        combdelegate(QObject  * parent = 0);
        ~combdelegate();
        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        void  setEditorData(QWidget *editor,const  QModelIndex &index)const;
        void  setModelData(QWidget *editor,QAbstractItemModel*model,  const  QModelIndex &index)const;
        void  updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,  const  QModelIndex &index)const;
        void  visitNeedlBed(void);
   private:




};

#endif // COMBDELEGATE_H
