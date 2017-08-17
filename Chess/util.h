#ifndef UTIL_H
#define UTIL_H
#include <QPoint>
#include <QRect>


class Util
{
public:
    Util();
    static int d;

    static QPoint getCenterPoint(int row, int col);
    static QRect getStonRect(int row, int col);
    //row, col做传出参数
    static bool getPlateCood(QPoint p, int &row, int &col);
};

#endif // UTIL_H
