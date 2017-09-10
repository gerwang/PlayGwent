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
    SAVEDECK
};

enum TextLabelEnum {
    //0~11 for each battlefield//TODO: why is this link indented?
            Player_CardChooser_Title = 12,
    Player1_Name = 13,
    Player0_Name = 14,
    Label_ALL_DeckBuilder = 15,
    Label_Gold_DeckBuilder = 16,
    Label_Silver_DeckBuilder = 17,
    Label_Bronze_DeckBuilder = 18
};

const int RoundEndDuration = 3000, GameEndDuration = 6000;//the duration of messagebox presented in msec

const int BUTTON_NUM = 3, LABEL_NUM = 19;

enum Weather {
    Sunny = 0, Rainy = 1, Foggy = 2, Frozen = 3
};

const QString WeatherMSG[4] = {
        "Sunny", "Rainy", "Foggy", "Frozen"
};
//TODO there should be a lineedit to edit the deck name

enum class Command {
    LeftClick, RightClick, Pass, Escape, SaveDeck
};

#endif //PLAYGWENT_GAMECONSTANT_H_H
