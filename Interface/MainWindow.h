#include "Technical/Headers.h"
#include "Technical/ProgrammSettings.h"
#include "Technical/AnimationManager.h"
#include "BasicElements/TextObject.h"
#include "Game/GameRules.h"

#include "SystemControlPanel.h"
#include "GameLogic/GameWindow.h"
#include "MenuWindow.h"
#include "Background.h"

class MainWindow : public QGraphicsView
{
    Q_OBJECT

    enum {MENU, GAME} state = MENU;

    QGraphicsScene * scene;

    Background * background;
    Object * backgroundFrame;
    SystemControlPanel * control_panel;

    GameWindow * game_window = NULL;
    MenuWindow * menu_window;

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

        connect(client, SIGNAL(CreateGame(qint8, QList <qint32>)), SLOT(CreateGame(qint8, QList <qint32>)));

        scene = new QGraphicsScene(this);  // сцена
        scene->setItemIndexMethod(QGraphicsScene::NoIndex);
        this->setScene(scene);

        background = new Background(scene, "MainWindow");  // объявление объектов
        connect(background, SIGNAL(moved(QPoint)), SLOT(moved(QPoint)));

        CreateWindows();

        backgroundFrame = new Object();
        backgroundFrame->setPixmap(pictures->get("MainWindowFrame"));
        scene->addItem(backgroundFrame);

        control_panel = new SystemControlPanel();
        scene->addItem(control_panel);
        connect(control_panel, SIGNAL(minimize_clicked()), this, SLOT(showMinimized()));
        connect(control_panel, SIGNAL(maximize_clicked()), this, SLOT(MaximizeClicked()));
        connect(control_panel, SIGNAL(close_clicked()), this, SLOT(close()));

        move(settings->APPLICATION_START_POSITION);  // устанавливаем сохранённые
        resize(settings->APPLICATION_START_SIZE);           // размер и положение

        if (settings->IS_APPLICATION_MAXIMIZED)
            this->showMaximized();
    }
    void CreateWindows()
    {
        menu_window = new MenuWindow(background);

        if (settings->PROGRAMM_VERSION == REAL_CLIENT)
        {
            client->TryToConnect();  // Просим подсоединиться.
        }
        else  // иначе убираем меню и начинаем новую игру
        {
            state = GAME;
            Game * game = new Game(new GameRules());
            game_window = new GameWindow(game, 0, background);

            menu_window->setVisible(false);
        }
    }

    void resizeEvent(QResizeEvent * qre)
    {
        qreal W = qre->size().width();
        qreal H = qre->size().height();
        animations->stopAll();  // Останавливаем всю анимацию

        QGraphicsView::resizeEvent(qre);

        scene->setSceneRect(0, 0, W, H);

        background->setGeometry(0, 0, W, H);
        backgroundFrame->setGeometry(0, 0, W, H);
        control_panel->setGeometry(W - constants->controlPanelWidth, 0,
                                   constants->controlPanelWidth, constants->controlPanelHeight);

        resizeWindows(W, H);
    }
    void resizeWindows(qreal W, qreal H)
    {
        menu_window->setGeometry(0, 0, W, H);
        if (game_window != NULL)
            game_window->setGeometry(0, 0, W, H);
    }


    // СЛОТЫ
public slots:
    void CreateGame(qint8 PlayerIndex, QList <qint32> random) // создание новой игры по запросу сервера
    {
        if (state == GAME)  // если это произошло во время игры, автор программы мартыш.
            qDebug() << "GLOBAL ERROR: creating game during... game";
        state = GAME;  // меняем состояние

        if (game_window != NULL)  // удаляем предыдущие графические данные игры
            game_window->Delete();

        // Создаём игру и отображаем её на экране
        GameRules * rules = new GameRules();
        Game * game = new Game(rules, random);
        game_window = new GameWindow(game, PlayerIndex, background);
        resizeWindows(width(), height());

        menu_window->AnimationStart(OPACITY, 0);  // анимация появления
        game_window->setVisible(true);  // TODO а если окон будет больше чем два?
        game_window->setOpacity(0);
        game_window->AnimationStart(OPACITY, 1);
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


    // СОХРАНЕНИЕ НАСТРОЕК ПО ЗАВЕРШЕНИЮ РАБОТЫ---------------------
    void closeEvent(QCloseEvent *)
    {
        settings->IS_APPLICATION_MAXIMIZED = this->isMaximized();

        if (this->isMaximized())  // по-другому истинные координаты не получишь
            this->showNormal();
        settings->APPLICATION_START_POSITION = pos();
        settings->APPLICATION_START_SIZE = size();

        settings->write();
    }
};
