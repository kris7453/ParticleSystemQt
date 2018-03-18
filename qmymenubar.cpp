#include "qmymenubar.h"

QMyMenuBar::QMyMenuBar(QWidget *parent) : QWidget(parent), mainWindow(parent->parentWidget()), mouseLastPosition(QPoint(0,0))
{
}

void QMyMenuBar::mousePressEvent(QMouseEvent *e)
{
    mouseLastPosition = e->pos();
    //qDebug() << mouseLastPosition.x() << " " << mouseLastPosition.y();
}

void QMyMenuBar::mouseMoveEvent(QMouseEvent *e)
{

    QPoint p = e->pos();
    //qDebug() << p.x() << " " << p.y();
    p -= mouseLastPosition;
    mainWindow->move( mainWindow->pos()+p );
}

void QMyMenuBar::mouseDoubleClickEvent(QMouseEvent *e)
{
    mainWindow->isMaximized() ? mainWindow->showNormal() : mainWindow->showMaximized();
}
