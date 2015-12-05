#include "Pictures.h"

void Pictures::InitPictures()
{
    names << PictureNameFolder(
                 ":/Interface/Content/ProgrammInterface/System/",
                {"MainWindow",
                 "MainWindowFrame",
                 "CloseButton",
                 "MaximizeButton",
                 "MinimizeButton",
                 "ButtonLayer",
                 "BluePlayerPanel",
                 "RedPlayerPanel",
                 "WhiteDialogWindow"},
                 ".png");

    names << PictureNameFolder(
                 ":/InterfaceElements/Content/ProgrammInterface/InterfaceElements/",
                {"up",
                 "down",
                 "right",
                 "left",
                 "SimpleLayer"},
                 ".png");

    names << PictureNameFolder(
                  ":/Icons/Content/ProgrammInterface/Icons/",
                 {"SandClock"},
                 ".png");

    names << PictureNameFolder(
                 ":/Text/Content/ProgrammInterface/AdditionalLabels/",
                {"GatherLabel",
                 "StartLabel"},
                 ".png");

    names << PictureNameFolder(
                 ":/Buttons/Content/ProgrammInterface/Buttons/",
                {"BlueButton",
                 "BlueButtonClicked",
                 "RedButton",
                 "RedButtonClicked",
                 "NeutralButton",
                 "NeutralButtonClicked",
                 "GoButton",
                 "PressedGoButton",
                 "DisabledGoButton"},
                 ".png");

    names << PictureNameFolder(
                 ":Panels/Content/ProgrammInterface/Panels/",
                {"UnitPanel"},
                 ".png");

    names << PictureNameFolder(
                 ":/Field/Content/ProgrammInterface/Field/",
                {"DarkYellowHex",
                "GreenHexFrame",
                "BlueHex",
                "NeutralHex",
                "LightGreenHex",
                "RedHex",
                "PurpleHexFrame",
                "RedHexFrame",
                "YellowHexFrame",
                 "YellowLightedGreyHexFrame",
                 "BrightYellowHex",
                 "GoldenHex",
                 "GoldenHexFrame",
                 "BluePhantomHex",
                 "RedPhantomHex"},
                ".png");

    names << PictureNameFolder(
                 ":/Decorations/Content/Decorations/",
                {"Explosion1",
                 "Explosion2",
                 "Explosion3",
                 "SmallExplosion",
                 "MillSelection",
                 "Burn",
                 "DayTimeSelection",
                 "BlueWay",
                 "RedWay"},
                 ".png");
    picture_rows["BlueWay"] = 1;
    picture_columns["BlueWay"] = 8;
    picture_rows["RedWay"] = 1;
    picture_columns["RedWay"] = 8;
    picture_rows["DayTimeSelection"] = 6;
    picture_columns["DayTimeSelection"] = 5;
    picture_rows["Explosion1"] = 1;
    picture_columns["Explosion1"] = 40;
    picture_rows["Explosion2"] = 1;
    picture_columns["Explosion2"] = 64;
    picture_rows["Explosion3"] = 1;
    picture_columns["Explosion3"] = 48;    
    picture_rows["MillSelection"] = 4;
    picture_columns["MillSelection"] = 5;    
    picture_rows["Burn"] = 8;
    picture_columns["Burn"] = 8;

    names << PictureNameFolder(
                 ":/Decorations/Content/Decorations/",
                {"BlueSimpleBlur",
                 "UnderwaterBlur",
                 "OceanBlur"},
                 ".jpg");

    names << PictureNameFolder(
                 ":/GameElements/Content/GameElements/",
                {"Fort",
                 "Mill",
                 "Mountain",
                 "BlueUnitHome",
                 "RedUnitHome"},
                 ".png");

    names << PictureNameFolder(
                 ":/Orders/Content/GameElements/Orders/",
                {"Attack",
                 "Capture",
                 "Cure",
                 "Fire",
                 "Fortify",
                 "Go",
                 "Siege",
                 "Gather",
                 "Unknown",
                 "Wait"},
                 ".png");

    names << PictureNameFolder(
                 ":/Units/Content/GameElements/Units/",
                {"BlueKarkun",
                 "BlueScrat",
                 "BluePig",
                 "BlueHippo",
                 "RedKarkun",
                 "RedScrat",
                 "RedPig",
                 "RedHippo",
                 "Scrat",
                 "Pig",
                 "Hippo",
                 "Karkun"},
                 ".png");

    names << PictureNameFolder(
                 ":/UnitElements/Content/GameElements/UnitsElements/",
                {"BlueUnitFrame",
                 "BlueUnitHealth",
                 "BlueUnitSelectionLayer",
                 "RedUnitFrame",
                 "RedUnitHealth",
                 "RedUnitSelectionLayer"},
                 ".png");
    picture_rows["BlueUnitSelectionLayer"] = 4;
    picture_columns["BlueUnitSelectionLayer"] = 5;
    picture_rows["RedUnitSelectionLayer"] = 4;
    picture_columns["RedUnitSelectionLayer"] = 5;
    picture_rows["RedUnitHealth"] = 1;
    picture_columns["RedUnitHealth"] = 16;
    picture_rows["BlueUnitHealth"] = 1;
    picture_columns["BlueUnitHealth"] = 16;

    names << PictureNameFolder(
                 ":/DayTime/Content/GameElements/DayTime/",
                {"Breakfast",
                 "Dinner",
                 "Supper"},
                 ".png");
}

void Pictures::InitDesignPictures()
{
    designPictures[CLASSIC]["DialogWindowFrame"] = "WhiteDialogWindow";
    designPictures[CLASSIC]["DialogTextGradient"] = "OceanBlur";

    designPictures[CLASSIC]["SimpleHex"] = "NeutralHex";
    designPictures[CLASSIC]["MillHex"] = "DarkYellowHex";
    designPictures[CLASSIC]["FortHex"] = "LightGreenHex";
    designPictures[CLASSIC]["MountainHex"] = "NeutralHex";

    designPictures[CLASSIC]["SimpleHexFrame"] = "GreenHexFrame";
    designPictures[CLASSIC]["MillHexFrame"] = "GreenHexFrame";
    designPictures[CLASSIC]["FortHexFrame"] = "GreenHexFrame";
    designPictures[CLASSIC]["MountainHexFrame"] = "GreenHexFrame";
    designPictures[CLASSIC]["ForbiddenSelectionHexFrame"] = "RedHexFrame";

    designPictures[CLASSIC]["SimpleHexClickedFrame"] = "RedHexFrame";
    designPictures[CLASSIC]["MillHexClickedFrame"] = "RedHexFrame";
    designPictures[CLASSIC]["FortHexClickedFrame"] = "RedHexFrame";
    designPictures[CLASSIC]["MountainHexClickedFrame"] = "RedHexFrame";

    designPictures[CLASSIC]["SelectedHex"] = "BrightYellowHex";
    designPictures[CLASSIC]["LightedHex"] = "GoldenHex";
}
