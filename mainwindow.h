#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>

#include <QMenuBar>
#include <QMenu>
#include <QWidget>
#include <QToolBox>

#include <QBoxLayout>
#include <QSpacerItem>

#include <QListWidget>
#include "qdrawablelistwidget.h"


namespace Ui {
class MainWindow;
}
class QdrawableListWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        static QdrawableListWidget *drawableListWidget;

        static QToolBox *emiterMode;

        static QWidget *drawableParameters;
        static QWidget *gravityParameters;
        static QWidget *radialParameters;
        static QWidget *outlook;
        static QLabel *texturePath;
        static QPushButton *textureButton;
        static QPushButton *addSystemButton;
        static QMenu *mainMenu;

    public slots:

    private:
        Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
