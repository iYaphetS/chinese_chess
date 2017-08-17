#include "stone.h"

Stone::Stone() : d(Util::d)
{
    dead = false;

}

void Stone::draw(QPainter &p, int selectId)
{

    if(dead)
    {
        return;
    }
    //图像保真，反锯齿
    p.setRenderHint(QPainter::Antialiasing);
    //设置画刷
    if(selectId == id)
    {
       p.setBrush(Qt::gray);
    }
    else
    {
        p.setBrush(Qt::yellow);
    }
    //设置画笔
    p.setPen(QPen(Qt::black));
    //1-----画圆圈
    QPoint center = Util::getCenterPoint(row, col);
    p.drawEllipse(center, d/2, d/2);
    //2-----画字
    QString text = "帅仕相车马炮兵将士象车马炮卒";
    int index = 7 * (id > 16) + (int)type;
    QChar ch = text.at(index);

    //设置字体
    p.setFont(QFont("楷体", d/2));
    if(id < 16)
    {
        p.setPen(QPen(Qt::red));
    }
    else
    {
        p.setPen(QPen(Qt::black));
    }

    p.drawText(Util::getStonRect(row, col), Qt::AlignCenter, QString(ch));
}
