#ifndef QMYMENUBAR_H
#define QMYMENUBAR_H

#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QObject>

#include <QMouseEvent>
#include <QDebug>
#include <QWidget>

class QMyMenuBar : public QWidget
{
    Q_OBJECT

public:
    explicit QMyMenuBar(QWidget *parent = 0);


signals:

public slots:

private:
    QWidget *mainWindow;
    bool maximalized;
    QRect minSize;
    QPoint mouseLastPosition;
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
};

#endif // QMYMENUBAR_H
