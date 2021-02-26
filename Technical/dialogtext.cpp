#include "DialogText.h"
#include "Technical/ProgrammSettings.h"

DialogText::DialogText()
{
    text["rus"]["victory"] = "ПОБЕДА!!!";
    text["eng"]["victory"] = "VICTORY!!!";

    text["rus"]["defeat"] = "ПОРАЖЕНИЕ!!!";
    text["eng"]["defeat"] = "DEFEAT!!!";

    text["rus"]["timeisup"] = "Время вышло!";
    text["eng"]["timeisup"] = "Time is up!";

    text["rus"]["areyousure"] = "УВЕРЕНЫ?";
    text["eng"]["areyousure"] = "ARE YOU SURE?";

    text["rus"]["connectionerror"] = "ОШИБКА!<br>Соединение с сервером разорвано! Пытаемся переподключиться...";
    text["eng"]["connectionerror"] = "ERROR!<br>Connection is broken! Trying to reconnect...";

    text["rus"]["Ireconnected"] = "Соединение с сервером было восстановлено после сбоя.";
    text["eng"]["Ireconnected"] = "Succesfully reconnected to server after connection failure.";

    text["rus"]["askgiveup"] = "Сдаётесь?";
    text["eng"]["askgiveup"] = "Surrender?";

    text["rus"]["connecting..."] = "ИДЁТ ПОДКЛЮЧЕНИЕ К СЕРВЕРУ...";
    text["eng"]["connecting..."] = "CONNECTING TO SERVER...";

    text["rus"]["cantconnect"] = "ОШИБКА!<br>НЕВОЗМОЖНО ПОДКЛЮЧИТЬСЯ К СЕРВЕРУ :(";
    text["eng"]["cantconnect"] = "ERROR!<br>FAILED TO CONNECT TO SERVER :(<br>Try later?";

    text["rus"]["menuconnectionbreak"] = "ОШИБКА!<br>СОЕДИНЕНИЕ С СЕРВЕРОМ РАЗОРВАНО!";
    text["eng"]["menuconnectionbreak"] = "ERROR!<br>Server has just disconnected!";

    text["rus"]["serverupdates"] = "Сервер обновляется. Попробуйте зайти позже.";
    text["eng"]["serverupdates"] = "Server is updating. Try later?";

    text["rus"]["versiontooold"] = "Эта версия игры устарела!<br>Найдите новую версию.";
    text["eng"]["versiontooold"] = "This version of the game is too old!<br>Find new version!";
}

QString DialogText::get(QString key)
{
    return text[settings->language][key];
}

QString DialogText::playerGaveUp(QString player)
{
    if (settings->language == "rus")
        return "Игрок " + player + " сдался!";
    return "Player " + player + " has surrendered!";
}
QString DialogText::lostConnection(QString player)
{
    if (settings->language == "rus")
        return "Потеряно соединение с игроком " + player + "! Пытаемся переподключиться.";
    return "Lost connection with player " + player + "! Trying to reconnect.";
}
QString DialogText::otherPlayerReconnected(QString player)
{
    if (settings->language == "rus")
        return "Игрок " + player + " успешно переподключился!";
    return "Player " + player + " has reconnected succesfully after connection failure!";

}
