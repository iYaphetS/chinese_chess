#include "board.h"
#include <QDebug>
#include <QFile>
#include <QDataStream>


board::board(QWidget *parent) : QWidget(parent), d(Util::d)
{
    init();
}

void board::init()
{
    static struct
    {
        Stone::Type type;
        int row;
        int col;
    } p[16] =
    {
     //从红方开始，红方的右下角的车为下标0
     {Stone::CHE, 9, 8},
     {Stone::MA, 9, 7},
     {Stone::XIANG, 9, 6},
     {Stone::SHI, 9, 5},
     {Stone::JIANG, 9, 4},
     {Stone::SHI, 9, 3},
     {Stone::XIANG, 9, 2},
     {Stone::MA, 9, 1},
     {Stone::CHE, 9, 0},
     {Stone::PAO, 7, 1},
     {Stone::PAO, 7, 7},
     {Stone::BING, 6, 0},
     {Stone::BING, 6, 2},
     {Stone::BING, 6, 4},
     {Stone::BING, 6, 6},
     {Stone::BING, 6, 8}
    };
    //初始化32个棋子
    for(int i = 0; i < 32; i++)
    {
        s[i].id = i;
        if(i < 16)
        {
            s[i].type = p[i].type;
            s[i].row = p[i].row;
            s[i].col = p[i].col;
        }
        else
        {
            s[i].type = p[i - 16].type;
            s[i].row = 9 - p[i - 16].row;
            s[i].col = 8 - p[i - 16].col;

        }
        s[i].dead = false;//*****
    }
    selectId = -1;
    redTurn = true;
}

void board::save(QString filename)
{
    QFile file(filename);
    file.open(QFile::WriteOnly);

    QDataStream ds(&file);

    ds << this->selectId << this->redTurn;

    for(int i = 0; i < 32; i++)
    {
        int type = (int)s[i].type;
        ds << s[i].col << s[i].row << s[i].id
           << s[i].dead << type;
    }
#if 1
    ds << (int)steps.size();
    foreach(Step *step, steps)
    {
        ds << step->colFrom << step->colTo << step->killid
           << step->moveid << step->rowFrom;
    }
#endif
    file.close();
}

void board::load(QString filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);

    QDataStream ds(&file);

    ds >> this->selectId >> this->redTurn;

    for(int i = 0; i < 32; i++)
    {
        int type = (int)s[i].type;
        ds >> s[i].col >> s[i].row >> s[i].id
           >> s[i].dead >> type;
    }
#if 1
    int size;
    ds >> size;
    for(int i = 0; i < size; i++)
    {
        Step *step = new Step;
        ds >> step->colFrom >> step->colTo >> step->killid
           >> step->moveid >> step->rowFrom;
        steps.push_back(step);
    }
#endif
    file.close();
    update();
}

void board::backOne()
{
    if(steps.size() == 0)
    {
        return;
    }
    Step *step = steps.last();
    steps.removeLast();

    s[step->moveid].row = step->rowFrom;
    s[step->moveid].col = step->colFrom;
    if(step->killid != -1)
    {
        s[step->killid].dead = false;
    }
    this->redTurn = !redTurn;
    selectId = -1;

    delete step;
    update();
}

void board::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    drawPlate(p);

}

void board::drawPlate(QPainter &p)
{

    //1.画网格线
    //画横向隔线
    for(int i = 1; i <= 10; i++)
    {
        p.drawLine(QPoint(d, i * d), QPoint(9 * d, i * d));
    }

    //画纵向隔线
    for(int i = 1; i <= 9; i++)
    {
        //第1， 9列画一条线
        if(i == 1 || i == 9)
        {
            p.drawLine(QPoint(i * d, d), QPoint(i * d, 10 * d));
        }
        //第2， 8列画两条线，中间空出来的画楚河汉界
        else
        {
            p.drawLine(QPoint(i * d, d), QPoint(i *d, 5 * d));
            p.drawLine(QPoint(i * d, 6 * d), QPoint(i *d, 10 *d));
        }
    }

    //2.画九宫格
    p.drawLine(QPoint(4 * d, d), QPoint(6 * d, 3 *d));
    p.drawLine(QPoint(4 * d, 3 * d), QPoint(6 * d, d));

    p.drawLine(QPoint(4 * d, 8 * d), QPoint(6 * d, 10 * d));
    p.drawLine(QPoint(4 * d, 10 * d), QPoint(6 * d, 8 * d));

    //3.画棋位的花边
    drawCross(p, 2, 1);
    drawCross(p, 2, 7);

    drawCross(p, 3, 0);
    drawCross(p, 3, 2);
    drawCross(p, 3, 4);
    drawCross(p, 3, 6);
    drawCross(p, 3, 8);

    drawCross(p, 7, 1);
    drawCross(p, 7, 7);

    drawCross(p, 6, 0);
    drawCross(p, 6, 2);
    drawCross(p, 6, 4);
    drawCross(p, 6, 6);
    drawCross(p, 6, 8);

    //画棋子
    for(int i = 0; i < 32; i++)
    {
        s[i].draw(p, selectId);
    }

    //画楚河汉界
    QString ch1 = "楚";
    p.drawText(2 * d, 5 * d, d, d, Qt::AlignCenter, QString(ch1));
    QString ch2 = "河";
    p.drawText(3 * d, 5 * d, d, d, Qt::AlignCenter, QString(ch2));
    QString ch3 = "汉";
    p.drawText(6 * d, 5 * d, d, d, Qt::AlignCenter, QString(ch3));
    QString ch4 = "界";
    p.drawText(7 * d, 5 * d, d, d, Qt::AlignCenter, QString(ch4));



}

void board::drawCross(QPainter &p, int row, int col)
{
    //得到中心坐标
    QPoint center = Util::getCenterPoint(row, col);

    if (col != 8)
    {
        //右下方
        QPoint origin = center + QPoint(d/6, d/6);
        p.drawLine(origin, origin + QPoint(0, d/3));
        p.drawLine(origin, origin + QPoint(d/3, 0));

        //右上方
        origin = center + QPoint(d/6, - d/6);
        p.drawLine(origin, origin + QPoint(0, - d/3));
        p.drawLine(origin, origin + QPoint(d/3, 0));
    }

    if(col != 0)
    {
        //左上方
        QPoint origin = center - QPoint(d/6, d/6);
        p.drawLine(origin, origin - QPoint(0, d/3));
        p.drawLine(origin, origin - QPoint(d/3, 0));

        //左下方
        origin = center + QPoint(- d/6, d/6);
        p.drawLine(origin, origin + QPoint(0, d/3));
        p.drawLine(origin, origin + QPoint(- d/3, 0));

    }

}


void board::mousePressEvent(QMouseEvent *ev)
{

    if(ev->button() == Qt::RightButton)
    {
        return;
    }

    //棋盘的屏幕坐标----棋盘坐标
    QPoint p = ev->pos();
    int row = -1;
    int col = -1;
    if(!Util::getPlateCood(p, row, col))
    {
        return;
    }
    //qDebug() << "1------" << row << col;

    //判断坐标上是否有棋子
    int clickId = getStoneId(row, col);


    //判断状态：选择状态，还是走棋状态

    //如果selectId为-1时，是在选择状态
    if(selectId == -1)
    {
        //选择了对方的棋子，无效
       selectStone(clickId);
    }
    //否则就是走棋状态，棋子已经选定了
    else
    {
        //更换选择，避免相互残杀
        if(clickId != -1 && ((clickId < 16) == (selectId < 16)))
        {
            selectStone(clickId);
            return;
        }
        if(canMove(selectId, clickId, row, col))
        {
            moveStone(selectId, clickId, row, col);
        }
    }

}

int board::getStoneId(int row, int col)
{
    //0-31内的是有效值
     for(int i = 0; i < 32; i++)
     {
         if(s[i].row == row && s[i].col == col && !s[i].dead)
         {
             return i;
         }
     }
     return -1;
}

void board::selectStone(int click)
{
#if 0
        if(redTurn && clickId > 15)
        {
            return;
        }
        if(!redTurn && clickId < 16)
        {
            return;
        }
#endif
    if(redTurn != (click < 16))
    {
        return;
    }
    selectId = click;

    qDebug() << "selectid" << selectId;
    update();

}


void board::moveStone(int select, int click, int row, int col)
{
    //记录棋子原来的点，新点击的点，行列
    addStep(select, click, row, col);

    //将选中的棋子，行列坐标更新一下，进行绘图
    s[select].row = row;
    s[select].col = col;
    //将选中的棋子原来位子的棋子图去掉
    /*
    这里走棋，就是在要走的位置上绘制一个图片
    所以这个棋子在原来的位置上的图片需要删除
    将其死亡状态置为true
    */
    if(click != -1)
    {
        s[click].dead = true;
    }

    selectId = -1;
    redTurn = !redTurn;

    update();//update放到最后
}

bool board::canMove(int select, int click, int row, int col)
{
    switch (s[select].type)
    {
    case Stone::JIANG:
        return canMoveJIANG(select, click, row, col);
        break;
    case Stone::SHI:
        return canMoveSHI(select, click, row, col);
        break;
    case Stone::XIANG:
        return canMoveXIANG(select, click, row, col);
        break;
    case Stone::CHE:
        return canMoveCHE(select, click, row, col);
        break;
    case Stone::MA:
        return canMoveMA(select, click, row, col);
        break;
    case Stone::PAO:
        return canMovePAO(select, click, row, col);
        break;
    case Stone::BING:
        return canMoveBING(select, click, row, col);
        break;
    }
    return true;
}
/*
1.只能走一步
2.在九宫格内
3.将不能照面
*/
bool board::canMoveJIANG(int select, int click, int row, int col)
{
    //将不能照面
    if(click != -1 && s[click].type == Stone::JIANG)
    {
        int ret = getStoneCount(s[select].row, s[select].col, row, col);
        if(ret == 0)
        {
            return true;
        }
    }

    //只能走一步
    int drow = qAbs(s[select].row - row);
    int dcol = qAbs(s[select].col - col);
    if(drow + dcol != 1)
    {
        return false;
    }

    //只能在九宫格内
    if(col < 3 || col > 5)
    {
        return false;
    }
    if(s[select].id < 16)
    {
        if(row < 7 || row > 9)
        {
            return false;
        }
    }
    else
    {
        if(row < 0 || row > 2)
        {
            return false;
        }
    }

    return true;

}

/*
1.斜着走
2.在九宫内
*/
bool board::canMoveSHI(int select, int, int row, int col)
{
    //只能在九宫格内
    if(col < 3 || col > 5)
    {
        return false;
    }
    if(s[select].id < 16)
    {
        if(row < 7 || row > 9)
        {
            return false;
        }
    }
    else
    {
        if(row < 0 || row > 2)
        {
            return false;
        }
    }

    //斜着走
    int drow = qAbs(s[select].row - row);
    int dcol = qAbs(s[select].col - col);
    if(drow * 10 + dcol != 11)
    {
        return false;
    }

    return true;
}

/*
1.不能过河
2.飞田
3.象眼不能有子
*/
bool board::canMoveXIANG(int select, int, int row, int col)
{
    //不能过河
    if(s[select].id < 16)
    {
        if(row < 5)
        {
            return false;
        }
    }
    else
    {
        if(row > 4)
        {
            return false;
        }
    }

    //斜着走
    int drow = qAbs(s[select].row - row);
    int dcol = qAbs(s[select].col - col);
    if(drow * 10 + dcol != 22)
    {
        return false;
    }

    //象眼不能有子(田字中间有棋子不能移动)
    int erow = (s[select].row + row) / 2;
    int ecol = (s[select].col + col) / 2;
    int eye = getStoneId(erow, ecol);
    if(eye != -1)
    {
        return false;
    }

    return true;
}

/*
1.直线走
2.中间不能有棋子
*/
bool board::canMoveCHE(int select, int, int row, int col)
{
    //getStoneCount就能判断是不是在一条直线上
    int ret = getStoneCount(s[select].row, s[select].col, row, col);
    if(ret != 0)
    {
        return false;
    }

    return true;
}

/*
1.马走日
2.马不能碍马腿
*/
bool board::canMoveMA(int select, int, int row, int col)
{
    //马走日
    int drow = qAbs(s[select].row - row);
    int dcol = qAbs(s[select].col - col);
    int d = drow * 10 + dcol;
    if(d != 21 && d != 12)
    {
        return false;
    }

    //马腿的行列
    //竖走日
    if(d == 21)
    {
        int erow = (s[select].row + row) / 2;
        int ecol = s[select].col;
        //马腿的位置有棋子，不能走
        if(getStoneId(erow, ecol) != -1)
        {
            return false;
        }
    }
    //横走日
    else if(d == 12)
    {
        int erow = s[select].row;
        int ecol = (s[select].col + col) / 2;
        if(getStoneId(erow, ecol) != -1)
        {
            return false;
        }
    }

    return true;


}

/*
1.直线走，点击的位置没有棋子，中间不能有棋子
        点击的位置是要吃掉的棋子，中间要有一个棋子
*/
bool board::canMovePAO(int select, int click, int row, int col)
{
    //点击位置没有棋子
    if(click == -1)
    {
        int ret = getStoneCount(s[select].row, s[select].col, row, col);
        if(ret != 0)
        {
            return false;
        }
    }
    else
    {
        int ret = getStoneCount(s[select].row, s[select].col, row, col);
        if(ret != 1)
        {
            return false;
        }
    }
    return true;
}

/*
1.不能后退，过河前不能平移
2.只能走一步
3.过河后可以平移
*/
bool board::canMoveBING(int select, int, int row, int col)
{
    //只能走一步
    int drow = qAbs(s[select].row - row);
    int dcol = qAbs(s[select].col - col);
    if(drow + dcol != 1)
    {
        return false;
    }

    //过河
    //红棋
    if(s[select].id < 16)
    {
        //后退
        if(row > s[select].row)
        {
            return false;
        }
        //没过河
        if(s[select].row >= 5)
        {
            if(row == s[select].row)
            {
                return false;
            }
        }
    }
    //黑棋
    else
    {
        if(row < s[select].row)
        {
            return false;
        }

        if(s[select].row <= 4)
        {
           if(row == s[select].row)
           {
               return false;
           }
        }
    }

    return true;
}

//获得一条直线上两个棋子中间棋子的数量
//如果不是直线返回-1
int board::getStoneCount(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    //不在一条直线上
    if(row1 != row2 && col1 != col2)
    {
        return -1;
    }
    if(row1 == row2)
    {
        if(col1 > col2)
        {
            int temp = col1;
            col1 = col2;
            col2 = temp;
        }
        for(int col = col1 + 1; col < col2; col++)
        {
            if(getStoneId(row1, col) != -1)
            {
                ret++;
            }
        }
    }
    else
    {
        if(row1 > row2)
        {
            int temp = row1;
            row1 = row2;
            row2 = temp;
        }
        //for(int row = row1 + 1; row1 < row2; row++)---花了半天调试的错误。。
        for(int row = row1 + 1; row < row2; row++)
        {
            //注意参数顺序
            if(getStoneId(row, col1) != -1)
            {
                ret++;
            }
        }
    }
    return ret;
}

void board::addStep(int select, int kill, int row, int col)
{
    Step* step = new Step;
    step->moveid = select;
    step->killid = kill;
    step->rowTo = row;
    step->colTo = col;
    step->rowFrom = s[select].row;
    step->colFrom = s[select].col;
    steps.append(step);
}

void board::clearSteps()
{
    while(steps.size())
    {
        Step *step = steps.first();
        delete step;
        steps.removeFirst();
    }
}













