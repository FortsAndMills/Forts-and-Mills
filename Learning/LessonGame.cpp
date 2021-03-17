#include "LessonGame.h"

void LessonGame::GenerateField()
{
    // FAIL: костыль, очищающий предыдущее поле
    for (int x = 0; x < hexes.size(); ++x)
    {
        hexes[x].clear();
    }
    hexes.clear();

    // заполняем всё пустыми клетками
    HexType types[rules->fieldH][rules->fieldW];
    for (int x = 0; x < rules->fieldH; ++x)
    {
        for (int y = 0; y < rules->fieldW; ++y)
        {
            types[x][y] = "Simple";
        }
    }

    // карты уроков
    if (N == 0)
    {
        types[3][5] = "Fort";
        types[3][9] = "Mill";
        types[5][1] = "Mill";

        types[4][9] = "Lake";
        types[5][2] = "Lake";
    }
    if (N == 1)
    {
        types[4][6] = "Fort";
        types[3][5] = "Fort";
        types[0][0] = "Fort";
        types[0][1] = "Fort";

        types[4][3] = "Lake";
    }
    if (N == 2)
    {
        types[3][3] = "Fort";
        types[2][2] = "Fort";
        types[4][2] = "Mill";

        types[4][6] = "Fort";
        types[5][6] = "Fort";

        types[3][4] = "Lake";
        types[3][2] = "Mountain";
        types[1][3] = "Mountain";
        types[4][3] = "Lake";
    }
    if (N == 3)
    {
        types[4][5] = "Fort";
        types[0][1] = "Fort";
        types[2][2] = "Fort";
        types[2][3] = "Fort";
        types[5][2] = "Fort";

        types[3][0] = "Mill";
        types[2][0] = "Mill";
        types[1][3] = "Mill";
        types[3][5] = "Mill";

        types[4][0] = "Mountain";
        types[0][0] = "Mountain";
        types[0][3] = "Mountain";
        types[3][7] = "Lake";
        types[0][2] = "Lake";
    }

    for (int x = 0; x < rules->fieldH; ++x)
    {
        hexes << QList <GameHex *> ();
        for (int y = 0; y < rules->fieldW; ++y)
        {
            hexes[x] << new GameHex(types[x][y], Coord(x, y));
        }
    }

    // ресурсы и речки уроков
    if (N == 0)
    {
        hexes[4][1]->rivers[DOWN] = true;
        hexes[5][1]->rivers[UP] = true;
        hexes[4][1]->rivers[LEFT_DOWN] = true;
        hexes[5][0]->rivers[RIGHT_UP] = true;
        hexes[4][0]->rivers[DOWN] = true;
        hexes[5][0]->rivers[UP] = true;
    }
    if (N == 1)
    {
        hexes[4][6]->resources << "Capture";
        hexes[3][5]->resources << "Capture";
        hexes[0][0]->resources << "Capture";
        hexes[0][1]->resources << "Capture";

        hexes[3][3]->resources << "Attack" << "Go";
        hexes[4][7]->resources << "Go" << "Go";
        hexes[3][7]->resources << "Go" << "Go";
        hexes[4][4]->resources << "Liberate";

        hexes[3][3]->rivers[RIGHT_DOWN] = true;
        hexes[4][4]->rivers[LEFT_UP] = true;
    }
    if (N == 2)
    {
        hexes[2][3]->resources << "Attack";
        hexes[3][5]->resources << "Attack" << "Liberate";
        hexes[4][4]->resources << "Attack" << "Go" << "Go";

        hexes[3][3]->resources << "Capture";
        hexes[2][2]->resources << "Capture";
        hexes[4][6]->resources << "Capture";
        hexes[5][6]->resources << "Capture";
        hexes[4][2]->resources << "Recruit";

        hexes[2][3]->rivers[RIGHT_UP] = true;
        hexes[2][4]->rivers[LEFT_DOWN] = true;
    }
    if (N == 3)
    {
        hexes[4][5]->resources << "Capture";
        hexes[0][1]->resources << "Capture";
        hexes[2][2]->resources << "Capture";
        hexes[2][3]->resources << "Capture";
        hexes[5][2]->resources << "Capture";

        hexes[3][0]->resources << "Recruit";
        hexes[2][0]->resources << "Recruit";
        hexes[1][3]->resources << "Recruit";
        hexes[3][5]->resources << "Recruit";

        hexes[1][0]->resources << "Attack";
        hexes[1][2]->resources << "Liberate";
        hexes[3][2]->resources << "Go";
        hexes[3][3]->resources << "Attack" << "Go";

        hexes[3][4]->resources << "Go";
        hexes[1][1]->resources << "Attack" << "Attack" << "Liberate";
        hexes[3][6]->resources << "Go" << "Go";

        hexes[4][1]->resources << "Go";
        hexes[4][2]->resources << "Liberate";
        hexes[4][3]->resources << "Attack" << "Liberate";

        hexes[4][4]->resources << "Go";
        hexes[4][6]->resources << "Attack" << "Attack";
        hexes[4][7]->resources << "Go";

        hexes[1][3]->rivers[RIGHT_UP] = true;
        hexes[1][4]->rivers[LEFT_DOWN] = true;
        hexes[1][3]->rivers[RIGHT_DOWN] = true;
        hexes[2][4]->rivers[LEFT_UP] = true;
        hexes[2][3]->rivers[RIGHT_UP] = true;
        hexes[2][4]->rivers[LEFT_DOWN] = true;
        hexes[3][4]->rivers[UP] = true;
        hexes[2][4]->rivers[DOWN] = true;
        hexes[3][4]->rivers[RIGHT_UP] = true;
        hexes[2][5]->rivers[LEFT_DOWN] = true;
        hexes[3][5]->rivers[UP] = true;
        hexes[2][5]->rivers[DOWN] = true;
        hexes[3][5]->rivers[RIGHT_UP] = true;
        hexes[3][6]->rivers[LEFT_DOWN] = true;
        hexes[4][6]->rivers[UP] = true;
        hexes[3][6]->rivers[DOWN] = true;

        hexes[1][0]->rivers[LEFT_UP] = true;
        hexes[1][0]->rivers[LEFT_DOWN] = true;
        hexes[2][0]->rivers[LEFT_UP] = true;
        hexes[2][0]->rivers[LEFT_DOWN] = true;
        hexes[3][0]->rivers[LEFT_UP] = true;
        hexes[3][0]->rivers[LEFT_DOWN] = true;
        hexes[0][9]->rivers[RIGHT_DOWN] = true;
        hexes[1][9]->rivers[RIGHT_UP] = true;
        hexes[1][9]->rivers[RIGHT_DOWN] = true;
        hexes[2][9]->rivers[RIGHT_UP] = true;
        hexes[2][9]->rivers[RIGHT_DOWN] = true;
        hexes[3][9]->rivers[RIGHT_UP] = true;
    }
}

void LessonGame::StartGame()
{
    if (N == 0)
    {
        rules->doesCaptureRecruits = false;
        rules->limitatingResources = false;

        unit1 = NewUnit(players[rules->players[0]], "Scrat", Coord(4, 1));
        unit2 = NewUnit(players[rules->players[0]], "Scrat", Coord(2, 3));
        enemy_unit1 = NewUnit(players[rules->players[1]], "Pig", Coord(4, 7));

        players[rules->players[0]]->resources["Capture"] = 6;
        players[rules->players[0]]->resources["Attack"] = 6;
        players[rules->players[1]]->resources["Attack"] = 6;

        StartPlanning();
        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));

        AddEvent()->HidePlayerPanels();
        AddEvent()->HidePlayerForces();
        AddEvent()->HideGo();
        AddEvent()->HideDayTimes();
        AddEvent()->HideFieldControl();
        AddEvent()->HideHomes();

        // Вступление
        text["rus"] = "Добро пожаловать в «Форты и мельницы», стратегию с одновременными ходами!";
        text["eng"] = "Welcome to «Forts and Mills», simultaneous turn-based strategy!";
        AddEvent()->ShowMessage(text);

        text["rus"] = "Каждый раунд вам предстоит придумать план: раздать всем своим юнитам приказы, кому что делать.";
        text["eng"] = "Each round players come up with a plan by giving orders to all their units.";
        AddEvent()->ShowMessage(text);

        text["rus"] = "Мы играем за синих. Давайте попробуем захватить расположенный на карте форт.";
        text["eng"] = "Our units are blue. Let's try to capture a fort nearby.";
        AddEvent()->ShowMessageAndPicture(text, "Fort");

        // Отдаём приказ атаки на завтрак
        text["rus"] = "Этот юнит не доберётся до форта...";
        text["eng"] = "This unit will not be able to reach the fort...";
        AddEvent()->ReactionOnUnitClick(unit1, Reaction(BLOCK, text));

        text["rus"] = "Это вражеский юнит. Ему отдавать приказы будет наш противник.";
        text["eng"] = "This is enemy unit. Our opponent is giving him orders right now, but they remain a mystery to us.";
        AddEvent()->ReactionOnUnitClick(enemy_unit1, Reaction(BLOCK, text));
        AddEvent()->ReactionOnUnitClick(unit2, Reaction(NEXT_PHASE));

        text["rus"] = "Каждому юниту нам нужно выдать три приказа. Чтобы отдать юниту приказ, по нему нужно кликнуть.";
        text["eng"] = "Every unit must be given three orders. To give order, click on the unit.";
        AddEvent()->ShowMessage(text, false);

        // Реакции на приказы
        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));

        text["rus"] = "Это приказ захвата. Нам нужна атака!";
        text["eng"] = "This is capture order. We need attack!";
        AddEvent()->ReactionOnOrderChoice("Capture", Reaction(BLOCK, text, "Attack"));
        AddEvent()->ReactionOnOrderChoice("Attack", Reaction(NEXT_PHASE));

        text["rus"] = "Это приказ пропуска хода. Он выставляется по умолчанию. Нам нужна атака!";
        text["eng"] = "This is inaction order. It is set by default. We need attack!";
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(BLOCK, text, "Attack"));

        text["rus"] = "Эта кнопка отменяет последний приказ. Выберите атаку!";
        text["eng"] = "This button cancels the last order. Select attack!";
        AddEvent()->ReactionOnOrderChoice(DeleteLastOrder, Reaction(BLOCK, text, "Attack"));

        text["rus"] = "Теперь выберем приказ, который мы хотим отдать - приказ атаки в соседнюю клетку.";
        text["eng"] = "Now select order of attacking adjacent hex.";
        AddEvent()->ShowMessageAndPicture(text, "Attack", false, true);

        // Выбор соседней клетки
        text["rus"] = "Нам нужно добраться до форта за две атаки, поэтому стоит пойти в другом направлении!";
        text["eng"] = "We need to reach the fort as fast as we can, so we should go  in the other direction!";
        AddEvent()->DefaultHexChoiceReaction(Reaction(BLOCK, text, "Fort"));
        AddEvent()->ReactionOnHexChoice(hexes[3][4], Reaction(NEXT_PHASE));

        text["rus"] = "Осталось выбрать клетку, которую юнит атакует. Нам нужно идти к форту.";
        text["eng"] = "Now select a hex which you want to attack. We want to go to the fort.";
        AddEvent()->ShowMessageAndPicture(text, "Fort", false, true);

        // Отдаём приказ на обед
        text["rus"] = "Это приказ захвата. Нам нужна атака!";
        text["eng"] = "This is capture order. We need attack!";
        AddEvent()->ReactionOnOrderChoice("Capture", Reaction(BLOCK, text, "Attack"));

        text["rus"] = "Это приказ пропуска хода. Он выставляется по умолчанию. Нам нужна атака!";
        text["eng"] = "This is inaction order. It is set by default. We need attack!";
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(BLOCK, text, "Attack"));

        text["rus"] = "Эта кнопка отменяет последний приказ. Выберите атаку!";
        text["eng"] = "This button cancels the last order. Select attack!";
        AddEvent()->ReactionOnOrderChoice(DeleteLastOrder, Reaction(BLOCK, text, "Attack"));

        text["rus"] = "Первый приказ запланирован. Теперь можно отдать ему второй приказ - атаку в форт.";
        text["eng"] = "First order is given! Now we can give this unit a second one: to attack the fort.";
        AddEvent()->ReactionOnOrderChoice("Attack", Reaction(NEXT_PHASE));
        AddEvent()->ShowMessageAndPicture(text, "Attack", false, true);

        text["rus"] = "Нам нужно атаковать форт!";
        text["eng"] = "We need to attack the fort!";
        AddEvent()->DefaultHexChoiceReaction(Reaction(BLOCK, text, "Fort"));
        AddEvent()->ReactionOnHexChoice(hexes[3][5], Reaction(NEXT_PHASE));

        text["rus"] = "Выбираем атаковать форт!";
        text["eng"] = "Select attacking the fort!";
        AddEvent()->ShowMessageAndPicture(text, "Fort", false);

        // Отдаём приказ захвата
        text["rus"] = "Это приказ атаки. Нам нужен захват!";
        text["eng"] = "This is attack order. We need capture!";
        AddEvent()->ReactionOnOrderChoice("Attack", Reaction(BLOCK, text, "Capture"));

        text["rus"] = "Это приказ пропуска хода. Он выставляется по умолчанию. Нам нужна захват!";
        text["eng"] = "This is inaction order. It is set by default. We need capture!";
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(BLOCK, text, "Capture"));

        text["rus"] = "Эта кнопка отменяет последний приказ. Выберите захват!";
        text["eng"] = "This button cancels the last order. Select capture!";
        AddEvent()->ReactionOnOrderChoice(DeleteLastOrder, Reaction(BLOCK, text, "Capture"));

        text["rus"] = "Наконец, третьим выполним приказ захвата клетки. Тогда форт будет наш!";
        text["eng"] = "Finally, our third order will be to capture this hex. Then we'll have the fort!";
        AddEvent()->ReactionOnOrderChoice("Capture", Reaction(NEXT_PHASE));
        AddEvent()->ShowMessageAndPicture(text, "Capture", false, true);

        // Переходим к другому юниту
        AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));

        text["rus"] = "Этому юниту мы уже составили план. У нас есть ещё один юнит, план для которого пока не составлен.";
        text["eng"] = "We have already finished planning for this unit. We have another unit waiting for plan.";
        AddEvent()->ReactionOnUnitClick(unit2, Reaction(BLOCK, text));

        text["rus"] = "Это вражеский юнит. Ему отдавать приказы будет наш противник.";
        text["eng"] = "This is enemy unit. Our opponent is giving him orders right now, but they remain a mystery to us.";
        AddEvent()->ReactionOnUnitClick(enemy_unit1, Reaction(BLOCK, text));

        text["rus"] = "Щёлкните по другому нашему юниту. Поскольку ему ещё не отдано ни одного приказа, интерфейс автоматически откатится к первому действию.";
        text["eng"] = "Click on our second unit. Since he was not given any orders yet, our interface will automatically switch to the first time slot.";
        AddEvent()->ReactionOnUnitClick(unit1, Reaction(NEXT_PHASE));
        AddEvent()->ShowMessage(text, false, false, true);

        // отдаём первый приказ второму юниту
        text["rus"] = "Это приказ захвата. Нам нужна атака!";
        text["eng"] = "This is capture order. We need attack!";
        AddEvent()->ReactionOnOrderChoice("Capture", Reaction(BLOCK, text, "Attack"));

        text["rus"] = "Это приказ пропуска хода. Он выставляется по умолчанию. Нам нужна атака!";
        text["eng"] = "This is inaction order. It is set by default. We need attack!";
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(BLOCK, text, "Attack"));

        text["rus"] = "Эта кнопка отменяет последний приказ. Выберите атаку!";
        text["eng"] = "This button cancels the last order. Select attack!";
        AddEvent()->ReactionOnOrderChoice(DeleteLastOrder, Reaction(BLOCK, text, "Attack"));

        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
        AddEvent()->ReactionOnOrderChoice("Attack", Reaction(NEXT_PHASE));

        text["rus"] = "Вторым юнитом попробуем заполучить мельницу!";
        text["eng"] = "Let's capture a mill with our second unit!";
        AddEvent()->ShowMessageAndPicture(text, "Mill", false, false);

        text["rus"] = "Давайте пойдём в мельницу, расположенную клеткой ниже.";
        text["eng"] = "Let's go to the mill located below!";
        AddEvent()->DefaultHexChoiceReaction(Reaction(BLOCK, text, "Mill"));
        AddEvent()->ReactionOnHexChoice(hexes[5][1], Reaction(NEXT_PHASE));

        text["rus"] = "В озёра ходить нельзя.";
        text["eng"] = "Units can't go into lakes.";
        AddEvent()->ShowMessageAndPicture(text, "Lake", false, false);

        // Речка, циклическое поле и отмотка назад.        
        text["rus"] = "Однако, юнит пересёк речку, и поэтому ему больше нельзя выдать приказы.";
        text["eng"] = "However, unit can't execute any orders after crossing the river.";
        AddEvent()->ShowMessageAndPicture(text, "River");

        text["rus"] = "Значит, эту мельницу нам не захватить. Есть другая мельница, но она далеко... Или нет?";
        text["eng"] = "This means we can't capture this mill. There is another one, but it is far away... is it really?";
        AddEvent()->ShowMessageAndPicture(text, "Mill");

        AddEvent()->ShowFieldControl();

        text["rus"] = "Вовсе нет, потому что карта циклическая. Для удобства её можно прокручивать стрелками внизу.";
        text["eng"] = "The map is cyclic, so the second mill is closer than it looks. You can scroll the map using arrows at the bottom.";
        AddEvent()->ShowMessage(text);

        text["rus"] = "Итак, нам надо стереть неверный приказ атаки и пойти в другом направлении.";
        text["eng"] = "So, we need to cancel our last attack order and go in another direction.";
        AddEvent()->ShowMessage(text);

        text["rus"] = "Выбирая юнита, у которого на данное время дня уже что-то запланировано, вы отменяете приказ вместе со всем последующим планом для этого юнита.";
        text["eng"] = "When you click on the unit which already has some plan for this time slot, you cancel it along with all further orders.";
        AddEvent()->ShowMessage(text);

        AddEvent()->ShowDayTimes();

        text["rus"] = "Вы можете переключаться между временами дня на специальной панели внизу экрана.";
        text["eng"] = "You can switch between time slots using a time panel at the bottom.";
        AddEvent()->ShowMessage(text);

        // отменяем неверный приказ второму юниту
        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
        AddEvent()->ReactionOnUnitClick(unit1, Reaction(NEXT_PHASE));

        text["rus"] = "У этого юнита с планом всё в порядке; не надо его стирать!";
        text["eng"] = "Plan for this unit is good and you don't need to cancel it!";
        AddEvent()->ReactionOnUnitClick(unit2, Reaction(BLOCK, text));

        text["rus"] = "Это вражеский юнит. Ему отдавать приказы будет наш противник.";
        text["eng"] = "This is enemy unit. Our opponent is giving him orders right now, but they remain a mystery to us.";
        AddEvent()->ReactionOnUnitClick(enemy_unit1, Reaction(BLOCK, text));

        text["rus"] = "Кликнем на юните ещё раз, чтобы стереть неудачную атаку и начать планировать ещё раз.";
        text["eng"] = "Click again on our unit to cancel wrong attack order and start planning again.";
        AddEvent()->ShowMessage(text, false, false, false);

        // Даём творчество игроку
        AddEvent()->ShowGo();

        AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
        AddEvent()->ReactionOnUnitClick(unit1, Reaction(DEFAULT));

        text["rus"] = "Приказ пропуска хода выставляется по умолчанию - его не обязательно выставлять вручную, если только юнит не собирается делать что-то другое после него.";
        text["eng"] = "Inaction is default order; you do not need to give it manually unless you want unit to skip one time slot and do something different afterwards.";
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(DEFAULT, text));

        text["rus"] = "Итак, наша задача - захватить форт и мельницу. Когда ваш план будет готов, нажимайте кнопку Go!";
        text["eng"] = "So, our goal is to capture a fort and a mill. When your plan is ready, press Go button!";
        AddEvent()->ShowMessage(text, false, false, true, QSet<GameUnit*>(), "BlueGoButton");
    }
    if (N == 1)
    {
        rules->doesCaptureRecruits = false;

        unit1 = NewUnit(players[rules->players[0]], "Scrat", Coord(3, 1));
        enemy_unit1 = NewUnit(players[rules->players[1]], "Pig", Coord(3, 4));
        enemy_unit2 = NewUnit(players[rules->players[1]], "Scrat", Coord(1, 3));

        CaptureHex(hexes[0][0], rules->players[0]);
        CaptureHex(hexes[0][1], rules->players[0]);
        CaptureHex(hexes[3][5], rules->players[1]);
        CaptureHex(hexes[3][7], rules->players[1]);
        CaptureHex(hexes[4][6], rules->players[1]);
        CaptureHex(hexes[4][7], rules->players[1]);
        GatherResources(hexes[3][1], rules->players[0], QList<QString>() << "Capture" << "Capture" << "Attack" << "Attack");
        GatherResources(hexes[3][4], rules->players[1], QList<QString>() << "Capture" << "Capture" << "Go" << "Go");

        AddEvent()->Plan();
        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
        AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
        text["rus"] = "Приказ пропуска хода выставляется по умолчанию - его не обязательно выставлять вручную, если только юнит не собирается делать что-то другое после него.";
        text["eng"] = "Inaction is default order; you do not need to give it manually unless you want unit to skip one time slot and do something different afterwards.";
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(DEFAULT, text));

        AddEvent()->HideGo();
        AddEvent()->HideHomes();
        AddEvent()->HidePlayerForces();

        // Вступление
        text["rus"] = "Все приказы, которые вы отдаёте своим юнитам, являются расходуемыми ресурсами.";
        text["eng"] = "All orders that players give their units are expendable resources.";
        AddEvent()->ShowMessage(text);

        text["rus"] = "Каждый игрок знает свои и чужие ресурсы. Они отображаются на боковых панелях.";
        text["eng"] = "All players know the amount of resources of every player. They are displayed on side panels.";
        AddEvent()->ShowMessage(text);

        text["rus"] = "Мы снова играем за синих, и у нас есть две атаки и два захвата. Это сильно ограничивает наши действия.";
        text["eng"] = "We are again the blue player and we have two attack orders and two captures. This limits our possibilities...";
        AddEvent()->ShowMessage(text);

        text["rus"] = "У нас даже нет источников ресурса передвижения! Нам срочно нужны клетки с ресурсами!";
        text["eng"] = "We do not even have the sources of new movement orders! We have to find new hexes with resources!";
        AddEvent()->ShowMessage(text);

        text["rus"] = "Захватив клетку, вы будете получать с неё указанные ресурсы в конце каждого раунда.";
        text["eng"] = "Players gather resources from all their captured hexes at the end of each round.";
        AddEvent()->ShowMessage(text);

        AddEvent()->ShowGo();
        AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));

        text["rus"] = "Это вражеский юнит. Мы играем за синих.";
        text["eng"] = "This is enemy's unit. We are playing as blue player.";
        AddEvent()->ReactionOnUnitClick(enemy_unit1, Reaction(DEFAULT, text));
        AddEvent()->ReactionOnUnitClick(enemy_unit2, Reaction(DEFAULT, text));

        text["rus"] = "Давайте захватим клетку с ресурсами! Вперёд!";
        text["eng"] = "Let's capture the hex with resources! Go ahead!";
        AddEvent()->ShowMessage(text, false);
    }
    if (N == 2)
    {
        rules->doesCaptureRecruits = false;

        CaptureHex(hexes[2][3], rules->players[0]);
        unit1 = NewUnit(players[rules->players[0]], "Pig", Coord(2, 3));

        CaptureHex(hexes[3][3], rules->players[0]);
        unit2 = NewUnit(players[rules->players[0]], "Scrat", Coord(4, 8));
        SwitchHome(unit2, Coord(3, 3));

        CaptureHex(hexes[4][2], rules->players[0]);
        unit3 = NewUnit(players[rules->players[0]], "Scrat", Coord(3, 3));
        SwitchHome(unit3, Coord(4, 2), GameHex::HOME);

        CaptureHex(hexes[3][5], rules->players[1]);
        enemy_unit1 = NewUnit(players[rules->players[1]], "Scrat", Coord(3, 5));

        CaptureHex(hexes[4][4], rules->players[1]);
        enemy_unit2 = NewUnit(players[rules->players[1]], "Scrat", Coord(3, 5));
        SwitchHome(enemy_unit2, Coord(4, 4), GameHex::HOME);

        CaptureHex(hexes[4][6], rules->players[1]);
        NewUnit(players[rules->players[1]], "Pig", Coord(4, 6));
        CaptureHex(hexes[5][6], rules->players[1]);
        NewUnit(players[rules->players[1]], "Pig", Coord(5, 6));
        gatherResources();

        AddEvent()->HidePlayerForces();

        AddEvent()->Plan();
        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
        AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));

        text["rus"] = "Да, если мы захватим эту клетку, она будет соединена с нашей мельницей!";
        text["eng"] = "Yes, if we capture this hex, it will be connected to our mill!";
        AddEvent()->ReactionOnHexChoice(hexes[2][2], Reaction(DEFAULT, text));

        text["rus"] = "После пересечения речки юнит не может выполнять других приказов. В этом раунде мы не сможем здесь рекрутировать.";
        text["eng"] = "Unit can't execute any orders after crossing the river, so we can't recruit in this hex in this round.";
        AddEvent()->ReactionOnHexChoice(hexes[2][4], Reaction(DEFAULT, text));

        text["rus"] = "Из этой клетки уже рекрутировал наш враг. Мы даже освободить её не сможем!";
        text["eng"] = "Our enemy has already recruited a unit on this hex, so we are too late.";
        AddEvent()->ReactionOnHexChoice(hexes[4][4], Reaction(DEFAULT, text));

        text["rus"] = "В этой клетке юнит уже создан.";
        text["eng"] = "We have already recruited from this hex.";
        AddEvent()->ReactionOnHexChoice(hexes[4][2], Reaction(DEFAULT, text));

        //text["rus"] = "При рекрутировании нужно выбрать тип создаваемого юнита. Пока выберите любой.";
        //text["eng"] = "When you recruit, you have to choose a type of unit to create. For now, choose any!";
        //AddEvent()->ReactionOnOrderChoice("Recruit", Reaction(DEFAULT, text));

        AddEvent()->HideGo();

        // Вступление
        text["rus"] = "У каждого юнита есть свой дом - клетка, в которой он родился.";
        text["eng"] = "Each unit has a home: a hex, where it was created.";
        AddEvent()->ShowMessageAndPicture(text, "BlueUnitHome");

        text["rus"] = "Чтобы узнать, где находится дом юнита, наведите на него мышкой.";
        text["eng"] = "To find out, where is unit's home, hover mouse over it.";
        AddEvent()->ShowMessageAndPicture(text, "BlueUnitHome");

        text["rus"] = "За всю игру в каждой клетке можно получить лишь одного юнита. Для этого нужно рекрутирование.";
        text["eng"] = "During the game each hex can provide only one unit to only one of the players. Recruiting is a dedicated order/resource.";
        AddEvent()->ShowMessageAndPicture(text, "Recruit");

        text["rus"] = "Клетка при этом должна быть захвачена и соединена вашими территориями с вашей мельницей.";
        text["eng"] = "To be able to recruit, the hex must be captured and connected to one of your mills through your territories.";
        AddEvent()->ShowMessageAndPicture(text, "Mill");

        AddEvent()->ShowGo();
        AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));

        text["rus"] = "Рядом нет клеток, которые мы могли бы соединить со своей мельницей.";
        text["eng"] = "There are no hexes around, which we could connect with our mill.";
        AddEvent()->ReactionOnUnitClick(unit2, Reaction(DEFAULT, text));

        text["rus"] = "В горы, как и в озёра, ходить нельзя.";
        text["eng"] = "Units can't go to mountains, same as with lakes.";
        AddEvent()->ReactionOnOrderChoice("Attack", Reaction(DEFAULT, text, "Mountain"));

        text["rus"] = "Давайте рекрутируем нового юнита. Для это нужно найти клетку, где мы можем провести рекрутирование!";
        text["eng"] = "Let's recruit a new unit! Find an appropriate hex, capture it and use recruit order!";
        AddEvent()->ShowMessage(text, false);
    }
    if (N == 3)
    {
        players["Red"]->playerType = GamePlayer::AI;

        ChooseOneOfTheStartHexes();

        AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));
        AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));

        AddEvent()->HideGo();
        text["rus"] = "Каждая партия начинается с того, что игроки выбирают место своего старта на карте.";
        text["eng"] = "Each game starts with players selecting their starting hexes.";
        AddEvent()->ShowMessage(text);

        text["rus"] = "Выбранная клетка будет захвачена, в ней появится юнит выбранного типа, а также соберутся ресурсы.";
        text["eng"] = "Chosen hexes will be captured, they will provide a unit and their resources.";
        AddEvent()->ShowMessage(text);

        AddEvent()->ShowGo();

        text["rus"] = "Что ж, выберите любую клетку!";
        text["eng"] = "So, select any hex!";
        AddEvent()->ShowMessage(text, false);
    }
}

bool LessonGame::checkIfPlanIsGood()
{
    if (N == 0)
    {
        if (checkIfCaptured(Coord(3, 9), "Blue"))
        {
//            if (checkIfCaptured(Coord(3, 5), "Blue"))
//                return true;
//            else
//            {
//                if (antirepeat)
//                {
//                    text["rus"] = "План для первого юнита был ошибочно стёрт и не восстановлен! Следует сначала восстановить для него план.";
//                    text["eng"] = "Welcome to «Forts and Mills», simultaneous turns strategy!";

//                    AddEvent()->ShowErrorMessage(text);
//                }
//                else
//                {
//                    AddEvent()->ShowErrorMessage("Видимо, по ошибке план для первого юнита был стёрт! Следует сначала восстановить его.");
//                }
//                antirepeat = !antirepeat;
//                return false;
//            }
            return true;
        }
        else if (checkIfBlocked(Coord(3,9), "Blue"))
        {
            if (antirepeat)
            {
                text["rus"] = "Так второй юнит только заходит в мельницу, но не захватывает её!";
                text["eng"] = "This way second unit enters the hex with mill, but does not capture it!";
                AddEvent()->ShowErrorMessage(text);
            }
            else
            {
                text["rus"] = "Второй юнит не захватывает мельницу! Надо это исправить.";
                text["eng"] = "Second unit does not capture the mill! You have to fix this.";
                AddEvent()->ShowErrorMessage(text);
            }

            antirepeat = !antirepeat;
            return false;
        }
        else
        {
            if (antirepeat)
            {
                text["rus"] = "Не, этот план никуда не годится - второму юниту нужно дойти до мельницы и захватить её!";
                text["eng"] = "Not a good plan: second unit should go to the mill and capture it.";
                AddEvent()->ShowErrorMessage(text);
            }
            else
            {
                text["rus"] = "Нам нужно захватить и форт, и мельницу, поэтому этот план не годится!";
                text["eng"] = "We need to capture both fort and mill, so the plan should be changed!";
                AddEvent()->ShowErrorMessage(text);
            }
            antirepeat = !antirepeat;
            return false;
        }
    }
    if (N == 1)
    {
        if (round == 0)
        {
            if (checkIfCaptured(Coord(3, 3), "Blue"))
            {
                return true;
            }
            else if (checkIfBlocked(Coord(3, 3), "Blue"))
            {
                if (antirepeat)
                {
                    text["rus"] = "Нужно не только войти в клетку, но и отдать приказ захвата!";
                    text["eng"] = "You forgot to capture the hex with resources!";
                    AddEvent()->ShowErrorMessage(text);
                }
                else
                {
                    text["rus"] = "Чтобы начать получать ресурсы, клетку нужно захватить, то есть исполнить приказ захвата!";
                    text["eng"] = "To start getting resources, you need to capture hex using a dedicated order!";
                    AddEvent()->ShowErrorMessage(text);
                }
                antirepeat = !antirepeat;
                return false;
            }
            else
            {
                if (antirepeat)
                {
                    text["rus"] = "Нам нужно захватить клетку с ресурсами, поэтому этот план не годится!";
                    text["eng"] = "We need to go to a hex with resources and capture it!";
                    AddEvent()->ShowErrorMessage(text);
                }
                else
                {
                    text["rus"] = "Не, этот план не годится - нам нужны ресурсы!";
                    text["eng"] = "It is not a good plan to spend last resources and not gather any new!";
                    AddEvent()->ShowErrorMessage(text);
                }
                antirepeat = !antirepeat;
                return false;
            }
        }
        else if (round == 1)
        {
            if (checkIfCaptured(Coord(4, 4), "Blue"))
            {
                return true;
            }
            else if (checkIfBlocked(Coord(4, 4), "Blue"))
            {
                if (antirepeat)
                {
                    text["rus"] = "Нужно ещё захватить клетку с ресурсом, чтобы получить его!";
                    text["eng"] = "To get the resource you need to capture the hex, not just enter it!";
                    AddEvent()->ShowErrorMessage(text, "Liberate");
                }
                else
                {
                    text["rus"] = "Клетка с освобождением не захвачена!";
                    text["eng"] = "You forgot to capture the hex with liberation.";
                    AddEvent()->ShowErrorMessage(text, "Liberate");
                }
                antirepeat = !antirepeat;
                return false;
            }
            else
            {
                if (antirepeat)
                {
                    text["rus"] = "Не, этот план никуда не годится - нужно получить ресурс освобождения!";
                    text["eng"] = "We want to get the liberation resource!";
                    AddEvent()->ShowErrorMessage(text, "Liberate");
                }
                else
                {
                    text["rus"] = "Нам нужно получить освобождение, поэтому этот план не годится!";
                    text["eng"] = "To get the resource, go to the hex and capture it!";
                    AddEvent()->ShowErrorMessage(text, "Liberate");
                }
                antirepeat = !antirepeat;
                return false;
            }
        }
        else if (round == 2)
        {
            if (checkIfExecuted(Coord(3, 5), "Liberate", "Blue"))
            {
                if (checkIfBlocked(Coord(4, 6), "Blue"))
                {
                    return true;
                }
                else
                {
                    if (antirepeat)
                    {
                        text["rus"] = "Так мы освободим один форт, но не заблокируем второй!";
                        text["eng"] = "Following this plan, we will liberate one fort, but the other one will remain unblocked!";
                        AddEvent()->ShowErrorMessage(text);
                    }
                    else
                    {
                        text["rus"] = "Так один вражеский форт останется незаблокированным!";
                        text["eng"] = "This way one enemy fort will remain unblocked!";
                        AddEvent()->ShowErrorMessage(text);
                    }
                    antirepeat = !antirepeat;
                    return false;
                }
            }
            else if (checkIfExecuted(Coord(4, 6), "Liberate", "Blue"))
            {
                if (checkIfBlocked(Coord(3, 5), "Blue"))
                {
                    return true;
                }
                else
                {
                    if (antirepeat)
                    {
                        text["rus"] = "Так мы освободим один форт, но не заблокируем второй!";
                        text["eng"] = "Following this plan, we will liberate one fort, but the other one will remain unblocked!";
                        AddEvent()->ShowErrorMessage(text);
                    }
                    else
                    {
                        text["rus"] = "Так один вражеский форт останется незаблокированным!";
                        text["eng"] = "This way one enemy fort will remain unblocked!";
                        AddEvent()->ShowErrorMessage(text);
                    }
                    antirepeat = !antirepeat;
                    return false;
                }
            }
            else
            {
                if (antirepeat)
                {
                    text["rus"] = "Не, этот план никуда не годится - нам нужно лишить противника фортов! Их нужно освободить или заблокировать, войдя в них.";
                    text["eng"] = "We want to deprive our enemy of two his forts! Either liberate or block (by entering) each of them!";
                    AddEvent()->ShowErrorMessage(text);
                }
                else
                {
                    text["rus"] = "Нам нужно лишить противника фортов: освободить их или заблокировать (войти в них).";
                    text["eng"] = "Deprive enemy of his forts by liberating them or blocking (entering the hex).";
                    AddEvent()->ShowErrorMessage(text);
                }
                antirepeat = !antirepeat;
                return false;
            }
        }
    }
    if (N == 2)
    {
        if (round == 0)
        {
            if (checkIfExecuted(Coord(2, 2), "Recruit", "Blue"))
            {
                return true;
            }
            else if (checkIfBlocked(Coord(2, 2), "Blue"))
            {
                if (antirepeat)
                {
                    text["rus"] = "Приказ рекрутирования так и не был отдан. Возможно, вы забыли выбрать тип юнита.";
                    text["eng"] = "No recruit order was given! You probably forgot to select a type of unit to create.";
                    AddEvent()->ShowErrorMessage(text);
                }
                else
                {
                    text["rus"] = "Сейчас мы входим в подходящую клетку, но не рекрутируем в ней. Возможно, вы забыли выбрать тип юнита.";
                    text["eng"] = "This way you enter the appropriate hex, but do not recruit. Perhaps you forgot to select a type of unit to create?";
                    AddEvent()->ShowErrorMessage(text);
                }
                antirepeat = !antirepeat;
                return false;
            }
            else
            {
                if (antirepeat)
                {
                    text["rus"] = "Не, этот план никуда не годится - нам нужно зарекрутировать нового юнита!";
                    text["eng"] = "We want to recruit a new unit!";
                    AddEvent()->ShowErrorMessage(text);
                }
                else
                {
                    text["rus"] = "Мы хотели зарекрутировать нового юнита, поэтому этот план не годится!";
                    text["eng"] = "Find a hex where you can recruit and use a dedicated order.";
                    AddEvent()->ShowErrorMessage(text);
                }
                antirepeat = !antirepeat;
                return false;
            }
        }
    }
    return true;
}

int LessonGame::NextStage()
{
    if (N == 0)
    {
        state = Game::GS_PLAN;

        enemy_unit1->plan[0] = new GameOrder(rules, enemy_unit1->type, "Attack");
        enemy_unit1->plan[0]->actions[1].target = Coord(4, 6);
        enemy_unit1->plan[1] = new GameOrder(rules, enemy_unit1->type, "Attack");
        enemy_unit1->plan[1]->actions[1].target = Coord(3, 5);

        text["rus"] = "Наш противник уже спланировал свой ход. Что ж, теперь мы не можем повлиять на исход раунда.";
        text["eng"] = "Our enemy has already planned his move. Well, now we can't influence the outcome of the round.";
        AddEvent()->ShowMessage(text, true, false, true);

        RealizePlan();

        int i = 0;
        while (events[i]->type != TIME_STARTED) {++i;}
        text["rus"] = "Сначала юниты одновременно выполнят свои первые приказы. Щёлкайте на кнопку перехода к следующему событию!";
        text["eng"] = "Units will simultenously execute their first orders. Click on the big \"next event\" button.";
        AddEvent(i)->ShowMessage(text, false, true, true, QSet <GameUnit *>() << events[i]->unit, "BlueNext");

        i = 0;
        while (events[i]->type != UNIT_LEAVES || events[i]->unit->color != rules->players[1]) {++i;}
        text["rus"] = "Вражеская хрюшка тоже выполняет приказ атаки...";
        text["eng"] = "Enemy pig is also executing attack order...";
        AddEvent(i)->ShowMessage(text, false, true, true, QSet <GameUnit *>() << events[i]->unit, "Attack");

        i = 0;
        while (events[i]->type != TIME_STARTED) {++i;}
        ++i;
        while (events[i]->type != TIME_STARTED) {++i;}
        text["rus"] = "Юниты переходят к выполнению своих вторых приказов.";
        text["eng"] = "Units finished their first order and now will execute their second ones.";
        AddEvent(i)->ShowMessage(text, false, true, true);

        while (events[i]->type != UNIT_LEAVES || events[i]->unit->color != rules->players[1]) {++i;}
        text["rus"] = "Наш юнит столкнулся с вражеским юнитом! Между ними должен произойти бой.";
        text["eng"] = "Our unit faced with enemy unit! Now, they will fight.";
        AddEvent(i)->ShowMessage(text, false, true, true, QSet <GameUnit *>() << events[i]->unit);

        while (events[i]->type != UNIT_ENTERS || events[i]->unit->color != rules->players[1]) {++i;}
        text["rus"] = "У каждого юнита есть один параметр – мощность юнита, что также является его здоровьем. У нашей белки, например, она равна 5, а у вражеской хрюшки всего лишь 4.";
        text["eng"] = "Each unit has only one parameter - unit's power, or health. Our scrat, for example, has power 5, while enemy unit has power 4.";
        AddEvent(i + 1)->ShowMessage(text, false, false, true, QSet <GameUnit *>() << events[i]->unit);
        AddEvent(i + 1)->EnableNext(QSet <GameUnit *>() << events[i]->unit);

        i = 0;
        while (events[i]->type != UNITS_FIGHT) {++i;}
        text["rus"] = "Наш юнит побеждает и остаётся с 1 здоровья.";
        text["eng"] = "Our unit wins and stays with 1 health.";
        AddEvent(i)->ShowMessage(text, false, true, false, events[i]->fighters);

        i = 0;
        while (events[i]->type != UNIT_DIES) {++i;}
        text["rus"] = "Вражеская хрюшка доблестно погибает.";
        text["eng"] = "Enemy pig valiantly dies.";
        AddEvent(i)->ShowMessage(text, false, true, false, QSet <GameUnit *>() << events[i]->unit);

        ++i;
        ++i;
        GameUnit * hero = unit2;
        while (!events[i]->involved().contains(hero)) {++i;}
        text["rus"] = "Наш юнит продолжит выполнять отданные ему приказы.";
        text["eng"] = "Our unit will continue executing his orders.";
        AddEvent(i)->ShowMessage(text, false, true, true, QSet <GameUnit *>() << hero);

        text["rus"] = "Мы успешно захватили форт и мельницу! Поздравляем, вы сделали свой первый шаг в большой мир!";
        text["eng"] = "We succesfully captured fort and mill! Congratulations, you've made your first steps into a big world!";
        AddEvent()->ShowMessage(text, false, false, true);
        AddEvent()->Exit();
    }
    if (N == 1)
    {
        state = Game::GS_PLAN;

        if (round == 0)
        {
            AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
            text["rus"] = "Похоже, враг тоже пытается заполучить ресурсы!";
            text["eng"] = "It seems our enemy wants to get these resources too!";
            AddEvent()->ShowMessage(text, false, true, true);

            enemy_unit1->plan[0] = new GameOrder(rules, enemy_unit1->type, "Go");
            enemy_unit1->plan[0]->actions[1].target = Coord(3, 3);
            enemy_unit1->plan[1] = new GameOrder(rules, enemy_unit1->type, "Capture");

            enemy_unit2->plan[0] = new GameOrder(rules, enemy_unit2->type, "Go");
            enemy_unit2->plan[0]->actions[1].target = Coord(1, 2);

            RealizePlan();
            gatherResources();
            defenseFill();

            int i = 0;
            // TODO: подумать над выделением при помощи <span style=\"color:blue\">
            while (events[i]->type != UNIT_FINISHES_ENTER || events[i]->unit != enemy_unit1) {++i;}
            text["rus"] = "Враги используют приказ хода: перемещения со штрафом в бою -1.";
            text["eng"] = "Enemy pig uses move order. Unlike attack order, it has -1 penalty in fight.";
            AddEvent(i-1)->ShowMessage(text, false, true, true, events[i]->involved(), "Go");

            while (events[i]->type != UNITS_FIGHT) {++i;}
            text["rus"] = "Мы поймали врага в тот момент, когда он исполнял приказ захвата! Враг получает штраф в бою -1, и мы остаёмся с 2 здоровья.";
            text["eng"] = "We catched our enemy capturing! He will be penalized -1 in fight.";
            AddEvent(i)->ShowMessage(text, false, true, true, events[i]->involved(), "Capture");

            while (events[i]->type != GATHER_RESOURCES) {++i;}
            text["rus"] = "Наступил конец раунда, и игроки получают ресурсы со всех принадлежащих им клеток.";
            text["eng"] = "This is the end of the round, and all players gather resources from all their captured hexes.";
            AddEvent(i)->ShowMessage(text, true, true, true);

            StartPlanning();
            AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
            AddEvent()->DisableGo();

            text["rus"] = "Наступил конец раунда, и игроки получают ресурсы со всех принадлежащих им клеток.";
            text["eng"] = "This is the end of the round, and all players gather resources from all their captured hexes.";
            AddEvent()->ShowMessage(text, true, false, true);

            text["rus"] = "Хотя у врага целых четыре источника ресурса хода, он получил лишь два. Это потому, что у врага всего два форта.";
            text["eng"] = "Enemy has four move order sources, but he received only two. This is because he has only two forts.";
            AddEvent()->ShowMessage(text);

            text["rus"] = "Каждый форт увеличивает лимит ресурсов одного типа на один.";
            text["eng"] = "Every fort increases the limit on storing resources of the same type by one.";
            AddEvent()->ShowMessageAndPicture(text, "Fort");

            AddEvent()->EnableGo();
            AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));
            text["rus"] = "После пересечения реки юнит не может выполнять других приказов. Воспользуйтесь вторым перемещением!";
            text["eng"] = "After crossing the river unit can't execute any other orders. Use move order!";
            AddEvent()->ReactionOnRiverCrossInPlan(Reaction(DEFAULT, text, "River"));

            text["rus"] = "Чтобы лишать противника клеток, понадобится приказ освобождения. Давайте получим его!";
            text["eng"] = "To deprive enemy of his territories, we will need liberation order. Let's get one!";
            AddEvent()->ShowMessageAndPicture(text, "Liberate", false);

            round = 1;
        }
        else if (round == 1)
        {
            enemy_unit2->plan[0] = new GameOrder(rules, enemy_unit2->type, "Go");
            enemy_unit2->plan[0]->actions[1].target = Coord(0, 1);

            text["rus"] = "Противник между тем вошёл в наш форт!";
            text["eng"] = "Meanwhile our enemy enters our fort!";
            AddEvent()->ShowMessage(text, false, true, true, QSet <GameUnit *>() << enemy_unit2);

            RealizePlan();
            gatherResources();
            defenseFill();
            burnExtraResources();

            StartPlanning();
            AddEvent()->DisableGo();
            text["rus"] = "Как и захват, освобождение в случае сражения дало бы штраф -1.";
            text["eng"] = "Like capture, liberation order has combat penalty -1.";
            AddEvent()->ReactionOnOrderChoice("Liberate", Reaction(DEFAULT, text, "Liberate"));

            text["rus"] = "Пока в клетке находится враг, она перестаёт давать ресурсы. А форт ещё и перестаёт увеличивать лимит ресурсов!";
            text["eng"] = "While enemy unit stands in our hex, it stops providing resources. Beside that, fort stops increasing limit on resources of one type!";
            AddEvent()->ShowMessage(text);

            text["rus"] = "Но клетка будет принадлежать игроку, пока враг не применит в ней приказ освобождения.";
            text["eng"] = "The hex itself still belongs to you until enemy executes liberation in it.";
            AddEvent()->ShowMessageAndPicture(text, "Liberate");

            AddEvent()->EnableGo();

            text["rus"] = "Сейчас мы при помощи освобождения сможем полностью обездвижить противника! Давайте заблокируем оба форта противника!";
            text["eng"] = "Now we have an opportunity to completely immobilize our enemy! Let's liberate or block both of his forts!";
            AddEvent()->ShowMessage(text, false, false, false);

            round = 2;
        }
        else if (round == 2)
        {
            text["rus"] = "Вперёд к победе!";
            text["eng"] = "To the victory!";
            AddEvent()->ShowMessage(text, false, true, true);

            RealizePlan();
            gatherResources();
            defenseFill();
            burnExtraResources();

            text["rus"] = "Противник потерял все ресурсы и полностью обездвижен! Ура!";
            text["eng"] = "Enemy has lost all his resources and is immobilized! Yes!";
            AddEvent()->ShowMessage(text, false, false, true);
            AddEvent()->Exit();
        }
    }
    if (N == 2)
    {
        state = Game::GS_PLAN;

        if (round == 0)
        {
            AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
            AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
            AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));

            text["rus"] = "Что это делает противник?!..";
            text["eng"] = "What is our enemy doing?!..";
            AddEvent()->ShowMessage(text, false, true, true);

            enemy_unit1->plan[0] = new GameOrder(rules, enemy_unit1->type, "Go");
            enemy_unit1->plan[0]->actions[1].target = Coord(4, 4);
            enemy_unit1->plan[1] = new GameOrder(rules, enemy_unit1->type, "Attack");
            enemy_unit1->plan[1]->actions[1].target = Coord(3, 3);
            enemy_unit1->plan[2] = new GameOrder(rules, enemy_unit1->type, "Capture");

            enemy_unit2->plan[0] = new GameOrder(rules, enemy_unit2->type, "Go");
            enemy_unit2->plan[0]->actions[1].target = Coord(4, 4);
            enemy_unit2->plan[1] = new GameOrder(rules, enemy_unit2->type, "Attack");
            enemy_unit2->plan[1]->actions[1].target = Coord(3, 3);
            enemy_unit2->plan[2] = new GameOrder(rules, enemy_unit2->type, "Liberate");

            RealizePlan();
            destroyHomelessUnits();
            gatherResources();
            burnExtraResources();
            defenseFill();            

            int i = 0;
            while (events[i]->type != UNIT_CAPTURES_HEX) {++i;}
            text["rus"] = "В захваченных клетках, в которых ещё не был создан юнит, отображается соответствующая иконка.";
            text["eng"] = "Captured hexes, which can still provide a unit, display the corresponding icon.";
            AddEvent(i)->ShowMessage(text, false, true, true, events[i]->involved(), "NotBlueUnitHome");

            i = 0;
            while (events[i]->type != UNITS_FIGHT) {++i;}

            text["rus"] = "Нападение! Нас атакуют сразу две белки с силой 5 каждая. Мы же находимся в своём форте.";
            text["eng"] = "Assault! We are attacked by two scrats with power 5 each. We are defending in our fort.";
            AddEvent(i)->ShowMessage(text, true, false, true, events[i]->involved());

            text["rus"] = "Форт имеет бонус в обороне +2. Если в клетке стоял и никуда не двигался наш юнит, бонус в обороне действует.";
            text["eng"] = "Fort has defense bonus +2. If our unit is not moving anywhere, this bonus triggers.";
            AddEvent(i+1)->ShowMessage(text, true, false, false, events[i]->involved(), "BlueShield");

            text["rus"] = "Одновременно сражающиеся юниты всегда теряют одинаковое количество здоровья.";
            text["eng"] = "Simultaneously fighting units always lose the same amount of health.";
            AddEvent(i+2)->ShowMessage(text, true, false, false, events[i]->involved());

            text["rus"] = "Чтобы пробить бонус +2 форта, каждый из двух нападающих потеряет по 1 здоровья.";
            text["eng"] = "To beat +2 defense bonus of our fort, each of two attacking units lose 1 health.";
            AddEvent(i+3)->ShowMessage(text, false, false, false, events[i]->involved());

            text["rus"] = "Чтобы победить защитника силы 5, каждый из двух нападающих потеряет по 3 здоровья.";
            text["eng"] = "To beat our defender unit with power 5, each of two attacking units lose 3 health.";
            AddEvent(i+5)->ShowMessage(text, false, false, false, events[i]->involved());

            while (events[i]->type != UNIT_DIES) {++i;}
            text["rus"] = "Наш юнит умер. В клетке его дома появился гробик.";
            text["eng"] = "Our unit has died. His home now displays a tombstone.";
            AddEvent(i)->ShowMessage(text, false, true, true, events[i]->involved(), "Tombstone");

            // TODO: обработка, понятно, та же, но название сбивает. Причина сгорания другая!
            while (events[i]->type != RECRUIT_FAILS_BECAUSE_OF_AGITE) {++i;}
            text["rus"] = "Мы потеряли соединение с мельницей! Наше рекрутирование сгорает!";
            text["eng"] = "We lost connection to our mill! Our recruiting order burns!";
            AddEvent(i)->ShowMessage(text, false, true, true, events[i]->involved(), "NotConnected");

            i = 0;
            while (events[i]->type != UNIT_DECAPTURES_HEX) {++i;}
            text["rus"] = "Двое нападавших одновременно применяют освобождение и захват. Оба приказа будут выполнены! :(";
            text["eng"] = "Two enemy units simultaneously execute liberation and capture. Both orders will be executed! :(";
            AddEvent(i)->ShowMessage(text, true, true, true, events[i]->involved());

            while (events[i]->type != UNIT_DIES) {++i;}
            text["rus"] = "В разграбленном форте был дом ещё одного нашего юнита! В конце раунда он погибает. :(";
            text["eng"] = "The lost fort was a home to another one of our units! In the end of the round he dies. :(";
            AddEvent(i)->ShowMessage(text, true, true, true, events[i]->involved());

            AddEvent()->Plan();
            AddEvent()->DisableGo();

            text["rus"] = "Безобразие!";
            text["eng"] = "What a nightmare!";
            AddEvent()->ShowMessage(text, true, false, true);

            text["rus"] = "Кажется, мы проиграли :( Следите за домами своих юнитов!";
            text["eng"] = "Seems like we have lost :( Look after homes of your units!";
            AddEvent()->ShowMessage(text);
            AddEvent()->Exit();
            round = 1;
        }
    }
    if (N == 3)
    {
        QList <Coord> vars;
        vars << Coord(2, 2) << Coord(3, 3) << Coord(3, 5) << Coord(0, 1) << Coord(4, 6) << Coord(4, 3) << Coord(2, 0) << Coord(1, 1)
             << Coord(4, 5) << Coord(5, 2) << Coord(4, 7) << Coord(3, 0) << Coord(1, 0) << Coord(3, 1) << Coord(2, 3);
        if (round == 0)
        {
            this->chosenHex[rules->players[1]] = vars[0];
            if (chosenHex[rules->players[0]] == vars[0])  // первый выбор точно делаем разным
                chosenHex[rules->players[1]] = vars[1];

            text["rus"] = "Игроки выбрали разные клетки, поэтому каждый получает желаемое! На игроков уже действует лимит ресурсов от фортов.";
            text["eng"] = "Players selected different hexes, so every player gets what he wanted. Gathered resources are already bounded by the number of forts.";
            AddEvent()->ShowMessage(text, true, true, true);

            Game::NextStage();

            AddEvent()->DefaultHexChoiceReaction(Reaction(NEXT_PHASE));
            text["rus"] = "Игроки выбрали разные клетки, поэтому каждый получает желаемое! На игроков уже действует лимит ресурсов от фортов.";
            text["eng"] = "Players selected different hexes, so every player gets what he wanted. Gathered resources are already bounded by the number of forts.";
            AddEvent()->ShowMessage(text, true, false, true);

            AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
            text["rus"] = "Игроки делают следующий выбор.";
            text["eng"] = "Players make second choice.";
            AddEvent()->ShowMessage(text, false, false, true);
            round = 1;
        }
        else if (round == 1)
        {
            chosenHex[rules->players[1]] = chosenHex[rules->players[0]];

            Game::NextStage();

            AddEvent()->DefaultHexChoiceReaction(Reaction(NEXT_PHASE));
            text["rus"] = "Игроки выбрали одну и ту же клетку, поэтому она никому не достаётся и выкидывается из вариантов.";
            text["eng"] = "Players selected the same hex, so it doesn't go to anyone.";
            AddEvent()->ShowMessage(text, true, false, true);

            AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
            text["rus"] = "Выбор стартовых клеток повторяется " + QString::number(rules->start_choices) + " раз.";
            text["eng"] = "The choice of starting hexes repeats " + QString::number(rules->start_choices) + " times.";
            AddEvent()->ShowMessage(text, false, false, true);
            round = 2;
        }
        else
        {
            if (round <= rules->start_choices - 1)
            {
                int i = 0;
                while (!hexes[vars[i].x][vars[i].y]->canBeChosenAsStartPoint) { ++i; }
                chosenHex[rules->players[1]] = vars[i];
            }

            if (round == 2)
            {
                AddEvent()->ShowStartProgressBar();
                final_message["rus"] = "Количество оставшихся выборов показано в верху экрана.";
                final_message["eng"] = "The number of remaining choices is shown in the progress bar.";
            }

            Game::NextStage();

            if (round < rules->start_choices - 1)
            {
                if (round == 4 && players["Blue"]->resources["Capture"] == 0)
                {
                    final_message["rus"] = "Стоит взять хотя бы один форт, иначе вы останетесь без ресурсов!";
                    final_message["eng"] = "How about taking at least one fort? Otherwise all your resources will burn!";
                }
                AddEvent()->ShowMessage(final_message, false, false, true);
                round++;
            }
            else if (round == rules->start_choices - 1)
            {
                PlayerColor winner = "Neutral";
                if (events.last()->type == WIN)
                {
                    winner = events.last()->color;
                    events.removeLast();
                }

                AddEvent()->DisableGo();
                text["rus"] = "Старт закончился, и началась игра! Цель игры - уничтожить всех юнитов противника!";
                text["eng"] = "The starting phase is over, and the game is on! The goal is to destroy all enemy forces!";
                AddEvent()->ShowMessage(text, true, false, true);

                text["rus"] = "Теперь вы знаете все правила \"Фортов и мельниц\"! Попробуйте победить глупенького бота!";
                text["eng"] = "Now you know all the rules of \"Forts and mills\"! Try to beat a silly bot!";
                AddEvent()->ShowMessage(text, true, false, true);

                if (winner == "Blue")
                {
                    text["rus"] = "Победа! Попробуйте теперь сыграть с людьми.";
                    text["eng"] = "Victory! Now, try to play with real people.";
                    AddEvent()->ShowMessage(text);
                    AddEvent()->Exit();
                }
                else if (winner == "Draw")
                {
                    text["rus"] = "Ничья! Попробуйте ещё раз.";
                    text["eng"] = "Draw! Try again!";
                    AddEvent()->ShowMessage(text);
                    AddEvent()->Exit(false);
                }
                else if (winner == "Red")
                {
                    text["rus"] = "К сожалению, без фортов у вас не может быть никаких ресурсов, поэтому это поражение! Попробуйте ещё раз.";
                    text["eng"] = "Unfortunately, without forts you can't have any resources, so you've lost! Try again!";
                    AddEvent()->ShowMessage(text);
                    AddEvent()->Exit(false);
                }
                else
                {
                    AddEvent()->EnableGo();
                    AddEvent()->AllowHelp();
                    text["rus"] = "Чтобы получить справку о любом игровом элементе, кликните по нему правой кнопкой мыши.";
                    text["eng"] = "You can get help about any game object by clicking on it with right mouse button.";
                    AddEvent()->ShowMessage(text, false, false, false);
                    round += 1;
                }
            }
            else
            {
                if (events.last()->type == WIN)
                {
                    bool win = events.last()->color == "Blue";
                    events.removeLast();
                    AddEvent()->DisappearNext();

                    if (win)
                    {
                        text["rus"] = "Победа! Попробуйте теперь сыграть с людьми.";
                        text["eng"] = "Victory! Now, try to play with real people.";
                        AddEvent()->ShowMessage(text);
                        AddEvent()->Exit();
                    }
                    else if (events.last()->color == "Draw")
                    {
                        text["rus"] = "Ничья! Попробуйте ещё раз.";
                        text["eng"] = "Draw! Try again!";
                        AddEvent()->ShowMessage(text);
                        AddEvent()->Exit(false);
                    }
                    else
                    {
                        text["rus"] = "Поражение! Попробуйте ещё раз.";
                        text["eng"] = "Defeat! Try again!";
                        AddEvent()->ShowMessage(text);
                        AddEvent()->Exit(false);
                    }
                }
                else
                {
                    AddEvent()->Pause();
                }
            }
        }
    }

    return 0;
}
