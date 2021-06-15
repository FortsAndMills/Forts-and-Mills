#include "Technical/Headers.h"
#include "Technical/ProgrammSettings.h"
#include "Technical/AnimationManager.h"
#include "BasicElements/TextObject.h"
#include "Game/GameRules.h"
#include "SystemControlPanel.h"
#include "GameWindow/GameWindow.h"
#include "MenuWindow.h"
#include "CreatingGameWindow.h"
#include "HowToPlayWindow.h"
#include "Background.h"
#include "Learning/LessonGame.h"
#include "Learning/LessonGameWindow.h"

class MainWindow : public QGraphicsView
{
    Q_OBJECT

    enum {MENU, CREATING_GAME, LESSONS, GAME} state = MENU;

    QGraphicsScene * scene;

    Background * background;
    SystemControlPanel * control_panel;

    GameWindow * game_window = NULL;
    MenuWindow * menu_window;
    HowToPlayWindow * how_to_play_window;
    CreatingGameWindow * creating_game_window;

    // ИНИЦИАЛИЗАЦИЯ--------------------------------------------------------------------------
public:
    explicit MainWindow(QWidget * parent = NULL)
        : QGraphicsView(parent)
    {
        this->setWindowFlags(Qt::FramelessWindowHint);  // настроечки
        this->setStyleSheet("background-color: transparent;");
        this->setFrameShape(QFrame::NoFrame);
        this->setAttribute(Qt::WA_TranslucentBackground);
        this->setAttribute(Qt::WA_TransparentForMouseEvents);
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setOptimizationFlags(QGraphicsView::DontClipPainter);
        this->setDragMode(QGraphicsView::NoDrag);
        this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
        this->setCacheMode(QGraphicsView::CacheBackground);

        QCursor c = QCursor(images->get("Cursor").scaled(35, 41), 0, 0);
        setCursor(c);

        connect(client, SIGNAL(CreateGame(qint8, QList <qint32>)), SLOT(CreateGame(qint8, QList <qint32>)));

        scene = new QGraphicsScene(this);  // сцена
        scene->setItemIndexMethod(QGraphicsScene::NoIndex);
        this->setScene(scene);

        background = new Background(scene);  // объявление объектов
        connect(background, SIGNAL(moved(QPoint)), SLOT(moved(QPoint)));

        menu_window = new MenuWindow(background);
        connect(menu_window->CreateNewGame, SIGNAL(leftClicked()), SLOT(StartCreatingGame()));
        connect(menu_window->HowToPlayFrame, SIGNAL(leftClicked()), SLOT(ShowLessons()));

        creating_game_window = new CreatingGameWindow(background);
        connect(creating_game_window->cancel, SIGNAL(leftClicked()), SLOT(returnToMainMenu()));
        connect(creating_game_window->create, SIGNAL(leftClicked()), SLOT(createGame()));
        connect(menu_window->language_selector, SIGNAL(languageChanged()), creating_game_window, SLOT(languageChanged()));

        how_to_play_window = new HowToPlayWindow(background);
        connect(how_to_play_window->HomeButton, SIGNAL(leftClicked()), SLOT(returnToMainMenu()));
        foreach (LessonLabel * LL, how_to_play_window->lessons)
        {
            connect(LL, SIGNAL(clicked(int)), SLOT(CreateLearningGame(int)));
        }
        connect(menu_window->language_selector, SIGNAL(languageChanged()), how_to_play_window, SLOT(languageChanged()));

        control_panel = new SystemControlPanel();
        scene->addItem(control_panel);
        connect(control_panel, SIGNAL(minimize_clicked()), this, SLOT(showMinimized()));
        connect(control_panel, SIGNAL(maximize_clicked()), this, SLOT(MaximizeClicked()));
        connect(control_panel, SIGNAL(close_clicked()), this, SLOT(check_for_close()));

        move(settings->APPLICATION_START_POSITION);  // устанавливаем сохранённые
        resize(settings->APPLICATION_START_SIZE);           // размер и положение

        if (settings->IS_APPLICATION_MAXIMIZED)
            this->showMaximized();

        client->TryToConnect();  // Просим подсоединиться.
    }

    void resizeEvent(QResizeEvent * qre)
    {
        qreal W = qre->size().width();
        qreal H = qre->size().height();
        animations->stopAll();  // Останавливаем всю анимацию

        QGraphicsView::resizeEvent(qre);

        scene->setSceneRect(0, 0, W, H);

        background->setGeometry(0, 0, W, H);
        control_panel->setGeometry(W - constants->controlPanelWidth, 0,
                                   constants->controlPanelWidth, constants->controlPanelHeight);

        resizeWindows(W, H);
    }
    void resizeWindows(qreal W, qreal H)
    {
        menu_window->setGeometry(0, 0, W, H);
        creating_game_window->setGeometry(0, 0, W, H);
        how_to_play_window->setGeometry(0, 0, W, H);
        if (game_window != NULL)
            game_window->setGeometry(0, 0, W, H);
    }


    // СЛОТЫ
public slots:
    void StartCreatingGame()
    {
        state = CREATING_GAME;  // меняем состояние

        menu_window->turn(false);
        creating_game_window->turn(true);
    }
    void createGame()
    {
        state = MENU;  // меняем состояние

        menu_window->turn(true);
        creating_game_window->turn(false);

        client->sendCreateGameMessage();
    }
    void returnToMainMenu()
    {
        state = MENU;  // меняем состояние

        menu_window->turn(true);

        creating_game_window->turn(false);
        how_to_play_window->turn(false);

        if (game_window != NULL)
        {
            // запускаем анимацию пересоздания списка доступных игр
            menu_window->recreateList();

            game_window->AnimationStart(OPACITY, 0);
            game_window->setEnabled(false);
            connect(game_window, SIGNAL(movieFinished()), game_window, SLOT(Delete()));
            game_window = NULL;

            background->setState("Neutral");
            control_panel->setState("Neutral");
        }
        else
        {
            help->HelpAsked("HowToAskHelp");
        }
    }
    void returnToLessons()
    {
        state = LESSONS;  // меняем состояние

        how_to_play_window->turn(true);

        game_window->AnimationStart(OPACITY, 0);
        game_window->setEnabled(false);
        connect(game_window, SIGNAL(movieFinished()), game_window, SLOT(Delete()));
        game_window = NULL;

        background->setState("Neutral");
        control_panel->setState("Neutral");
    }
    void ShowLessons()
    {
        state = LESSONS;
        menu_window->turn(false);
        how_to_play_window->turn(true);
    }
    void CreateGame(qint8 PlayerIndex, QList <qint32> random_numbers) // создание новой игры по запросу сервера
    {
        if (state == GAME)  // если это произошло во время игры, автор программы мартыш.
            debug << "GLOBAL ERROR: creating game during... game\n";
        state = GAME;  // меняем состояние

        // Создаём игру и отображаем её на экране
        GameRules * rules = menu_window->getRules();
        Game * game = new Game(rules, new Random(random_numbers));
        game_window = new GameWindow(game, PlayerIndex, background);
        resizeWindows(width(), height());
        connect(game_window, SIGNAL(GoHome()), SLOT(returnToMainMenu()));
        connect(game_window, SIGNAL(DecidedToQuit()), SLOT(close()));

        menu_window->turn(false);

        game_window->setOpacity(0);
        game_window->AnimationStart(OPACITY, 1);
        background->setState(rules->players[PlayerIndex]);
        control_panel->setState(rules->players[PlayerIndex]);

        qApp->alert(this);
    }
    void CreateLearningGame(int LessonNumber) // создание новой игры по запросу сервера
    {
        if (state == GAME)  // если это произошло во время игры, автор программы мартыш.
            debug << "GLOBAL ERROR: creating lesson game during... game\n";
        state = GAME;  // меняем состояние

        // Создаём игру и отображаем её на экране
        GameRules * rules = new GameRules();
        LessonGame * game = new LessonGame(rules, LessonNumber);
        LessonGameWindow * lgame_window = new LessonGameWindow(background, game);
        game_window = lgame_window;
        resizeWindows(width(), height());
        connect(game_window, SIGNAL(GoHome()), SLOT(returnToLessons()));
        connect(lgame_window, SIGNAL(LessonPassed(int)), SLOT(LessonPassed(int)));
        connect(lgame_window, SIGNAL(DecidedToQuit()), SLOT(close()));

        menu_window->turn(false);
        how_to_play_window->turn(false);

        game_window->setOpacity(0);
        game_window->AnimationStart(OPACITY, 1);
    }
    void LessonPassed(int n)
    {
        settings->lessonsPassed[n] = true;
        menu_window->CheckForLessonsPassed();
        how_to_play_window->CheckForLessonsPassed();
    }

    // РАБОТА КОНТРОЛЬНОЙ ПАНЕЛИ---------------------------------------------------
private slots:
    void MaximizeClicked()
    {
        if (this->isMaximized())
            this->showNormal();
        else
            this->showMaximized();
    }

    void moved(QPoint shift)
    {
        this->move(pos() + shift);
    }

    void check_for_close()
    {
        if (state == GAME)
        {
            game_window->ask_for_close();
        }
        else
            close();
    }

    // СОХРАНЕНИЕ НАСТРОЕК ПО ЗАВЕРШЕНИЮ РАБОТЫ---------------------
    void closeEvent(QCloseEvent *)
    {
        if (state == GAME)
            game_window->giveup();
        client->sendLeaveMessage();

        settings->IS_APPLICATION_MAXIMIZED = this->isMaximized();

        if (this->isMaximized())  // по-другому истинные координаты не получишь
            this->showNormal();
        settings->APPLICATION_START_POSITION = pos();
        settings->APPLICATION_START_SIZE = size();

        settings->write();
        if (logFile->size() != 0)
            qDebug() << "log file is not empty";
        logFile->close();
    }
};
