#-------------------------------------------------
#
# Project created by QtCreator 2015-04-26T14:57:16
#
#-------------------------------------------------

QT       += core gui
QT       += core network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl

QMAKE_CXXFLAGS += -std=c++11 -g -Wall

TARGET = FortsAndMills-II
TEMPLATE = app

RC_FILE = icon.rc


SOURCES += main.cpp \
    GameExecution/gameGeometry.cpp \
    GameExecution/gameLog.cpp \
    GameExecution/gameUtils.cpp \
    Learning/LessonGame.cpp \
    Technical/headers.cpp \
    GameElements/field.cpp \
    GameElements/hex.cpp \
    BasicElements/graphicobject.cpp \
    BasicElements/object.cpp \
    BasicElements/spriteobject.cpp \
    Technical/pictures.cpp \
    GameElements/unit.cpp \
    BasicElements/animation.cpp \
    GameElements/hexPoints.cpp \
    Game/gameUnits.cpp \
    Game/gameOrder.cpp \
    Game/gameRules.cpp \
    GameExecution/gameEvents.cpp \
    Game/gameHex.cpp \
    GameExecution/game.cpp \
    GameExecution/gameTurns.cpp \
    GameExecution/gameSteps.cpp \
    GameExecution/gameFight.cpp \
    GameExecution/gameOrderExecution.cpp \
    GameExecution/gameField.cpp \
    GameWindow/eventsRealization.cpp \
    GameWindow/interaction.cpp \
    GameWindow/interfaceOrganization.cpp \
    GameWindow/statesControl.cpp \
    GameWindow/events.cpp \
    GameWindow/gameWindow.cpp \
    GameWindow/gameElementsOrganization.cpp \
    Technical/elementary.cpp \
    Technical/constants.cpp \
    Technical/Help.cpp

HEADERS  += mainwindow.h \
    GameExecution/GameLog.h \
    GameExecution/GameStructures.h \
    Interface/StartChoiceProgress.h \
    Programm/Basic/headers.h \
    Basic/ProgrammSettings.h \
    Product/MainWindow.h \
    Basic/Headers.h \
    Basic/Button.h \
    Basic/Application.h \
    Technical/Headers.h \
    Technical/ProgrammSettings.h \
    Interface/MainWindow.h \
    Technical/Pictures.h \
    Technical/Constants.h \
    Interface/SystemControlPanel.h \
    GameElements/Hex.h \
    GameElements/Field.h \
    GameElements/FieldWindow.h \
    Interface/GameWindow.h \
    Game/GameRules.h \
    Game/GameHex.h \
    Interface/PlayerResources.h \
    GameElements/Unit.h \
    Game/GameUnit.h \
    Game/GamePlayer.h \
    Interface/BasicElements/VisualizedResource.h \
    Game/Game.h \
    Technical/Application.h \
    GameElements/Order.h \
    Interface/GraphicObject.h \
    Interface/Object.h \
    BasicElements/GraphicObject.h \
    BasicElements/Object.h \
    Interface/Background.h \
    BasicElements/SpriteObject.h \
    Technical/AnimationManager.h \
    BasicElements/TextObject.h \
    Interface/Dialog.h \
    Interface/MenuWindow.h \
    Technical/Client.h \
    Interface/GoButton.h \
    BasicElements/StateObject.h \
    Game/GameOrder.h \
    GameElements/UnitPanel.h \
    Interface/DayTimePanel.h \
    GameElements/DayTimeButton.h \
    GameElements/UnitWay.h \
    BasicElements/Animation.h \
    GameElements/Rocket.h \
    GameElements/Shield.h \
    BasicElements/MergingObject.h \
    GameElements/CureIcon.h \
    Technical/Elementary.h \
    Game/GameAction.h \
    GameExecution/Game.h \
    Game/GameMessages.h \
    GameExecution/GameMessages.h \
    GameWindow/GameWindow.h \
    Interface/LeftPanelsSwitch.h \
    Interface/CreatingGameWindow.h \
    Interface/Tunes.h \
    BasicElements/DigitObject.h \
    Interface/GameLabel.h \
    Interface/SpecialButton.h \
    Interface/HowToPlayWindow.h \
    Learning/LessonGameWindow.h \
    Learning/LessonGame.h \
    Learning/LessonMessages.h \
    Technical/HelpManager.h

RESOURCES += \
    Resources/Buttons.qrc \
    Resources/ColoredDecorations.qrc \
    Resources/Decorations.qrc \
    Resources/Field.qrc \
    Resources/Fonts.qrc \
    Resources/Icons.qrc \
    Resources/InterfaceElements.qrc \
    Resources/MainGameElements.qrc \
    Resources/Numbers.qrc \
    Resources/Orders.qrc \
    Resources/Panels.qrc \
    Resources/UnitElements.qrc \
    Resources/Units.qrc \
