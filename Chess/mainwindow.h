#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <board.h>

class mainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit mainWindow(QWidget *parent = 0);

    board *newboard;

signals:

public slots:
    void onNew();
    void onSave();
    void onLoad();
    void onBack();
    void onQuit();
};

#endif // MAINWINDOW_H
