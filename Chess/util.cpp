#include "util.h"

int Util::d = 40;
Util::Util()
{

}

QPoint Util::getCenterPoint(int row, int col)
{
    int x = col * d + d;
    int y = row * d + d;

    return QPoint(x, y);
}

QRect Util::getStonRect(int row, int col)
{
    QPoint center = getCenterPoint(row, col);
    center -= QPoint(d/2, d/2);
    return QRect(center.x(), center.y(), d, d);
}

bool Util::getPlateCood(QPoint p, int &row, int &col)
{
    int t[] = {-1, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4,
              5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10};

    int colIndex = p.x() / (d/2);
    int rowIndex = p.y() / (d/2);

    if(rowIndex == 0 || colIndex == 0 || rowIndex > 20 || colIndex > 18)
    {
        return false;
    }
    row = t[rowIndex];
    col = t[colIndex];
    return true;
}
