#include "Pictures.h"

void Images::InitPictures()
{
    QFontDatabase::addApplicationFont("://Content/Fonts/Rubius.ttf");

    names << PictureNameFolder("Panels",
                {"NeutralMainWindow",
                 "BlueMainWindow",
                 "RedMainWindow",
                 "GreenMainWindow",
                 "YellowMainWindow",
                 "NeutralMainWindowFrame",
                 "BlueMainWindowFrame",
                 "GreenMainWindowFrame",
                 "RedMainWindowFrame",
                 "YellowMainWindowFrame",
                 "BlueCloseButton",
                 "BlueMaximizeButton",
                 "BlueMinimizeButton",
                 "RedCloseButton",
                 "RedMaximizeButton",
                 "RedMinimizeButton",
                 "GreenCloseButton",
                 "GreenMaximizeButton",
                 "GreenMinimizeButton",
                 "YellowMaximizeButton",
                 "YellowMinimizeButton",
                 "YellowCloseButton",
                 "NeutralCloseButton",
                 "NeutralMaximizeButton",
                 "NeutralMinimizeButton",
                 "NeutralButtonLayer",
                 "BlueButtonLayer",
                 "RedButtonLayer",
                 "GreenButtonLayer",
                 "YellowButtonLayer",
                 "LeftBluePlayerPanel",
                 "LeftRedPlayerPanel",
                 "LeftYellowPlayerPanel",
                 "LeftGreenPlayerPanel",
                 "RightBluePlayerPanel",
                 "RightRedPlayerPanel",
                 "RightYellowPlayerPanel",
                 "RightGreenPlayerPanel",
                 "NeutralDialogWindow",
                 "BlueDialogWindow",
                 "RedDialogWindow",
                 "GreenDialogWindow",
                 "YellowDialogWindow",
                 "UnitPanel",
                 "Border",
                 "GameLabelPanel1",
                 "GameLabelPanel2",
                 "ChosenGameLabelPanel",
                 "ChosenGameLabelLayer",
                 "LessonFrame",
                 "LessonFrameSelection",
                 "CreateGameFrameSelection",
                 "NewGameFrameSelection"},
                 ".png");

    names << PictureNameFolder("InterfaceElements",
               {"up",
                "down",
                 "right",
                 "left",
                 "SimpleLayer",
                 "PlayerOnline",
                 "PlayerOffline",
                 "PlayersLabel",
                 "TimerLabel",
                 "timer_1",
                 "timer_2",
                 "timer_3",
                 "timer_sp",
                 "timer_no",
                 "timer_1_off",
                 "timer_2_off",
                 "timer_3_off",
                 "timer_sp_off",
                 "timer_no_off",
                 "startChoiceBarBlue",
                 "startChoiceBarRed",
                 "startChoiceBarGreen",
                 "startChoiceBarYellow",
                 "startChoiceProgressBase",
                 "startChoiceProgressFrame",
                 "eng",
                 "rus"},
                 ".png");

    names << PictureNameFolder("Numbers",
               {"MillDigit0",
                "MillDigit1",
                "MillDigit2",
                "MillDigit3",
                "MillDigit4",
                "MillDigit5",
                "MillDigit6",
                "MillDigit7",
                "MillDigit8",
                "MillDigit9",
                "FortDigit9",
                "FortDigit8",
                "FortDigit7",
                "FortDigit6",
                "FortDigit5",
                "FortDigit4",
                "FortDigit3",
                "FortDigit2",
                "FortDigit1",
                "FortDigit0",
                "x",
                "colon"},
                 ".png");

    names << PictureNameFolder("Icons",
                 {"SandClock",
                  "Round1",
                  "Round1 selected",
                  "Round2",
                  "Round2 selected",
                  "Round3",
                  "Round3 selected",
                  "Round4",
                  "Round4 selected",
                  "Round5",
                  "Round5 selected"},
                 ".png");

    names << PictureNameFolder("Buttons",
                {"BlueGoButton",
                 "RedGoButton",
                 "YellowGoButton",
                 "GreenGoButton",
                 "PressedGoButton",
                 "BlueOkButton",
                 "RedOkButton",
                 "GreenOkButton",
                 "YellowOkButton",
                 "NeutralOkButton",
                 "BlueCancelButton",
                 "RedCancelButton",
                 "GreenCancelButton",
                 "YellowCancelButton",
                 "NeutralCancelButton",
                 "DisabledGoButton",
                 "BlueBackButton",
                 "RedBackButton",
                 "GreenBackButton",
                 "YellowBackButton",
                 "NeutralBackButton",
                 "FireButtonFrame",
                 "BlueNext",
                 "RedNext",
                 "YellowNext",
                 "GreenNext",
                 "WhiteFlag",
                 "Question",
                 "CancelLabelButton",
                 "JoinButton",
                 "LeaveButton",
                 "HomeButton"},
                 ".png");

    names << PictureNameFolder("LanguageInterface",
                {"CreateNewGame_rus",
                 "CreateButton_rus",
                 "HowToPlayButton_rus",
                 "Lesson1_rus",
                 "Lesson2_rus",
                 "Lesson3_rus",
                 "Lesson4_rus",
                 "CreateNewGame_eng",
                 "CreateButton_eng",
                 "HowToPlayButton_eng",
                 "Lesson1_eng",
                 "Lesson2_eng",
                 "Lesson3_eng",
                 "Lesson4_eng",},
                 ".png");

    names << PictureNameFolder("Field",
                {"DarkYellowHex",
                "GreenHexFrame",
                "BlueHex",
                "NeutralHex",
                "LightGreenHex",
                "RedHex",
                "StrangePurpleHex",
                "GreenHex",
                "YellowHex",
                "PurpleHexFrame",
                "RedHexFrame",
                "YellowHexFrame",
                 "YellowLightedGreyHexFrame",
                 "BrightYellowHex",
                 "GoldenHex",
                 "GoldenHexFrame",
                 "BluePhantomHex",
                 "RedPhantomHex",
                 "River"},
                ".png");

    names << PictureNameFolder("Decorations",
                {"Logo",
                 "Explosion1",
                 "Explosion2",
                 "Explosion3",
                 "SmallExplosion",
                 "MillSelection",
                 "Burn",
                 "DayTimeSelection",
                 "Blast",
                 "DecorativeShield",
                 "OrdersFrame",
                 "OrdersFrameRed"},
                 ".png");
    picture_rows["DayTimeSelection"] = 6;
    picture_columns["DayTimeSelection"] = 5;
    picture_rows["Explosion1"] = 1;
    picture_columns["Explosion1"] = 40;
    picture_rows["Explosion2"] = 1;
    picture_columns["Explosion2"] = 64;
    picture_rows["Explosion3"] = 1;
    picture_columns["Explosion3"] = 48;
    picture_rows["SmallExplosion"] = 1;
    picture_columns["SmallExplosion"] = 12;
    picture_rows["MillSelection"] = 4;
    picture_columns["MillSelection"] = 5;
    picture_rows["Burn"] = 8;
    picture_columns["Burn"] = 8;
    picture_rows["OrdersFrame"] = 2;
    picture_columns["OrdersFrame"] = 8;
    picture_rows["OrdersFrameRed"] = 2;
    picture_columns["OrdersFrameRed"] = 8;

    names << PictureNameFolder("ColoredDecorations",
                {"BlueWay",
                 "RedWay",
                 "GreenWay",
                 "YellowWay",
                 "BlueLight",
                 "RedLight",
                 "YellowLight",
                 "GreenLight"},
                 ".png");
    picture_rows["BlueWay"] = 1;
    picture_columns["BlueWay"] = 8;
    picture_rows["RedWay"] = 1;
    picture_columns["RedWay"] = 8;
    picture_rows["GreenWay"] = 1;
    picture_columns["GreenWay"] = 8;
    picture_rows["YellowWay"] = 1;
    picture_columns["YellowWay"] = 8;
    picture_rows["BlueLight"] = 4;
    picture_columns["BlueLight"] = 5;
    picture_rows["RedLight"] = 4;
    picture_columns["RedLight"] = 5;
    picture_rows["YellowLight"] = 4;
    picture_columns["YellowLight"] = 5;
    picture_rows["GreenLight"] = 4;
    picture_columns["GreenLight"] = 5;

//    names << PictureNameFolder("Decorations",
//                {"OceanBlur",
//                 "TextTexture"},
//                 ".jpg");

    names << PictureNameFolder("MainGameElements",
                {"Fort",
                 "Mill",
                 "Mountain",
                 "Lake",
                 "BlueUnitHome",
                 "RedUnitHome",
                 "GreenUnitHome",
                 "YellowUnitHome",
                 "NotBlueUnitHome",
                 "NotRedUnitHome",
                 "NotYellowUnitHome",
                 "NotGreenUnitHome",
                 "BlueRocket",
                 "RedRocket",
                 "GreenRocket",
                 "YellowRocket",
                 "NeutralShield",
                 "BlueShield",
                 "RedShield",
                 "GreenShield",
                 "YellowShield",
                 "BlueFortification",
                 "RedFortification",
                 "GreenFortification",
                 "YellowFortification",
                 "CureIcon",
                 "Tombstone",
                 "NotConnected"},
                 ".png");

    names << PictureNameFolder("Orders",
                {"Agite",
                 "Attack",
                 "Build",
                 "Capture",
                 "Cure",
                 "Fire",
                 "Fortify",
                 "Go",
                 "Liberate",
                 "Pursue",
                 "Siege",
                 "Recruit",
                 "Retreat",
                 "Unknown",
                 DefaultOrder,
                 "AgiteTurn",
                 "AttackTurn",
                 "BuildTurn",
                 "CureTurn",
                 "CaptureTurn",
                 "FireTurn",
                 "FortifyTurn",
                 "GoTurn",
                 "LiberateTurn",
                 "PursueTurn",
                 "SiegeTurn",
                 "RecruitTurn",
                 "RetreatTurn",
                 DefaultOrder + QString("Turn")},
                 ".png");
    picture_rows["AgiteTurn"] = 3;
    picture_columns["AgiteTurn"] = 7;
    picture_rows["AttackTurn"] = 3;
    picture_columns["AttackTurn"] = 7;
    picture_rows["BuildTurn"] = 3;
    picture_columns["BuildTurn"] = 7;
    picture_rows["CureTurn"] = 3;
    picture_columns["CureTurn"] = 7;
    picture_rows["CaptureTurn"] = 3;
    picture_columns["CaptureTurn"] = 7;
    picture_rows["FireTurn"] = 3;
    picture_columns["FireTurn"] = 7;
    picture_rows["FortifyTurn"] = 3;
    picture_columns["FortifyTurn"] = 7;
    picture_rows["GoTurn"] = 3;
    picture_columns["GoTurn"] = 7;
    picture_rows["LiberateTurn"] = 3;
    picture_columns["LiberateTurn"] = 7;
    picture_rows["PursueTurn"] = 3;
    picture_columns["PursueTurn"] = 7;
    picture_rows["SiegeTurn"] = 3;
    picture_columns["SiegeTurn"] = 7;
    picture_rows["RecruitTurn"] = 3;
    picture_columns["RecruitTurn"] = 7;
    picture_rows["RetreatTurn"] = 3;
    picture_columns["RetreatTurn"] = 7;
    picture_rows[DefaultOrder + QString("Turn")] = 3;
    picture_columns[DefaultOrder + QString("Turn")] = 7;

    names << PictureNameFolder("Units",
                {"BlueKarkun",
                 "BlueScrat",
                 "BluePig",
                 "BlueHippo",
                 "BlueMumusha",
                 "BlueRabbit",
                 "RedKarkun",
                 "RedScrat",
                 "RedPig",
                 "RedHippo",
                 "RedRabbit",
                 "RedMumusha",
                 "GreenKarkun",
                 "GreenScrat",
                 "GreenPig",
                 "GreenHippo",
                 "GreenRabbit",
                 "GreenMumusha",
                 "YellowKarkun",
                 "YellowScrat",
                 "YellowPig",
                 "YellowHippo",
                 "YellowRabbit",
                 "YellowMumusha",
                 "NeutralKarkun",
                 "NeutralScrat",
                 "NeutralPig",
                 "NeutralHippo",
                 "NeutralRabbit",
                 "NeutralMumusha",
                 "Scrat",
                 "Pig",
                 "Hippo",
                 "Karkun",
                 "Rabbit",
                 "Mumusha"},
                 ".png");

    names << PictureNameFolder("UnitElements",
                {"BlueUnitFrame",
                 "BlueUnitHealth",
                 "BlueUnitSelectionLayer",
                 "RedUnitFrame",
                 "RedUnitHealth",
                 "RedUnitSelectionLayer",
                 "YellowUnitFrame",
                 "YellowUnitHealth",
                 "YellowUnitSelectionLayer",
                 "GreenUnitFrame",
                 "GreenUnitHealth",
                 "GreenUnitSelectionLayer",
                 "NeutralUnitFrame",
                 "NeutralUnitHealth",},
                 ".png");
    picture_rows["BlueUnitSelectionLayer"] = 4;
    picture_columns["BlueUnitSelectionLayer"] = 5;
    picture_rows["RedUnitSelectionLayer"] = 4;
    picture_columns["RedUnitSelectionLayer"] = 5;
    picture_rows["GreenUnitSelectionLayer"] = 4;
    picture_columns["GreenUnitSelectionLayer"] = 5;
    picture_rows["YellowUnitSelectionLayer"] = 4;
    picture_columns["YellowUnitSelectionLayer"] = 5;
    picture_rows["RedUnitHealth"] = 1;
    picture_columns["RedUnitHealth"] = 16;
    picture_rows["BlueUnitHealth"] = 1;
    picture_columns["BlueUnitHealth"] = 16;
    picture_rows["GreenUnitHealth"] = 1;
    picture_columns["GreenUnitHealth"] = 16;
    picture_rows["YellowUnitHealth"] = 1;
    picture_columns["YellowUnitHealth"] = 16;
    picture_rows["NeutralUnitHealth"] = 1;
    picture_columns["NeutralUnitHealth"] = 16;
}

void Images::InitDesignPictures()
{
    designPictures["SimpleHex"] = "NeutralHex";
    designPictures["MillHex"] = "DarkYellowHex";
    designPictures["FortHex"] = "StrangePurpleHex";
    designPictures["MountainHex"] = "NeutralHex";
    designPictures["LakeHex"] = "NeutralHex";

    designPictures["SimpleHexFrame"] = "GreenHexFrame";
    designPictures["MillHexFrame"] = "GreenHexFrame";
    designPictures["FortHexFrame"] = "GreenHexFrame";
    designPictures["MountainHexFrame"] = "GreenHexFrame";
    designPictures["LakeHexFrame"] = "NeutralHex";
    designPictures["ForbiddenSelectionHexFrame"] = "RedHexFrame";

    designPictures["SimpleHexClickedFrame"] = "RedHexFrame";
    designPictures["MillHexClickedFrame"] = "RedHexFrame";
    designPictures["FortHexClickedFrame"] = "RedHexFrame";
    designPictures["MountainHexClickedFrame"] = "RedHexFrame";
    designPictures["LakeHexClickedFrame"] = "RedHexFrame";

    designPictures["SelectedHex"] = "BrightYellowHex";
    designPictures["LightedHex"] = "GoldenHex";

    designPictures["BluePanelButton"] = "BlueHex";
    designPictures["RedPanelButton"] = "RedHex";
    designPictures["YellowPanelButton"] = "YellowHex";
    designPictures["GreenPanelButton"] = "GreenHex";
}
