#ifndef GAME_WINDOW
#define GAME_WINDOW

#include "EventsRealization.h"

class GameWindow : public EventsRealization
{
    Q_OBJECT

public:
    explicit GameWindow(Game * game, qint8 PlayerIndex, GraphicObject * parent);

    bool isRealizingFinished = false;

    // упорядочивание событий от игры!
    void MixEvents();

protected slots:
    void NextPhase();  // Обработка очереди событий

    GameMessage * lastDayTimeEvent(int base);  // находит последнее событие на данное время дня
};

#endif
