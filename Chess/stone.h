#ifndef STONE_H
#define STONE_H
#include <QPainter>
#include <util.h>
#include <QString>
#include <QChar>

class Stone
{
public:
    Stone();
    enum Type{JIANG, SHI, XIANG, CHE, MA, PAO, BING};
    int row;
    int col;
    Type type;
    int id;//规定0-15 红色 16-31 黑色
    bool dead;

    int &d;

    void draw(QPainter &p, int selectId);
};

#endif // STONE_H
