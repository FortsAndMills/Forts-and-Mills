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
        AddEvent()->HideGo();
        AddEvent()->HideDayTimes();
        AddEvent()->HideFieldControl();
        AddEvent()->HideHomes();

        // Вступление
        AddEvent()->ShowMessage("Добро пожаловать в «Форты и мельницы», уникальную стратегию современности!");
        AddEvent()->ShowMessage("Каждый раунд вам предстоит придумать план: раздать всем своим юнитам приказы, кому что делать.");
        AddEvent()->ShowMessageAndPicture("Мы играем за синих. Давайте попробуем захватить расположенный на карте форт.", "Fort");

        // Отдаём приказ атаки на завтрак
        AddEvent()->ReactionOnUnitClick(unit1, Reaction(BLOCK, "Этот юнит не доберётся до форта..."));
        AddEvent()->ReactionOnUnitClick(enemy_unit1, Reaction(BLOCK, "Это вражеский юнит. Ему отдавать приказы будет наш противник."));
        AddEvent()->ReactionOnUnitClick(unit2, Reaction(NEXT_PHASE));
        AddEvent()->ShowMessage("Каждому юниту нам нужно выдать три приказа. Чтобы отдать юниту приказ, по нему нужно кликнуть.", false);

        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
        AddEvent()->ReactionOnOrderChoice("Capture", Reaction(BLOCK, "Это приказ захвата. Нам нужна атака!", "Attack"));
        AddEvent()->ReactionOnOrderChoice("Attack", Reaction(NEXT_PHASE));
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(BLOCK, "Это приказ пропуска хода. Нам нужна атака!", "Attack"));
        AddEvent()->ReactionOnOrderChoice(DeleteLastOrder, Reaction(BLOCK, "Эта кнопка отменяет последний приказ. Выберите атаку!", "Attack"));
        AddEvent()->ShowMessageAndPicture("Теперь выберем приказ, который мы хотим отдать - приказ атаки в соседнюю клетку.", "Attack", false, true);

        AddEvent()->DefaultHexChoiceReaction(Reaction(BLOCK, "Нам нужно добраться до форта за две атаки, поэтому стоит пойти в другом направлении!", "Fort"));
        AddEvent()->ReactionOnHexChoice(hexes[3][4], Reaction(NEXT_PHASE));
        AddEvent()->ShowMessageAndPicture("Осталось выбрать клетку, которую юнит атакует. Нам нужно идти к форту.", "Fort", false, true);

        // Отдаём приказ на обед
        AddEvent()->ReactionOnOrderChoice("Capture", Reaction(BLOCK, "Это приказ захвата. Нам нужна атака!", "Attack"));
        AddEvent()->ReactionOnOrderChoice("Attack", Reaction(NEXT_PHASE));
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(BLOCK, "Это приказ пропуска хода. Нам нужна атака!", "Attack"));
        AddEvent()->ReactionOnOrderChoice(DeleteLastOrder, Reaction(BLOCK, "Эта кнопка отменяет последний приказ. Выберите атаку!", "Attack"));
        AddEvent()->ShowMessageAndPicture("Первый приказ запланирован. Теперь можно отдать ему второй приказ - атаку в форт.", "Attack", false, true);

        AddEvent()->DefaultHexChoiceReaction(Reaction(BLOCK, "Нам нужно атаковать форт!", "Fort"));
        AddEvent()->ReactionOnHexChoice(hexes[3][5], Reaction(NEXT_PHASE));
        AddEvent()->ShowMessageAndPicture("Выбираем атаковать форт!", "Fort", false);


        // Отдаём приказ захвата
        AddEvent()->ReactionOnOrderChoice("Capture", Reaction(NEXT_PHASE));
        AddEvent()->ReactionOnOrderChoice("Attack", Reaction(BLOCK, "Это приказ атаки. Нам нужен захват!", "Capture"));
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(BLOCK, "Это приказ пропуска хода. Нам нужен захват!", "Capture"));
        AddEvent()->ReactionOnOrderChoice(DeleteLastOrder, Reaction(BLOCK, "Эта кнопка отменяет последний приказ. Выберите захват!", "Capture"));
        AddEvent()->ShowMessageAndPicture("Наконец, третьим выполним приказ захвата клетки. Тогда форт будет наш!", "Capture", false, true);


        // Переходим к другому юниту
        AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
        AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));

        AddEvent()->ReactionOnUnitClick(unit1, Reaction(NEXT_PHASE));
        AddEvent()->ReactionOnUnitClick(unit2, Reaction(BLOCK, "Этому юниту мы уже составили план. У нас есть ещё один юнит, план для которого пока не составлен."));
        AddEvent()->ReactionOnUnitClick(enemy_unit1, Reaction(DEFAULT, "Это вражеский юнит"));
        AddEvent()->ShowMessage("Щёлкните по другому нашему юниту. Поскольку ему ещё не отдано ни одного приказа, интерфейс автоматически откатится к первому действию.", false, false, true);


        AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
        AddEvent()->ReactionOnOrderChoice("Capture", Reaction(BLOCK, "Это приказ захвата. Нам нужна атака!", "Attack"));
        AddEvent()->ReactionOnOrderChoice("Attack", Reaction(NEXT_PHASE));
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(BLOCK, "Это приказ пропуска хода. Нам нужна атака!", "Attack"));
        AddEvent()->ReactionOnOrderChoice(DeleteLastOrder, Reaction(BLOCK, "Эта кнопка отменяет последний приказ. Выберите атаку!", "Attack"));
        AddEvent()->ShowMessageAndPicture("Вторым юнитом попробуем заполучить мельницу!", "Mill", false, false);

        AddEvent()->DefaultHexChoiceReaction(Reaction(BLOCK, "Давайте пойдём в мельницу, расположенную клеткой ниже.", "Mill"));
        AddEvent()->ReactionOnHexChoice(hexes[5][1], Reaction(NEXT_PHASE));
        AddEvent()->ShowMessageAndPicture("В озёра ходить нельзя.", "Lake", false, false);

        // Речка, циклическое поле и отмотка назад.
        AddEvent()->ShowMessageAndPicture("Однако, юнит пересёк речку, и поэтому ему больше нельзя выдать приказы.", "River");
        AddEvent()->ShowMessageAndPicture("Значит, эту мельницу нам не захватить. Есть другая мельница, но она далеко... Или нет?", "Mill");

        AddEvent()->ShowFieldControl();
        AddEvent()->ShowMessage("Вовсе нет, потому что карта циклическая. Для удобства её можно прокручивать стрелками внизу.");

        AddEvent()->ShowMessage("Итак, нам надо стереть неверный приказ атаки и пойти в другом направлении.");
        AddEvent()->ShowMessage("Выбирая юнита, у которого на данное время дня уже что-то запланировано, вы отменяете приказ вместе со всем последующим планом для этого юнита.");

        AddEvent()->ShowDayTimes();
        AddEvent()->ShowMessage("Вы можете переключаться между временами дня на специальной панели внизу экрана.");

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
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(DEFAULT, "Приказ пропуска хода выставляется по умолчанию - его не обязательно выставлять вручную, если только юнит не собирается делать что-то другое после него."));
        AddEvent()->ReactionOnOrderChoice(DeleteLastOrder, Reaction(DEFAULT, "Эта кнопка стирает последний приказ."));
        AddEvent()->ShowMessage("Итак, наша задача - захватить форт и мельницу. Когда ваш план будет готов, нажимайте кнопку Go.", false, false, true, QSet<GameUnit*>(), "BlueGoButton");
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
        AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(DEFAULT, "Приказ пропуска хода выставляется по умолчанию - его не обязательно выставлять вручную, если только юнит не собирается делать что-то другое после него."));

        AddEvent()->HideGo();
        AddEvent()->HideHomes();

        // Вступление
        AddEvent()->ShowMessage("Все приказы, которые вы отдаёте своим юнитам, являются расходуемыми ресурсами.");
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

        AddEvent()->Plan();
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
    if (N == 3)
    {
        players["Red"]->playerType = GamePlayer::AI;

        ChooseOneOfTheStartHexes();

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
        if (checkIfCaptured(Coord(3, 9), "Blue"))
        {
            if (checkIfCaptured(Coord(3, 5), "Blue"))
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
        else if (checkIfBlocked(Coord(3,9), "Blue"))
        {
            if (antirepeat)
                AddEvent()->ShowErrorMessage("Так второй юнит только заходит в мельницу, но не захватывает её! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
            else
                AddEvent()->ShowErrorMessage("Второй юнит не захватывает мельницу! Надо это исправить. Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
            antirepeat = !antirepeat;
            return false;
        }
        else
        {
            if (antirepeat)
                AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - второму юниту нужно дойти до мельницы и захватить её! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
            else
                AddEvent()->ShowErrorMessage("Нам нужно захватить и форт, и мельницу, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
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
                    AddEvent()->ShowErrorMessage("Чтобы начать получать ресурсы, клетку нужно захватить, то есть исполнить приказ захвата! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
                else
                    AddEvent()->ShowErrorMessage("Нужно не только войти в клетку, но и отдать приказ захвата! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
                antirepeat = !antirepeat;
                return false;
            }
            else
            {
                if (antirepeat)
                    AddEvent()->ShowErrorMessage("Не, этот план не годится - нам нужны ресурсы! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
                else
                    AddEvent()->ShowErrorMessage("Нам нужно захватить клетку с ресурсами, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
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
                    AddEvent()->ShowErrorMessage("Нужно ещё захватить клетку с ресурсом, чтобы получить его! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.", "Liberate");
                else
                    AddEvent()->ShowErrorMessage("Клетка с освобождением не захвачена! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.", "Liberate");
                antirepeat = !antirepeat;
                return false;
            }
            else
            {
                if (antirepeat)
                    AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - нужно получить ресурс освобождения! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.", "Liberate");
                else
                    AddEvent()->ShowErrorMessage("Нам нужно получить освобождение, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.", "Liberate");
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
                        AddEvent()->ShowErrorMessage("Так мы освободим один форт, но не заблокируем второй! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
                    else
                        AddEvent()->ShowErrorMessage("Так один вражеский форт останется незаблокированным! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
                    antirepeat = !antirepeat;
                    return false;
                }
            }
            else if (checkIfExecuted(Coord(4, 6), "Recruit", "Blue"))
            {
                if (checkIfBlocked(Coord(3, 5), "Blue"))
                {
                    return true;
                }
                else
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Так мы освободим один форт, но не заблокируем второй! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
                    else
                        AddEvent()->ShowErrorMessage("Так один вражеский форт останется незаблокированным! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
                    antirepeat = !antirepeat;
                    return false;
                }
            }
            else
            {
                if (antirepeat)
                    AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - нам нужно лишить противника фортов! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
                else
                    AddEvent()->ShowErrorMessage("Нам нужно лишить противника фортов, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
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
            else
            {
                if (antirepeat)
                    AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - нам нужно зарекрутировать нового юнита! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
                else
                    AddEvent()->ShowErrorMessage("Мы хотели зарекрутировать нового юнита, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту.");
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

        AddEvent()->ShowMessage("Наш противник уже спланировал свой ход. Что ж, теперь мы не можем повлиять на исход раунда.", true, false, true);

        RealizePlan();

        int i = 0;
        while (events[i]->type != TIME_STARTED) {++i;}
        AddEvent(i)->ShowMessage("Сначала юниты одновременно выполнят свои первые приказы. В пошаговом режиме мы можем посмотреть, что же получилось. Щёлкайте на кнопку перехода к следующему событию!", false, true, true, QSet <GameUnit *>() << events[i]->unit, "BlueNext");

        i = 0;
        while (events[i]->type != UNIT_LEAVES || events[i]->unit->color != rules->players[1]) {++i;}
        AddEvent(i)->ShowMessage("Вражеская хрюшка тоже выполняет приказ атаки...", false, true, true, QSet <GameUnit *>() << events[i]->unit, "Attack");

        i = 0;
        while (events[i]->type != TIME_STARTED) {++i;}
        ++i;
        while (events[i]->type != TIME_STARTED) {++i;}
        AddEvent(i)->ShowMessage("Юниты переходят к выполнению своих вторых приказов.", false, true, true);

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
        state = Game::GS_PLAN;

        if (round == 0)
        {
            AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
            AddEvent()->ShowMessage("Похоже, враг тоже пытается заполучить ресурсы!", false, true, true);

            enemy_unit1->plan[0] = new GameOrder(rules, enemy_unit1->type, "Go");
            enemy_unit1->plan[0]->actions[1].target = Coord(3, 3);
            enemy_unit1->plan[1] = new GameOrder(rules, enemy_unit1->type, "Capture");

            enemy_unit2->plan[0] = new GameOrder(rules, enemy_unit2->type, "Go");
            enemy_unit2->plan[0]->actions[1].target = Coord(1, 2);

            RealizePlan();
            gatherResources();
            defenceFill();

            int i = 0;
            // TODO: подумать над выделением при помощи <span style=\"color:blue\">
            while (events[i]->type != UNIT_FINISHES_ENTER || events[i]->unit != enemy_unit1) {++i;}
            AddEvent(i-1)->ShowMessage("Враги используют приказ хода: перемещения со штрафом в бою -1.", false, true, true, events[i]->involved(), "Go");

            while (events[i]->type != UNITS_FIGHT) {++i;}
            AddEvent(i)->ShowMessage("Мы поймали врага в тот момент, когда он исполнял приказ захвата! Враг получает штраф в бою -1, и мы остаёмся с 2 здоровья.", false, true, true, events[i]->involved(), "Capture");

            while (events[i]->type != GATHER_RESOURCES) {++i;}
            AddEvent(i)->ShowMessage("Наступил конец раунда, и игроки получают ресурсы со всех принадлежащих им клеток.", true, true, true);

            StartPlanning();
            AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
            AddEvent()->DisableGo();

            AddEvent()->ShowMessage("Наступил конец раунда, и игроки получают ресурсы со всех принадлежащих им клеток.", true, false, true);
            AddEvent()->ShowMessage("Хотя у врага целых четыре источника ресурса хода, он получил лишь два. Это потому, что у врага всего два форта.");
            AddEvent()->ShowMessageAndPicture("Каждый форт увеличивает лимит ресурсов одного типа на один.", "Fort");

            AddEvent()->EnableGo();
            AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));
            AddEvent()->ShowMessageAndPicture("Чтобы лишать противника клеток, понадобится приказ освобождения. Давайте получим его!", "Liberate", false);
            round = 1;
        }
        else if (round == 1)
        {
            enemy_unit2->plan[0] = new GameOrder(rules, enemy_unit2->type, "Go");
            enemy_unit2->plan[0]->actions[1].target = Coord(0, 1);

            AddEvent()->ShowMessage("Противник между тем вошёл в наш форт!", false, true, true, QSet <GameUnit *>() << enemy_unit2);

            RealizePlan();
            gatherResources();
            defenceFill();
            burnExtraResources();

            StartPlanning();
            AddEvent()->DisableGo();
            AddEvent()->ReactionOnOrderChoice("Liberate", Reaction(DEFAULT, "Как и захват, освобождение в случае сражения дало бы штраф -1.", "Liberate"));
            AddEvent()->ShowMessage("Пока в клетке находится враг, она перестаёт давать ресурсы. А форт ещё и перестаёт увеличивать лимит ресурсов!");
            AddEvent()->ShowMessageAndPicture("Но клетка будет принадлежать игроку, пока враг не применит в ней приказ освобождения.", "Liberate");

            AddEvent()->EnableGo();
            AddEvent()->ShowMessage("Сейчас мы при помощи освобождения сможем полностью обездвижить противника! Давайте заблокируем оба форта противника!", false, false, false);
            round = 2;
        }
        else if (round == 2)
        {
            AddEvent()->ShowMessage("Вперёд к победе!", false, true, true);

            RealizePlan();
            gatherResources();
            defenceFill();
            burnExtraResources();

            AddEvent()->ShowMessage("Противник потерял все ресурсы и полностью обездвижен! Ура!", false, false, true);
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

            AddEvent()->Plan();
            AddEvent()->DisableGo();

            AddEvent()->ShowMessage("Безобразие!", true, false, true);
            AddEvent()->ShowMessage("Кажется, мы проиграли :( Следите за домами своих юнитов!");
            //AddEvent()->EnableGo();
            //AddEvent()->ShowMessageAndPicture("Хрюшка, несмотря на маленькое здоровье, отличается от других юнитов тем, что умеет стрелять - исполнять приказ выстрела. Давайте раздобудем этот ресурс!", "Fire", false);
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

            AddEvent()->ShowMessage("Игроки выбрали разные клетки, поэтому каждый получает желаемое! На игроков уже действует лимит ресурсов от фортов.", true, true, true);

            Game::NextStage();

            AddEvent()->DefaultHexChoiceReaction(Reaction(NEXT_PHASE));
            AddEvent()->ShowMessage("Игроки выбрали разные клетки, поэтому каждый получает желаемое! На игроков уже действует лимит ресурсов от фортов.", true, false, true);

            AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
            AddEvent()->ShowMessage("Игроки делают следующий выбор.", false, false, true);
            round = 1;
        }
        else if (round == 1)
        {
            chosenHex[rules->players[1]] = chosenHex[rules->players[0]];

            Game::NextStage();

            AddEvent()->DefaultHexChoiceReaction(Reaction(NEXT_PHASE));
            AddEvent()->ShowMessage("Игроки выбрали одну и ту же клетку, поэтому она никому не достаётся и выкидывается из вариантов.", true, false, true);

            AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
            AddEvent()->ShowMessage("Выбор стартовых клеток повторяется 7 раз.", false, false, true);
            round = 2;
        }
        else
        {
            if (round <= 6)
            {
                int i = 0;
                while (!hexes[vars[i].x][vars[i].y]->canBeChosenAsStartPoint) { ++i; }
                chosenHex[rules->players[1]] = vars[i];
            }

            if (round == 2)
            {
                AddEvent()->ShowStartProgressBar();
                final_message = "Количество оставшихся выборов показано в верху экрана.";
            }

            Game::NextStage();

            if (round < 6)
            {
                if (round == 4 && players["Blue"]->resources["Capture"] == 0)
                {
                    final_message = "Стоит взять хотя бы один форт, иначе вы останетесь без ресурсов!";
                }
                AddEvent()->ShowMessage(final_message, false, false, true);
                round++;
            }
            else if (round == 6)
            {
                AddEvent()->DisableGo();
                AddEvent()->ShowMessage("Старт закончился, и началась игра! Цель игры - уничтожить всех юнитов противника!", true, false, true);

                AddEvent()->EnableGo();
                AddEvent()->ShowMessage("Теперь вы знаете все правила \"Фортов и мельниц\"! Попробуйте победить глупенького бота!", false, false, false);
                round += 1;
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
                        AddEvent()->ShowMessage("Победа! Попробуйте теперь сыграть с людьми.");
                        AddEvent()->Exit();
                    }
                    else
                    {
                        AddEvent()->ShowMessage("Поражение! Попробуйте ещё раз.");
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

