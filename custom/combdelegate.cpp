#include "combdelegate.h"



combdelegate::combdelegate(QObject *parent):QItemDelegate(parent)
{


}

combdelegate::~combdelegate()
{

}

QWidget *combdelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(index.column()==1)
   {
          QComboBox * editor  = new QComboBox(parent);
          editor->addItem("软包");
          editor->addItem("方型");
          editor->addItem("圆柱");
          editor->installEventFilter(const_cast<combdelegate*>(this));
          return  editor;
    }
    else if(index.column() == 2)
     {
        QSpinBox  * negAirValue  = new QSpinBox(parent);
        negAirValue->setMaximum(5000);
        negAirValue->installEventFilter(const_cast<combdelegate*>(this));
        return  negAirValue;
     }
    else if(index.column()==3)
    {
        QSpinBox  * motorEditor = new QSpinBox(parent);
        motorEditor->setMaximum(15000);
        motorEditor->installEventFilter(const_cast<combdelegate*>(this));
        return  motorEditor;
    }
    else if(index.column()==6)  //保压时间
    {
        QSpinBox  *editor =  new QSpinBox(parent);
        editor->setMaximum(60000);
        editor->installEventFilter(const_cast<combdelegate*>(this));
        return  editor;
    }
    else if(index.column() == 7)//高负压控制
    {
         QComboBox  *editor   =   new QComboBox(parent);
         editor ->addItem("打开");
         editor->addItem("关闭");
         editor->installEventFilter(const_cast<combdelegate*>(this));
         return  editor;
    }
    else if(index.column() == 8) //低负压控制
     {
          QComboBox   *editor = new QComboBox(parent);
          editor ->addItem("打开");
          editor ->addItem("关闭");
           editor->installEventFilter(const_cast<combdelegate*>(this));
          return editor;
    }
    else  if(index.column() == 9)//破真空控制
    {
          QComboBox   *editor  = new QComboBox(parent);
          editor ->addItem("打开");
          editor ->addItem("关闭");
          editor->installEventFilter(const_cast<combdelegate *>(this));
          return  editor;

    }
    return QItemDelegate::createEditor(parent, option, index);

}

//初始化代理
void combdelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    if(index.column()==1)
   {
        QString  str  = index.model()->data(index).toString();
        QComboBox  *box = static_cast <QComboBox *>(editor);
        int i = box->findText(str);
        box->setCurrentIndex(i);
    }
    else if(index.column()==2)
     {
         uint num = index.model()->data(index).toUInt();
         QSpinBox  *box = static_cast < QSpinBox *>(editor);
         box->setValue(num );

     }
   else if(index.column()==3)
    {
        uint num = index.model()->data(index).toUInt();
        QSpinBox  *box = static_cast < QSpinBox *>(editor);
        box->setValue(num );
      
    }
    else if(index.column()==6)
    {
        uint  num = index.model()->data(index).toUInt();
        QSpinBox  *box = static_cast<QSpinBox *>(editor);
        box->setValue(num);
    }
    else if(index.column()==7)
    {
        QString  str = index.model()->data(index).toString();
        QComboBox  *box = static_cast<QComboBox *>(editor);
        uint i = box->findText(str);
        box->setCurrentIndex(i);
    }
    else if(index.column() == 8)
    {
        QString  str = index.model()->data(index).toString();
        QComboBox  *box = static_cast<QComboBox *>(editor);
        uint i = box->findText(str);
        box->setCurrentIndex(i);
    }
    else if(index.column() == 9)
    {
        QString str = index.model()->data(index).toString();
        QComboBox  *box = static_cast<QComboBox *>(editor);
        uint i = box->findText(str);
        box->setCurrentIndex(i);
    }
    
}

//更新Model中的数据
void combdelegate::setModelData(QWidget *editor, QAbstractItemModel*model, const QModelIndex &index)const
{
    if(index.column()==1)
    {
        QComboBox  *box = static_cast<QComboBox*>(editor);
        QString  str =  box->currentText();
        model->setData(index,str);
    }
    else if(index.column()==2)  //负压值
    {
         QSpinBox *negAirBox = static_cast<QSpinBox*>(editor);
         uint num = negAirBox->text().toUInt();
        ;// this->combNeedleBed->needlebedPara[0] = num;

         model->setData(index,num);
    }
    else if(index.column()==3)  //电机步数
    {
         QSpinBox *motorBox = static_cast<QSpinBox*>(editor);
         uint num = motorBox->text().toUInt();
        ;// this->combNeedleBed->needlebedPara[1] = num;
         model->setData(index,num);
    }
    else if(index.column() ==6)  //保压时间
    {
          QSpinBox *keepNegair = static_cast<QSpinBox *>(editor);
          uint num = keepNegair->text().toUInt();
        ;//  this->combNeedleBed->needlebedPara[2]= num;

          model->setData(index,num);
    }
   else if(index.column() == 7)  //高负压控制
   {
          QComboBox *highNegair = static_cast<QComboBox *>(editor);
          QString str = highNegair->currentText();
          if(str == "打开")
            ;//  this->combNeedleBed->needlebedPara[3] = 0x01;
          else
           ;//this->combNeedleBed->needlebedPara[3]=0;
          model->setData(index,str);
    }
    else if(index.column()== 8) //低负压控制
     {
         QComboBox *lowNegair = static_cast<QComboBox *>(editor);
         QString  str = lowNegair->currentText();
         if(str == "打开")
         ;//  this->combNeedleBed->needlebedPara[4]= 0x01;
         else
          ;// this->combNeedleBed->needlebedPara[4] = 0;
         model->setData(index,str);
     }
    else if(index.column() == 9) //破真空控制
    {
        QComboBox  *breakVacuum= static_cast<QComboBox *>(editor);
        QString  str = breakVacuum->currentText();
        if(str == "打开")
            ;
         //  this->combNeedleBed->needlebedPara[5] =0x01;
        else
       //   this->combNeedleBed->needlebedPara[5] = 0;
            ;
        model->setData(index,str);
    }
}

//更新控件的显示
void combdelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option, const QModelIndex &index)const
{
    editor->setGeometry(option.rect);
}



