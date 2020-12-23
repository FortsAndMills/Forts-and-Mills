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

        types[5][0] = "Mountain";
        types[5][2] = "Lake";
    }
    if (N == 1)
    {
        types[4][6] = "Fort";
        types[3][5] = "Fort";
        types[0][0] = "Fort";
        types[0][1] = "Fort";
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
//    if (N == 3)
//    {
//        types[1][2] = "Fort";
//        types[2][2] = "Fort";
//        types[3][2] = "Mill";
//        types[3][5] = "Mill";
//        types[4][7] = "Mountain";
//        types[4][6] = "Mountain";
//        types[2][7] = "Mountain";
//        types[3][7] = "Mountain";
//        types[1][0] = "Mountain";
//        types[2][0] = "Mountain";
//    }
    if (N == 3)
    {
        types[4][5] = "Fort";
        types[0][1] = "Fort";
        types[2][2] = "Fort";
        types[2][3] = "Fort";

        types[3][0] = "Mill";
        types[2][0] = "Mill";
        types[3][1] = "Mill";
        types[1][1] = "Mill";

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
//    if (N == 3)
//    {
//        hexes[2][1]->resources << "Capture" << "Capture" << "Attack" << "Go" << "Siege";
//        hexes[4][5]->resources << "Capture" << "Attack";
//    }
    if (N == 3)
    {
        hexes[4][5]->resources << "Capture";
        hexes[0][1]->resources << "Capture";
        hexes[2][2]->resources << "Capture";
        hexes[2][3]->resources << "Capture";

        hexes[3][0]->resources << "Recruit";
        hexes[2][0]->resources << "Recruit";
        hexes[3][1]->resources << "Recruit";
        hexes[1][1]->resources << "Recruit";

        hexes[1][0]->resources << "Attack";
        hexes[3][2]->resources << "Go";
        hexes[3][3]->resources << "Attack" << "Go";

        hexes[3][4]->resources << "Go";
        hexes[3][5]->resources << "Attack" << "Attack" << "Liberate";
        hexes[3][6]->resources << "Go" << "Go";

        hexes[4][1]->resources << "Go";
        hexes[4][2]->resources << "Liberate";
        hexes[4][3]->resources << "Attack" << "Liberate";

        hexes[4][4]->resources << "Go";
        hexes[4][6]->resources << "Attack" << "Attack";
        hexes[4][7]->resources << "Go";
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

        AddEvent()->GetReadyToPlan();
        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));

        AddEvent()->HidePlayerPanels();
        AddEvent()->HideGo();
        AddEvent()->HideDayTimes();
        AddEvent()->HideFieldControl();
        AddEvent()->HideHomes();

        // Вступление
        AddEvent()->ShowMessage("Добро пожаловать в «Форты и мельницы», уникальную стратегию современности!");
        AddEvent()->ShowMessage("Каждый раунд вам предстоит придумать план, который приведёт вас к победе или поражению. В этом уроке вы обучитесь главному игровому действию - как отдавать своим юнитам приказы.");

        AddEvent()->ShowMessage("Наша задача - составить для своих юнитов план действий, то есть что они будут делать в этот раунд.");

        AddEvent()->ShowMessage("Мы играем за синих. Давайте попробуем захватить расположенный на карте форт.", "Fort");

        // Отдаём приказ атаки на завтрак
        AddEvent()->ReactionOnUnitClick(unit1, Reaction(BLOCK, "Этот юнит не доберётся до форта..."));
        AddEvent()->ReactionOnUnitClick(enemy_unit1, Reaction(BLOCK, "Это вражеский юнит. Нужно щёлкнуть по своему"));
        AddEvent()->ReactionOnUnitClick(unit2, Reaction(NEXT_PHASE));
        AddEvent()->ShowMessage("Каждому юниту мы должны отдать три приказа – на завтрак, обед и ужин. Давайте отдадим первому юниту приказ атаки на завтрак – для этого по нему нужно кликнуть.", false);

        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
        AddEvent()->ReactionOnOrderChoice("Capture", Reaction(BLOCK, "Это приказ захвата. Нам нужна атака!", "Attack"));
        AddEvent()->ReactionOnOrderChoice("Attack", Reaction(NEXT_PHASE));
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(BLOCK, "Это приказ пропуска хода. Нам нужна атака!", "Attack"));
        AddEvent()->ShowMessageAndPicture("Теперь выберем приказ, который мы хотим отдать - приказ атаки", "Attack", false, true);

        AddEvent()->DefaultHexChoiceReaction(Reaction(BLOCK, "Нам нужно добраться до форта за две атаки, поэтому стоит пойти в другом направлении", "Fort"));
        AddEvent()->ReactionOnHexChoice(hexes[3][4], Reaction(NEXT_PHASE));
        AddEvent()->ShowMessageAndPicture("Осталось выбрать клетку, которую юнит атакует. Нам нужно идти к форту.", "Fort", false, true);

        // Отдаём приказ на обед
        AddEvent()->ReactionOnOrderChoice("Capture", Reaction(BLOCK, "Это приказ захвата. Нам нужна атака!", "Attack"));
        AddEvent()->ReactionOnOrderChoice("Attack", Reaction(NEXT_PHASE));
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(BLOCK, "Это приказ пропуска хода. Нам нужна атака!", "Attack"));
        AddEvent()->ShowMessageAndPicture("Приказ на завтрак запланирован. Теперь можно отдать ему приказ на обед - атаку в форт", "Attack", false, true);

        AddEvent()->DefaultHexChoiceReaction(Reaction(BLOCK, "Нам нужно атаковать форт!", "Fort"));
        AddEvent()->ReactionOnHexChoice(hexes[3][5], Reaction(NEXT_PHASE));
        AddEvent()->ShowMessageAndPicture("Выбираем атаковать форт", "Fort", false);


        // Отдаём приказ захвата
        AddEvent()->ReactionOnOrderChoice("Capture", Reaction(NEXT_PHASE));
        AddEvent()->ReactionOnOrderChoice("Attack", Reaction(BLOCK, "Это приказ атаки. Нам нужен захват!", "Capture"));
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(BLOCK, "Это приказ пропуска хода. Нам нужен захват!", "Capture"));
        AddEvent()->ShowMessageAndPicture("На ужин отдадим юниту приказ захвата клетки. Тогда форт будет наш!", "Capture", false, true);


        // Переходим к другому юниту
        AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));

        AddEvent()->ReactionOnUnitClick(unit1, Reaction(NEXT_PHASE));
        AddEvent()->ReactionOnUnitClick(unit2, Reaction(BLOCK, "Этому юниту мы уже составили план. У нас есть ещё один юнит, план для которого пока не составлен."));
        AddEvent()->ReactionOnUnitClick(enemy_unit1, Reaction(DEFAULT, "Это вражеский юнит"));
        AddEvent()->ShowMessage("Щёлкните по другому нашему юниту. Поскольку ему ещё не отдан приказ на завтрак, произойдёт автоматическое переключение на завтрак", false, false, true);


        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
        AddEvent()->ReactionOnOrderChoice("Capture", Reaction(BLOCK, "Это приказ захвата. Нам нужна атака!", "Attack"));
        AddEvent()->ReactionOnOrderChoice("Attack", Reaction(NEXT_PHASE));
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(BLOCK, "Это приказ пропуска хода. Нам нужна атака!", "Attack"));
        AddEvent()->ShowMessageAndPicture("Вторым юнитом попробуем заполучить мельницу!", "Mill", false, false);

        AddEvent()->DefaultHexChoiceReaction(Reaction(BLOCK, "Давайте пойдём в мельницу, расположенную клеткой ниже.", "Mill"));
        AddEvent()->ReactionOnHexChoice(hexes[5][1], Reaction(NEXT_PHASE));
        AddEvent()->ShowMessageAndPicture("В горы и озёра ходить нельзя. Мы идём в мельницу!", "Mill", false, false);

        // Речка, циклическое поле и отмотка назад.
        AddEvent()->ShowMessageAndPicture("Однако, юнит пересёк речку, и поэтому ему больше нельзя выдать приказы.", "River", true, false);
        AddEvent()->ShowMessageAndPicture("Значит, эту мельницу нам не захватить. Есть другая мельница, но она далеко... Или нет?", "Mill", true, false);

        AddEvent()->ShowFieldControl();
        AddEvent()->ShowMessage("Вовсе нет, потому что карта циклическая. Для удобства её можно прокручивать стрелками внизу.", true, false, false);

        AddEvent()->ShowMessage("Нам надо стереть неверный приказ атаки и пойти в другом направлении.", true, false, false);
        AddEvent()->ShowMessage("Выбрав юнита, у которого на данное время дня уже что-то запланировано, вы удаляете приказы на это и последующее время дня. Таким образом можно удалять ошибочные приказы", true, false, false);

        AddEvent()->ShowDayTimes();
        AddEvent()->ShowMessage("Вы можете переключаться между завтраком, обедом и ужином на специальной панели внизу экрана.");

        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
        AddEvent()->ReactionOnUnitClick(unit1, Reaction(NEXT_PHASE));
        AddEvent()->ReactionOnUnitClick(unit2, Reaction(BLOCK, "У этого юнита с планом всё в порядке; не надо его стирать!"));
        AddEvent()->ReactionOnUnitClick(enemy_unit1, Reaction(BLOCK, "Это вражеский юнит."));
        AddEvent()->ShowMessage("Кликнем на юните ещё раз, чтобы стереть неудачную атаку и начать планировать ещё раз.", false, false, false);

        // Даём творчество игроку
        AddEvent()->ShowGo();

        AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(DEFAULT, "Приказ пропуска хода выставляется по умолчанию - его не обязательно выставлять вручную, если только юнит не собирается делать что-то другое после него"));
        AddEvent()->ShowMessage("Итак, наша задача - захватить форт и мельницу. Когда ваш план будет готов, нажимайте кнопку Go", false, false, true);
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

        AddEvent()->GetReadyToPlan();
        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
        AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(DEFAULT, "Приказ пропуска хода выставляется по умолчанию - его не обязательно выставлять вручную, если только юнит не собирается делать что-то другое после него"));

        AddEvent()->HideGo();
        AddEvent()->HideHomes();

        // Вступление
        AddEvent()->ShowMessage("Все приказы, которые вы отдаёте своим юнитам, являются истрачиваемыми ресурсами.");
        AddEvent()->ShowMessage("Каждый игрок знает свои и чужие ресурсы. Они отображаются на боковых панелях.");
        AddEvent()->ShowMessage("Мы снова играем за синих, и у нас есть две атаки и два захвата. Это сильно ограничивает наши действия.");
        AddEvent()->ShowMessage("У нас даже нет источников ресурса передвижения! Нам срочно нужны клетки с ресурсами!");
        AddEvent()->ShowMessage("Захватив клетку, вы будете получать с неё указанные ресурсы в конце каждого раунда.");

        AddEvent()->ShowGo();
        AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));
        AddEvent()->ReactionOnUnitClick(enemy_unit1, Reaction(DEFAULT, "Это вражеский юнит. Мы играем за синих."));
        AddEvent()->ReactionOnUnitClick(enemy_unit2, Reaction(DEFAULT, "Это вражеский юнит. Мы играем за синих."));
        AddEvent()->ShowMessage("Давайте захватим клетку с ресурсами! Вперёд!", false);
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

        AddEvent()->GetReadyToPlan();
        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
        AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));

        AddEvent()->ReactionOnHexChoice(hexes[2][2], Reaction(DEFAULT, "Да, если мы захватим эту клетку, она будет соединена с нашей мельницей!"));
        AddEvent()->ReactionOnHexChoice(hexes[2][4], Reaction(DEFAULT, "После пересечения речки юнит не может выполнять других приказов. В этом раунде мы не сможем здесь рекрутировать."));
        AddEvent()->ReactionOnHexChoice(hexes[4][4], Reaction(DEFAULT, "Из этой клетки уже рекрутировал наш враг. Мы даже освободить её не сможем!"));
        AddEvent()->ReactionOnHexChoice(hexes[4][2], Reaction(DEFAULT, "В этой клетке юнит уже создан."));

        AddEvent()->ReactionOnOrderChoice("Recruit", Reaction(DEFAULT, "При рекрутировании нужно выбрать тип создаваемого юнита. Пока выберите любой."));

        AddEvent()->HideGo();

        // Вступление
        AddEvent()->ShowMessageAndPicture("У каждого юнита есть свой дом - клетка, в которой он родился", "BlueUnitHome");
        AddEvent()->ShowMessageAndPicture("Чтобы узнать, где находится дом юнита, наведите на него мышкой.", "BlueUnitHome");
        AddEvent()->ShowMessageAndPicture("За всю игру в каждой клетке можно получить лишь одного юнита. Для этого нужно рекрутирование.", "Recruit");
        AddEvent()->ShowMessageAndPicture("Клетка при этом должна быть захвачена и соединена вашими территориями с вашей мельницей", "Mill");

        AddEvent()->ShowGo();
        AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));
        AddEvent()->ReactionOnUnitClick(unit2, Reaction(DEFAULT, "Рядом нет клеток, которые мы могли бы соединить со своей мельницей."));
        AddEvent()->ShowMessage("Давайте рекрутируем нового юнита. Для это нужно найти клетку, где мы можем провести рекрутирование!", false);
    }
//    if (N == 3)
//    {
//        unit1 = NewUnit(players[rules->players[0]], "Hippo", Coord(3, 1));
//        unit1->home = Coord(2, 1);
//        unit2 = NewUnit(players[rules->players[0]], "Karkun", Coord(3, 2));

//        enemy_unit1 = NewUnit(players[rules->players[1]], "Hippo", Coord(3, 4));
//        enemy_unit1->home = Coord(1, 2);
//        enemy_unit2 = NewUnit(players[rules->players[1]], "Pig", Coord(4, 2));
//        enemy_unit2->home = Coord(4, 5);
//        enemy_unit3 = NewUnit(players[rules->players[1]], "Karkun", Coord(3, 5));

//        CaptureHex(hexes[3][2], rules->players[0]);
//        CaptureHex(hexes[2][1], rules->players[0]);
//        CaptureHex(hexes[1][2], rules->players[1]);
//        CaptureHex(hexes[3][5], rules->players[1]);
//        CaptureHex(hexes[4][5], rules->players[1]);
//        GatherResources(hexes[3][2], rules->players[0], QList<QString>() << "Capture" << "Attack" << "Go" << "Siege");
//        GatherResources(hexes[4][5], rules->players[1], QList<QString>() << "Capture" << "Attack");

//        AddEvent()->GetReadyToPlan();
//        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
//        AddEvent()->DefaultOrderChoiceReaction(Reaction(BLOCK));
//        AddEvent()->DefaultHexChoiceReaction(Reaction(BLOCK));

//        AddEvent()->HideGo();
//        AddEvent()->ShowMessage("В этом уроке вы узнаете, как и откуда берутся новые войска.");
//        AddEvent()->ShowMessageAndPicture("У каждого юнита есть свой дом - некоторая клетка на карте, форт, мельница или обычная клетка с ресурсами. Её можно узнать, наведя мышкой на юнит. ", "BlueUnitHome");
//        AddEvent()->ShowMessage("Если потерять клетку с домом юнита, он погибнет в конце раунда. Давайте войдём в незащищённый вражеский форт - тогда мы уничтожим вражеского бегемота с домом в этой клетке! Заодно форт хорошо бы захватить.");

//        AddEvent()->ShowGo();
//        AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));
//        AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
//        AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
//        AddEvent()->ReactionOnOrderChoice("Capture", Reaction(DEFAULT, "Захватывая новую клетку, вы получите на ней нового юнита в конце раунда. Тип юнита вы выбираете сами, когда планируете приказ."));

//        AddEvent()->ShowMessage("Однако учтите, что вражеская хрюшка может попробовать войти в нашу единственную мельницу!", false);
//    }
    if (N == 3)
    {
        ChooseOneOfTheStartHexes();
        GameMessage * true_message = events.last();
        AddEvent()->GetReadyToChooseHex(true_message);
        events.removeAt(events.size() - 2);

        AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));
        AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));

        AddEvent()->HideGo();
        AddEvent()->ShowMessage("Каждая партия начинается с того, что игроки выбирают место своего старта на карте.");
        AddEvent()->ShowMessage("Выбранная клетка будет захвачена, в ней появится юнит выбранного типа, а также соберутся ресурсы.");
        AddEvent()->ShowGo();
        AddEvent()->ShowMessage("Что ж, выберите любую клетку!", false);
    }
}

bool LessonGame::checkIfPlanIsGood()
{
    if (N == 0)
    {
        if (checkIfCaptured(Coord(3, 9)))
        {
            if (checkIfCaptured(Coord(3, 5)))
                return true;
            else
            {
                if (antirepeat)
                    AddEvent()->ShowErrorMessage("План для первого юнита был ошибочно стёрт и не восстановлен! Следует сначала восстановить для него план.");
                else
                    AddEvent()->ShowErrorMessage("Видимо, по ошибке план для первого юнита был стёрт! Следует сначала восстановить его.");
                antirepeat = !antirepeat;
                return false;
            }
        }
        else if (checkIfBlocked(Coord(3,9)))
        {
            if (antirepeat)
                AddEvent()->ShowErrorMessage("Так второй юнит только заходит в мельницу, но не захватывает её! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
            else
                AddEvent()->ShowErrorMessage("Второй юнит не захватывает мельницу! Надо это исправить. Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
            antirepeat = !antirepeat;
            return false;
        }
        else
        {
            if (antirepeat)
                AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - второму юниту нужно дойти до мельницы и захватить её! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
            else
                AddEvent()->ShowErrorMessage("Нам нужно захватить и форт, и мельницу, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
            antirepeat = !antirepeat;
            return false;
        }
    }
    if (N == 1)
    {
        if (round == 0)
        {
            if (checkIfCaptured(Coord(3, 3)))
            {
                return true;
            }
            else if (checkIfBlocked(Coord(3, 3)))
            {
                if (antirepeat)
                    AddEvent()->ShowErrorMessage("Чтобы начать получать ресурсы, клетку нужно захватить, то есть исполнить приказ захвата! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                else
                    AddEvent()->ShowErrorMessage("Нужно не только войти в клетку, но и отдать приказ захвата! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                antirepeat = !antirepeat;
                return false;
            }
            else
            {
                if (antirepeat)
                    AddEvent()->ShowErrorMessage("Не, этот план не годится - нам нужны ресурсы! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                else
                    AddEvent()->ShowErrorMessage("Нам нужно захватить клетку с ресурсами, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                antirepeat = !antirepeat;
                return false;
            }
        }
        else if (round == 1)
        {
            if (checkIfCaptured(Coord(4, 4)))
            {
                return true;
            }
            else if (checkIfBlocked(Coord(4, 4)))
            {
                if (antirepeat)
                    AddEvent()->ShowErrorMessage("Нужно ещё захватить клетку с ресурсом, чтобы получить его! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту", "Liberate");
                else
                    AddEvent()->ShowErrorMessage("Клетка с освобождением не захвачена! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту", "Liberate");
                antirepeat = !antirepeat;
                return false;
            }
            else
            {
                if (antirepeat)
                    AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - нужно получить ресурс освобождения! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту", "Liberate");
                else
                    AddEvent()->ShowErrorMessage("Нам нужно получить освобождение, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту", "Liberate");
                antirepeat = !antirepeat;
                return false;
            }
        }
        else if (round == 2)
        {
            if (checkIfExecuted(Coord(3, 5), "Liberate"))
            {
                if (checkIfBlocked(Coord(4, 6)))
                {
                    return true;
                }
                else
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Так мы освободим один форт, но не заблокируем второй! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    else
                        AddEvent()->ShowErrorMessage("Так один вражеский форт останется незаблокированным! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    antirepeat = !antirepeat;
                    return false;
                }
            }
            else if (checkIfExecuted(Coord(4, 6), "Recruit"))
            {
                if (checkIfBlocked(Coord(3, 5)))
                {
                    return true;
                }
                else
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Так мы освободим один форт, но не заблокируем второй! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    else
                        AddEvent()->ShowErrorMessage("Так один вражеский форт останется незаблокированным! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    antirepeat = !antirepeat;
                    return false;
                }
            }
            else
            {
                if (antirepeat)
                    AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - нам нужно лишить противника фортов! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                else
                    AddEvent()->ShowErrorMessage("Нам нужно лишить противника фортов, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                antirepeat = !antirepeat;
                return false;
            }
        }
    }
    if (N == 2)
    {
        if (round == 0)
        {
            if (checkIfExecuted(Coord(2, 2), "Recruit"))
            {
                return true;
            }
            else
            {
                if (antirepeat)
                    AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - нам нужно зарекрутировать нового юнита! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                else
                    AddEvent()->ShowErrorMessage("Мы хотели зарекрутировать нового юнита, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                antirepeat = !antirepeat;
                return false;
            }
        }
//        else if (round == 1)
//        {
//            if (checkIfCaptured(Coord(1, 1)))
//            {
//                return true;
//            }
//            else
//            {
//                if (antirepeat)
//                    AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - нам нужны выстрелы! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
//                else
//                    AddEvent()->ShowErrorMessage("Нам нужно получить ресурс выстрела, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
//                antirepeat = !antirepeat;
//                return false;
//            }
//        }
//        else if (round == 2)
//        {
//            if (!checkIfEmpty())
//            {
//                return true;
//            }
//            else
//            {
//                if (antirepeat)
//                    AddEvent()->ShowErrorMessage("Вряд ли план ничего не делать - это хорошая идея...");
//                else
//                    AddEvent()->ShowErrorMessage("Ничего не делать целый раунд вряд ли как-то поможет...");
//                antirepeat = !antirepeat;
//                return false;
//            }
//        }
    }
//    if (N == 3)
//    {
//        if (round == 0)
//        {
//            if (checkIfCaptured(Coord(1, 2)))
//            {
//                GameUnit * hippo = unit1;

//                if (hippo->plan[0] == NULL ||
//                        hippo->plan[0]->type == "Capture" ||
//                        hippo->plan[0]->actions[1].target != Coord(3, 2))
//                {
//                    if (hippo->plan[0] != NULL &&
//                            hippo->plan[0]->type != "Capture" &&
//                            hippo->plan[0]->actions[1].target == Coord(4, 2))
//                    {
//                        if (antirepeat)
//                            AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - вражеская хрюшка преспокойно войдёт в нашу мельницу на завтрак, так и не встретившись с нашим бегемотом! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
//                        else
//                            AddEvent()->ShowErrorMessage("Так мы потеряем свою мельницу на завтрак, поскольку наш бегемот не пересечётся с вражеской хрюшкой, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
//                        antirepeat = !antirepeat;
//                        return false;
//                    }
//                    else
//                    {
//                        if (antirepeat)
//                            AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - наша мельница осталась незащищённой! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
//                        else
//                            AddEvent()->ShowErrorMessage("Так мы потеряем свою мельницу, ведь в неё сможет войти вражеская хрюшка! Поэтому этот план не годится. Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
//                        antirepeat = !antirepeat;
//                        return false;
//                    }
//                }

//                return true;
//            }
//            else if (checkIfBlocked(Coord(1, 2)))
//            {
//                if (antirepeat)
//                    AddEvent()->ShowMessage("Форт ещё нужно захватить! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту", false);
//                else
//                    AddEvent()->ShowMessage("Нужно не только отбить форт у противника, но и захватить! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту", false);
//                antirepeat = !antirepeat;
//                return false;
//            }
//            else
//            {
//                if (antirepeat)
//                    AddEvent()->ShowMessage("Не, этот план никуда не годится - нам нужно захватить форт, сейчас принадлежащий противнику! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту", false);
//                else
//                    AddEvent()->ShowMessage("Нам нужно захватить форт противника, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту", false);
//                antirepeat = !antirepeat;
//                return false;
//            }
//        }
//        if (round == 1)
//        {
//            if (checkIfCaptured(Coord(2, 2)))
//            {
//                return true;
//            }
//            else
//            {
//                if (antirepeat)
//                    AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - нам нужно захватить оставшийся форт! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
//                else
//                    AddEvent()->ShowErrorMessage("Нам нужно захватить оставшийся форт, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
//                antirepeat = !antirepeat;
//                return false;
//            }
//        }
//    }
    return true;
}

void LessonGame::PlanRealisation()
{
    if (N == 0)
    {
        enemy_unit1->plan[0] = new GameOrder(rules, enemy_unit1->type, "Attack");
        enemy_unit1->plan[0]->actions[1].target = Coord(4, 6);
        enemy_unit1->plan[1] = new GameOrder(rules, enemy_unit1->type, "Attack");
        enemy_unit1->plan[1]->actions[1].target = Coord(3, 5);

        AddEvent()->ShowMessage("Наш противник уже спланировал свой ход. Что ж, теперь мы не можем повлиять на исход раунда. В пошаговом режиме мы можем посмотреть, что же получилось.", true, false, true);

        RealizePlan();

        int i = 0;
        while (events[i]->type != TIME_STARTED) {++i;}
        AddEvent(i)->ShowMessage("Сначала юниты выполнят приказы на завтрак. Сделают они это как бы одновременно, но анимация произойдёт пошагово. Щёлкайте на кнопку перехода к следующему событию!", false, true, true, QSet <GameUnit *>() << events[i]->unit, "BlueNext");

        i = 0;
        while (events[i]->type != UNIT_LEAVES || events[i]->unit->color != rules->players[1]) {++i;}
        AddEvent(i)->ShowMessage("Вражеская хрюшка на завтрак тоже выполняет приказ атаки...", false, true, true, QSet <GameUnit *>() << events[i]->unit);

        i = 0;
        while (events[i]->type != TIME_STARTED) {++i;}
        ++i;
        while (events[i]->type != TIME_STARTED) {++i;}
        AddEvent(i)->ShowMessage("Завтрак окончен. Начался обед.", false, true, true);

        while (events[i]->type != UNIT_LEAVES || events[i]->unit->color != rules->players[1]) {++i;}
        AddEvent(i)->ShowMessage("Наш юнит столкнулся с вражеским юнитом! Между ними должен произойти бой.", false, true, true, QSet <GameUnit *>() << events[i]->unit);

        while (events[i]->type != UNIT_ENTERS || events[i]->unit->color != rules->players[1]) {++i;}
        AddEvent(i + 1)->ShowMessage("У каждого юнита есть один параметр – мощность юнита, что также является его здоровьем. У нашей белки, например, она равна 5, а у вражеской хрюшки всего лишь 4.", false, false, true, QSet <GameUnit *>() << events[i]->unit);
        AddEvent(i + 1)->EnableNext(QSet <GameUnit *>() << events[i]->unit);

        i = 0;
        while (events[i]->type != UNITS_FIGHT) {++i;}
        AddEvent(i)->ShowMessage("Наш юнит побеждает и остаётся с 1 здоровья.", false, true, false, events[i]->fighters);

        i = 0;
        while (events[i]->type != UNIT_DIES) {++i;}
        AddEvent(i)->ShowMessage("Вражеская хрюшка доблестно погибает.", false, true, false, QSet <GameUnit *>() << events[i]->unit);

        ++i;
        ++i;
        GameUnit * hero = unit2;
        while (!events[i]->involved().contains(hero)) {++i;}
        AddEvent(i)->ShowMessage("Наш юнит продолжит выполнять отданные ему приказы.", false, true, true, QSet <GameUnit *>() << hero);

        AddEvent()->ShowMessage("Мы успешно захватили форт и мельницу! Поздравляем, вы сделали свой первый шаг в большой мир!", false, false, true);
        AddEvent()->Exit();
    }
    if (N == 1)
    {
        if (round == 0)
        {
            AddEvent()->ShowMessage("Похоже, враг тоже попытался самому получить ресурсы!", false, true, true);

            enemy_unit1->plan[0] = new GameOrder(rules, enemy_unit1->type, "Go");
            enemy_unit1->plan[0]->actions[1].target = Coord(3, 3);
            enemy_unit1->plan[1] = new GameOrder(rules, enemy_unit1->type, "Capture");

            enemy_unit2->plan[0] = new GameOrder(rules, enemy_unit2->type, "Go");
            enemy_unit2->plan[0]->actions[1].target = Coord(1, 2);

            RealizePlan();
            gatherResources();
            defenceFill();

            int i = 0;
            while (events[i]->type != UNIT_FINISHES_ENTER || events[i]->unit != enemy_unit1) {++i;}
            AddEvent(i-1)->ShowMessage("Враги используют приказ хода: перемещения со штрафом в бою -1.", false, true, true, events[i]->involved(), "Go");

            while (events[i]->type != UNITS_FIGHT) {++i;}
            AddEvent(i)->ShowMessage("Мы поймали врага в тот момент, когда он исполнял приказ захвата! Враг получает штраф в бою -1, и мы остаёмся с 2 здоровья.", false, true, true, events[i]->involved(), "Capture");

            while (events[i]->type != GATHER_RESOURCES) {++i;}
            AddEvent(i)->ShowMessage("Наступил конец раунда, и игроки получают ресурсы со всех принадлежащих им клеток.", true, true, true);

            AddEvent()->GetReadyToPlan();
            AddEvent()->DisableGo();

            AddEvent()->ShowMessage("Наступил конец раунда, и игроки получают ресурсы со всех принадлежащих им клеток.", true, false, true);
            AddEvent()->ShowMessage("Хотя у врага целых четыре источника ресурса хода, он получил лишь два. Это потому, что у врага всего два форта.");
            AddEvent()->ShowMessageAndPicture("Каждый форт увеличивает лимит ресурсов одного типа на один.", "Fort");

            AddEvent()->EnableGo();
            AddEvent()->ShowMessageAndPicture("Чтобы лишать противника клеток, понадобится приказ освобождения. Давайте получим его!", "Liberate", false);
            round = 1;
        }
        else if (round == 1)
        {
            enemy_unit2->plan[0] = new GameOrder(rules, enemy_unit2->type, "Go");
            enemy_unit2->plan[0]->actions[1].target = Coord(0, 1);

            //AddEvent()->ShowMessage("У противника нету ресурсов, поэтому он не может ничего делать. Так что вражеская хрюшка \"исполняет\" приказ пропуска хода.", false, true, true, QSet <GameUnit *>(), DefaultOrder);
            AddEvent()->ShowMessage("Противник между тем вошёл в наш форт!", false, true, true, QSet <GameUnit *>() << enemy_unit2);

            RealizePlan();
            gatherResources();
            defenceFill();
            burnExtraResources();

            AddEvent()->GetReadyToPlan();
            AddEvent()->DisableGo();
            AddEvent()->ReactionOnOrderChoice("Liberate", Reaction(DEFAULT, "Как и захват, освобождение в случае сражения дало бы штраф -1.", "Liberate"));
            AddEvent()->ShowMessage("Пока враг находится в вашей клетке, она перестаёт давать ресурсы. А форт ещё и перестаёт увеличивать лимит ресурсов!");
            AddEvent()->ShowMessage("Мы же при помощи освобождения сможем полностью обездвижить противника!");

            AddEvent()->EnableGo();
            AddEvent()->ShowMessage("Давайте заблокируем оба форта противника!", false, false, true);
            round = 2;
        }
        else if (round == 2)
        {
            AddEvent()->ShowMessage("Вперёд к победе!", false, true, true);

            RealizePlan();
            gatherResources();
            defenceFill();
            burnExtraResources();

//            int i = 0;
//            while (events[i]->type != UNITS_FIGHT) {++i;}
//            AddEvent(i)->ShowMessage("Наш бонус от штурма +2 тратится на то, чтобы пробить оборону форта, также равную 2", false, true, true, events[i]->involved());
//            AddEvent(i + 2)->ShowMessage("В результате, наш Каркун побеждает и остаётся с 1 здоровья.", false, true, false, events[i + 1]->involved());

//            i = 0;
//            while (events[i]->type != UNIT_DECAPTURES_HEX) {++i;}
//            AddEvent(i)->ShowMessage("Мы вошли в клетку противника, и поэтому она тут же перестала ему принадлежать.", false, true, true, QSet <GameUnit *>() << events[i]->unit);

            AddEvent()->ShowMessage("Противник потерял все ресурсы и полностью обездвижен! Ура!", false, false, true);
            AddEvent()->Exit();
        }
    }
    if (N == 2)
    {
        if (round == 0)
        {
            AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
            AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
            AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));
            AddEvent()->ShowMessage("Что это делает противник?!..", false, true, true);

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
            defenceFill();            

            int i = 0;
            while (events[i]->type != UNIT_CAPTURES_HEX) {++i;}
            AddEvent(i)->ShowMessage("В захваченных клетках, в которых ещё не был создан юнит, отображается соответствующая иконка.", false, true, true, events[i]->involved(), "NotBlueUnitHome");

            i = 0;
            while (events[i]->type != UNITS_FIGHT) {++i;}
            AddEvent(i)->ShowMessage("Нападение! Нас атакуют сразу две белки с силой 5 каждая. Мы же находимся в своём форте.", true, false, true, events[i]->involved());
            AddEvent(i+1)->ShowMessage("Форт имеет бонус в обороне +2. Если в клетке стоял и никуда не двигался наш юнит, бонус в обороне действует.", true, false, false, events[i]->involved(), "BlueShield");
            AddEvent(i+2)->ShowMessage("Одновременно сражающиеся юниты всегда теряют одинаковое количество здоровья.", true, false, false, events[i]->involved());
            AddEvent(i+3)->ShowMessage("Чтобы пробить бонус +2 форта, каждый из двух нападающих потеряет по 1 здоровья.", false, false, false, events[i]->involved());
            AddEvent(i+5)->ShowMessage("Чтобы победить защитника силы 5, каждый из двух нападающих потеряет по 3 здоровья.", false, false, false, events[i]->involved());

            while (events[i]->type != UNIT_DIES) {++i;}
            AddEvent(i)->ShowMessage("Наш юнит умер. В клетке его дома появился гробик: больше рекрутировать в этой клетке нельзя.", false, true, true, events[i]->involved(), "Tombstone");

            // TODO: обработка, понятно, та же, но название сбивает. Причина сгорания другая!
            while (events[i]->type != RECRUIT_FAILS_BECAUSE_OF_AGITE) {++i;}
            AddEvent(i)->ShowMessage("Мы потеряли соединение с мельницей! Наше рекрутирование сгорает!", false, true, true, events[i]->involved(), "NotConnected");

            i = 0;
            while (events[i]->type != UNIT_DECAPTURES_HEX) {++i;}
            AddEvent(i)->ShowMessage("Двое нападавших одновременно применяют освобождение и захват. Оба приказа будут выполнены! :(", true, true, true, events[i]->involved());

            while (events[i]->type != UNIT_DIES) {++i;}
            AddEvent(i)->ShowMessage("В разграбленном форте был дом ещё одного нашего юнита! В конце раунда он погибает. :(", true, true, true, events[i]->involved());

            AddEvent()->GetReadyToPlan();
            AddEvent()->DisableGo();

            AddEvent()->ShowMessage("Безобразие!", true, false, true);
            AddEvent()->ShowMessage("Кажется, мы проиграли :( Следите за домами своих юнитов!");
            //AddEvent()->EnableGo();
            //AddEvent()->ShowMessageAndPicture("Хрюшка, несмотря на маленькое здоровье, отличается от других юнитов тем, что умеет стрелять - исполнять приказ выстрела. Давайте раздобудем этот ресурс!", "Fire", false);
            AddEvent()->Exit();
            round = 1;
        }
//        else if (round == 1)
//        {
//            AddEvent()->ShowMessage("Противник обездвижен, так что мы можем не беспокоиться о его действиях!", false, true, true);

//            RealizePlan();
//            burnExtraResources();
//            gatherResources();
//            defenceFill();

//            AddEvent()->GetReadyToPlan();
//            AddEvent()->ReactionOnUnitClick(enemy_unit1, Reaction(DEFAULT, "Выбирая гекс, щёлкайте по гексу, а не по юниту!"));
//            AddEvent()->ReactionOnOrderChoice("Fire", Reaction(DEFAULT, "Стрелять можно не далее чем на две клетки по прямой, однако стрелять через горы, форты и мельницы нельзя."));
//            AddEvent()->ShowMessage("Теперь мы можем стрелять! Попадание в юнита отнимет у него 2 здоровья, но нужно учесть бонус бегемота в обороне +2! Поэтому на то, чтобы захватить форт, тут потребуется больше одного раунда.", false, false, true);

//            round = 2;
//        }
//        else if (round == 2)
//        {
//            if (hexes[1][2]->color == "Neutral" && !checkIfCaptured(Coord(1,2)))
//                AddEvent()->ShowMessage("Подсказка: надо захватить вторую мельницу, чтобы за раунд можно было использовать сразу два приказа выстрела!", false, true);

//            RealizePlan();
//            burnExtraResources();
//            gatherResources();
//            defenceFill();

//            int i = 0;
//            bool isDefenceMentioned = false;
//            while (i < events.size() && events[i]->type != UNIT_DEFENCE_FILLED) {++i;}
//            if (i < events.size())
//            {
//                isDefenceMentioned = true;
//                AddEvent(i + 1)->ShowMessage("Раунд закончился, поэтому бонус бегемота в обороне восполнился. В следующем раунде нужно будет пробивать его снова.", false, true, true);
//            }

//            if (hexes[2][3]->color == rules->players[0])
//            {
//                AddEvent()->ShowMessage("Победа!", false, false, true);
//                AddEvent()->Exit();
//            }
//            else if (africa.contains(unit1))
//            {
//                AddEvent()->ShowMessage("Поражение!", false, false, true);
//                AddEvent()->Exit(false);
//            }
//            else
//            {
//                AddEvent()->GetReadyToPlan();
//                if (isDefenceMentioned)
//                {
//                    AddEvent()->ShowMessage("Раунд закончился, поэтому бонус бегемота в обороне восполнился. В следующем раунде нужно будет пробивать его снова.", false, false, true);
//                }
//                else if (players[rules->players[1]]->units.size() == 3)
//                    AddEvent()->ShowMessage("Учтите бонус бегемота в обороне +2!", false, false);
//                else
//                    AddEvent()->ShowMessage("Осталось только захватить!", false, false);
//            }
//        }
    }
//    if (N == 3)
//    {
//        if (round == 0)
//        {
//            AddEvent()->ShowMessage("Да, так мы гарантированно защищаем свою мельницу!", false, true, true);

//            enemy_unit2->plan[0] = new GameOrder(rules, enemy_unit2->type, "Attack");
//            enemy_unit2->plan[0]->actions[1].target = Coord(3, 2);

//            RealizePlan();
//            destroyHomelessUnits();
//            recruitNewUnits();
//            killAllies();
//            burnExtraResources();
//            gatherResources();
//            defenceFill();

//            int i = 0;
//            while (events[i]->type != UNIT_LEAVES || events[i]->unit->color != rules->players[1]) {++i;}
//            AddEvent(i)->ShowMessage("Наш Каркун вышел из клетки в тот же момент, как в неё начала входить вражеская хрюшка, поэтому боя не произошло.", false, true, true, QSet <GameUnit *>() << events[i]->unit);

//            i = 0;
//            while (events[i]->type != UNIT_LEAVES || events[i]->unit != unit1) {++i;}
//            AddEvent(i)->ShowMessage("Бонус бегемота не работает, так как он передвигается в другую клетку, что не является обороной.", false, true, true, QSet <GameUnit *>() << events[i]->unit);

//            i = 0;
//            while (events[i]->type != UNIT_IS_GOING_TO_RECRUIT) {++i;}
//            AddEvent(i)->ShowMessage("В конце раунда в этой клетке появится новый юнит выбранного типа.", false, true, true, QSet <GameUnit *>() << events[i]->unit);

//            i = 0;
//            while (events[i]->type != UNIT_DIES || events[i]->unit->home != Coord(1, 2)) {++i;}
//            AddEvent(i)->ShowMessage("Дом этого бегемота перестал принадлежать нашему оппоненту, поэтому он погибает в конце раунда.", false, true, true, QSet <GameUnit *>() << events[i]->unit);


//            AddEvent()->GetReadyToPlan();
//            AddEvent()->ShowMessage("Дело осталось за малым - захватить оставшийся форт!", false, false, true);
//            round = 1;
//        }
//        else if (round == 1)
//        {
//            AddEvent()->ShowMessage("Вражеская хрюшка, которая погибла в прошлом раунде, имела дом в клетке с ресурсами, всё ещё принадлежащей врагу. В этой клетке противник может снова использовать приказ захвата, чтобы получить нового юнита.", false, true, true, QSet<GameUnit*>(), "NotRedUnitHome");

//            enemy_unit1->plan[0] = new GameOrder(rules, enemy_unit1->type, "Attack");
//            enemy_unit1->plan[0]->actions[1].target = Coord(4, 5);
//            enemy_unit1->plan[1] = new GameOrder(rules, enemy_unit1->type, "Capture");
//            enemy_unit1->plan[1]->actions[2].unitType = "Karkun";

//            RealizePlan();
//            destroyHomelessUnits();
//            recruitNewUnits();
//            killAllies();
//            burnExtraResources();
//            gatherResources();
//            defenceFill();

//            AddEvent()->ShowMessage("Победа!", false, false, true);
//            AddEvent()->Exit();
//        }
//    }
    if (N == 3)
    {
        QList <Coord> vars;
        vars << Coord(2, 2) << Coord(3, 5) << Coord(0, 1) << Coord(4, 6) << Coord(2, 0) << Coord(3, 0) << Coord(4, 3)
             << Coord(4, 5) << Coord(4, 6) << Coord(4, 7) << Coord(1, 1) << Coord(1, 0) << Coord(3, 1) << Coord(2, 3);
        if (round == 0)
        {
            this->chosenHex[rules->players[1]] = vars[0];
            if (chosenHex[rules->players[0]] == vars[0])
                chosenHex[rules->players[1]] = vars[1];

            AddEvent()->ShowMessage("Игроки выбрали разные клетки, поэтому каждый получает желаемое! На игроков уже действует лимит ресурсов от фортов.", true, true, true);

            HexChosen();
            GameMessage* true_message = events.last();
            AddEvent()->GetReadyToChooseHex(true_message);
            events.removeAt(events.size() - 2);

            AddEvent()->DefaultHexChoiceReaction(Reaction(BLOCK));
            AddEvent()->ShowMessage("Игроки выбрали разные клетки, поэтому каждый получает желаемое! На игроков уже действует лимит ресурсов от фортов.", true, false, true);

            AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
            AddEvent()->ShowMessage("Игроки делают следующий выбор.", false, true, true);
            round = 1;
        }
        else if (round == 1)
        {
            chosenHex[rules->players[1]] = chosenHex[rules->players[0]];

            AddEvent()->ShowMessage("Игроки выбрали одну и ту же клетку, поэтому она никому не достаётся и выкидывается из вариантов.", false, true, true);

            HexChosen();
            GameMessage* true_message = events.last();
            AddEvent()->GetReadyToChooseHex(true_message);
            events.removeAt(events.size() - 2);
            round = 2;

            AddEvent()->DefaultHexChoiceReaction(Reaction(BLOCK));
            AddEvent()->ShowMessage("Игроки выбрали одну и ту же клетку, поэтому она никому не достаётся и выкидывается из вариантов.", true, false, true);

            AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
            AddEvent()->ShowMessage("Выбор стартовых клеток повторяется 7 раз.", false, false, true);
        }
        else
        {
            int i = 0;
            while (!hexes[vars[i].x][vars[i].y]->canBeChosenAsStartPoint) { ++i; }
            chosenHex[rules->players[1]] = vars[i];

            AddEvent()->ShowMessage("Количество оставшихся выборов (пока не) показано в верху экрана.", false, true, true);

            HexChosen();
            if (round < 6)
            {
                GameMessage* true_message = events.last();
                AddEvent()->GetReadyToChooseHex(true_message);
                events.removeAt(events.size() - 2);
                round++;
            }
            else
            {
                events.removeLast();
                AddEvent()->GetReadyToPlan();
                AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
                AddEvent()->DisableGo();

                AddEvent()->ShowMessage("Старт закончился, и началась игра! Цель игры - уничтожить всех юнитов противника!", true, false, true);
                AddEvent()->ShowMessage("Теперь вы знаете все правила \"Фортов и мельниц\"! Успехов в игре!", true, false, false);
                AddEvent()->Exit();
            }
        }
    }
}

