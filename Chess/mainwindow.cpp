#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QFileDialog>

mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->newboard = new board;
    this->setCentralWidget(newboard);

    QMenu* m = menuBar()->addMenu("&file");
    m->addAction("新开一局", this, SLOT(onNew()));
    m->addAction("保存", this, SLOT(onSave()));
    m->addAction("读盘", this, SLOT(onLoad()));
    m->addAction("悔棋", this, SLOT(onBack()));
    m->addSeparator();
    m->addAction("退出", this, SLOT(onQuit()));


}

void mainWindow::onNew()
{
    newboard->init();
    newboard->update();
}

void mainWindow::onSave()
{
    QString filename = QFileDialog::getSaveFileName(this,
                        "请保存",
                         "Board.argo",
                         "中国象棋文件(*.argo)");
    if(filename.length() > 0)
    {
        newboard->save(filename);
    }
}

void mainWindow::onLoad()
{
    QString filename = QFileDialog::getOpenFileName(this,
                        "请选择",
                         "Board.argo",
                         "中国象棋文件(*.argo)");
    if(filename.length() > 0)
    {
        newboard->load(filename);
    }
}

void mainWindow::onBack()
{
    newboard->backOne();
}

void mainWindow::onQuit()
{
    this->close();
}
