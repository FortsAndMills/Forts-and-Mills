#ifndef CONSTANTS
#define CONSTANTS

#include "Headers.h"
#include "Game/GameRules.h"

class Constants
{
public:
    // АНИМАЦИЯ
    const qreal spriteFrameChangeSpeed = 40;  // Частота смена кадров спрайтов
    const qreal framesPerSecond = 20;  // Частота смена кадров анимации

    // частные случаи
    const qreal defaultAnimationTime = 1000;  // по дефолту

    const qreal objectStateChangeTime = 150;  // время изменения состояния

    const qreal dayTimePanelAppearTime = 1000;  // время появления таблицы дня

    const qreal ordersTurnTime = 100;  // время затухания приказа

    const qreal orderDisappearTime = 500;  // время исчезновения приказа
    const qreal orderBurnTime = 850;  // время сожжения приказа
    const qreal orderReconfigureTime = 1200;  // время расположения приказов
    const qreal burningOrdersDelay = 200;  // пауза между подлётами к таблицам

    const qreal unitDisappearTime = 900;  // время исчезновения юнита
    const qreal unitReconfigureTime = 600;  // время перемещения юнитов на их законные места

    const qreal unitPanelAppearTime = 250;  // время появления и исчезновения панели
    const qreal unitOrderReconfigureTime = 100;  // время полёта картинки приказа от панели к юниту

    // ОФОРМЛЕНИЕ-------------------------------------------------------------------------------------------
    const QString MainFontFamily = "Book Antiqua";  // базовый шрифт

    const qreal orderTurnedOffOpacity = 0.3;  // прозрачность использованного ресурса

    int minOrdersInRow = 3;  // минимальное количество "места" для приказов в таблице ресурсов

    qreal minAnimationVelocity = 0.0001;  // меньше этого значения по модулю считаем нулём


    // РАСПОЛОЖЕНИЕ-----------------------------------------------------------------------------------------
    const qreal statesZpos = -0.7;  // положение стэйтов-бэкграундов граф. объектов
    const qreal HexSelectionZpos = -0.6;  // положение подсветки выбранного гекса
    const qreal selectionZpos = -0.8;  // положение подсветки stateobject-а

    const qreal fieldZPos = -0.5;
    const qreal wayZPos = -0.1;
    const qreal unitZPos = 0;
    const qreal selectedUnitZPos = 0.1;
    const qreal unitPanelZPos = 0.2;
    const qreal orderZPos = 0.3;


    // ГЕОМЕЕЕЕТРИИИЯЯ!!!---------------------------------------------------------------------------------
    const int controlPanelHeight = 26;  // Для контрольной панели и переноса окна
    const qreal controlPanelWidth = 126;  // Ширина контрольной панели

    qreal dialogWidth = 0.4;  // Размеры диалогового окна
    qreal dialogHeight = 0.4;

    qreal playerPanelWidth = 0.1;  // Ширина панели игрока
    qreal upperPanelSize = 0.1;  // Высота панели сверху
    qreal playerPanelHeight = 0.675;  // Высота панели игры
    qreal downPanelSize = 0.125;  // Высота панели внизу

    qreal resourcesTableMargin = 0.05;  // Отступы в таблице имеющихся ресурсов

    qreal fieldSideMargin = 0.015;  // Отступы справа и слева от интерфейса поля

    qreal fieldWidth = 0;  // Требует высчитываний, см. ниже функцию
    qreal fieldHeight = 0; // recountSizes
    qreal hexWidth = 0;
    qreal hexHeight = 0;

    qreal specialButtonShiftX = -0.1;  // изменение позиции анимированной кнопки при наведении
    qreal specialButtonShiftY = -0.1;
    qreal specialButtonWidthShift = 1.2;
    qreal specialButtonHeightShift = 1.2;

    qreal dayTimeTableMargin = 0.2;  // расстояние между картинками времени дня от их размера
    qreal dayTimeTableUpSpace = 0.15;  // доля пустого пространства в нижней панели
    qreal dayTimeTableWidth = 0.25;  // доля ширины таблицы дня от ширины всего окна

    qreal dayTimeButtonSelectedShiftX = -0.07;  // размеры в состоянии выделения
    qreal dayTimeButtonSelectedShiftY = -0.07;
    qreal dayTimeButtonSelectedWidthShift = 1.14;
    qreal dayTimeButtonSelectedHeightShift = 1.14;

    qreal artificialPanelX = 0.85;  // Искусственная контрольная панель
    qreal artificialPanelY = 0.85;
    qreal artificialPanelSize = 0.1;

    qreal unitCreatorMargin = 0.08;
    qreal unitCreatorPanelX = 0.03;  // Искусственная панель создания юнитов
    qreal unitCreatorPanelY = 0.8;
    qreal unitCreatorPanelSize = 0.15;


    // Гексы
    const qreal hexShift = 0.26;  // Константа, зависящая от картинки гекса
    const qreal hexWtoH = 1.3;  // Отношение длины гекса к высоте

    // Картинка на гексе:
    const qreal hexPictureOffsetW = 1 / 6.0;
    const qreal hexPictureOffsetH = 1 / 12.0;

    // Таблица ресурсов на гексах:
    const qreal resourceTableMargin = 0.015;  // отступы между картинками
    const qreal resourceTablePicSize = 0.15;  // размер каждой картинки
    const qreal resourceTableBorders = 0.05;  // от границы гекса

    // Картинка домика на гексе:
    const qreal unitHomePointX = 0.25;  // верхний левый угол
    const qreal unitHomePointY = 0.25;
    const qreal unitHomeWidth = 0.5;  // размер
    const qreal unitHomeHeight = 0.5;

    // Путь
    const qreal unitWayWidth = 0.18;  // ширина пути

    // Юниты
    const qreal unitsSizeCoeff = 0.4;  // размер юнита от размера гекса
    qreal unitsSize = 150;  // высчитывается в функции recountSizes()

    const qreal healthPictureOffset = 1 / 5.0;  // отступ по высоте от вертикального здоровья
    const qreal degreeBetweenUnitHealth = 20;  // угол между двумя значками здоровья
    const qreal healthIconWidth = 1 / 4.0;  // размеры иконки здоровья
    const qreal healthIconHeight = 1 / 3.0;

    const qreal unitExplosionPointX = -0.5;  // верхний левый угол
    const qreal unitExplosionPointY = -0.5;
    const qreal unitExplosionWidth = 2;  // размер
    const qreal unitExplosionHeight = 2;

    const qreal unitSelectionPointX = -0.7;  // верхний левый угол
    const qreal unitSelectionPointY = -0.6;
    const qreal unitSelectionWidth = 2.4;  // размер
    const qreal unitSelectionHeight = 2.2;

    // Приказы юниту

    const qreal unitOrderPointX = 0.75;  // верхний левый угол
    const qreal unitOrderPointY = 0.75;
    const qreal unitOrderWidth = 0.5;  // размер
    const qreal unitOrderHeight = 0.5;

    const qreal orderStackShiftX = 0.4;  // В стэке приказов разницы
    const qreal orderStackShiftY = 0.4;

    // Панель юнитов
    const qreal unitPanelPointX = 1;  // верхний левый угол панели
    const qreal unitPanelPointY = 1;
    const qreal unitPanelShiftX = 0;  // смещение панели при появлении
    const qreal unitPanelShiftY = -0.33;

    const qreal orderVariantK = 0.5;  // размер варианта приказа отн. размера юнита
    qreal orderVariantSize;  // вычисляемый размер варианта приказа

    const qreal orderVariantTableSideMarginK = 0.15;  // отступы в таблице от размера юнита
    const qreal orderVariantTableMarginK = 0.1;
    qreal orderVariantTableSideMargin;
    qreal orderVariantTableMargin;

    const int orderVariantTableMaxColumns = 4;  // число колонн в таблице вариантов

    // Приказы
    const qreal orderDisappearPointX = 0.5;  // точка, куда исчезают приказы
    const qreal orderDisappearPointY = 0.75;
    const qreal orderDisappearBurnPointX = -0.5;  // левый верхний угол горения
    const qreal orderDisappearBurnPointY = -0.75;
    const qreal orderBurnWidth = 2;  // размер горения приказа
    const qreal orderBurnHeight = 2;

    explicit Constants() {}
    void recountSizes(qreal W, qreal H, GameRules * rules)
    {
        fieldWidth = W * (1 - 2 * playerPanelWidth - 2 * fieldSideMargin);  // вычисляем размеры поля
        fieldHeight = H * (1 - upperPanelSize - downPanelSize) - 2 * fieldSideMargin * W;

        hexWidth = fieldWidth / (rules->fieldW - (rules->fieldW - 1) * hexShift);  // из него - размеры гексов
        hexHeight = 2 * fieldHeight / (2 * rules->fieldH + 1);

        if (hexWidth < hexHeight * hexWtoH)  // квадрируем гексы и изменяем размеры поля
        {
            hexHeight = hexWidth / hexWtoH;
            fieldHeight = hexHeight * (2 * rules->fieldH + 1) / 2;
        }
        else if (hexHeight * hexWtoH < hexWidth)
        {
            hexWidth = hexHeight * hexWtoH;
            fieldWidth = hexWidth * (rules->fieldW - (rules->fieldW - 1) * hexShift);
        }

        unitsSize = qMin(hexWidth, hexHeight) * unitsSizeCoeff;  // вычисляем размер юнитов

        orderVariantSize = unitsSize * this->orderVariantK;  // размер варианта приказа
        orderVariantTableSideMargin = unitsSize * this->orderVariantTableSideMarginK;
        orderVariantTableMargin = unitsSize * this->orderVariantTableMarginK;
    }
};

#endif
