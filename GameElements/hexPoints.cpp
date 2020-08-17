#include "Hex.h"

// эта функция оказалась достойна отдельного cpp файла
// в силу нетривиальности, несовершенности и важности выбранного алгоритма

// Задача следующая. Есть несколько "позиций" - групп юнитов, которые
// могут входить, выходить из гекса в определённом направлении
// требуется для каждой позиции определить, как располагать её на гексе

void Hex::recountPoints()
{
    // Первый проход: ищем откуда идёт "движение"
    WAY where_entered = NO_WAY;
    // Заодно устанавливаем ответ для тех, кому явно указано, что он входит или выходит
    for (QSet<int>::iterator it = ids.begin(); it != ids.end(); ++it)
    {
        int i = (*it);
        if (pointPositionState[i] == ENTERING)
        {
            points[i] = EnterPoint[pointsWay[i]];
            where_entered = pointsWay[i];
        }
        else if (pointPositionState[i] == LEAVING)
        {
            points[i] = LeavePoint[pointsWay[i]];
        }
    }

    // если движения нет "ниоткуда", смотрим в какой они стороне
    // и назначаем эту сторону той, откуда "есть движение".
    if (where_entered == NO_WAY && ids.size() > 1)
    {
        for (QSet<int>::iterator it = ids.begin(); it != ids.end(); ++it)
        {
            int i = (*it);
            if (pointPositionState[i] == STAY && pointsWay[i] != NO_WAY)
            {
                where_entered = pointsWay[i];
            }
        }
    }

    // теперь устанавливаем якоря для тех, кто находится в клетке
    for (QSet<int>::iterator it = ids.begin(); it != ids.end(); ++it)
    {
        int i = (*it);
        if (pointPositionState[i] == STAY)
        {
            if (where_entered == NO_WAY)  // если движения таки нет, то центр
            {
                points[i] = QPointF(width() / 2, height() / 2);
            }
            else if (pointsWay[i] == NO_WAY)  // если не совпадает с движением, то противоположное
            {
                points[i] = EnterPoint[game->oppositeWay(where_entered)];
            }
            else
            {
                points[i] = EnterPoint[pointsWay[i]];
            }
        }
    }

    // теперь если несколько объектов оказались в одной точке,
    // их надо "раздвинуть" на указанные в ShiftMap константы
    QMap <int, bool> changed;
    for (QSet<int>::iterator it = ids.begin(); it != ids.end(); ++it)
    {
        int i = *it;
        if (!changed[i])  // если этот объект ещё не раздвинут
        {
            int num = 0;  // считаем, сколько всего объектов в той же точке
            for (QSet<int>::iterator jt = it; jt != ids.end(); ++jt)
            {
                int j = *jt;
                if (points[j] == points[i])
                {
                    ++num;
                    changed[j] = true;
                }
            }

            // и раздвигаем их все!
            if (num <= ShiftMap.size())  // защита на случай немеренного количества объектов
            {
                int index = 0;
                QPointF base = points[i];
                for (QSet<int>::iterator jt = it; jt != ids.end(); ++jt)
                {
                    int j = *jt;
                    if (points[j] == base)
                    {
                        points[j] += this->ShiftMap[num][index];
                        ++index;
                    }
                }
            }
        }
    }
}
