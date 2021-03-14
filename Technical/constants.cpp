#include "Constants.h"

// АНИМАЦИЯ
qreal Constants::spriteFrameChangeSpeed = 40;  // Частота смена кадров спрайтов
qreal Constants::framesPerSecond = 20;  // Частота смена кадров анимации

// частные случаи
qreal Constants::defaultAnimationTime = 600;  // по дефолту

qreal Constants::stateObjectChangeTime = 125;  // время изменения состояния
qreal Constants::mergeTime = 500;  // время появления/исчезновения информации

qreal Constants::waitingClockRotationTime = 1000;

qreal Constants::dayTimesChangeAnimationTime = 200;

qreal Constants::gameMainPhaseStartPanelsAppearTime = 1000;  // время появления таблицы дня

qreal Constants::panelsChangeTime = 100;  // время смены правой панели

qreal Constants::ordersTurnTime = 100;  // время затухания приказа

qreal Constants::orderDisappearTime = 500;  // время исчезновения приказа
qreal Constants::orderBurnTime = 850;  // время сожжения приказа
qreal Constants::orderReconfigureTime = 1200;  // время расположения приказов
qreal Constants::burningOrdersDelay = 200;  // пауза между подлётами к таблицам

qreal Constants::shieldReconfigureTime = 300;  // время расположения щитов
qreal Constants::shieldTurnTime = 300;

qreal Constants::unitDisappearTime = 900;  // время исчезновения юнита
qreal Constants::unitReconfigureTime = 400;  // время перемещения юнитов на их законные места

qreal Constants::unitPanelAppearTime = 250;  // время появления и исчезновения панели
qreal Constants::unitOrderReconfigureTime = 100;  // время полёта картинки приказа от панели к юниту

qreal Constants::goButtonAppearTime = 1000;  // смена основных кнопок

qreal Constants::progressBarUpdateTime = 500;  // смена прогресс бара

qreal Constants::mainOrderAppearTime = 300;  // время появления основного приказа
qreal Constants::mainOrderOpenTime = 300;  // время переворачивания основного приказа

qreal Constants::rocketAppearTime = 179;  // время появления ракеты.
qreal Constants::rocketFlyTime = 600;  // время полёта ракеты.
qreal Constants::blastAppearTime = 80;  // время появления бонуса приказа.
qreal Constants::blastFlyTime = 360;  // время полёта бонуса приказа
qreal Constants::rocketPreexplosionTime = 0;
qreal Constants::rocketExplosionTime = 300;

qreal Constants::cureIconAppearTime = 300;
qreal Constants::cureIconAnimationTime = 1200;

qreal Constants::hexResourcesShiftTime = 100;  // время сдвига ресурсов на клетках

// ОФОРМЛЕНИЕ-------------------------------------------------------------------------------------------
QString Constants::MainFontFamily = "Rubius";  // базовый шрифт
int Constants::MaxTextSize = 30;

qreal Constants::orderTurnedOffOpacity = 0.3;  // прозрачность использованного ресурса

int Constants::minOrdersInRow = 3;  // минимальное количество "места" для приказов в таблице ресурсов

qreal Constants::minAnimationVelocity = 0.001;  // меньше этого значения по модулю считаем нулём

int Constants::serverReconnectionTime = 100;  // время переподсоединения к серверу
int Constants::serverConnectionCheckTime = 10000;  // время переподсоединения к серверу

qreal Constants::hexOpaqueState = 0.4;  // прозрачность гексика

qreal Constants::order_opacity_when_impossible = 0.3;  // прозрачность невозможных приказов в панели выбора

// РАСПОЛОЖЕНИЕ-----------------------------------------------------------------------------------------
qreal Constants::statesZpos = -0.7;  // положение стэйтов-бэкграундов граф. объектов
qreal Constants::HexSelectionZpos = -0.6;  // положение подсветки выбранного гекса
qreal Constants::selectionZpos = -0.8;  // положение подсветки stateobject-а

qreal Constants::fieldZPos = -0.5;
qreal Constants::wayZPos = -0.2;
qreal Constants::unitZPos = -0.1;
qreal Constants::selectedUnitZPos = 0.1;
qreal Constants::unitPanelZPos = 0.3;
qreal Constants::orderZPos = 0.2;
qreal Constants::fortificationZPos = 0.05;
qreal Constants::cureIconZPos = 0.7;


// ГЕОМЕЕЕЕТРИИИЯЯ!!!---------------------------------------------------------------------------------
int Constants::controlPanelHeight = 26;  // Для контрольной панели и переноса окна
qreal Constants::controlPanelWidth = 126;  // Ширина контрольной панели

qreal Constants::lessonDialogX = 0.5;
qreal Constants::lessonDialogY = 0.11;
qreal Constants::dialogWidth = 0.36;  // Размеры диалогового окна
qreal Constants::dialogHeight = 0.3;

qreal Constants::cancelButtonX = 0.05;
qreal Constants::cancelButtonY = 0.875;
//qreal Constants::cancelButtonWidth = 0.05;
qreal Constants::cancelButtonHeight = 0.075;
qreal Constants::createButtonX = 0.35;
qreal Constants::createButtonY = 0.85;
qreal Constants::createButtonWidth = 0.3;
qreal Constants::createButtonHeight = 0.12;

qreal Constants::howToPlayButtonX = 0.35;
qreal Constants::howToPlayButtonY = 0.85;
qreal Constants::howToPlayButtonWidth = 0.3;
qreal Constants::howToPlayButtonHeight = 0.12;
qreal Constants::howToPlayLabelX = 0.35;//0.425;
qreal Constants::howToPlayLabelY = 0.85;//0.865;
qreal Constants::howToPlayLabelWidth = 0.3;//0.15;
qreal Constants::howToPlayLabelHeight = 0.12;//0.9;

qreal Constants::questionX = 0.8;
qreal Constants::questionY = 0.85;
qreal Constants::questionWidth = 0.07;
qreal Constants::questionHeight = 0.12;

qreal Constants::playersTuneX = 0.05;
qreal Constants::playersTuneY = 0.05;
qreal Constants::playersTuneWidth = 0.3;
qreal Constants::playersTuneHeight = 0.2;
qreal Constants::playersTuneMargin = 0.1;
qreal Constants::playersTuneInsideMargin = 0.2;

qreal Constants::timerTuneX = 0.7;
qreal Constants::timerTuneY = 0.76;
qreal Constants::timerTuneWidth = 0.23;
qreal Constants::timerTuneHeight = 0.2;
qreal Constants::timerTuneMargin = 0.1;
qreal Constants::timerTuneInsideMargin = 0.2;

qreal Constants::textXShift = 0.3;

qreal Constants::fieldTuneX = 0.65;
qreal Constants::fieldTuneY = 0.05;
qreal Constants::fieldTuneWidth = 0.3;
qreal Constants::fieldTuneHeight = 0.2;

qreal Constants::unitsTuneY = 0.3;
qreal Constants::unitsTuneYshift = 0.1;
qreal Constants::unitsMaxHeight = 0.7;
qreal Constants::unitsTuneHeight = 0.1;
qreal Constants::unitsTuneInsideMargin = 0.2;

qreal Constants::borderY = 0.43;
qreal Constants::borderWidth = 0.96;
qreal Constants::borderHeight = 0.04;

qreal Constants::ordersTuneY = 0.5;
qreal Constants::ordersMaxHeight = 0.75;
qreal Constants::ordersTuneHeight = 0.1;
qreal Constants::ordersTuneMargin = 0.1;
qreal Constants::ordersTuneInsideMargin = 0.2;

qreal Constants::dayTimesTuneY = 0.75;
qreal Constants::dayTimesTuneWidth = 0.4;
qreal Constants::dayTimesTuneHeight = 0.1;

qreal Constants::labelsMarginX = 0.02;
qreal Constants::playersLabelX = 0.06;
qreal Constants::unitsLabelX = 0.24;
qreal Constants::fieldLabelX = 0.4;
qreal Constants::ordersLabelX = 0.52;
qreal Constants::dayTimesLabelX = 0.78;
qreal Constants::joinButtonX = 0.9;
qreal Constants::joinButtonY = 0.25;

qreal Constants::lessonLabelX = 0.35;
qreal Constants::gameLabelX = 0.03;
qreal Constants::gameLabelsYStart = 0.21;
qreal Constants::gameLabelsHeight = 0.1;
qreal Constants::gameLabelsMarginY = 0.02;

// ИГРОВОЕ ОКНО

qreal Constants::homeButtonX = 0.015;  // позиция "домой"
qreal Constants::homeButtonHeight = 0.07;  // высота "домой"
qreal Constants::homeButtonWidth = 0.04;  // ширина "домой"
qreal Constants::whiteFlagX = 0.06;  // позиция белого флага
qreal Constants::whiteFlagHeight = 0.07;  // высота белого флага
qreal Constants::whiteFlagWidth = 0.04;  // ширина белого флага

qreal Constants::playerPanelWidth = 0.115;  // Ширина панели игрока
qreal Constants::upperPanelSize = 0.1;  // Высота панели сверху
qreal Constants::playerPanelHeight = 0.600;  // Высота панели игры
qreal Constants::downPanelSize = 0.125;  // Высота панели внизу

qreal Constants::leftPanelSwitchersMargin = 0.2;  // отступы между кнопками переключения панелей от их ширины
qreal Constants::leftPanelSwitchersY = 0.05;
qreal Constants::leftPanelSwitchersHeight = 0.04;

qreal Constants::resourcesTableMarginX = 0.07;  // Отступы в таблице имеющихся ресурсов
qreal Constants::resourcesTableMarginY = 0.02;
qreal Constants::resourcesTableInsideMargin = 0.05;

qreal Constants::fieldSideMargin = 0.015;  // Отступы справа и слева от интерфейса поля

qreal Constants::fieldY = 0.08;  // позиция поля от высоты

qreal Constants::hexCopyLimitPointX = 0.015;
qreal Constants::hexCopyLimitPointYmargin = 0.005;

qreal Constants::specialButtonShiftX = -0.1;  // изменение позиции анимированной кнопки при наведении
qreal Constants::specialButtonShiftY = -0.1;
qreal Constants::specialButtonWidthShift = 1.2;
qreal Constants::specialButtonHeightShift = 1.2;

qreal Constants::dayTimeTableMargin = 0.2;  // расстояние между картинками времени дня от их размера
qreal Constants::dayTimeTableUpSpace = 0.15;  // доля пустого пространства в нижней панели
qreal Constants::dayTimePictureWidth = 0.0735;  // доля ширины одной картинки времени дня от ширины всего окна

qreal Constants::dayTimeButtonSelectedShiftX = -0.07;  // размеры в состоянии выделения
qreal Constants::dayTimeButtonSelectedShiftY = -0.07;
qreal Constants::dayTimeButtonSelectedWidthShift = 1.14;
qreal Constants::dayTimeButtonSelectedHeightShift = 1.14;

qreal Constants::dayTimeSelectionX = -0.5;  // размеры спрайта выделения
qreal Constants::dayTimeSelectionY = -0.5;
qreal Constants::dayTimeSelectionWidth = 2;
qreal Constants::dayTimeSelectionHeight = 2;

qreal Constants::shiftBetweenGoAndFieldControl = 0.05;  // расстояние между go и контролем поля
qreal Constants::fieldControlWidth = 0.07;  // ширина от размера окна
qreal Constants::fieldControlHeight = 0.7;  // высота от размера нижнего пространства

qreal Constants::goX = 0.875;  // Искусственная контрольная панель
qreal Constants::goY = 0.8;
qreal Constants::goSize = 0.165;

qreal Constants::timerX = 0.165;  // Таймер
qreal Constants::timerY = 0.85;
qreal Constants::timerWidth = 0.135;
qreal Constants::timerHeight = 0.15;

qreal Constants::startChoiceProgressBarY = 0.025;  // прогресс бар стартового выбора
qreal Constants::startChoiceProgressBarWidth = 0.4;
qreal Constants::startChoiceProgressBarHeight = 0.04;


// Гексы
qreal Constants::hexShift = 0.26;  // Константа, зависящая от картинки гекса
qreal Constants::hexWtoH = 1.3;  // Отношение длины гекса к высоте

// Картинка на гексе:
qreal Constants::hexPictureOffsetW = 1 / 6.0;
qreal Constants::hexPictureOffsetH = 1 / 12.0;

// Таблица ресурсов на гексах:
qreal Constants::resourceTableMargin = 0.015;  // отступы между картинками
qreal Constants::resourceTablePicSize = 0.17;  // размер каждой картинки
qreal Constants::resourceTableBorders = 0.05;  // от границы гекса

// Картинка домика на гексе:
qreal Constants::unitHomePointX = 0.25;  // верхний левый угол
qreal Constants::unitHomePointY = 0.25;
qreal Constants::unitHomeWidth = 0.5;  // размер
qreal Constants::unitHomeHeight = 0.5;

// Картинка информации на гексе:
qreal Constants::informationPointX = 0.38;  // верхний левый угол
qreal Constants::informationPointY = 0.06;
qreal Constants::informationWidth = 0.24;  // размер
qreal Constants::informationHeight = 0.24;

// Картинки щитов на гексе:
qreal Constants::ShieldSmallStateWidthShift = 0.5;
qreal Constants::ShieldSmallStateHeightShift = 0.5;

qreal Constants::hexShieldsLineY = 0.58;
qreal Constants::hexShieldsMergeX = 0.05;
qreal Constants::hexShieldsWidth = 0.18;  // размер
qreal Constants::hexShieldsHeight = 0.20;

// Речки
qreal Constants::riverHeight = 0.15;

// Путь
qreal Constants::unitWayWidth = 0.18;  // ширина пути

// Укрепления
qreal Constants::fortificationWidth = 0.8; // от размера юнитов
qreal Constants::fortificationHeight = 0.8;
qreal Constants::fortificationStackShiftX = 0.15;  // В стэке укреплений разницы
qreal Constants::fortificationStackShiftY = 0.12;

// Юниты
qreal Constants::unitsSizeCoeff = 0.4;  // размер юнита от размера гекса

qreal Constants::healthPictureOffset = 1 / 5.0;  // отступ по высоте от вертикального здоровья
qreal Constants::degreeBetweenUnitHealth = 20;  // угол между двумя значками здоровья
qreal Constants::healthIconWidth = 3 / 8.0;//1 / 4.0;  // размеры иконки здоровья
qreal Constants::healthIconHeight = 1 / 2.0;//1 / 3.0;

qreal Constants::unitExplosionPointX = -0.5;  // верхний левый угол
qreal Constants::unitExplosionPointY = -0.5;
qreal Constants::unitExplosionWidth = 2;  // размер
qreal Constants::unitExplosionHeight = 2;

qreal Constants::unitSelectionPointX = -0.7;  // верхний левый угол
qreal Constants::unitSelectionPointY = -0.6;
qreal Constants::unitSelectionWidth = 2.4;  // размер
qreal Constants::unitSelectionHeight = 2.2;

qreal Constants::unitBigSelectionPointX = -1.5;  // верхний левый угол
qreal Constants::unitBigSelectionPointY = -1.3;
qreal Constants::unitBigSelectionWidth = 4;  // размер
qreal Constants::unitBigSelectionHeight = 3.6;

qreal Constants::unitLightingPointX = -0.65;  // верхний левый угол
qreal Constants::unitLightingPointY = -0.65;
qreal Constants::unitLightingWidth = 2.3;  // размер
qreal Constants::unitLightingHeight = 2.3;

// Приказы юниту

qreal Constants::unitOrderPointX = 0.75;  // верхний левый угол
qreal Constants::unitOrderPointY = 0.75;
qreal Constants::unitOrderWidth = 0.6;  // размер
qreal Constants::unitOrderHeight = 0.6;

qreal Constants::mainOrderX = 0.75;  // верхний левый угол
qreal Constants::mainOrderY = 0.75;
qreal Constants::mainOrderWidth = 0.8;  // размер
qreal Constants::mainOrderHeight = 0.8;

qreal Constants::mainOrderTurnCoeff = 1.5;  // увеличение размера при открытии

qreal Constants::orderStackShiftX = 0.4;  // В стэке приказов разницы
qreal Constants::orderStackShiftY = 0.4;

// Панель юнитов
qreal Constants::unitPanelPointX = 1;  // верхний левый угол панели
qreal Constants::unitPanelPointY = 1;
qreal Constants::unitPanelShiftX = 0;  // смещение панели при появлении
qreal Constants::unitPanelShiftY = -0.33;

qreal Constants::orderVariantK = 0.75;  // размер варианта приказа отн. размера юнита

qreal Constants::orderVariantTableSideMarginK = 0.15;  // отступы в таблице от размера юнита
qreal Constants::orderVariantTableMarginK = 0.1;

int Constants::orderVariantTableMaxColumns = 4;  // число колонн в таблице вариантов

// Приказы
qreal Constants::orderDisappearPointX = 0.5;  // точка, куда исчезают приказы
qreal Constants::orderDisappearPointY = 0.75;
qreal Constants::orderDisappearBurnPointX = -0.5;  // левый верхний угол горения
qreal Constants::orderDisappearBurnPointY = -0.75;
qreal Constants::orderBurnWidth = 2;  // размер горения приказа
qreal Constants::orderBurnHeight = 2;

// Ракеты
qreal Constants::rocketWidth = 0.5;  // от размеров юнитов
qreal Constants::rocketHeight = 1.2;
qreal Constants::blastWidth = 0.6;
qreal Constants::blastHeight = 1.2;

qreal Constants::smallExpWidth = 1.2;  // от размера юнитов
qreal Constants::smallExpHeight = 1.2;

// Лечилка
qreal Constants::cureIconWidth = 0.5;
qreal Constants::cureIconHeight = 0.5;
qreal Constants::cureIconTargetPointY = -2;
qreal Constants::cureIconTargetPointX = 2;
qreal Constants::cureIconTargetPointWidth = 2;
qreal Constants::cureIconTargetPointHeight = 2;

// Броня
qreal Constants::unitShieldsLineY = 0.65;
qreal Constants::unitShieldsMergeX = 0.15;
qreal Constants::unitShieldsWidth = 0.36;  // размер
qreal Constants::unitShieldsHeight = 0.40;

