#ifndef RANDOM
#define RANDOM

#include "Technical/Headers.h"

class Random
{
    QQueue <int> numbers;

public:
    explicit Random(QList <int> new_numbers)
    {
        foreach (int a, new_numbers)
            numbers.enqueue(a);
    }
    explicit Random() {}
    int next()
    {
        if (numbers.isEmpty())
            return rand();
        else
        {
            if (numbers.size() == 1)
                qDebug() << "FATAL ERROR: random queue has FINISHED!!!";
            return numbers.dequeue();
        }
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

#endif // ELEMENTARY_H
