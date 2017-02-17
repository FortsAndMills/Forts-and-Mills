#include "Elementary.h"

bool operator == (const Coord a, const Coord b)
{
    return a.x == b.x && a.y == b.y;
}
bool operator != (const Coord a, const Coord b)
{
    return a.x != b.x || a.y != b.y;
}
bool operator < (const Coord a, const Coord b)
{
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}
