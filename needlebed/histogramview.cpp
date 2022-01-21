#include "histogramview.h"

HistogramView::HistogramView(QWidget * parent):QAbstractItemView(parent)
{


}



HistogramView::~HistogramView()
{

}
// paintEvent()函数具体完成柱状统计图绘制的工作
void HistogramView::paintEvent(QPaintEvent *)
{
    QPainter painter(viewport());
    painter.setPen(Qt::black);

    int x0=10;
    int y0=30;
    //y坐标轴
    painter.drawLine(x0,y0,10,400);
    painter.drawLine(8,32,10,30);
    painter.drawLine(10,30,12,32);
    painter.drawText(20,20,tr("温度值"));
    //    for(int i=1;i<5;i++)
    //    {
    //        painter.drawLine(-1,-i*50,1,-i*50);
    //        painter.drawText(-20,-i*50,tr("%1").arg(i*5));
    //    }
    //x 坐标轴

    painter.drawLine(x0,400,1090,400);
    painter.drawLine(1088,398,1090,400);
    painter.drawLine(1090,400,1088,402);
    painter.drawText(1105,400,tr("通道序号"));

    int posD=x0+10;
    int row;
    for(row=1;row<model()->rowCount(rootIndex());row++)
    {
        QModelIndex index=model()->index(row,0,rootIndex());
        QString dep=model()->data(index).toString();
        painter.drawText(posD,400+20,dep);
        posD+=25;
    }
#if 1
    //男
    int posM=x0+10;
    for(row=1;row<model()->rowCount(rootIndex());row++)
    {
        QModelIndex index=model()->index(row,1,rootIndex());
        int male=model()->data(index).toDouble();

        int width=8;
        if(selections->isSelected(index))
            painter.setBrush(QBrush(Qt::  green,Qt::Dense3Pattern));
        else
            painter.setBrush(Qt::  green);

        painter.drawRect(QRect(posM,400-male*3,width,male*3));
        QRegion regionM(posM,y0-male*3,width,male*3);
        MRegionList<<regionM;
        posM+=22;
    }

#endif
#if 0
    //女
    int posF=x0+30;
    for(row=0;row<model()->rowCount(rootIndex());row++)
    {
        QModelIndex index=model()->index(row,2,rootIndex());
        int female=model()->data(index).toDouble();

        int width=10;
        if(selections->isSelected(index))
            painter.setBrush(QBrush(Qt::red,Qt::Dense3Pattern));
        else
            painter.setBrush(Qt::red);
        painter.drawRect(QRect(posF,y0-female*10,width,female*10));
        QRegion regionF(posF,y0-female*10,width,female*10);
        FRegionList<<regionF;

        posF+=50;
    }
    //退休
    int posS=x0+40;
    for(row=0;row<model()->rowCount(rootIndex());row++)
    {
        QModelIndex index=model()->index(row,3,rootIndex());
        int retire=model()->data(index).toDouble();

        int width=10;
        if(selections->isSelected(index))
            painter.setBrush(QBrush(Qt::green,Qt::Dense3Pattern));
        else
            painter.setBrush(Qt::green);

        painter.drawRect(QRect(posS,y0-retire*10,width,retire*10));
        QRegion regionS(posS,y0-retire*10,width,retire*10);
        SRegionList<<regionS;

        posS+=50;
    }
#endif
}

//void HistogramView::dataChanged(const QModelIndex &topLeft,const QModelIndex &bottomRight)
//{
//    QAbstractItemView::dataChanged(topLeft,bottomRight);
//    viewport()->update();
//}

void HistogramView::setSelectionModel(QItemSelectionModel *selectionModel)
{
    selections=selectionModel;
}

void HistogramView::selectionChanged(const QItemSelection &selected,const QItemSelection &deselected)
{
    viewport()->update();
}

void HistogramView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    QAbstractItemView::dataChanged(topLeft,bottomRight);
    viewport()->update();
}

void HistogramView::mousePressEvent(QMouseEvent *event)
{
    QAbstractItemView::mousePressEvent(event);
    setSelection(QRect(event->pos().x(),event->pos().y(),1,1),QItemSelectionModel::SelectCurrent);
}

void HistogramView::setSelection(const QRect &rect,QItemSelectionModel::SelectionFlags flags)
{
    int rows = model()->rowCount(rootIndex());
    int columns = model()->columnCount(rootIndex());
    QModelIndex selectedIndex;

    for (int row=0; row<rows; ++row)
    {
        for (int column=1; column<columns; ++column)
        {
            QModelIndex index=model()->index(row,column,rootIndex());
            QRegion region=itemRegion(index);

            if (!region.intersected(rect).isEmpty())
                selectedIndex = index;
        }
    }
    if(selectedIndex.isValid())
        selections->select(selectedIndex,flags);
    else
    {
        QModelIndex noIndex;
        selections->select(noIndex,flags);
    }
}


QModelIndex HistogramView::indexAt(const QPoint &point)const
{
    QPoint newPoint(point.x(),point.y());
    QRegion region;
    foreach(region,MRegionList)				// 男 列
    {
        if (region.contains(newPoint))
        {
            int row = MRegionList.indexOf(region);
            QModelIndex index = model()->index(row,1,rootIndex());
            return index;
        }
    }
    foreach(region,FRegionList)				// 女 列
    {
        if (region.contains(newPoint))
        {
            int row = FRegionList.indexOf(region);
            QModelIndex index = model()->index(row,2,rootIndex());
            return index;
        }
    }
    foreach(region,SRegionList)				// 合计 列
    {
        if (region.contains(newPoint))
        {
            int row = SRegionList.indexOf(region);
            QModelIndex index = model()->index(row,3,rootIndex());
            return index;
        }
    }
    return QModelIndex();
}

QRect HistogramView::visualRect(const QModelIndex &index)const{}

void HistogramView::scrollTo(const QModelIndex &index,ScrollHint){}

QModelIndex HistogramView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers){}

int HistogramView::horizontalOffset()const{}

int HistogramView::verticalOffset()const{}

bool HistogramView::isIndexHidden(const QModelIndex &index)const{}

QRegion HistogramView::visualRegionForSelection(const QItemSelection &selection)const{}

QRegion HistogramView::itemRegion(QModelIndex index)
{

    QRegion region;
    if (index.column() == 1)		// 男
        region = MRegionList[index.row()];
    if (index.column() == 2)		// 女
        region = FRegionList[index.row()];
    if (index.column() == 3)		// 退休
        region = SRegionList[index.row()];
    return region;

}
