#include "GameRules.h"
#include "GameUnit.h"

QList <QString> GameRules::AllPlayers = {"Blue", "Red", "Green", "Yellow"};
QList <QString> GameRules::AllOrders = {"Capture", "Recruit", "Liberate", "Agite", "Attack", "Go", "Retreat", "Siege", "Fire", "Pursue", "Cure", "Fortify"};
QList <QString> GameRules::AllUnits = {"Scrat", "Pig", "Hippo", "Karkun", "Bruter", "Mumusha"};

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

    NumOfPlayers = 2;

    ordersInGame << "Capture" << "Recruit"  << "Liberate"
                 << "Attack" << "Go" << "Siege"
                 << "Fire" << "Cure" << "Fortify";

    dayTimes = 3;
    changingDayTimes = false;

    unitsInGame << "Scrat" << "Pig" << "Karkun" << "Hippo";

    // настройка обучающей игры
    limitatingResources = true;

    setTestOptions();
    recountGenerationParameters();
}
void GameRules::setTestOptions()
{
    // тестовые настройки
    damageToHomellesUnits = 179;
    fortsBurnCaptures = false;
    captureIsLiberate = false;
    recruitsInMills = true;
    capturesInForts = true;
    peacefullOrdersBurns = false;
    everything_is_starting = true;
    start_choices = 7;
    mill_connections = true;
    rivers = true;
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
                      >> s >> limitatingResources
                      >> s >> fortsBurnCaptures
                      >> s >> captureIsLiberate
                      >> s >> capturesInForts
                      >> s >> recruitsInMills
                      >> s >> peacefullOrdersBurns
                      >> s >> everything_is_starting
                      >> s >> start_choices
                      >> s >> mill_connections
                      >> s >> rivers
                      >> s >> fort_gp_min >> fort_gp_max
                      >> s >> mill_gp_min >> mill_gp_max
                      >> s >> river_gp_min >> river_gp_max
                      >> s >> K_change;

        if(tstream.status() != QTextStream::Ok)
        {
            qDebug() << "ERROR: Ошибка чтения файла особых настроек\n";
        }
        hostFile->close();
    }

    hostFile = new QFile("Experiments_options.txt");
    if (hostFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream tstream(hostFile);
        tstream << "DamageToHomelessUnits: " << damageToHomellesUnits << endl
                      << "MillsLimitResources: " << limitatingResources << endl
                      << "FortsBurnEnemyCaptures: " << fortsBurnCaptures << endl
                      << "CaptureIsAlsoLiberation: " << captureIsLiberate << endl
                      << "CapturesAreInForts: " << capturesInForts << endl
                      << "RecruitsAreInMills: " << recruitsInMills << endl
                      << "PeacefullOrdersBurns: " << peacefullOrdersBurns << endl
                      << "EverythingCanBeStart: " << everything_is_starting << endl
                      << "NumOfStartingChoices: " << start_choices << endl
                      << "RecruitWhenConnectedToMill: " << mill_connections << endl
                      << "Rivers: " << rivers << endl
                      << "FortsGenerationLimits: " << fort_gp_min << " " << fort_gp_max << endl
                      << "MillsGenerationLimits: " << mill_gp_min << " " << mill_gp_max << endl
                      << "RiversGenerationLimits: " << river_gp_min << " " << river_gp_max << endl
                      << "ResourcesAmountCoeff: " << K_change;
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
    doesEnteringEnemyHexLiberates = !ordersInGame.contains("Liberate") && !captureIsLiberate;
    if (changingDayTimes)
        dayTimes = 1;

    qreal K = (fieldW * fieldH) / 60.0;  // во сколько раз увеличить кол. всех ресурсов

    // диапазоны генерации фортов, мельниц и гор
    numOfRivers = Range(river_gp_min, river_gp_max, K);
    numOfHexTypes["Fort"] = Range(fort_gp_min, fort_gp_max, K);
    numOfHexTypes["Mill"] = Range(mill_gp_min, mill_gp_max, K);
    numOfHexTypes["Mountain"] = Range(5, 12, K);

    // для количества генерящихся приказов приходится использовать магию
    int cb = 0;
    if (!ordersInGame.contains("Recruit"))
        ++cb;

    int ab = 0;
    if (ordersInGame.contains("Retreat"))
        --ab;

    int cub = 0;
    if (ordersInGame.contains("Mumusha"))
        ++cub;

    K *= K_change;

    numOfResourcesOnField["Capture"] = Range(9 + cb, 13 + cb, K);
    numOfResourcesOnField["Go"] = Range(6 + ab, 11 + ab, K);
    numOfResourcesOnField["Recruit"] = Range(6, 10, K);
    numOfResourcesOnField["Liberate"] = Range(6, 10, K);
    numOfResourcesOnField["Attack"] = Range(5 + ab, 9 + ab, K);
    numOfResourcesOnField["Agite"] = Range(3, 8, K);
    numOfResourcesOnField["Retreat"] = Range(3, 6, K);
    numOfResourcesOnField["Pursue"] = Range(3, 6, K);
    numOfResourcesOnField["Fire"] = Range(3, 5, K);
    numOfResourcesOnField["Fortify"] = Range(3, 5, K);
    numOfResourcesOnField["Siege"] = Range(3, 5, K);
    numOfResourcesOnField["Cure"] = Range(3 + cub, 5 + cub, K);

    if (recruitsInMills)
        numOfResourcesOnField["Recruit"] = Range(0, 0, K);
    if (capturesInForts)
        numOfResourcesOnField["Capture"] = Range(0, 0, K);
}
void GameRules::FormPlayersList(Random *rand)
{
    // составляем список цветов игроков
    QList <QString> vars = AllPlayers;

    for (int i = 0; i < NumOfPlayers; ++i)
    {
        int k = rand->next() % vars.size();
        players << vars[k];
        vars.removeAt(k);
    }
}

// консервация и деконсервация в список int-ов
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

    setTestOptions();
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

