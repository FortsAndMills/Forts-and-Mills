#ifndef RANDOM
#define RANDOM

#include "Technical/Headers.h"

class Random
{
    QList <int> numbers;
    int i = 0;

public:
    explicit Random(QList <int> new_numbers)
    {
        numbers = new_numbers;
    }
    explicit Random() {}
    int next()
    {
        int ans = numbers[i++];
        if (numbers.size() == i)
        {
            debug << "FATAL ERROR: random queue has FINISHED!!!\n";
            i = 0;
        }
        return ans;
    }
};

class Range  // угадайте для чего
{
public:
    int min, max;

    explicit Range(int a, int b, qreal K = 1)
    {
        if (a > b)
            qSwap(a, b);

        min = qFloor(a * K);
        max = qCeil(b * K);
    }
    explicit Range(int a) : Range(a, a) {}
    explicit Range() : Range(0) {}

    int get_rand(Random * rand)
    {
        return rand->next() % (max - min + 1) + min;
    }
};

class Coord
{
public:
    qint8 x, y;
    explicit Coord(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    explicit Coord()
    {
        x = UNDEFINED;
        y = UNDEFINED;
    }    

    friend bool operator == (const Coord left, const Coord right);
    friend bool operator != (const Coord left, const Coord right);
    friend bool operator < (const Coord left, const Coord right);

    friend QTextStream & operator << (QTextStream &stream, const Coord c)
    {
        return stream << "(" << c.x << ", " << c.y << ")";
    }
    friend QTextStream & operator >> (QTextStream &stream, Coord &)
    {
        return stream;// >> c.x >> c.y;
    }
    friend QDataStream &operator << (QDataStream &stream, const Coord c)
    {
        return stream << c.x << c.y;
    }
    friend QDataStream &operator >>(QDataStream &stream, Coord & c)
    {
        return stream >> c.x >> c.y;
    }
};

inline uint qHash (const QPointF & key)
{
    return qHash (QPair<float,float>(key.x(), key.y()) );
}
#define ANY Coord(-2, -2)
#define NOWHERE Coord()

#endif // ELEMENTARY_H
