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


SOURCES += main.cpp \
    Game/gamehex.cpp \
    Game/gameplayer.cpp \
    Game/gamerules.cpp \
    Game/gameunit.cpp \
    Technical/headers.cpp \
    GameElements/field.cpp \
    GameElements/hex.cpp \
    Game/gameInit.cpp \
    BasicElements/graphicobject.cpp \
    BasicElements/object.cpp \
    BasicElements/spriteobject.cpp \
    Technical/pictures.cpp \
    GameElements/unit.cpp \
    Game/gameorder.cpp \
    BasicElements/animation.cpp \
    Interface/GameLogic/gamewindowInit.cpp \
    Interface/GameLogic/interaction.cpp \
    Game/gamePhases.cpp \
    Game/gameEvents.cpp \
    Interface/GameLogic/graphicLogic.cpp \
    Interface/GameLogic/graphicHelpFunctions.cpp \
    Interface/GameLogic/gameEventsProcessed.cpp

HEADERS  += mainwindow.h \
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
    Interface/ArtificialGameControl.h \
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
    Technical/Random.h \
    Interface/GoButton.h \
    BasicElements/StateObject.h \
    Game/GameOrder.h \
    GameElements/UnitPanel.h \
    Interface/DayTimePanel.h \
    GameElements/DayTimeButton.h \
    GameElements/UnitWay.h \
    BasicElements/Animation.h \
    Interface/GameLogic/GameWindow.h

RESOURCES += \
    Images.qrc
