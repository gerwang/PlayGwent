//
// Created by Gerwa on 2017/9/6.
//

#ifndef PLAYGWENT_GAMECONSTANT_H_H
#define PLAYGWENT_GAMECONSTANT_H_H

const int ROW_NUM = 16;//the num of row on battlefield

const QString cardslotMsg[ROW_NUM] =
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
                "player0_graveyard"//11
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

    Player1_Candidate = 12,
    Player1_Seleted = 13,

    Player0_Candidate = 14,
    Player0_Seleted = 15,

    DeckBuilder_Candidate = 16,
    DeckBuilder_NoHP = 17,
    DeckBuilder_Siege = 18,
    DeckBuilder_Ranged = 19,
    DeckBuilder_Melee_Event = 20,

    Main_Menu = 21
};

enum PushButtonEnum {
    ESCAPE0,
    ESCAPE1,
    BUTTON_PASS,
    SAVEDECK
};

enum TextLabelEnum {
    //0~11 for each battlefield
            Player1_CardChooser_Title = 12,//TODO: why is this link indented?
    Player0_CardChooser_Title = 13,
    Player1_Name = 14,
    Player0_Name = 15,
    Label_ALL_DeckBuilder = 16,
    Label_Gold_DeckBuilder = 17,
    Label_Silver_DeckBuilder = 18,
    Label_Bronze_DeckBuilder = 19
};

//TODO there should be a linkedit to edit the deck name

#endif //PLAYGWENT_GAMECONSTANT_H_H
