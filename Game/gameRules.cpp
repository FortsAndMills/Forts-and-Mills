#include "GameRules.h"
#include "GameUnit.h"
#include "Technical/Constants.h"

QList <QString> GameRules::AllPlayers = {"Blue", "Red", "Green", "Yellow"};
QList <QString> GameRules::AllTimerTypes = {"1", "2", "3", "sp", "no"};
QList <QString> GameRules::AllOrders = {"Capture", "Recruit", "Liberate", "Agite", "Attack", "Go", "Retreat", "Siege", "Fire", "Pursue", "Cure", "Fortify"};
QList <QString> GameRules::AllUnits = {"Scrat", "Pig", "Hippo", "Karkun", "Mumusha", "Rabbit"};

QMap <TimerType, int> GameRules::timer_per_plan = {
    {"1", 120},
    {"2", 240},
    {"3", 360}
};
QMap <TimerType, int> GameRules::timer_per_choice = {
    {"1", 30},
    {"2", 60},
    {"3", 90}
};
QMap <TimerType, int> GameRules::timer_per_plan_after_opponent = {
    {"sp", 120}
};
QMap <TimerType, int> GameRules::timer_per_choice_after_opponent = {
    {"sp", 30}
};

// вспомогательная функция: упорядочивает
// элементы a в зависимости от их порядка в example
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
    fieldW = 10;
    fieldH = 6;

    numOfPlayers = 2;
    timer = "3";

    ordersInGame << "Capture" << "Recruit"  << "Liberate"
                 << "Attack" << "Go" << "Cure" << "Fortify";

    dayTimes = 3;
    changingDayTimes = false;

    unitsInGame << "Scrat";

    setTestOptions();
    recountGenerationParameters();
}
void GameRules::setTestOptions()
{
    // тестовые настройки
    damageToHomellesUnits = 179;
    peacefullOrdersBurns = false;
    start_choices = 7;
    pig_can_shoot_attacker = false;
    fire_has_penalty = true;
    fort_gp_min = 6;
    fort_gp_max = 9;
    mill_gp_min = 4;
    mill_gp_max = 8;
    river_gp_min = 2;
    river_gp_max = 4;
    K_change = 1;

    QFile * hostFile = new QFile("Experiments_options.txt");
    if (hostFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream tstream(hostFile);
        QString s;
        tstream >> s >> damageToHomellesUnits
                      >> s >> peacefullOrdersBurns
                      >> s >> start_choices
                      >> s >> pig_can_shoot_attacker
                      >> s >> fire_has_penalty
                      >> s >> fort_gp_min >> fort_gp_max
                      >> s >> mill_gp_min >> mill_gp_max
                      >> s >> river_gp_min >> river_gp_max
                      >> s >> K_change
                      >> s >> constants->unitReconfigureTime;  // TODO: del

        if(tstream.status() != QTextStream::Ok)
        {
            debug << "ERROR: reading programm settings file error" << Qt::endl;
        }
        hostFile->close();
    }

    hostFile = new QFile("Experiments_options.txt");
    if (hostFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream tstream(hostFile);
        tstream << "DamageToHomelessUnits: " << damageToHomellesUnits << Qt::endl
                      << "PeacefullOrdersBurns: " << peacefullOrdersBurns << Qt::endl
                      << "NumOfStartingChoices: " << start_choices << Qt::endl
                      << "PigCanShootAttacker: " << pig_can_shoot_attacker << Qt::endl
                      << "FireHasPenalty: " << fire_has_penalty << Qt::endl
                      << "FortsGenerationLimits: " << fort_gp_min << " " << fort_gp_max << Qt::endl
                      << "MillsGenerationLimits: " << mill_gp_min << " " << mill_gp_max << Qt::endl
                      << "RiversGenerationLimits: " << river_gp_min << " " << river_gp_max << Qt::endl
                      << "ResourcesAmountCoeff: " << K_change << Qt::endl
                      << Qt::endl  // TODO: снести вместе с хэдером констант
                      << "AnimationSpeed: " << constants->unitReconfigureTime << Qt::endl;
        hostFile->close();
    }
}
void GameRules::recountGenerationParameters()
{
    // в зависимости от остальных настроек меняет параметры генерации поля
    // и вспомогательные поля
    sort(ordersInGame, AllOrders);
    sort(unitsInGame, AllUnits);

    doesCaptureRecruits = !ordersInGame.contains("Recruit");
    doesEnteringEnemyHexLiberates = !ordersInGame.contains("Liberate");
    if (changingDayTimes)
        dayTimes = 1;

    qreal K = (fieldW * fieldH) / 60.0;  // во сколько раз увеличить кол. всех ресурсов

    river_interrupt_probability = 20; // шансы реки прерваться

    // диапазоны генерации фортов, мельниц и гор
    numOfRivers = Range(river_gp_min, river_gp_max, K);
    numOfHexTypes["Fort"] = Range(fort_gp_min, fort_gp_max, K);
    numOfHexTypes["Mill"] = Range(mill_gp_min, mill_gp_max, K);
    numOfHexTypes["Mountain"] = Range(3, 7, K);
    numOfHexTypes["Lake"] = Range(3, 7, K);

    // если включено отступление, то ходов и атак генерится на 1 меньше
    int ab = 0;
    if (ordersInGame.contains("Retreat"))
        --ab;

    K *= K_change;

    numOfResourcesOnField["Capture"] = Range(0, 0, K); //Range(9 + cb, 13 + cb, K);
    numOfResourcesOnField["Recruit"] = Range(0, 0, K); //Range(6, 10, K);
    numOfResourcesOnField["Liberate"] = Range(6, 10, K);
    numOfResourcesOnField["Go"] = Range(6 + ab, 11 + ab, K);
    numOfResourcesOnField["Attack"] = Range(5 + ab, 9 + ab, K);
    numOfResourcesOnField["Agite"] = Range(3, 8, K);
    numOfResourcesOnField["Retreat"] = Range(3, 5, K);
    numOfResourcesOnField["Pursue"] = Range(3, 5, K);
    numOfResourcesOnField["Fire"] = Range(3, 5, K);
    numOfResourcesOnField["Fortify"] = Range(3, 5, K);
    numOfResourcesOnField["Siege"] = Range(3, 5, K);
    numOfResourcesOnField["Cure"] = Range(3, 5, K);
}
void GameRules::formPlayersList(Random *rand)
{
    // составляем список цветов игроков
    QList <QString> vars = AllPlayers;

    for (int i = 0; i < numOfPlayers; ++i)
    {
        int k = rand->next() % vars.size();
        players << vars[k];
        vars.removeAt(k);
    }
}

// консервация и деконсервация в список int-ов
// сервер использует тот факт, что первое число - число игроков.
QList<qint32> GameRules::get()
{
    QList <qint32> ans;
    ans << numOfPlayers << qint32(timerTypeIndex(timer)) << fieldW << fieldH;

    ans << 0;
    foreach (QString u, AllUnits)
    {
        ans[4] <<= 1;
        if (unitsInGame.contains(u))
            ans[4] |= 1;
    }

    ans << 0;
    foreach (QString r, AllOrders)
    {
        ans[5] <<= 1;
        if (ordersInGame.contains(r))
            ans[5] |= 1;
    }

    ans << (changingDayTimes ? 179 : dayTimes);

    return ans;
}
GameRules::GameRules(QList<qint32> hash)
{
    numOfPlayers = hash[0];
    timer = AllTimerTypes[hash[1]];
    fieldW = hash[2];
    fieldH = hash[3];

    for (int i = AllUnits.size() - 1; i >= 0; --i)
    {
        if (hash[4] % 2)
            unitsInGame.push_back(AllUnits[i]);
        hash[4] >>= 1;
    }

    for (int i = AllOrders.size() - 1; i >= 0; --i)
    {
        if (hash[5] % 2)
            ordersInGame.push_back(AllOrders[i]);
        hash[5] >>= 1;
    }

    changingDayTimes = hash[6] == 179;
    dayTimes = hash[6] == 179 ? 1 : hash[6];

    setTestOptions();
    recountGenerationParameters();
}

// чтение и запись
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
            stream >> r->numOfPlayers;
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
           << r->timerTypeIndex(r->timer)
            << r->numOfPlayers
            << r->ordersInGame
            << r->unitsInGame
            << r->dayTimes << r->changingDayTimes;
    return stream;
}
QDataStream &operator >>(QDataStream &stream, GameRules * r)
{
    qint8 timer_index;
    stream >> r->fieldH >> r->fieldW
            >> timer_index
            >> r->numOfPlayers
            >> r->ordersInGame
            >> r->unitsInGame
            >> r->dayTimes >> r->changingDayTimes;

    r->timer = r->AllTimerTypes[timer_index];
    r->recountGenerationParameters();
    return stream;
}

