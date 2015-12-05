#include "GameRules.h"
#include "Technical/ProgrammSettings.h"  // <undone> лишнее

GameRules::GameRules()
{
    fieldW = settings->W;
    fieldH = settings->H;
    qreal K = (fieldW * fieldH) / 40.0;  // во сколько раз увеличить кол. всех ресурсов

    players << "Blue" << "Red";

    ordersInGame << "Capture" << "Attack" << "Go" << "Siege"
                             << "Fire" << "Cure";// << "Fortify";

    numOfHexes["Fort"] = Range(3, 5, K);
    numOfHexes["Mill"] = Range(6, 8, K);
    numOfHexes["Mountain"] = Range(4, 8, K);

    numOfResources["Attack"] = Range(5, 7, K);
    numOfResources["Capture"] = Range(8, 12, K);
    numOfResources["Go"] = Range(7, 11, K);
    numOfResources["Fire"] = Range(3, 5, K);
    numOfResources["Fortify"] = Range(3, 5, K);
    numOfResources["Siege"] = Range(3, 5, K);
    numOfResources["Cure"] = Range(3, 5, K);

    startResources["Scrat"] << "Capture" << "Attack" << "Go";
    startResources["Karkun"] << "Capture" << "Siege" << "Go";
    startResources["Pig"] << "Capture" << "Fire" << "Go";
    startResources["Hippo"] << "Capture" << "Attack" << "Go";

    orderLimitPerMill = 1;

    unitsInGame << "Scrat" << "Pig" << "Hippo" << "Karkun";

    dayTimes << "Breakfast" << "Dinner" << "Supper";
}
