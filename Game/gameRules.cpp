#include "GameRules.h"
#include "GameUnit.h"

QList <QString> GameRules::AllPlayers = {"Blue", "Red", "Green", "Yellow"};
QList <QString> GameRules::AllOrders = {"Capture", "Recruit", "Liberate", "Attack", "Go", "Retreat", "Siege", "Fire", "Cure", "Fortify"};
QList <QString> GameRules::AllUnits = {"Scrat", "Pig", "Hippo", "Karkun", "Bruter", "Mumusha"};
void sort(QList <QString> & a, QList <QString> example)
{
    int k = 0;
    foreach (QString s, example)
    {
        int i = k;
        while (i < a.size() && a[i] != s)
            ++i;

        if (i < a.size())
        {
            qSwap(a[i], a[k]);
            ++k;
        }
    }
}

GameRules::GameRules()
{
    fieldW = 8;
    fieldH = 5;

    NumOfPlayers = 2;

    ordersInGame << "Capture" << "Attack" << "Go" << "Siege"
                             << "Fire" << "Cure" << "Fortify";

    dayTimes = 3;
    changingDayTimes = false;

    unitsInGame << "Scrat" << "Pig" << "Karkun" << "Hippo";

    damageToHomellesUnits = 179;
    limitatingResources = true;
    fortsAreCool = false;

    recountGenerationParameters();
}
void GameRules::recountGenerationParameters()
{
    sort(ordersInGame, AllOrders);
    sort(unitsInGame, AllUnits);

    doesCaptureRecruits = !ordersInGame.contains("Recruit");
    doesEnteringEnemyHexLiberates = !ordersInGame.contains("Liberate");
    if (changingDayTimes)
        dayTimes = 1;

    qreal K = (fieldW * fieldH) / 40.0;  // во сколько раз увеличить кол. всех ресурсов


    numOfHexTypes["Fort"] = Range(3, 5, K);
    numOfHexTypes["Mountain"] = Range(4, 8, K);
    numOfHexTypes["Mill"] = Range(6, 8, K);

    int cb = 0;
    if (ordersInGame.contains("Recruit"))
        --cb;

    int ab = 0;
    if (ordersInGame.contains("Liberate"))
        --ab;
    if (ordersInGame.contains("Retreat"))
        --ab;

    int cub = 0;
    if (ordersInGame.contains("Mumusha"))
        ++cub;

    numOfResourcesOnField["Capture"] = Range(7 + cb, 11 + cb, K);
    numOfResourcesOnField["Recruit"] = Range(6, 10, K);
    numOfResourcesOnField["Attack"] = Range(5 + ab, 7 + ab, K);
    numOfResourcesOnField["Liberate"] = Range(6, 12, K);
    numOfResourcesOnField["Go"] = Range(7 + ab, 11 + ab, K);
    numOfResourcesOnField["Retreat"] = Range(4, 6, K);
    numOfResourcesOnField["Fire"] = Range(3, 5, K);
    numOfResourcesOnField["Fortify"] = Range(3, 5, K);
    numOfResourcesOnField["Siege"] = Range(3, 5, K);
    numOfResourcesOnField["Cure"] = Range(3 + cub, 5 + cub, K);
}
void GameRules::FormPlayersList(Random *rand)
{
    QList <QString> vars = AllPlayers;

    for (int i = 0; i < NumOfPlayers; ++i)
    {
        int k = rand->next() % vars.size();
        players << vars[k];
        vars.removeAt(k);
    }
}

QList<qint32> GameRules::get()
{
    QList <qint32> ans;
    ans << NumOfPlayers << fieldW << fieldH;

    ans << 0;
    foreach (QString u, AllUnits)
    {
        ans[3] <<= 1;
        if (unitsInGame.contains(u))
            ans[3] |= 1;
    }

    ans << 0;
    foreach (QString r, AllOrders)
    {
        ans[4] <<= 1;
        if (ordersInGame.contains(r))
            ans[4] |= 1;
    }

    ans << (changingDayTimes ? 179 : dayTimes);

    return ans;
}
GameRules::GameRules(QList<qint32> hash)
{
    NumOfPlayers = hash[0];
    fieldW = hash[1];
    fieldH = hash[2];

    for (int i = AllUnits.size() - 1; i >= 0; --i)
    {
        if (hash[3] % 2)
            unitsInGame.push_back(AllUnits[i]);
        hash[3] >>= 1;
    }

    for (int i = AllOrders.size() - 1; i >= 0; --i)
    {
        if (hash[4] % 2)
            ordersInGame.push_back(AllOrders[i]);
        hash[4] >>= 1;
    }

    changingDayTimes = hash[5] == 179;
    dayTimes = hash[5] == 179 ? 1 : hash[5];

    damageToHomellesUnits = 179;
    limitatingResources = true;
    fortsAreCool = false;

    recountGenerationParameters();
}

QTextStream &operator << (QTextStream &stream, const GameRules *r)
{
    stream << "Field: " << r->fieldH << "x" << r->fieldW << "\n";
    stream << "Players: " << r->players;
    stream << "Orders: " << r->ordersInGame;
    stream << "Units: " << r->unitsInGame;
    stream << "DayTimes: " << r->dayTimes << "\n";
    stream << "\n";
    return stream;
}
QTextStream &operator >>(QTextStream &stream, GameRules * r)
{
    QString s;
    char C;

    while (!stream.atEnd())
    {
        stream >> s;
        if (s == "Field:")
            stream >> r->fieldH >> C >> r->fieldW;
        if (s == "Players:")
            stream >> r->NumOfPlayers;
        if (s == "Orders:")
            stream >> r->ordersInGame;
        if (s == "Units:")
            stream >> r->unitsInGame;
        if (s == "DayTimes:")
            stream >> r->dayTimes;
        if (s == "ChangingDayTimes:")
        {
            stream >> s;
            r->changingDayTimes = (s == "on");
        }
    }

    sort(r->ordersInGame, r->AllOrders);
    sort(r->unitsInGame, r->AllUnits);

    r->recountGenerationParameters();
    return stream;
}

QDataStream &operator << (QDataStream &stream, const GameRules *r)
{
    stream << r->fieldH << r->fieldW
                << r->NumOfPlayers
                << r->ordersInGame
                << r->unitsInGame
                << r->dayTimes << r->changingDayTimes;
    return stream;
}
QDataStream &operator >>(QDataStream &stream, GameRules * r)
{
    stream >> r->fieldH >> r->fieldW
                >> r->NumOfPlayers
                >> r->ordersInGame
                >> r->unitsInGame
                >> r->dayTimes >> r->changingDayTimes;

    r->recountGenerationParameters();
    return stream;
}

