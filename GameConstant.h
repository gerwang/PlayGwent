//
// Created by Gerwa on 2017/9/6.
//

#ifndef PLAYGWENT_GAMECONSTANT_H_H
#define PLAYGWENT_GAMECONSTANT_H_H

const int ROW_NUM = 20;//the num of row on battlefield

const QString CardSlotMSG[ROW_NUM] =
        {
                "player1_graveyard",//0
                "player1_deck",//1
                "player1_hand",//2
                "player1_siege",//3
                "player1_ranged",//4
                "player1_melee",//5
                "player0_melee",//6
                "player0_ranged",//7
                "player0_siege",//8
                "player0_hand",//9
                "player0_deck",//10
                "player0_graveyard", //11

                "Player_Candidate",//12,
                "Player_Seleted",// 13,

                "DeckBuilder_Candidate",// 14,
                "DeckBuilder_NoHP",// 15,
                "DeckBuilder_Siege",// 16,
                "DeckBuilder_Ranged",// 17,
                "DeckBuilder_Melee_Event",// 18

                "Main_Menu"// 19
        };

const QString SceneMSG[4] = {
        "MainMenuScene", "GameScene", "DeckBuilderScene", "PlayerChooserScene"
};

const QString InputStateMSG[3] = {
        "RejectAll", "MustValidTarget", "MustValidRow"
};

const int NumberToRedraw[3] = {3, 1, 1};
const int NumberToChoose[3] = {10, 2, 1};
const int MaxDeckCount[3] = {40, 6, 4};
const int MaxSameTypeCount[3] = {3, 1, 1};

enum CardArrayEnum {
    Player1_Graveyard = 0,
    Player1_Deck = 1,
    Player1_Hand = 2,

    Player1_Siege = 3,
    Player1_Ranged = 4,
    Player1_Melee = 5,
    Player0_Melee = 6,
    Player0_Ranged = 7,
    Player0_Siege = 8,

    Player0_Hand = 9,
    Player0_Deck = 10,
    Player0_Graveyard = 11,

    Player_Candidate = 12,
    Player_Seleted = 13,

    DeckBuilder_Candidate = 14,
    DeckBuilder_NoHP = 15,
    DeckBuilder_Siege = 16,
    DeckBuilder_Ranged = 17,
    DeckBuilder_Melee_Event = 18,

    Main_Menu = 19
};

enum PushButtonEnum {
    ESCAPE,
    BUTTON_PASS,
    SAVEDECK,
    CLIPBOARD_IMPORT,
    CLIPBOARD_EXPORT
};

enum TextLabelEnum {
    //0~11 for each battlefield//TODO: why is this link indented?
            Player_CardChooser_Title = 12,
    Player1_Name = 13,
    Player0_Name = 14,
    Label_Bronze_DeckBuilder = 15,
    Label_Silver_DeckBuilder = 16,
    Label_Gold_DeckBuilder = 17,
    Label_ALL_DeckBuilder = 18,
    Player1_CombatValue = 19,
    Player0_CombatValue = 20
};

const int RoundEndDuration = 3000, GameEndDuration = 6000;//the duration of messagebox presented in msec

const int BUTTON_NUM = 5, LABEL_NUM = 21;

enum Weather {
    Sunny = 0, Rainy = 1, Foggy = 2, Frozen = 3
};

const QString WeatherMSG[4] = {
        "Sunny", "Rainy", "Foggy", "Frozen"
};
//TODO there should be a lineedit to edit the deck name

enum Command {
    LeftClick, RightClick, Pass, Escape, SaveDeck, ClipboardImport, ClipboardExport
};

const QString PushButtonText[BUTTON_NUM] = {
        "escape",
        "pass",
        "save deck and exit",
        "import from clipboard",
        "export to clipboard"
};

const QList<QString> CardNameList = {//all cards can appear in DECK
        "Geralt_Igni",
        "Arachas",
        "Roach",
        "Vran_Warrior",
        "Arachas_Behemoth",
        "Thunderbolt_Potion",
        "Commanders_Horn",
        "Lacerate",
        "Celaeno_Harpy",
        "Earth_Elemental",
        "Crone_Brewess",
        "Crone_Weavess",
        "Crone_Whispess",
        "Frightener",
        "Geels",
        "Bekkers_Twisted_Mirror",
        "Biting_Frost",
        "Impenetrable_Fog",
        "Torrential_Rain",
        "First_Light",
        "Foglet",
        "Woodland_Spirit",
        "Archgriffin",
        "Wild_Hunt_Rider",
        "Caranthir"
};

const QList<QString> LeaderNameList = {
        "Unseen_Elder",
        "Dagon"
};

#endif //PLAYGWENT_GAMECONSTANT_H_H
