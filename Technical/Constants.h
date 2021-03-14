#ifndef CONSTANTS
#define CONSTANTS

#include "Headers.h"
#include "Game/GameRules.h"

class Constants
{
public:
    // АНИМАЦИЯ
    static qreal spriteFrameChangeSpeed;  // Частота смена кадров спрайтов
    static qreal framesPerSecond;  // Частота смена кадров анимации

    // частные случаи
    static qreal defaultAnimationTime;  // по дефолту

    static qreal stateObjectChangeTime;  // время изменения состояния
    static qreal mergeTime;  // время появления/исчезновения информации

    static qreal waitingClockRotationTime;

    static qreal dayTimesChangeAnimationTime;  // в меню подготовки игры

    static qreal gameMainPhaseStartPanelsAppearTime;  // время появления таблицы дня

    static qreal panelsChangeTime;  // время смены правой панели
    static qreal digitsChangeTime;  // время смены одной циферки в сумме сил игрока

    static qreal ordersTurnTime;  // время затухания приказа

    static qreal orderDisappearTime;  // время исчезновения приказа
    static qreal orderBurnTime;  // время сожжения приказа
    static qreal orderReconfigureTime;  // время расположения приказов
    static qreal burningOrdersDelay;  // пауза между подлётами к таблицам

    static qreal shieldReconfigureTime;  // время расположения щитов
    static qreal shieldTurnTime;

    static qreal unitDisappearTime;  // время исчезновения юнита
    static qreal unitReconfigureTime;  // время перемещения юнитов на их законные места

    static qreal unitPanelAppearTime;  // время появления и исчезновения панели
    static qreal unitOrderReconfigureTime;  // время полёта картинки приказа от панели к юниту

    static qreal goButtonAppearTime;  // смена основных кнопок

    static qreal progressBarUpdateTime;  // время увеличения прогресс бара

    static qreal mainOrderAppearTime;  // время появления основного приказа
    static qreal mainOrderOpenTime;  // время переворачивания основного приказа

    static qreal rocketAppearTime;  // время появления ракеты.
    static qreal rocketFlyTime;  // время полёта ракеты.
    static qreal blastAppearTime;  // время появления бонуса приказа.
    static qreal blastFlyTime;  // время полёта бонуса приказа
    static qreal rocketPreexplosionTime;
    static qreal rocketExplosionTime;

    static qreal cureIconAppearTime;
    static qreal cureIconAnimationTime;

    static qreal hexResourcesShiftTime;  // время сдвига ресурсов на клетках

    // ОФОРМЛЕНИЕ-------------------------------------------------------------------------------------------
    static QString MainFontFamily;  // базовый шрифт
    static int MaxTextSize;  // максимальный размер текста

    static qreal orderTurnedOffOpacity;  // прозрачность использованного ресурса

    static int minOrdersInRow;  // минимальное количество "места" для приказов в таблице ресурсов

    static qreal minAnimationVelocity;  // меньше этого значения по модулю считаем нулём

    static int serverReconnectionTime;  // время переподсоединения к серверу
    static int serverConnectionCheckTime;  // время переподсоединения к серверу

    static qreal hexOpaqueState;  // прозрачность гексика

    static qreal order_opacity_when_impossible;  // прозрачность невозможных приказов в панели выбора

    // РАСПОЛОЖЕНИЕ-----------------------------------------------------------------------------------------
    static qreal statesZpos;  // положение стэйтов-бэкграундов граф. объектов
    static qreal HexSelectionZpos;  // положение подсветки выбранного гекса
    static qreal selectionZpos;  // положение подсветки stateobject-а

    static qreal fieldZPos;
    static qreal wayZPos;
    static qreal unitZPos;
    static qreal selectedUnitZPos;
    static qreal unitPanelZPos;
    static qreal orderZPos;
    static qreal fortificationZPos;
    static qreal cureIconZPos;


    // ГЕОМЕЕЕЕТРИИИЯЯ!!!---------------------------------------------------------------------------------
    static int controlPanelHeight;  // Для контрольной панели и переноса окна
    static qreal controlPanelWidth;  // Ширина контрольной панели

    static qreal lessonDialogX;
    static qreal lessonDialogY;
    static qreal dialogWidth;  // Размеры диалогового окна
    static qreal dialogHeight;

    static qreal cancelButtonX;
    static qreal cancelButtonY;
    //static qreal cancelButtonWidth;
    static qreal cancelButtonHeight;
    static qreal createButtonX;
    static qreal createButtonY;
    static qreal createButtonWidth;
    static qreal createButtonHeight;

    static qreal howToPlayButtonX;
    static qreal howToPlayButtonY;
    static qreal howToPlayButtonWidth;
    static qreal howToPlayButtonHeight;
    static qreal howToPlayLabelX;
    static qreal howToPlayLabelY;
    static qreal howToPlayLabelWidth;
    static qreal howToPlayLabelHeight;

    static qreal questionX;
    static qreal questionY;
    static qreal questionWidth;
    static qreal questionHeight;

    static qreal playersTuneX;
    static qreal playersTuneY;
    static qreal playersTuneWidth;
    static qreal playersTuneHeight;
    static qreal playersTuneMargin;
    static qreal playersTuneInsideMargin;

    static qreal timerTuneX;
    static qreal timerTuneY;
    static qreal timerTuneWidth;
    static qreal timerTuneHeight;
    static qreal timerTuneMargin;
    static qreal timerTuneInsideMargin;

    static qreal textXShift;

    static qreal fieldTuneX;
    static qreal fieldTuneY;
    static qreal fieldTuneWidth;
    static qreal fieldTuneHeight;

    static qreal unitsTuneY;
    static qreal unitsTuneYshift;
    static qreal unitsMaxHeight;
    static qreal unitsTuneHeight;
    static qreal unitsTuneInsideMargin;

    static qreal ordersTuneY;
    static qreal ordersTuneHeight;
    static qreal ordersMaxHeight;
    static qreal ordersTuneMargin;
    static qreal ordersTuneInsideMargin;

    static qreal dayTimesTuneY;
    static qreal dayTimesTuneWidth;
    static qreal dayTimesTuneHeight;

    static qreal borderY;
    static qreal borderWidth;
    static qreal borderHeight;

    static qreal labelsMarginX;
    static qreal playersLabelX;
    static qreal fieldLabelX;
    static qreal unitsLabelX;
    static qreal ordersLabelX;
    static qreal dayTimesLabelX;
    static qreal joinButtonX;
    static qreal joinButtonY;

    static qreal lessonLabelX;
    static qreal gameLabelX;
    static qreal gameLabelsYStart;
    static qreal gameLabelsHeight;
    static qreal gameLabelsMarginY;


    // ИГРОВОЕ ОКНО--------------------------------------------------------------------------

    static qreal whiteFlagX;
    static qreal whiteFlagHeight;  // высота белого флага
    static qreal whiteFlagWidth;  // ширина белого флага
    static qreal homeButtonX;
    static qreal homeButtonHeight;  // высота кнопки "домой"
    static qreal homeButtonWidth;  // ширина кнопки "домой"

    static qreal playerPanelWidth;  // Ширина панели игрока
    static qreal upperPanelSize;  // Высота панели сверху
    static qreal playerPanelHeight;  // Высота панели игры
    static qreal downPanelSize;  // Высота панели внизу

    static qreal leftPanelSwitchersMargin;  // отступы между кнопками переключения панелей от их ширины
    static qreal leftPanelSwitchersY;
    static qreal leftPanelSwitchersHeight;

    static qreal resourcesTableMarginX;  // Отступы в таблице имеющихся ресурсов
    static qreal resourcesTableMarginY;
    static qreal resourcesTableInsideMargin;

    static qreal playerForcesHeight;  // высота подсчёта сил игрока
    static qreal digitWtoH;           // w / h для циферки

    static qreal fieldSideMargin;  // Отступы справа и слева от интерфейса поля

    qreal fieldWidth = 0;  // Требует высчитываний, см. ниже функцию recountSizes
    qreal fieldHeight = 0;
    qreal hexWidth = 0;
    qreal hexHeight = 0;

    qreal fieldXPos = 0;
    qreal fieldYPos = 0;
    static qreal fieldY;  // позиция поля от высоты

    static qreal hexCopyLimitPointX;
    static qreal hexCopyLimitPointYmargin;
    qreal hexCopyPointX, hexCopyPointY;
    qreal hexCopyWidth, hexCopyHeight;

    static qreal specialButtonShiftX;  // изменение позиции анимированной кнопки при наведении
    static qreal specialButtonShiftY;
    static qreal specialButtonWidthShift;
    static qreal specialButtonHeightShift;

    static qreal dayTimeTableMargin;  // расстояние между картинками времени дня от их размера
    static qreal dayTimeTableUpSpace;  // доля пустого пространства в нижней панели
    static qreal dayTimePictureWidth;  // доля ширины одной картинки времени дня от ширины всего окна

    static qreal dayTimeButtonSelectedShiftX;  // размеры в состоянии выделения
    static qreal dayTimeButtonSelectedShiftY;
    static qreal dayTimeButtonSelectedWidthShift;
    static qreal dayTimeButtonSelectedHeightShift;

    static qreal dayTimeSelectionX;  // размеры спрайта выделения
    static qreal dayTimeSelectionY;
    static qreal dayTimeSelectionWidth;
    static qreal dayTimeSelectionHeight;

    static qreal shiftBetweenGoAndFieldControl;  // расстояние между go и контролем поля
    static qreal fieldControlWidth;  // ширина от размера окна
    static qreal fieldControlHeight;  // высота от размера нижнего пространства

    static qreal goX;  // Кнопка go
    static qreal goY;
    static qreal goSize;

    static qreal timerX;  // Таймер
    static qreal timerY;
    static qreal timerWidth;
    static qreal timerHeight;

    static qreal startChoiceProgressBarY;  // прогресс стартового выбора
    static qreal startChoiceProgressBarWidth;
    static qreal startChoiceProgressBarHeight;

    // Гексы
    static qreal hexShift;  // Константа, зависящая от картинки гекса
    static qreal hexWtoH;  // Отношение длины гекса к высоте

    // Картинка на гексе:
    static qreal hexPictureOffsetW;
    static qreal hexPictureOffsetH;

    // Таблица ресурсов на гексах:
    static qreal resourceTableMargin;  // отступы между картинками
    static qreal resourceTablePicSize;  // размер каждой картинки
    static qreal resourceTableBorders;  // от границы гекса

    // Картинка домика на гексе:
    static qreal unitHomePointX;  // верхний левый угол
    static qreal unitHomePointY;
    static qreal unitHomeWidth;  // размер
    static qreal unitHomeHeight;

    // Картинка информации на гексе:
    static qreal informationPointX;  // верхний левый угол
    static qreal informationPointY;
    static qreal informationWidth;  // размер
    static qreal informationHeight;

    // Картинки щитов на гексе:
    static qreal ShieldSmallStateWidthShift;
    static qreal ShieldSmallStateHeightShift;

    static qreal hexShieldsLineY;
    static qreal hexShieldsMergeX;
    static qreal hexShieldsWidth;  // размер
    static qreal hexShieldsHeight;

    // Речки:
    static qreal riverHeight;

    // Путь
    static qreal unitWayWidth;  // ширина пути

    // Укрепления
    static qreal fortificationWidth; // от размера юнитов
    static qreal fortificationHeight;
    static qreal fortificationStackShiftX;  // В стэке укреплений разницы
    static qreal fortificationStackShiftY;

    // Юниты
    static qreal unitsSizeCoeff;  // размер юнита от размера гекса
    qreal unitsSize = 150;  // высчитывается в функции recountSizes()

    static qreal healthPictureOffset;  // отступ по высоте от вертикального здоровья
    static qreal degreeBetweenUnitHealth;  // угол между двумя значками здоровья
    static qreal healthIconWidth;  // размеры иконки здоровья
    static qreal healthIconHeight;

    static qreal unitExplosionPointX;  // верхний левый угол
    static qreal unitExplosionPointY;
    static qreal unitExplosionWidth;  // размер
    static qreal unitExplosionHeight;

    static qreal unitSelectionPointX;  // верхний левый угол
    static qreal unitSelectionPointY;
    static qreal unitSelectionWidth;  // размер
    static qreal unitSelectionHeight;

    static qreal unitBigSelectionPointX;  // верхний левый угол
    static qreal unitBigSelectionPointY;
    static qreal unitBigSelectionWidth;  // размер
    static qreal unitBigSelectionHeight;

    static qreal unitLightingPointX;  // верхний левый угол
    static qreal unitLightingPointY;
    static qreal unitLightingWidth;  // размер
    static qreal unitLightingHeight;

    // Приказы юниту

    static qreal unitOrderPointX;  // верхний левый угол
    static qreal unitOrderPointY;
    static qreal unitOrderWidth;  // размер
    static qreal unitOrderHeight;

    static qreal mainOrderX;  // верхний левый угол
    static qreal mainOrderY;
    static qreal mainOrderWidth;  // размер
    static qreal mainOrderHeight;

    static qreal mainOrderTurnCoeff;  // увеличение размера при открытии

    static qreal orderStackShiftX;  // В стэке приказов разницы
    static qreal orderStackShiftY;

    // Панель юнитов
    static qreal unitPanelPointX;  // верхний левый угол панели
    static qreal unitPanelPointY;
    static qreal unitPanelShiftX;  // смещение панели при появлении
    static qreal unitPanelShiftY;

    static qreal orderVariantK;  // размер варианта приказа отн. размера юнита
    qreal orderVariantSize;  // вычисляемый размер варианта приказа

    static qreal orderVariantTableSideMarginK;  // отступы в таблице от размера юнита
    static qreal orderVariantTableMarginK;
    qreal orderVariantTableSideMargin;
    qreal orderVariantTableMargin;

    static int orderVariantTableMaxColumns;  // число колонн в таблице вариантов

    // Приказы
    static qreal orderDisappearPointX;  // точка, куда исчезают приказы
    static qreal orderDisappearPointY;
    static qreal orderDisappearBurnPointX;  // левый верхний угол горения
    static qreal orderDisappearBurnPointY;
    static qreal orderBurnWidth;  // размер горения приказа
    static qreal orderBurnHeight;

    // Ракеты
    static qreal rocketWidth;  // от размеров юнитов
    static qreal rocketHeight;
    static qreal blastWidth;
    static qreal blastHeight;

    static qreal smallExpWidth;  // от размера юнитов
    static qreal smallExpHeight;

    // Лечилка
    static qreal cureIconWidth;
    static qreal cureIconHeight;
    static qreal cureIconTargetPointY;
    static qreal cureIconTargetPointX;
    static qreal cureIconTargetPointWidth;
    static qreal cureIconTargetPointHeight;

    // Броня
    static qreal unitShieldsLineY;
    static qreal unitShieldsMergeX;
    static qreal unitShieldsWidth;  // размер
    static qreal unitShieldsHeight;

    explicit Constants() {}
    void recountSizes(qreal W, qreal H, GameRules * rules)
    {
        fieldXPos = W * (playerPanelWidth + fieldSideMargin);
        fieldYPos = H * fieldY;

        fieldWidth = W * (1 - 2 * playerPanelWidth - 2 * fieldSideMargin);  // вычисляем размеры поля
        fieldHeight = H * (1 - fieldY - downPanelSize);

        hexWidth = fieldWidth / (rules->fieldW - (rules->fieldW - 1) * hexShift);  // из него - размеры гексов
        hexHeight = 2 * fieldHeight / (2 * rules->fieldH + 1);

        if (hexWidth < hexHeight * hexWtoH)  // квадрируем гексы и изменяем размеры поля
        {
            hexHeight = hexWidth / hexWtoH;
            qreal newFieldHeight = hexHeight * (2 * rules->fieldH + 1) / 2;
            fieldYPos += (fieldHeight - newFieldHeight) / 2;
            fieldHeight = newFieldHeight;
        }
        else if (hexHeight * hexWtoH < hexWidth)
        {
            hexWidth = hexHeight * hexWtoH;
            qreal newFieldWidth = hexWidth * (rules->fieldW - (rules->fieldW - 1) * hexShift);
            fieldXPos += (fieldWidth - newFieldWidth) / 2;
            fieldWidth = newFieldWidth;
        }

        unitsSize = qMin(hexWidth, hexHeight) * unitsSizeCoeff;  // вычисляем размер юнитов

        orderVariantSize = unitsSize * this->orderVariantK;  // размер варианта приказа
        orderVariantTableSideMargin = unitsSize * this->orderVariantTableSideMarginK;
        orderVariantTableMargin = unitsSize * this->orderVariantTableMarginK;

        // копия гекса
        hexCopyPointX = hexCopyLimitPointX * W;
        hexCopyPointY = H * (upperPanelSize + playerPanelHeight + hexCopyLimitPointYmargin);
        hexCopyWidth = fieldXPos - W * fieldSideMargin;
        hexCopyHeight = H * (1 - upperPanelSize - playerPanelHeight - 2 * hexCopyLimitPointYmargin);

        if (hexCopyWidth < hexCopyHeight * hexWtoH)  // квадрируем гексы и изменяем размеры поля
        {
            hexCopyPointY += (hexCopyHeight - hexCopyWidth / hexWtoH) / 2;
            hexCopyHeight = hexCopyWidth / hexWtoH;
        }
        else if (hexCopyHeight * hexWtoH < hexCopyWidth)
        {
            hexCopyPointX += (hexCopyWidth - hexCopyHeight * hexWtoH) / 2;
            hexCopyWidth = hexCopyHeight * hexWtoH;
        }
    }
};

#endif
