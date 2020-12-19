#include "HelpManager.h"
#include "Game/GameRules.h"
#include "Game/GameOrder.h"

QVector <QString> HelpManager::HelpInfo(QString name, GameRules * rules)
{
    if (name == "Capture")
    {
        QVector<QString> ans;
        if (rules == NULL)
        {
            ans << "<u>Захват</u> - захватывает клетку, в которой находится юнит. Если в игре нету приказа рекрута, также рекрутирует нового юнита в клетке.";
        }
        else if (GameOrderParameters(rules, "Scrat", "Capture").containsAction(GameAction::RECRUIT))
        {
               ans << "<u>Захват</u> - захватывает клетку, в которой находится юнит, и рекрутирует нового юнита выбранного типа, который появится в ней в конце раунда.";
        }
        else
        {
              ans << "<u>Захват</u> - захватывает клетку, в которой находится юнит.";
        }
        ans << "В случае боя юнит получает штраф -1.";
        //ans << "Приказ сжигается, если в момент исполнения в соседней клетке есть уже захваченный противником форт.";
        return ans;
    }
    if (name == "Recruit")
    {
        QVector<QString> ans;
        ans << "<u>Рекрут</u> - рекрутирует в клетке нового юнита выбранного типа."
            << "Клетка должна быть захвачена и соединятся вашими территориями с мельницей."
            << "Новый юнит появится в клетке в конце раунда. Враг не должен нарушить соединение до конца раунда."
            << "В каждой клетке можно создать за игру не более одного юнита.";

        if (rules == NULL)
            ans << "Если этот приказ включён в игре, при захвате клетки новый юнит не рекрутируется.";

        ans << "В случае боя юнит получает штраф -1.";

        return ans;
    }
    if (name == "Liberate")
    {
        QVector<QString> ans;
        ans << "<u>Освобождение</u> - клетка, в которой находится юнит, перестаёт принадлежать противнику.";
        ans << "В конце раунда юнит противника, созданный в освобождённой клетке, погибнет.";

        if (rules == NULL)
            ans << "Если этот приказ включён в игру, при входе в клетку противника она не перестаёт ему принадлежать.";

        ans << "В случае боя юнит получает штраф -1.";

        return ans;
    }
    if (name == "Agite")
    {
        QVector<QString> ans;
        ans << "<u>Агитация</u> - выбранная соседняя клетка не принесёт оппонентам никаких ресурсов в конце раунда";
        ans << "Также запрещает оппонентам рекрутирование на этой клетке до конца раунда";
        ans << "В случае боя юнит получает штраф -1.";

        return ans;
    }
    if (name == "Attack") return QVector<QString>()
                << "<u>Атака</u> - боевое перемещение в соседнюю клетку.";
    if (name == "Go") return QVector<QString>()
                << "<u>Ход</u> - перемещение в соседнюю клетку со штрафом в бою -1.";
    if (name == "Retreat") return QVector<QString>()
                << "<u>Отступление</u> - юнит, отстреливаясь, отступает в соседнюю клетку."
                << "Если противник в этот момент атакует исходную клетку, он получит 1 единицу урона."
                << "В клетке, в которую юнит отступает, юнит имеет в бою штраф -2.";
    if (name == "Fire") return QVector<QString>()
                << "<u>Выстрел</u> - наносит 2 единицы урона в выбранную клетку. Может использовать только хрюшка."
                << "Стрелять можно не далее чем на две клетки по прямой. Нельзя стрелять через форты, горы и мельницы."
                << "Если юнит перемещается, выстрел попадает, если стрельба велась по клетке, в которую юнит шёл."
                << "Если выстрел был по соседней клетке, с которой стрелявшего атаковали, выстрел попадает по атаковавшим."
                << "При попадании учитываются все боевые штрафы и бонусы.";
    if (name == "Siege") return QVector<QString>()
                << "<u>Штурм</u> - перемещение в соседнюю клетку с бонусом в бою +2. Может использовать только Каркун.";
    if (name == "Pursue") return QVector<QString>()
        << "<u>Преследование</u> - Если выбранный вражеский юнит оказывается в соседней клетке, атакует её, после чего возвращается обратно.";
    if (name == "Fortify") return QVector <QString>()
                << "<u>Укрепление</u> - строительство в клетке щита. Уже построенный щит даёт единоразовый бонус в обороне +1 и учитывается до бонусов в обороне фортов и бегемота."
                << "В случае боя юнит получает штраф -1.";
    if (name == "Cure") return QVector<QString>()
                << "<u>Лечение</u> - восстанавливает 1 единицу здоровья. Здоровье не может стать больше исходного."
                << "В случае боя юнит получает штраф -1.";


    if (name == "Scrat") return QVector<QString>()
            << "<u>Белка</u> - стандартный юнит. Полное здоровье - 5.";
    if (name == "Pig")  return QVector<QString>()
            << "<u>Хрюшка</u> - может использовать приказ выстрела. Полное здоровье - 4.";
    if (name == "Hippo") return QVector<QString>()
            << "<u>Бегемот</u> - имеет бонус в обороне +2. Полное здоровье - 4."
            << "Считается, что юнит находился в обороне, если он не перемещается в момент боя. Бонус восполняется в конце раунда.";
    if (name == "Karkun") return QVector<QString>()
            << "<u>Каркун</u> - может использовать приказ штурма. Полное здоровье - 4.";
    if (name == "Bruter") return QVector<QString>()
            << "<u>Брутер</u> - уничтожает всех союзников, попавших с ним в одну клетку. Полное здоровье - 6.";
    if (name == "Mumusha") return QVector<QString>()
            << "<u>Корова</u> - Полное здоровье - 7, но появляется с 3 здоровья.";

    if (name == "Fort") return QVector<QString>()
            << "<u>Форт</u>. Количество принадлежащих игроку фортов определяет лимит ресурсов одного типа. Излишки сжигаются в конце раунда. Даёт +2 в обороне.";
    if (name == "Mill") return QVector<QString>()
            << "<u>Мельница</u>. На соединённых с мельницами территориях можно проводить рекрутирование.";
    if (name == "Mountain") return QVector<QString>()
            << "<u>Гора</u>. По этой клетке нельзя перемещаться.";
    if (name == "Lake") return QVector<QString>()
            << "<u>Озеро</u>. По этой клетке нельзя перемещаться.";
    if (name == "Simple") return QVector<QString>()
            << "<u>Клетка с ресурсами</u>. Владелец этой клетки получает указанные на ней ресурсы в конце каждого раунда.";
    if (name == "River") return QVector<QString>()
            << "<u>Река</u>. После пересечения реки юнит не может выполнять других приказов до конца раунда.";


    if (name == "Fortification") return QVector<QString>()
            << "<u>Щит</u>. Даёт единоразовый бонус в обороне +1. Этот бонус учитывается до бонуса форта и бонусов юнита."
            << "Если противник входит в незащищённую юнитами клетку, все щиты на ней исчезают.";
    if (name == "UnitIsGoingToRecruit") return QVector<QString>()
            << "В этой клетке рекрутирован юнит. Он появится в конце раунда, если в клетку не войдёт противник.";
    if (name == "NotUnitHome")
    {
        QVector<QString> ans;
        if (rules->ordersInGame.contains("Recruit"))
            ans << "Юнит с домом в этой клетке ещё не был создан. Владелец может использовать приказ рекрута, чтобы получить нового юнита.";
        else
            ans << "Юнит с домом в этой клетке ещё не был создан. Владелец может снова использовать приказ захвата в этой клетке, чтобы получить нового юнита.";
        return ans;
    }
    if (name == "Tombstone") return QVector<QString>()
            << "Юнит с домом в этой клетке героически пал. Создавать юнитов здесь больше нельзя.";
    if (name == "NotConnected") return QVector<QString>()
            << "Клетка не подсоединена к мельнице. Рекрутировать здесь пока нельзя.";
    if (name.right(4) == "Turn") return QVector<QString>()
            << "Пока ещё непонятно, какой приказ отдал противник...";

    if (name == "PlayersTune") return QVector<QString>()
            << "На этой панели можно изменить количество игроков - от 2 до 4.";
    if (name == "NumberOfPlayers") return QVector<QString>()
            << "Здесь отображается количество игроков в партии.";
    if (name == "FieldTune") return QVector<QString>()
            << "На этой панели можно изменить размер игрового поля.";
    if (name == "FieldSize") return QVector<QString>()
            << "Здесь отображается размер игрового поля.";
    if (name == "UnitsInGame") return QVector<QString>()
            << "Здесь отображается, какие юниты участвуют в игре.";
    if (name == "OrdersInGame") return QVector<QString>()
            << "Здесь отображается, какие приказы участвуют в игре.";
    if (name == "DayTimesTune") return QVector<QString>()
            << "На этой панели можно настроить количество времён дня в игре: от 1 до 5 или изменяющийся режим, в котором количество времён дня будет циклически изменяться от 1 до 5.";
    if (name == "DayTimesInGame") return QVector<QString>()
            << "Здесь отображается, сколько времён дня в игре.";


    if (name == "HowToAskHelp") return QVector<QString>()
            << "Помните: чтобы получить справку о любом элементе игры, нажмите на нём правой кнопкой мыши!";

    return QVector<QString>() << "Ой! Справка по этому вопросу куда-то делась!";
}
QString HelpManager::HelpPicture(QString name)
{
    if (name == "Capture") return "Capture";
    if (name == "Recruit") return "Recruit";
    if (name == "Liberate") return "Liberate";
    if (name == "Agite") return "Agite";
    if (name == "Attack" ) return "Attack";
    if (name == "Go") return "Go";
    if (name == "Retreat") return "Retreat";
    if (name == "Fire") return "Fire";
    if (name == "Siege") return "Siege";
    if (name == "Pursue") return "Pursue";
    if (name == "Fortify") return "Fortify";
    if (name == "Cure") return "Cure";

    if (name == "Scrat") return "Scrat";
    if (name == "Pig") return "Pig";
    if (name == "Karkun") return "Karkun";
    if (name == "Hippo") return "Hippo";
    if (name == "Bruter") return "Bruter";
    if (name == "Mumusha") return "Mumusha";

    if (name == "Fort") return "Fort";
    if (name == "Mill") return "Mill";
    if (name == "Mountain") return "Mountain";
    if (name == "Lake") return "Lake";
    if (name == "River") return "River";
    if (name == "Simple") return "NeutralHex";

    if (name == "Fortification") return "BlueFortification";
    if (name == "UnitIsGoingToRecruit") return "Scrat";
    if (name == "NotUnitHome") return "NotBlueUnitHome";
    if (name == "Tombstone") return "Tombstone";
    if (name == "NotConnected") return "NotConnected";

    if (name == "PlayersTune") return "PlayerOnline";
    if (name == "NumberOfPlayers") return "PlayerOnline";
    if (name == "FieldTune") return "NeutralHex";
    if (name == "FieldSize") return "NeutralHex";
    if (name == "UnitsInGame") return "Scrat";
    if (name == "OrdersInGame") return DefaultOrder;
    if (name == "DayTimesTune") return "Supper";
    if (name == "DayTimesInGame") return "Supper";

    if (name == "HowToAskHelp") return "Unknown";

    return "Error";
}
