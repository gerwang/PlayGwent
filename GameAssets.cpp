//
// Created by Gerwa on 2017/9/6.
//

#include "GameAssets.h"
#include <QDebug>
#include <QtCore/QJsonArray>

bool GameAssets::getPlayerPass(int player) {
    return playerPass[player];
}

void GameAssets::setPlayerPass(int player, bool pass) {
    playerPass[player] = pass;
}

QList<CardInfo *> &GameAssets::getCardArray(int row) {
    return cardarray[row];
}

int GameAssets::getPlayerWinRound(int player) {
    return playerWinRound[player];
}

void GameAssets::setPlayerWinRound(int player, int winRound) {
    playerWinRound[player] = winRound;
}

Weather GameAssets::getRowWeather(int row) {
    return rowWeather[row];
}

void GameAssets::setRowWeather(int row, Weather weather) {
    rowWeather[row] = weather;
}

const QString &GameAssets::getPlayerName(int player) {
    return playerName[player];
}

void GameAssets::setPlayerName(int player, const QString &name) {
    playerName[player] = name;
}

bool GameAssets::isGameEnd() {
    return playerWinRound[0] >= 2 ||//player0  win
           playerWinRound[1] >= 2 ||//player1  win
           currentRound >= 3;
    //           playerWinRound[0] + playerWinRound[1] < 3;//not draw WRONG!
}

int GameAssets::getPlayerScore(int player, int round) {
    return playerScore[player][round];
}

void GameAssets::setPlayerScore(int player, int round, int score) {
    playerScore[player][round] = score;
}

unsigned int GameAssets::getRandomSeed() const {
    return randomSeed;
}

void GameAssets::setRandomSeed(unsigned int randomSeed) {
    GameAssets::randomSeed = randomSeed;
}

void GameAssets::resetRandomSeed() {
    qsrand(randomSeed);
}

int GameAssets::getPreviousWinner() const {
    return previousWinner;
}

void GameAssets::setPreviousWinner(int previousWinner) {
    GameAssets::previousWinner = previousWinner;
}

int GameAssets::getCurrentPlayer() const {
    return currentPlayer;
}

void GameAssets::setCurrentPlayer(int currentPlayer) {
    GameAssets::currentPlayer = currentPlayer;
}

int GameAssets::getCurrentRound() const {
    return currentRound;
}

void GameAssets::setCurrentRound(int currentRound) {
    GameAssets::currentRound = currentRound;
}

QList<CardInfo *> GameAssets::getTopNCards(const QList<CardInfo *> &deck, int n) {
    if (n > deck.size()) {
        qDebug() << "deck size not enough! 12345647";
        n = deck.size();
    }
    return deck.mid(deck.size() - n, n);
}

QList<CardInfo *> GameAssets::getPlayerDeckTopNCards(int player, int n) {
    return getTopNCards(cardarray[getDeckIndex(player)], n);
}

int GameAssets::getDeckIndex(int player) {
    if (player == 0) {
        return Player0_Deck;
    } else if (player == 1) {
        return Player1_Deck;
    } else {
        qWarning() << "Invalid Player number!123475867";
        return -1;
    }
}

void GameAssets::getCardPosition(CardInfo *card, int &row, int &column) {
    for (int i = 0; i < ROW_NUM; i++) {
        for (int j = 0; j < cardarray[i].size(); j++) {
            if (cardarray[i][j] == card) {//compare pointer,really
                row = i;
                column = j;
                return;
            }
        }
    }
    qWarning() << "card not in assets!";
}

void GameAssets::moveCardPosToPos(int fromR, int fromC, int toR, int toC) {
    CardInfo *card = cardarray[fromR].takeAt(fromC);
    cardarray[toR].insert(toC, card);
}

bool GameAssets::isBattleField(int row) {
    return Player1_Siege <= row && row <= Player0_Siege;
}

bool GameAssets::isGraveyard(int row) {
    return row == Player0_Graveyard || row == Player1_Graveyard;
}

int GameAssets::getHandIndex(int player) {
    if (player == 0) {
        return Player0_Hand;
    } else if (player == 1) {
        return Player1_Hand;
    } else {
        qWarning() << "invalid player number 2545274";
        return -1;
    }
}


int GameAssets::findTopmostANameNotInB(const QList<CardInfo *> &a, const QList<CardInfo *> &b) {
    //larger,topper
    for (int top = a.size() - 1; top >= 0; top--) {
        CardInfo *topCard = a[top];
        if (!listContainsName(b, topCard->getCardName())) {
            return top;
        }
    }
    return -1;
}

bool GameAssets::listContainsName(const QList<CardInfo *> &list, const QString &name) {
    for (auto info:list) {
        if (info->getCardName() == name) {
            return true;
        }
    }
    return false;
}

bool GameAssets::isPlayerHandEmpty(int player) {
    int handIndex = getHandIndex(player);
    return cardarray[handIndex].empty();
}

bool GameAssets::isAllPlayerPass() {
    return playerPass[0] && playerPass[1];
}

bool GameAssets::isHandled() const {
    return handled;
}

void GameAssets::setHandled(bool handled) {
    GameAssets::handled = handled;
}

int GameAssets::getRowCombatValueSum(int row) {
    int sum = 0;
    for (auto card:cardarray[row]) {
        sum += card->getCurrentStrength();
    }
    return sum;
}

int GameAssets::getPlayerCombatValueSum(int player) {
    int sum = 0;
    if (player == 0) {
        for (int row = Player0_Melee; row <= Player0_Siege; row++) {
            sum += getRowCombatValueSum(row);
        }
    } else if (player == 1) {
        for (int row = Player1_Siege; row <= Player1_Melee; row++) {
            sum += getRowCombatValueSum(row);
        }
    }
    return sum;
}

void GameAssets::addPlayerWinRound(int player) {
    playerWinRound[player]++;
}

void GameAssets::addCurrentRound() {
    currentRound++;
}

void GameAssets::clearAllGameRows() {
    for (int row = Player1_Graveyard; row <= Player0_Graveyard; row++) {
        cardarray[row].clear();
        rowWeather[row] = Weather::Sunny;
    }
}

void GameAssets::createCardsRandomlyOnNameListToRow(QList<QString> namelist, int row) {
    //randomize
    int n = namelist.size();
    for (int index = 0; index < n; index++) {
        int otherIndex = index + qrand() % (n - index);//range [index,n-1]
        std::swap(namelist[index], namelist[otherIndex]);//correct random algorithm
        cardarray[row].append(CardInfo::createByName(namelist[index]));//allocate NEW memory!
        //WARNING: FIND BUG: cardarray means &cardarray[0]!!
    }
}

int GameAssets::getEnemySameRowNumber(int rowNumber) {
    return Player0_Graveyard - rowNumber;
}

int GameAssets::getPlayerMelee(int player) {
    return player == 0 ? Player0_Melee : Player1_Melee;
}

int GameAssets::getPlayerRanged(int player) {
    return player == 0 ? Player0_Ranged : Player1_Ranged;
}

int GameAssets::getPlayerSiege(int player) {
    return player == 0 ? Player0_Siege : Player1_Siege;
}

void GameAssets::updateRowStrongest(int row, QList<CardInfo *> &result, CardInfo *exclude) {
    for (auto info:cardarray[row]) {
        if (info == exclude) {
            continue;
        } else if (result.empty()) {
            result.append(info);
        } else if (info->getCurrentStrength() > result.first()->getCurrentStrength()) {
            result.clear();
            result.append(info);
        } else if (info->getCurrentStrength() == result.first()->getCurrentStrength()) {
            result.append(info);
        }
    }
}

void GameAssets::removeCardFromGame(int row, int column) {
    CardInfo *temp = cardarray[row].takeAt(column);
    temp->deleteLater();//use deletelater to provide a safer way
}

bool GameAssets::cardAlreadyInGraveyard(CardInfo *card) {
    return cardarray[Player0_Graveyard].contains(card) ||
           cardarray[Player1_Graveyard].contains(card);
}

int GameAssets::whosePlayerRow(int row) {
    if (Player0_Melee <= row && row <= Player0_Graveyard) {
        return 0;
    } else if (Player1_Graveyard <= row && row <= Player1_Melee) {
        return 1;
    } else {
        return -1;
    }
}

int GameAssets::playerBattlefieldBegin(int player) {
    return player == 0 ? Player0_Melee : Player1_Siege;
}

int GameAssets::playerBattlefieldEnd(int player) {
    return player == 0 ? Player0_Siege : Player1_Melee;
}

bool GameAssets::isCardOnBattlefield(CardInfo *card) {
    for (int row = Player1_Siege; row <= Player0_Siege; row++) {
        if (cardarray[row].contains(card)) {
            return true;
        }
    }
    return false;
}

void GameAssets::updateRowWeakest(int row, QList<CardInfo *> &result, CardInfo *exclude) {
    for (auto info:cardarray[row]) {
        if (info == exclude) {
            continue;
        } else if (result.empty()) {
            result.append(info);
        } else if (info->getCurrentStrength() < result.first()->getCurrentStrength()) {
            result.clear();
            result.append(info);
        } else if (info->getCurrentStrength() == result.first()->getCurrentStrength()) {
            result.append(info);
        }
    }
}

void GameAssets::setDeckTypeCount(int type, int cnt) {
    deckTypeCount[type] = cnt;
}

int GameAssets::getDeckTypeCount(int type) {
    return deckTypeCount[type];
}

void GameAssets::updateDeckTypeCount() {
    memset(deckTypeCount, 0, sizeof(deckTypeCount));
    for (int row = DeckBuilder_NoHP; row <= DeckBuilder_Melee_Event; row++) {
        for (auto card:cardarray[row]) {
            deckTypeCount[card->getType()]++;//remeber the is not leader card in deck
        }
    }
    deckTotalCount = 0;
    for (int type = CardInfo::Bronze; type < CardInfo::Gold; type++) {
        deckTotalCount += deckTypeCount[type];
    }
}

int GameAssets::getDeckTotalCount() const {
    return deckTotalCount;
}

void GameAssets::setDeckTotalCount(int deckTotalCount) {
    GameAssets::deckTotalCount = deckTotalCount;
}

void GameAssets::fromJson(const QJsonObject &json) {
    roundStatus = json["roundStatus"].toInt();
    previousWinner = json["previousWinner"].toInt();
    currentPlayer = json["currentPlayer"].toInt();
    currentRound = json["currentRound"].toInt();
    handled = json["handled"].toBool();
    QJsonArray jsonCardArrays = json["cardarray"].toArray();
    for (int row = 0; row < jsonCardArrays.size(); row++) {
        cardarray[row].clear();
        QJsonArray childArray = jsonCardArrays.at(row).toArray();
        for (int column = 0; column < childArray.size(); column++) {
            auto info = new CardInfo;
            info->readFromJson(childArray.at(column).toObject());
            cardarray[row].append(info);
        }
    }
    QJsonArray jsonWeatherArray = json["rowWeather"].toArray();
    for (int row = 0; row < jsonWeatherArray.size(); row++) {
        rowWeather[row] = static_cast<Weather>(jsonWeatherArray.at(row).toInt());
    }
    QJsonArray jsonPlayerWinRound = json["playerWinRound"].toArray();
    QJsonArray jsonPlayerPass = json["playerPass"].toArray();
    QJsonArray jsonPlayerName = json["playerName"].toArray();
    QJsonArray jsonPlayerScore = json["playerScore"].toArray();
    for (int player = 0; player < 2; player++) {
        playerWinRound[player] = jsonPlayerWinRound.at(player).toInt();
        playerPass[player] = jsonPlayerPass.at(player).toBool();
        playerName[player] = jsonPlayerName.at(player).toString();
        QJsonArray jsonScoreArray = jsonPlayerScore.at(player).toArray();
        for (int rnd = 0; rnd < jsonScoreArray.size(); rnd++) {
            playerScore[player][rnd] = jsonScoreArray.at(rnd).toInt();
        }
    }
    randomSeed = static_cast<unsigned int>(json["randomSeed"].toInt());
}

void GameAssets::toJson(QJsonObject &json) {

    json.insert("roundStatus", roundStatus);
    json.insert("previosWinner", previousWinner);
    json.insert("currentPlayer", currentPlayer);
    json.insert("currentRound", currentRound);
    json.insert("handled", handled);

    QJsonArray jsonCardArrays;
    for (int row = 0; row < ROW_NUM; row++) {
        QJsonArray jsonArray;
        for (auto info:cardarray[row]) {
            QJsonObject infoObject;
            info->writeToJson(infoObject);
            jsonArray.append(infoObject);
        }
        jsonCardArrays.append(jsonArray);
    }
    json.insert("cardarray", jsonCardArrays);

    QJsonArray jsonWeatherArray;
    for (int row = 0; row < ROW_NUM; row++) {
        jsonWeatherArray.append(static_cast<int>(rowWeather[row]));
        //TODO check if it is okay
    }
    json.insert("rowWeather", jsonWeatherArray);

    QJsonArray jsonPlayerWinRound;
    QJsonArray jsonPlayerPass;
    QJsonArray jsonPlayerName;
    QJsonArray jsonPlayerScore;
    for (int player = 0; player < 2; player++) {
        jsonPlayerWinRound.append(playerWinRound[player]);
        jsonPlayerPass.append(playerPass[player]);
        jsonPlayerName.append(playerName[player]);
        QJsonArray jsonScoreArray;
        for (int rnd = 0; rnd < jsonScoreArray.size(); rnd++) {
            jsonScoreArray.append(playerScore[player][rnd]);
        }
        jsonPlayerScore.append(jsonScoreArray);
    }
    json.insert("playerWinRound", jsonPlayerWinRound);
    json.insert("playerPass", jsonPlayerPass);
    json.insert("playerName", jsonPlayerName);
    json.insert("playerScore", jsonPlayerScore);

    json.insert("randomSeed", static_cast<int>(randomSeed));
}

bool GameAssets::isDeckValidMove(CardInfo::Type cardType) {
    return deckTypeCount[cardType] + 1 <= MaxDeckCount[cardType] &&
           deckTotalCount + 1 <= 40;
}

bool GameAssets::isDeckValidSave() {
    return 25 <= deckTotalCount && deckTotalCount <= 40;
}

QList<Deck> &GameAssets::getDecks() {
    return decks;
}

void GameAssets::setDecks(const QList<Deck> &decks) {
    GameAssets::decks = decks;
}

const QString &GameAssets::getLeaderName() const {
    return leaderName;
}

void GameAssets::setLeaderName(const QString &leaderName) {
    GameAssets::leaderName = leaderName;
}

int GameAssets::getCardIndexByName(int row, const QString &name) {
    for (int column = 0; column < cardarray[row].size(); column++) {
        if (cardarray[row][column]->getCardName() == name) {
            return column;
        }
    }
    return -1;
}

int GameAssets::getDeckBuilderTargetRow(CardInfo *card) {
    if (card->getInitialStrength() == 0) {//this should be the first
        return DeckBuilder_NoHP;
    } else if (card->getRow() == CardInfo::Ranged) {
        return DeckBuilder_Ranged;
    } else if (card->getRow() == CardInfo::Siege) {
        return DeckBuilder_Siege;
    } else {
        return DeckBuilder_Melee_Event;
    }
}

CardInfo *GameAssets::getLeaderInfo() const {
    return leaderInfo;
}

void GameAssets::setLeaderInfo(CardInfo *leaderInfo) {
    GameAssets::leaderInfo = leaderInfo;
}

void GameAssets::loadPlayerFromDeck(int player, Deck &deck) {
    createCardsRandomlyOnNameListToRow(deck.getCards(), getDeckIndex(player));
    cardarray[getHandIndex(player)].append(CardInfo::createByName(deck.getLeader()));
}

int GameAssets::getWinner() {
    if (playerWinRound[0] < playerWinRound[1]) {
        return 1;
    } else if (playerWinRound[0] > playerWinRound[1]) {
        return 0;
    } else {
        return -1;
    }
}

int GameAssets::getRoundStatus() const {
    return roundStatus;
}

void GameAssets::setRoundStatus(int roundStatus) {
    GameAssets::roundStatus = roundStatus;
}

//you should always perform MOVE instead of REMOVE, case REMOVE is a really serios thing