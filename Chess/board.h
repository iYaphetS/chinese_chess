#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "stone.h"
#include "util.h"
#include <QString>
#include <QList>

struct Step
{
    int moveid;
    int killid;
    int rowFrom;
    int colFrom;
    int rowTo;
    int colTo;
};

class board : public QWidget
{
    Q_OBJECT
public:
    explicit board(QWidget *parent = 0);

    void init();
    void save(QString filename);
    void load(QString filename);
    void backOne();

    int &d;

    //绘制整体布局
    void paintEvent(QPaintEvent *ev);
    void drawPlate(QPainter &p);
    void drawCross(QPainter &p, int row, int col);

    void mousePressEvent(QMouseEvent *ev);
    int getStoneId(int row, int col);

    //选择棋子状态
    void selectStone(int click);
    //移动棋子状态----传出的参数select不要和全局的selectId同名
    void moveStone(int select, int click, int row, int col);

    bool canMove(int select, int click, int row, int col);
    bool canMoveJIANG(int select, int click, int row, int col);
    bool canMoveSHI(int select, int click, int row, int col);
    bool canMoveXIANG(int select, int click, int row, int col);
    bool canMoveCHE(int select, int click, int row, int col);
    bool canMoveMA(int select, int click, int row, int col);
    bool canMovePAO(int select, int click, int row, int col);
    bool canMoveBING(int select, int click, int row, int col);

    int getStoneCount(int row1, int col1, int row2, int col2);
    int selectId;
    bool redTurn;
    Stone s[32];

    QList<Step *> steps;
    void addStep(int select, int kill, int row, int col);
    void clearSteps();
signals:

public slots:
};

#endif // BOARD_H
