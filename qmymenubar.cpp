#include "qmymenubar.h"

QMyMenuBar::QMyMenuBar(QWidget *parent) : QWidget(parent), mainWindow(parent->parentWidget()), mouseLastPosition(QPoint(0,0)), maximalized(false)
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

    if(!maximalized)
    {
        minSize = mainWindow->geometry();
        QRect screen = QApplication::desktop()->availableGeometry(mainWindow);
        mainWindow->setGeometry(screen);
    }
    else
        mainWindow->setGeometry(minSize);

    maximalized = !maximalized;
    //mainWindow->isMaximized() ? mainWindow->showNormal() : mainWindow->showMaximized();
}
