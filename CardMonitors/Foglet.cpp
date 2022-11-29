//
// Created by Gerwa on 2017/9/10.
//

#include "Foglet.h"

void Foglet::slotOnHandSeleted(CardInfo *seletedCard, bool allowCancel) {
    if (seletedCard->getCardName() == "Foglet") {
        GameController::controller()->defaultHandleHand(seletedCard, allowCancel);
    }
}

void Foglet::slotAfterWeatherChanged(int row, Weather oldWeather) {
    GameAssets *assets = GameController::controller()->getAssets();
    if (assets->getRowWeather(row) == Weather::Foggy) {// a new fog activated
        int fogletPlayer = assets->whosePlayerRow(row) ^1;
        CardInfo *foundFoglet = nullptr;
        int deckIndex = assets->getDeckIndex(fogletPlayer);
        for (auto card:assets->getCardArray(deckIndex)) {
            if (card->getCardName() == "Foglet") {
                foundFoglet = card;
                break;
            }
        }
        if (foundFoglet == nullptr) {
            int graveyardIndex = (fogletPlayer == 0 ? Player0_Graveyard : Player1_Graveyard);
            for (auto deadCard:assets->getCardArray(graveyardIndex)) {
                if (deadCard->getCardName() == "Foglet") {
                    foundFoglet = deadCard;
                    break;
                }
            }
        }
        if (foundFoglet != nullptr) {//really found a foglet
            int Begin = assets->playerBattlefieldBegin(fogletPlayer);
            int End = assets->playerBattlefieldEnd(fogletPlayer);
            int randomIndex = Begin + assets->randint() % (End - Begin + 1);
            GameController::controller()->moveCardToRightTop(foundFoglet, randomIndex);
        }
    } else if (oldWeather == Weather::Foggy) {
        //check if there is still a fog on the WHOLE battlefield
        bool hasFog = false;
        for (int battlefieldRow = Player1_Siege; battlefieldRow <= Player0_Siege; battlefieldRow++) {
            if (assets->getRowWeather(battlefieldRow) == Weather::Foggy) {
                hasFog = true;
                break;
            }
        }
        if (!hasFog) {
            //destroy all the foglet on the battlefield
            QList<CardInfo *> foglets;
            for (int battlefieldRow = Player1_Siege; battlefieldRow <= Player0_Siege; battlefieldRow++) {
                for (auto unit:assets->getCardArray(battlefieldRow)) {
                    if (unit->getCardName() == "Foglet") {
                        foglets.append(unit);
                    }
                }
            }
            for (auto foglet:foglets) {
                GameController::controller()->DestroyCard(foglet);
            }
        }
    }
}
