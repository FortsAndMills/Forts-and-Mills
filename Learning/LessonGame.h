#ifndef LESSONGAME_H
#define LESSONGAME_H

#include "GameExecution/Game.h"
#include "LessonMessages.h"

class LessonGame : public Game
{
public:
    int N;

    explicit LessonGame(GameRules * rules, int N) :
        Game(rules, new Random(QList<int>() << 0))
    {
        this->N = N;
        GenerateField();  // из конструктора Games виртуальные функции нас подводят
    }

    int round = 0;
    LessonMessage * AddEvent()
    {
        LessonMessage * Le = new LessonMessage();
        events << Le;
        return Le;
    }
    LessonMessage * AddEvent(int i)
    {
        LessonMessage * Le = new LessonMessage();
        events.insert(i, Le);
        return Le;
    }
    bool antirepeat = false;

    void GenerateField()
    {
        for (int x = 0; x < hexes.size(); ++x)
        {
            hexes[x].clear();
        }
        hexes.clear();

        HexType types[rules->fieldH][rules->fieldW];
        for (int x = 0; x < rules->fieldH; ++x)
        {
            for (int y = 0; y < rules->fieldW; ++y)
            {
                types[x][y] = "Simple";
            }
        }

        if (N == 0)
        {
            types[3][5] = "Fort";
            types[0][0] = "Fort";
        }
        if (N == 1)
        {
            types[2][3] = "Fort";
            types[0][0] = "Fort";
        }
        if (N == 2)
        {
            types[2][3] = "Fort";
            types[2][2] = "Fort";
            types[3][2] = "Mill";
            types[1][2] = "Mill";
            types[1][3] = "Mountain";
            types[1][0] = "Mountain";
        }
        if (N == 3)
        {
            types[1][2] = "Fort";
            types[2][2] = "Fort";
            types[3][2] = "Mill";
            types[3][5] = "Mill";
            types[4][7] = "Mountain";
            types[4][6] = "Mountain";
            types[2][7] = "Mountain";
            types[3][7] = "Mountain";
            types[1][0] = "Mountain";
            types[2][0] = "Mountain";
        }
        if (N == 4)
        {
            types[4][5] = "Fort";
            types[0][1] = "Fort";
            types[2][2] = "Fort";

            types[3][0] = "Mill";
            types[2][3] = "Mill";
            types[2][0] = "Mill";
            types[3][1] = "Mill";
            types[1][1] = "Mill";

            types[4][0] = "Mountain";
            types[0][0] = "Mountain";
            types[0][3] = "Mountain";
            types[3][7] = "Mountain";
            types[0][2] = "Mountain";
        }

        for (int x = 0; x < rules->fieldH; ++x)
        {
            hexes << QList <GameHex *> ();
            for (int y = 0; y < rules->fieldW; ++y)
            {
                hexes[x] << new GameHex(types[x][y], Coord(x, y));
            }
        }

        if (N == 1)
        {
            hexes[2][2]->resources << "Capture";
            hexes[3][2]->resources << "Attack" << "Attack" << "Attack" << "Attack";
            hexes[3][1]->resources << "Capture" << "Attack" << "Attack";
            hexes[1][1]->resources << "Siege";
        }
        if (N == 2)
        {
            hexes[2][1]->resources << "Capture" << "Capture" << "Capture" << "Attack" << "Attack" << "Attack" << "Attack";
            hexes[3][5]->resources << "Attack" << "Go";
            hexes[4][5]->resources << "Capture" << "Attack" << "Attack" << "Go" << "Go";
            hexes[1][1]->resources << "Fire" << "Fire" << "Fire";
        }
        if (N == 3)
        {
            hexes[2][1]->resources << "Capture" << "Capture" << "Attack" << "Go" << "Siege";
            hexes[4][5]->resources << "Capture" << "Attack";
        }
        if (N == 4)
        {
            hexes[1][0]->resources << "Attack" << "Fire" << "Fire" << "Siege";
            hexes[1][2]->resources << "Capture";
            hexes[1][3]->resources << "Capture" << "Fire";

            hexes[2][1]->resources << "Capture" << "Capture";
            hexes[3][2]->resources << "Go";
            hexes[3][3]->resources << "Attack" << "Go";

            hexes[3][4]->resources << "Capture" << "Capture" << "Go" << "Fire";
            hexes[3][5]->resources << "Capture" << "Attack" << "Attack";
            hexes[3][6]->resources << "Capture" << "Capture" << "Attack" << "Go" << "Go";

            hexes[4][1]->resources << "Go";
            hexes[4][2]->resources << "Capture" << "Siege" << "Siege";
            hexes[4][3]->resources << "Attack" << "Siege";

            hexes[4][4]->resources << "Go";
            hexes[4][6]->resources << "Attack" << "Attack";
            hexes[4][7]->resources << "Go";

            hexes[3][0]->livingNation = "Karkun";
            hexes[2][3]->livingNation = "Scrat";
            hexes[2][0]->livingNation = "Scrat";
            hexes[3][1]->livingNation = "Pig";
            hexes[1][1]->livingNation = "Hippo";
        }
    }

    void StartGame()
    {
        if (N == 0)
        {
            rules->doesCaptureRecruits = false;
            rules->limitatingResources = false;

            GameUnit * unit1 = NewUnit(players[rules->players[0]], "Scrat", Coord(4, 1));
            GameUnit * unit2 = NewUnit(players[rules->players[0]], "Scrat", Coord(2, 3));
            GameUnit * enemy_unit = NewUnit(players[rules->players[1]], "Pig", Coord(4, 7));

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
            AddEvent()->ShowMessage("Добро пожаловать в «Форты и мельницы», уникальную стратегию современности! Каждый раунд вам предстоит придумать план, который приведёт вас к победе или поражению. В этом уроке вы обучитесь главному игровому действию - как отдавать своим юнитам приказы.");

            AddEvent()->ShowMessageAndPicture("Цель игры – захватить все форты на карте. Тот, кто в конце раунда обладает всеми фортами, побеждает.", "Fort");
            AddEvent()->ShowMessage("Наша задача - составить для своих юнитов план действий, то есть что они будут делать в этот раунд.");

            AddEvent()->ShowMessage("На нашей карте всего два форта. Мы играем за синих, так что у нас как раз есть два юнита, чтобы эти форты захватить. Но чтобы их захватить, сначала нужно до них добраться.");

            // Отдаём приказ атаки на завтрак
            AddEvent()->ReactionOnUnitClick(unit1, Reaction(BLOCK, "Давай-ка начнём с другого юнита..."));
            AddEvent()->ReactionOnUnitClick(enemy_unit, Reaction(BLOCK, "Это вражеский юнит. Нужно щёлкнуть по своему"));
            AddEvent()->ReactionOnUnitClick(unit2, Reaction(NEXT_PHASE));
            AddEvent()->ShowMessage("Каждому юниту мы должны отдать три приказа – на завтрак, обед и ужин. Давайте отдадим первому юниту приказ атаки на завтрак – для этого по нему нужно кликнуть.", false);

            AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
            AddEvent()->ReactionOnOrderChoice("Capture", Reaction(BLOCK, "Это приказ захвата. Нам нужна атака!", "Attack"));
            AddEvent()->ReactionOnOrderChoice("Attack", Reaction(NEXT_PHASE));
            AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(BLOCK, "Это приказ пропуска хода. Нам нужна атака!", "Attack"));
            AddEvent()->ShowMessageAndPicture("Теперь выберем приказ, который мы хотим отдать - приказ атаки", "Attack", false, true);

            AddEvent()->DefaultHexChoiceReaction(Reaction(BLOCK, "Нам нужно добраться до форта за две атаки, поэтому стоит пойти в другом направлении", "Fort"));
            AddEvent()->ReactionOnHexChoice(hexes[3][4], Reaction(NEXT_PHASE));
            AddEvent()->ShowMessageAndPicture("Осталось выбрать клетку, которую юнит атакует. Нам нужно идти к ближайшему форту.", "Fort", false, true);

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
            AddEvent()->ShowFieldControl();
            AddEvent()->ShowMessage("С первым юнитом разобрались. Второй форт не так далёк от второго юнита, как кажется, потому что карта циклическая. Для удобства её можно прокручивать стрелками внизу.", true, false, true);

            AddEvent()->ShowDayTimes();
            AddEvent()->ShowMessage("Вы можете переключаться между завтраком, обедом и ужином на специальной панели внизу экрана.");

            AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
            AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
            AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));

            AddEvent()->ReactionOnUnitClick(unit1, Reaction(NEXT_PHASE));
            AddEvent()->ReactionOnUnitClick(unit2, Reaction(BLOCK, "Этому юниту мы уже составили план. У нас есть ещё один юнит, план для которого пока не составлен."));
            AddEvent()->ReactionOnUnitClick(enemy_unit, Reaction(DEFAULT, "Это вражеский юнит"));
            AddEvent()->ShowMessage("Щёлкните по другому нашему юниту. Поскольку ему ещё не отдан приказ на завтрак, произойдёт автоматическое переключение на завтрак", false);

            AddEvent()->DefaultHexChoiceReaction(Reaction(NEXT_PHASE));
            AddEvent()->DefaultOrderChoiceReaction(Reaction(NEXT_PHASE));
            AddEvent()->DefaultUnitClickReaction(Reaction(NEXT_PHASE));
            AddEvent()->ShowMessage("Выбрав юнита, у которого на данное время дня уже что-то запланировано, вы удаляете приказы на это и последующее время дня. Таким образом можно удалять ошибочные приказы", true, false, true);

            // Даём творчество игроку
            AddEvent()->ShowGo();

            AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
            AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
            AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));
            AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(DEFAULT, "Приказ пропуска хода выставляется по умолчанию - его не обязательно выставлять вручную, если только юнит не собирается делать что-то другое после него"));
            AddEvent()->ShowMessage("Что ж, пора завершать наш план на этот раунд. Наша задача - захватить оба форта. Когда ваш план будет готов, нажимайте кнопку Go", false);
        }
        if (N == 1)
        {
            rules->doesCaptureRecruits = false;
            rules->limitatingResources = false;

            NewUnit(players[rules->players[0]], "Karkun", Coord(2, 1));
            GameUnit * enemy_unit = NewUnit(players[rules->players[1]], "Pig", Coord(3, 5));

            CaptureHex(hexes[0][0], rules->players[0]);
            GatherResources(hexes[2][1], rules->players[0], QList<QString>() << "Capture" << "Attack");
            GatherResources(hexes[3][1], rules->players[1], QList<QString>() << "Capture" << "Attack" << "Attack");

            AddEvent()->GetReadyToPlan();
            AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));
            AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
            AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
            AddEvent()->ReactionOnOrderChoice(DefaultOrder, Reaction(DEFAULT, "Приказ пропуска хода выставляется по умолчанию - его не обязательно выставлять вручную, если только юнит не собирается делать что-то другое после него"));
            AddEvent()->ReactionOnUnitClick(enemy_unit, Reaction(DEFAULT, "Выбирая гекс, щёлкайте по гексу, а не по юниту!"));

            AddEvent()->HideGo();
            AddEvent()->HideHomes();

            // Вступление
            AddEvent()->ShowMessage("Важным аспектом игры является то, что все приказы, которые вы отдаёте своим юнитам, являются истрачиваемыми ресурсами.");
            AddEvent()->ShowMessage("Каждый игрок знает свои и чужие ресурсы. Они отображаются на боковых панелях. Например, здесь у нас есть одна атака и один захват, а у противника - две атаки и один захват. Это сильно ограничивает наши действия.");
            AddEvent()->ShowMessage("В этом раунде наш Каркун с 4 здоровья даже не может дойти до последнего незахваченного форта! Значит, нам нужны ещё ресурсы.");
            AddEvent()->ShowMessage("Источником ресурсов являются клетки. Захватив клетку, вы будете получать с неё указанные ресурсы в конце каждого раунда до тех пор, пока её не отобьёт противник.");
            AddEvent()->ShowGo();
            AddEvent()->ShowMessage("Вот только чтобы перейти в какую-либо клетку и захватить, мы истратим наши последние приказы атаки и захвата. Поэтому, нам нужна клетка, где есть и атаки, и захваты! Вперёд!", false);
        }
        if (N == 2)
        {
            rules->doesCaptureRecruits = false;

            NewUnit(players[rules->players[0]], "Pig", Coord(2, 2));
            NewUnit(players[rules->players[1]], "Hippo", Coord(3, 5));
            NewUnit(players[rules->players[1]], "Hippo", Coord(3, 5));
            NewUnit(players[rules->players[1]], "Hippo", Coord(4, 5));

            CaptureHex(hexes[3][2], rules->players[0]);
            CaptureHex(hexes[2][1], rules->players[0]);
            CaptureHex(hexes[2][2], rules->players[0]);
            CaptureHex(hexes[1][2], rules->players[1]);
            CaptureHex(hexes[3][5], rules->players[1]);
            CaptureHex(hexes[4][5], rules->players[1]);
            GatherResources(hexes[3][1], rules->players[0], QList<QString>() << "Capture" << "Attack");
            GatherResources(hexes[4][5], rules->players[1], QList<QString>() << "Capture" << "Attack" << "Go");

            AddEvent()->GetReadyToPlan();
            AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));
            AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
            AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
            AddEvent()->ReactionOnOrderChoice("Attack", Reaction(DEFAULT, "В горы ходить нельзя.", "Mountain"));

            AddEvent()->HideGo();
            AddEvent()->HideHomes();

            // Вступление
            AddEvent()->ShowMessageAndPicture("Настало время познакомиться с мельницами. Чем больше у игрока мельниц, тем больше ресурсов у него может быть.", "Mill");
            AddEvent()->ShowMessage("Так, если у игрока 1 мельница, то у него может быть не более 1 захвата, не более 1 атаки и так далее. Если 2 мельницы - не более 2 захватов, не более 2 атак и так далее. Если вы останетесь без мельниц - то останетесь и без ресурсов, и проиграете.");
            AddEvent()->ShowGo();
            AddEvent()->ShowMessage("Сейчас и у нас, и у противника по одной мельнице. Поэтому каждого вида ресурсов у нас может быть не более 1. Однако, единственная мельница противника оставлена незащищённой! Давайте попытаемся отбить её, и таким образом лишить врага всех ресурсов!", false);
        }
        if (N == 3)
        {
            GameUnit * unit = NewUnit(players[rules->players[0]], "Hippo", Coord(3, 1));
            unit->home = Coord(2, 1);
            NewUnit(players[rules->players[0]], "Karkun", Coord(3, 2));

            GameUnit * enemy_unit = NewUnit(players[rules->players[1]], "Hippo", Coord(3, 4));
            enemy_unit->home = Coord(1, 2);
            GameUnit * enemy_unit2 = NewUnit(players[rules->players[1]], "Pig", Coord(4, 2));
            NewUnit(players[rules->players[1]], "Karkun", Coord(3, 5));
            enemy_unit2->home = Coord(4, 5);

            CaptureHex(hexes[3][2], rules->players[0]);
            CaptureHex(hexes[2][1], rules->players[0]);
            CaptureHex(hexes[1][2], rules->players[1]);
            CaptureHex(hexes[3][5], rules->players[1]);
            CaptureHex(hexes[4][5], rules->players[1]);
            GatherResources(hexes[3][2], rules->players[0], QList<QString>() << "Capture" << "Attack" << "Go" << "Siege");
            GatherResources(hexes[4][5], rules->players[1], QList<QString>() << "Capture" << "Attack");

            AddEvent()->GetReadyToPlan();
            AddEvent()->DefaultUnitClickReaction(Reaction(BLOCK));
            AddEvent()->DefaultOrderChoiceReaction(Reaction(BLOCK));
            AddEvent()->DefaultHexChoiceReaction(Reaction(BLOCK));

            AddEvent()->HideGo();
            AddEvent()->ShowMessage("В этом уроке вы узнаете, как и откуда берутся новые войска.");
            AddEvent()->ShowMessageAndPicture("У каждого юнита есть свой дом - некоторая клетка на карте, форт, мельница или обычная клетка с ресурсами. Её можно узнать, наведя мышкой на юнит. ", "BlueUnitHome");
            AddEvent()->ShowMessage("Если потерять клетку с домом юнита, он погибнет в конце раунда. Давайте войдём в незащищённый вражеский форт - тогда мы уничтожим вражеского бегемота с домом в этой клетке! Заодно форт хорошо бы захватить.");

            AddEvent()->ShowGo();
            AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));
            AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
            AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));
            AddEvent()->ReactionOnOrderChoice("Capture", Reaction(DEFAULT, "Захватывая новую клетку, вы получите на ней нового юнита в конце раунда. Тип юнита вы выбираете сами, когда планируете приказ."));

            AddEvent()->ShowMessage("Однако учтите, что вражеская хрюшка может попробовать войти в нашу единственную мельницу!", false);
        }
        if (N == 4)
        {
            ChooseOneOfTheStartHexes();
            AddEvent()->GetReadyToChooseHex(events.last());
            events.removeAt(events.size() - 2);

            AddEvent()->DefaultUnitClickReaction(Reaction(DEFAULT));
            AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
            AddEvent()->DefaultHexChoiceReaction(Reaction(DEFAULT));

            AddEvent()->HideGo();
            AddEvent()->ShowMessage("Каждая партия начинается с того, что игроки выбирают место своего старта на карте. Этим стартом должна быть мельница.");
            AddEvent()->ShowMessage("На каждой мельнице нарисован тип юнита - юнит такого типа получит игрок, выбравший эту мельницу на старте.");
            AddEvent()->ShowGo();
            AddEvent()->ShowMessage("Также игрок получит стартовый комплект ресурсов - захват, ход и ещё один ресурс в зависимости от появившегося юнита: у бегемота и белки это атака, у Каркуна - штурм, у хрюшки - выстрел. Что ж, выберете любую мельницу!", false);
        }
    }

    bool checkIfCaptured(Coord c, bool decaptured = false)
    {
        foreach (GameUnit * unit, players[rules->players[0]]->units)
        {
            Coord unit_c = unit->position;
            for (int i = 0; i < rules->dayTimes; ++i)
            {
                if (unit->plan[i] == NULL)
                    break;

                if (unit->plan[i]->type == "Attack" ||
                     unit->plan[i]->type == "Siege" ||
                     unit->plan[i]->type == "Go")
                {
                    unit_c = unit->plan[i]->actions[1].target;

                    if (c == unit_c && decaptured)
                        return true;
                }
                if (unit->plan[i]->type == "Capture")
                {
                    if (c == unit_c)
                        return true;
                }
            }
        }
        return false;
    }
    bool checkIfEmpty()
    {
        foreach (GameUnit * unit, players[rules->players[0]]->units)
        {
            for (int i = 0; i < rules->dayTimes; ++i)
            {
                if (unit->plan[i] != NULL && unit->plan[i]->type != DefaultOrder)
                    return false;
            }
        }
        return true;
    }
    bool checkIfPlanIsGood()
    {
        if (N == 0)
        {
            if (checkIfCaptured(Coord(0, 0)))
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
            else if (checkIfCaptured(Coord(0,0), true))
            {
                if (antirepeat)
                    AddEvent()->ShowErrorMessage("Так второй юнит только заходит во второй форт, но не захватывает его! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                else
                    AddEvent()->ShowErrorMessage("Второй юнит не захватывает форт! Надо это исправить. Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                antirepeat = !antirepeat;
                return false;
            }
            else
            {
                if (antirepeat)
                    AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - второму юниту нужно дойти до второго форта и захватить его! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                else
                    AddEvent()->ShowErrorMessage("Нам нужно захватить оба форта, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                antirepeat = !antirepeat;
                return false;
            }
        }
        if (N == 1)
        {
            if (round == 0)
            {
                if (checkIfCaptured(Coord(3, 1)))
                {
                    return true;
                }
                else if (checkIfCaptured(Coord(1, 1)) ||
                            checkIfCaptured(Coord(3, 2)))
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Так мы потратим последний захват на клетку, где захватов нет, и останемся в принципе без захватов! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    else
                        AddEvent()->ShowErrorMessage("В захватываемой клетке нет захватов, поэтому так мы останемся без захватов и проиграем! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    antirepeat = !antirepeat;
                    return false;
                }
                else if (checkIfCaptured(Coord(2, 2)))
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Так мы потратим последнюю атаку, но захватим клетку, где атак нет, и останемся совсем без атаки! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    else
                        AddEvent()->ShowErrorMessage("В захватываемой клетке нет атак, поэтому так мы останемся без приказов атаки и проиграем! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    antirepeat = !antirepeat;
                    return false;
                }
                else if (checkIfCaptured(Coord(1, 1), true) ||
                            checkIfCaptured(Coord(3, 2), true) ||
                            checkIfCaptured(Coord(3, 1), true) ||
                            checkIfCaptured(Coord(2, 2), true))
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
                if (checkIfCaptured(Coord(1, 1)))
                {
                    return true;
                }
                else if (checkIfCaptured(Coord(2, 3), true))
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Мы проиграем бой хрюшке, поскольку наша сила равна 4, а её 3+2=5! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    else
                        AddEvent()->ShowErrorMessage("Сила хрюшки 3 + бонус форта в обороне 2 = 5, то есть силы нашего Каркуна 4 не хватит для победы в этом бою! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    antirepeat = !antirepeat;
                    return false;
                }
                else if (checkIfCaptured(Coord(1, 1), true))
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Клетку нужно захватить, чтобы начать получать с неё ресурсы! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    else
                        AddEvent()->ShowErrorMessage("Нам нужно получить ресурс штурма, поэтому клетку с ним нужно захватить! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    antirepeat = !antirepeat;
                    return false;
                }
                else if (checkIfCaptured(Coord(2, 2)) ||
                            checkIfCaptured(Coord(3, 2)))
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Нет, источник захватов и атак у нас уже есть, но нам нужен штурм! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту", "Siege");
                    else
                        AddEvent()->ShowErrorMessage("Нам нужно захватить клетку, где есть штурм, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту", "Siege");
                    antirepeat = !antirepeat;
                    return false;
                }
                else
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - нам нужен штурм! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    else
                        AddEvent()->ShowErrorMessage("Нам нужно получить ресурс штурма, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    antirepeat = !antirepeat;
                    return false;
                }
            }
            else if (round == 2)
            {
                if (checkIfCaptured(Coord(2, 3)))
                {
                    if (players[rules->players[0]]->units[0]->plan[1]->type == "Siege")
                    {
                        return true;
                    }
                    else
                    {
                        if (antirepeat)
                            AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - входя в форт, мы имеем силу 4, и проиграем бой с хрюшкой, имеющей силу 3 и бонус обороны в форту 2! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                        else
                            AddEvent()->ShowErrorMessage("Так мы проиграем бой хрюшке - наша сила 4, а у хрюшки 3 + бонус в обороне 2, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                        antirepeat = !antirepeat;
                        return false;
                    }
                }
                else if (checkIfCaptured(Coord(2, 3), true))
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Форт нужно захватить, то есть использовать приказ захвата! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    else
                        AddEvent()->ShowErrorMessage("Для захвата форта нужно использовать приказ захвата! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    antirepeat = !antirepeat;
                    return false;
                }
                else
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - нам нужно захватить форт! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    else
                        AddEvent()->ShowErrorMessage("Нам нужно захватить форт, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    antirepeat = !antirepeat;
                    return false;
                }
            }
        }
        if (N == 2)
        {
            if (round == 0)
            {
                if (checkIfCaptured(Coord(1, 2), true))
                {
                    return true;
                }
                else
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - нам нужно войти в мельницу врага! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    else
                        AddEvent()->ShowErrorMessage("Нам нужно отбить у врага мельницу, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    antirepeat = !antirepeat;
                    return false;
                }
            }
            else if (round == 1)
            {
                if (checkIfCaptured(Coord(1, 1)))
                {
                    return true;
                }
                else
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - нам нужны выстрелы! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    else
                        AddEvent()->ShowErrorMessage("Нам нужно получить ресурс выстрела, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    antirepeat = !antirepeat;
                    return false;
                }
            }
            else if (round == 2)
            {
                if (!checkIfEmpty())
                {
                    return true;
                }
                else
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Вряд ли план ничего не делать - это хорошая идея...");
                    else
                        AddEvent()->ShowErrorMessage("Ничего не делать целый раунд вряд ли как-то поможет...");
                    antirepeat = !antirepeat;
                    return false;
                }
            }
        }
        if (N == 3)
        {
            if (round == 0)
            {
                if (checkIfCaptured(Coord(1, 2)))
                {
                    GameUnit * hippo = players[rules->players[0]]->units[0];

                    if (hippo->plan[0] == NULL ||
                        hippo->plan[0]->type == "Capture" ||
                        hippo->plan[0]->actions[1].target != Coord(3, 2))
                    {
                        if (hippo->plan[0] != NULL &&
                            hippo->plan[0]->type != "Capture" &&
                            hippo->plan[0]->actions[1].target == Coord(4, 2))
                        {
                            if (antirepeat)
                                AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - вражеская хрюшка преспокойно войдёт в нашу мельницу на завтрак, так и не встретившись с нашим бегемотом! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                            else
                                AddEvent()->ShowErrorMessage("Так мы потеряем свою мельницу на завтрак, поскольку наш бегемот не пересечётся с вражеской хрюшкой, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                            antirepeat = !antirepeat;
                            return false;
                        }
                        else
                        {
                            if (antirepeat)
                                AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - наша мельница осталась незащищённой! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                            else
                                AddEvent()->ShowErrorMessage("Так мы потеряем свою мельницу, ведь в неё сможет войти вражеская хрюшка! Поэтому этот план не годится. Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                            antirepeat = !antirepeat;
                            return false;
                        }
                    }

                    return true;
                }
                else if (checkIfCaptured(Coord(1, 2), true))
                {
                    if (antirepeat)
                        AddEvent()->ShowMessage("Форт ещё нужно захватить! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту", false);
                    else
                        AddEvent()->ShowMessage("Нужно не только отбить форт у противника, но и захватить! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту", false);
                    antirepeat = !antirepeat;
                    return false;
                }
                else
                {
                    if (antirepeat)
                        AddEvent()->ShowMessage("Не, этот план никуда не годится - нам нужно захватить форт, сейчас принадлежащий противнику! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту", false);
                    else
                        AddEvent()->ShowMessage("Нам нужно захватить форт противника, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту", false);
                    antirepeat = !antirepeat;
                    return false;
                }
            }
            if (round == 1)
            {
                if (checkIfCaptured(Coord(2, 2)))
                {
                    return true;
                }
                else
                {
                    if (antirepeat)
                        AddEvent()->ShowErrorMessage("Не, этот план никуда не годится - нам нужно захватить оставшийся форт! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    else
                        AddEvent()->ShowErrorMessage("Нам нужно захватить оставшийся форт, поэтому этот план не годится! Неверные приказы всегда можно стереть, выбрав на панели времени дня нужное время, а затем щёлкнув по юниту");
                    antirepeat = !antirepeat;
                    return false;
                }
            }
        }
        return true;
    }

    void PlanRealisation()
    {
        if (N == 0)
        {
            GameUnit * enemy_unit = players[rules->players[1]]->units[0];
            enemy_unit->plan[0] = new GameOrder(rules, enemy_unit->type, "Attack", 0);
            enemy_unit->plan[0]->actions[1].target = Coord(4, 6);
            enemy_unit->plan[1] = new GameOrder(rules, enemy_unit->type, "Attack", 0);
            enemy_unit->plan[1]->actions[1].target = Coord(3, 5);

            AddEvent()->ShowMessage("Наш противник уже спланировал свой ход. Что ж, теперь мы не можем повлиять на исход раунда. В пошаговом режиме мы можем посмотреть, что же получилось.", true, false, true);

            RealizePlan();

            int i = 0;
            while (events[i]->type != TIME_STARTED) {++i;}
            AddEvent(i)->ShowMessage("Сначала юниты выполнят приказы на завтрак. Сделают они это как бы одновременно, но анимация произойдёт пошагово. Щёлкайте на кнопку перехода к следующему событию!", false, true, true, QList <GameUnit *>() << events[i]->unit, "BlueNext");

            i = 0;
            while (events[i]->type != UNIT_LEAVES || events[i]->unit->color != rules->players[1]) {++i;}
            AddEvent(i)->ShowMessage("Вражеская хрюшка на завтрак тоже выполняет приказ атаки...", false, true, true, QList <GameUnit *>() << events[i]->unit);

            i = 0;
            while (events[i]->type != TIME_STARTED) {++i;}
            ++i;
            while (events[i]->type != TIME_STARTED) {++i;}
            AddEvent(i)->ShowMessage("Завтрак окончен. Начался обед.", false, true, true);

            while (events[i]->type != UNIT_LEAVES || events[i]->unit->color != rules->players[1]) {++i;}
            AddEvent(i)->ShowMessage("Наш юнит столкнулся с вражеским юнитом! Между ними должен произойти бой.", false, true, true, QList <GameUnit *>() << events[i]->unit);

            while (events[i]->type != UNIT_ENTERS || events[i]->unit->color != rules->players[1]) {++i;}
            AddEvent(i + 1)->ShowMessage("У каждого юнита есть один параметр – мощность юнита, что также является его здоровьем. У нашей белки, например, она равна 5, а у вражеской хрюшки всего лишь 3.", false, false, true, QList <GameUnit *>() << events[i]->unit);
            AddEvent(i + 1)->EnableNext(QList <GameUnit *>() << events[i]->unit);

            i = 0;
            while (events[i]->type != UNITS_FIGHT) {++i;}
            AddEvent(i)->ShowMessage("Наш юнит побеждает и остаётся с 2 здоровья.", false, true, false, QList <GameUnit *>() << events[i]->fighters.toVector().toList());

            i = 0;
            while (events[i]->type != UNIT_DIES) {++i;}
            AddEvent(i)->ShowMessage("Вражеская хрюшка доблестно погибает.", false, true, false, QList <GameUnit *>() << events[i]->unit);

            ++i;
            ++i;
            GameUnit * hero = players[rules->players[0]]->units[1];
            while (!events[i]->involved().contains(hero)) {++i;}
            AddEvent(i)->ShowMessage("Наш юнит продолжит выполнять отданные ему приказы.", false, true, true, QList <GameUnit *>() << hero);

            AddEvent()->ShowMessage("Победа! Все форты захвачены, и мы выиграли игру! Поздравляем, вы сделали свой первый шаг в большой мир!", false, false, true);
            AddEvent()->Exit();
        }
        if (N == 1)
        {
            if (round == 0)
            {
                AddEvent()->ShowMessage("Вражеская хрюшка между тем использует свои последние ресурсы, чтобы захватить форт.", false, true, true);

                GameUnit * enemy_unit = players[rules->players[1]]->units[0];
                enemy_unit->plan[0] = new GameOrder(rules, enemy_unit->type, "Attack", 0);
                enemy_unit->plan[0]->actions[1].target = Coord(3, 4);
                enemy_unit->plan[1] = new GameOrder(rules, enemy_unit->type, "Attack", 0);
                enemy_unit->plan[1]->actions[1].target = Coord(2, 3);
                enemy_unit->plan[2] = new GameOrder(rules, enemy_unit->type, "Capture", 0);

                RealizePlan();
                gatherResources();
                defenceFill();

                int i = 0;
                while (events[i]->type != GATHER_RESOURCES) {++i;}
                AddEvent(i)->ShowMessage("Наступил конец раунда, и мы получаем ресурсы со всех принадлежащих нам клеток.", true, true, true);

                AddEvent()->GetReadyToPlan();
                AddEvent()->DisableGo();

                AddEvent()->ShowMessage("Наступил конец раунда, и мы получаем ресурсы со всех принадлежащих нам клеток.", true, false, true);
                AddEvent()->ShowMessage("Начинается новый раунд, теперь у нас есть ресурсы, и мы можем добраться до форта! Однако, там засела вражеская хрюшка силы 3. Более того, захваченный замок даёт в обороне бонус +2, поэтому так просто его Каркуну с силой 4 не победить!");

                AddEvent()->EnableGo();
                AddEvent()->ShowMessageAndPicture("Однако, у Каркуна есть особенность - он и только он может использовать приказ штурма, то есть атаки с бонусом +2! Но сначала нам нужно его заполучить!", "Siege", false);
                round = 1;
            }
            else if (round == 1)
            {
                AddEvent()->ShowMessage("У противника нету ресурсов, поэтому он не может ничего делать. Так что вражеская хрюшка \"исполняет\" приказ пропуска хода.", false, true, true, QList <GameUnit *>(), DefaultOrder);

                RealizePlan();
                gatherResources();
                defenceFill();

                AddEvent()->GetReadyToPlan();
                AddEvent()->ReactionOnOrderChoice("Siege", Reaction(DEFAULT, "Штурм - это по сути обычная атака, но которая в случае боя даст бонус +2. Входить в форт нужно при помощи штурма, чтобы получить бонус и победить хрюшку!", "Siege"));
                AddEvent()->ShowMessage("Осталось лишь отбить форт у противника и захватить его!", false, false, true);
                round = 2;
            }
            else if (round == 2)
            {
                AddEvent()->ShowMessage("Вперёд к победе!", false, true, true);

                RealizePlan();
                gatherResources();
                defenceFill();

                int i = 0;
                while (events[i]->type != UNITS_FIGHT) {++i;}
                AddEvent(i)->ShowMessage("Наш бонус от штурма +2 тратится на то, чтобы пробить оборону форта, также равную 2", false, true, true, events[i]->involved().toList());
                AddEvent(i + 2)->ShowMessage("В результате, наш Каркун побеждает и остаётся с 1 здоровья.", false, true, false, events[i + 1]->involved().toList());

                i = 0;
                while (events[i]->type != UNIT_DECAPTURES_HEX) {++i;}
                AddEvent(i)->ShowMessage("Мы вошли в клетку противника, и поэтому она тут же перестала ему принадлежать.", false, true, true, QList <GameUnit *>() << events[i]->unit);

                AddEvent()->ShowMessage("Победа!", false, false, true);
                AddEvent()->Exit();
            }
        }
        if (N == 2)
        {
            if (round == 0)
            {
                AddEvent()->DefaultOrderChoiceReaction(Reaction(DEFAULT));
                AddEvent()->ShowMessage("Противник между тем пытается захватить другой форт!..", false, true, true);

                GameUnit * enemy_unit = players[rules->players[1]]->units[0];
                enemy_unit->plan[0] = new GameOrder(rules, enemy_unit->type, "Go", 0);
                enemy_unit->plan[0]->actions[1].target = Coord(3, 4);
                enemy_unit->plan[1] = new GameOrder(rules, enemy_unit->type, "Attack", 0);
                enemy_unit->plan[1]->actions[1].target = Coord(2, 3);
                enemy_unit->plan[2] = new GameOrder(rules, enemy_unit->type, "Capture", 0);

                RealizePlan();
                gatherResources();
                burnExtraResources();
                defenceFill();

                int i = 0;
                while (events[i]->type != UNIT_LEAVES || events[i]->unit->color != rules->players[1]) {++i;}
                AddEvent(i)->ShowMessage("Здесь противник использует приказ хода - перемещения, при котором юнит не ожидает угодить в сражение. Это означает, что в бою у него будет штраф -1. Однако, в данном случае было очевидно, что боя не произойдёт, и потому бегемот не опасался штрафа.", false, true, true, QList <GameUnit *>() << events[i]->unit, "Go");

                i = 0;
                while (events[i]->type != CAPTURE_FAILS_BECAUSE_OF_CASTLE) {++i;}
                AddEvent(i)->ShowMessage("Однако приказ захвата у вражеского бегемота сжигается - рядом с клеткой, которую он попытался захватить, есть захваченный нами форт, который не даёт врагу захватывать соседние с ним клетки!", false, true, true, QList <GameUnit *>() << events[i]->unit);

                i = 0;
                while (events[i]->type != GATHER_RESOURCES) {++i;}
                AddEvent(i)->ShowMessage("Противник остался без мельниц, а значит, его лимит ресурсов одного типа равен нулю!", true, true, true);

                AddEvent()->GetReadyToPlan();
                AddEvent()->DisableGo();

                AddEvent()->ShowMessage("Противник остался без мельниц, а значит, его лимит ресурсов одного типа равен нулю!", true, false, true);
                AddEvent()->ShowMessage("Осталось теперь лишь захватить второй форт! Для этого надо как-то уничтожить засевшего там вражеского бегемота c силой 4, который к тому же имеет в обороне бонус +2...");
                AddEvent()->EnableGo();
                AddEvent()->ShowMessageAndPicture("Хрюшка, несмотря на маленькое здоровье, отличается от других юнитов тем, что умеет стрелять - исполнять приказ выстрела. Давайте раздобудем этот ресурс!", "Fire", false);

                round = 1;
            }
            else if (round == 1)
            {
                AddEvent()->ShowMessage("Противник обездвижен, так что мы можем не беспокоиться о его действиях!", false, true, true);

                RealizePlan();
                burnExtraResources();
                gatherResources();
                defenceFill();

                AddEvent()->GetReadyToPlan();
                GameUnit * enemy_unit = players[rules->players[1]]->units[0];
                AddEvent()->ReactionOnUnitClick(enemy_unit, Reaction(DEFAULT, "Выбирая гекс, щёлкайте по гексу, а не по юниту!"));
                AddEvent()->ReactionOnOrderChoice("Fire", Reaction(DEFAULT, "Стрелять можно не далее чем на две клетки по прямой, однако стрелять через горы, форты и мельницы нельзя."));
                AddEvent()->ShowMessage("Теперь мы можем стрелять! Попадание в юнита отнимет у него 2 здоровья, но нужно учесть бонус бегемота в обороне +2! Поэтому на то, чтобы захватить форт, тут потребуется больше одного раунда.", false, false, true);

                round = 2;
            }
            else if (round == 2)
            {
                GameUnit * hero = players[rules->players[0]]->units[0];

                if (hexes[1][2]->color == "Neutral" && !checkIfCaptured(Coord(1,2)))
                    AddEvent()->ShowMessage("Подсказка: надо захватить вторую мельницу, чтобы за раунд можно было использовать сразу два приказа выстрела!", false, true);

                RealizePlan();
                burnExtraResources();
                gatherResources();
                defenceFill();

                int i = 0;
                bool isDefenceMentioned = false;
                while (i < events.size() && events[i]->type != UNIT_DEFENCE_FILLED) {++i;}
                if (i < events.size())
                {
                    isDefenceMentioned = true;
                    AddEvent(i + 1)->ShowMessage("Раунд закончился, поэтому бонус бегемота в обороне восполнился. В следующем раунде нужно будет пробивать его снова.", false, true, true);
                }

                if (hexes[2][3]->color == rules->players[0])
                {
                    AddEvent()->ShowMessage("Победа!", false, false, true);
                    AddEvent()->Exit();
                }
                else if (africa.contains(hero))
                {
                    AddEvent()->ShowMessage("Поражение!", false, false, true);
                    AddEvent()->Exit(false);
                }
                else
                {
                    AddEvent()->GetReadyToPlan();
                    if (isDefenceMentioned)
                    {
                        AddEvent()->ShowMessage("Раунд закончился, поэтому бонус бегемота в обороне восполнился. В следующем раунде нужно будет пробивать его снова.", false, false, true);
                    }
                    else if (players[rules->players[1]]->units.size() == 3)
                        AddEvent()->ShowMessage("Учтите бонус бегемота в обороне +2!", false, false);
                    else
                        AddEvent()->ShowMessage("Осталось только захватить!", false, false);
                }
            }
        }
        if (N == 3)
        {
            if (round == 0)
            {
                AddEvent()->ShowMessage("Да, так мы гарантированно защищаем свою мельницу!", false, true, true);

                GameUnit * enemy = players[rules->players[1]]->units[1];
                enemy->plan[0] = new GameOrder(rules, enemy->type, "Attack", 0);
                enemy->plan[0]->actions[1].target = Coord(3, 2);

                RealizePlan();
                destroyHomelessUnits();
                recruitNewUnits();
                killAllies();
                burnExtraResources();
                gatherResources();
                defenceFill();

                int i = 0;
                while (events[i]->type != UNIT_LEAVES || events[i]->unit->color != rules->players[1]) {++i;}
                AddEvent(i)->ShowMessage("Наш Каркун вышел из клетки в тот же момент, как в неё начала входить вражеская хрюшка, поэтому боя не произошло.", false, true, true, QList <GameUnit *>() << events[i]->unit);

                i = 0;
                while (events[i]->type != UNIT_LEAVES || events[i]->unit != players[rules->players[0]]->units[0]) {++i;}
                AddEvent(i)->ShowMessage("Бонус бегемота не работает, так как он передвигается в другую клетку, что не является обороной.", false, true, true, QList <GameUnit *>() << events[i]->unit);

                i = 0;
                while (events[i]->type != UNIT_IS_GOING_TO_RECRUIT) {++i;}
                AddEvent(i)->ShowMessage("В конце раунда в этой клетке появится новый юнит выбранного типа.", false, true, true, QList <GameUnit *>() << events[i]->unit);

                i = 0;
                while (events[i]->type != UNIT_DIES || events[i]->unit->home != Coord(1, 2)) {++i;}
                AddEvent(i)->ShowMessage("Дом этого бегемота перестал принадлежать нашему оппоненту, поэтому он погибает в конце раунда.", false, true, true, QList <GameUnit *>() << events[i]->unit);


                AddEvent()->GetReadyToPlan();
                AddEvent()->ShowMessage("Дело осталось за малым - захватить оставшийся форт!", false, false, true);
                round = 1;
            }
            else if (round == 1)
            {
                AddEvent()->ShowMessage("Вражеская хрюшка, которая погибла в прошлом раунде, имела дом в клетке с ресурсами, всё ещё принадлежащей врагу. В этой клетке противник может снова использовать приказ захвата, чтобы получить нового юнита.", false, true, true, QList<GameUnit*>(), "NotRedUnitHome");

                GameUnit * enemy = players[rules->players[1]]->units[0];
                enemy->plan[0] = new GameOrder(rules, enemy->type, "Attack", 0);
                enemy->plan[0]->actions[1].target = Coord(4, 5);
                enemy->plan[1] = new GameOrder(rules, enemy->type, "Capture", 0);
                enemy->plan[1]->actions[2].unitType = "Karkun";

                RealizePlan();
                destroyHomelessUnits();
                recruitNewUnits();
                killAllies();
                burnExtraResources();
                gatherResources();
                defenceFill();

                AddEvent()->ShowMessage("Победа!", false, false, true);
                AddEvent()->Exit();
            }
        }
        if (N == 4)
        {
            QList <Coord> vars;
            vars << Coord(3, 0) << Coord(2, 3) << Coord(2, 0) << Coord(3, 1) << Coord(1, 1);
            if (round == 0)
            {
                this->chosenHex[rules->players[1]] = vars[0];
                if (chosenHex[rules->players[0]] == vars[0])
                    chosenHex[rules->players[1]] = vars[1];

                AddEvent()->ShowMessage("Игроки выбрали разные мельницы, поэтому каждый получает желаемое! Однако, выбор на этом не заканчивается - игроки выбирают ещё по мельнице. Так будет продолжаться, пока мельницы на карте не закончатся.", false, true, true);

                HexChosen();
                AddEvent()->GetReadyToChooseHex(events.last());
                events.removeAt(events.size() - 2);
                round = 1;

                AddEvent()->ShowMessage("Игроки выбрали разные мельницы, поэтому каждый получает желаемое! Однако, выбор на этом не заканчивается - игроки выбирают ещё по мельнице. Так будет продолжаться, пока мельницы на карте не закончатся.", false, false, true);
            }
            else if (round == 1)
            {
                chosenHex[rules->players[1]] = chosenHex[rules->players[0]];

                AddEvent()->ShowMessage("Игроки выбрали одну и ту же мельницу, поэтому она никому не достаётся и выкидывается из вариантов. Осталось ещё 2 мельницы, так что игроки могут выбрать ещё по одной!", false, true, true);

                HexChosen();
                AddEvent()->GetReadyToChooseHex(events.last());
                events.removeAt(events.size() - 2);
                round = 2;

                AddEvent()->ShowMessage("Игроки выбрали одну и ту же мельницу, поэтому она никому не достаётся и выкидывается из вариантов. Осталось ещё 2 мельницы, так что игроки могут выбрать ещё по одной!", false, false, true);
            }
            else if (round == 2)
            {
                for (int i = 0; i < rules->fieldH; ++i)
                    for (int j = 0; j < rules->fieldW; ++j)
                        if (hexes[i][j]->canBeChosenAsStartPoint)
                            if (chosenHex[rules->players[0]] != Coord(i, j))
                                chosenHex[rules->players[1]] = Coord(i, j);

                HexChosen();
                events.removeLast();
                AddEvent()->EnableGo();
                AddEvent()->ShowMessage("Мельницы закончились, и началась игра! Теперь вы знаете все правила \"Фортов и мельниц\"! Успехов в игре!", false, false, true);
                AddEvent()->Exit();
            }
        }
    }
};

#endif // LESSONGAME_H
