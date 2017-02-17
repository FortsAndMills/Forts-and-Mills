#include "Hex.h"

void Hex::recountPoints()
{
    WAY where_entered = "";
    int stayers = 0;
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
        else
            ++stayers;
    }

    if (where_entered == "" && ids.size() > 1)
    {
        for (QSet<int>::iterator it = ids.begin(); it != ids.end(); ++it)
        {
            int i = (*it);
            if (pointPositionState[i] == STAY && pointsWay[i] != "")
            {
                where_entered = pointsWay[i];
            }
        }
    }

    for (QSet<int>::iterator it = ids.begin(); it != ids.end(); ++it)
    {
        int i = (*it);
        if (pointPositionState[i] == STAY)
        {
            if (where_entered == "")
            {
                points[i] = QPointF(width() / 2, height() / 2);
            }
            else if (pointsWay[i] == "")
            {
                points[i] = EnterPoint[game->oppositeWay(where_entered)];
            }
            else
            {
                points[i] = EnterPoint[pointsWay[i]];
            }
        }
    }

    QMap <int, bool> changed;
    for (QSet<int>::iterator it = ids.begin(); it != ids.end(); ++it)
    {
        int i = *it;
        if (!changed[i])
        {
            int num = 0;
            for (QSet<int>::iterator jt = it; jt != ids.end(); ++jt)
            {
                int j = *jt;
                if (points[j] == points[i])
                {
                    ++num;
                    changed[j] = true;
                }
            }

            if (num <= ShiftMap.size())
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
