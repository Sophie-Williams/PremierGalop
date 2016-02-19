#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cassert>
#include <cstdio>
#include <cstdlib>

#include "Board.h"
#include "Case.h"
#include "Player.h"
#include "FileUtility.h"

Board::Board(U32 nbHorses) : m_nbHorses(nbHorses), m_maxladderCaseValue(0), m_timeScoreResolution(1) {
    srand((unsigned) time(nullptr));
}

bool Board::ReadBoardTopologyFromFile(const std::string &file) {
    std::ifstream inputFile(file.c_str());
    if (tryToLoadFromParentDirectory(inputFile, file)) {
        tStringList stringlist;
        unsigned int maxLength = 0;
        while (inputFile.good() && !inputFile.eof()) {
            std::string line;
            std::getline(inputFile, line);
            stringlist.push_back(line);
            unsigned int size = (unsigned int) stringlist.back().size();
            if (maxLength < size) {
                maxLength = size;
            }
        }
        return ReadBoardTopologyFromTable(stringlist, maxLength);
    }
    return false;
}

bool Board::ReadBoardTopologyFromTable(tStringList &stringlist, unsigned int maxLength) {
    m_stringlist = stringlist;
    return ReadBoardTopologyFromTable(maxLength);
}

bool Board::ReadBoardTopologyFromTable(unsigned int maxLength) {
    LogicalLocalization initialLocalization(0, 0);
    char maxLetter = '\0';
    char maxCapitalLetter = '\0';
    unsigned nbCases = 0;

    unsigned int currentLine = 0;
    for (auto& line : m_stringlist) {
        //search letters in the structure
        unsigned int currentChar = 0;
        for (const auto& chr : line) {
            if (chr != ' ') {//a non-space char is considered as a case
                nbCases++; //it will permits to determine the capacity of the m_cases vector
            }
            if (chr >= 'a' && chr <= 'z') {
                if (maxLetter < chr) {
                    maxLetter = chr;
                }
                if (chr == 'a') {
                    initialLocalization.setLocalization(currentChar, currentLine);
                }
            } else if (chr >= 'A' && chr <= 'Z') {
                if (maxCapitalLetter < chr) {
                    maxCapitalLetter = chr;
                }
            }
            currentChar++;
        }

        //add padding nullptr character to all the string with a size less than maxLength (avoid invalid access memory)
        if (line.size() < maxLength) {
            line.resize(maxLength, ' ');
        }
        currentLine++;
    }

    int nbPlayers = getPlayerNumber(maxLetter) + 1;

    m_startcases.resize(nbPlayers, CASE_ID_UNKNOWN);
    m_endcases.resize(nbPlayers, CASE_ID_UNKNOWN);
    m_cases.reserve(nbCases); //Very important, because we have reference on the internal elements.
    //If capacity is too low, vector is auto-resized, and addresses of the internal elements changes!

    //TODO test if initialLocalization is valid
    return BuildPlayers(nbPlayers)
            && BuildTopology(initialLocalization)
            && PrepareStartCase();
}

int Board::getPlayerNumber(char letter) {
    if (letter >= 'a' && letter <= 'z')
        return letter - 'a';
    else if (letter >= 'A' && letter <= 'Z')
        return letter - 'A';
    else {
        assert(false);
        return -1;
    }
}

char Board::getPlayerLetter(int playerNumber) {
    return static_cast<char> ('a' + playerNumber);
}

char Board::getPlayerCapitalLetter(int playerNumber) {
    return static_cast<char> ('A' + playerNumber);
}

Case& Board::getCase(tCaseId caseId) {
    assert(Case::isValidCaseId(caseId) && caseId.id < static_cast<int> (m_cases.size()));
    return m_cases[caseId.id];
}

bool Board::BuildPlayers(int nbPlayers) {
    char defaultNickname[] = "a";
    m_players.reserve(nbPlayers); //Very important, otherwise the players addresses are moved to another memory address by the vector reallocation
    for (char i = 0; i < nbPlayers; i++, defaultNickname[0]++) {
        m_players.push_back(Player(defaultNickname, defaultNickname[0]));
        m_players.back().AddHorses(this, m_nbHorses);
    }
    return true;
}

bool Board::BuildTopology(const LogicalLocalization &localization, tCaseId previousCase) {
    if (m_cases.size() > 0 && localization == m_cases.front().getLocalization()) {//it means that the build of the normal case is finished, cycle is reached!
        assert(Case::isValidCaseId(previousCase));
        if (Case::isValidCaseId(previousCase)) {
            getCase(previousCase).setNextCase(m_cases.front().getCaseId()); //set last link of the normal case
            return true;
        } else {
            return false;
        }
    }

    char caseID = getCharAt(localization);
    //if((caseID >= 'a' && caseID <= 'z')
    //	|| (caseID >= 'A' && caseID <= 'Z')
    //	|| (caseID == '#')
    //	|| (caseID == '='))
    if (Case::isANormalCase(caseID)
            || Case::isAPureLadderCase(caseID)) {
        LogicalLocalization newLocalization;
        tCaseId lastCase = {(int) m_cases.size()};
        m_cases.push_back(Case(this, localization, lastCase, caseID));
        m_casesLogicalMap.insert(tpCaseMap::value_type(localization, lastCase));

        //if(caseID != '=' && caseID != '$')
        if (Case::isANormalCase(caseID)) {
            bool returnOfFirstBuildTopology = true;
            if (Case::isValidCaseId(previousCase)) {
                getCase(previousCase).setNextCase(lastCase);
            }

            if (Case::isAStartCase(caseID))//if(caseID >= 'a' && caseID <= 'z')
            {
                m_startcases[getPlayerNumber(caseID)] = lastCase;
            } else if (Case::isABaseForLadder(caseID))//if(caseID >= 'A' && caseID <= 'Z')
            {
                getCase(lastCase).appendBaseLadderForPlayer(&m_players[getPlayerNumber(caseID)]);
                //Build topology of the ladder
                returnOfFirstBuildTopology = AnalyseAndChooseNextLocalization(true, getCase(lastCase).getLocalization(),
                        getCase(previousCase).getLocalization(),
                        newLocalization)
                        && BuildTopology(newLocalization, lastCase);
            }

            if (!Case::isValidCaseId(previousCase)) {//it means that there is no predecessor
                return AnalyseAndChooseNextLocalization(false, getCase(lastCase).getLocalization(),
                        getCase(lastCase).getLocalization(),
                        newLocalization)
                        && BuildTopology(newLocalization, lastCase);
            } else {
                //Build Topology of the normal case
                return returnOfFirstBuildTopology
                        && AnalyseAndChooseNextLocalization(false, getCase(lastCase).getLocalization(),
                        getCase(previousCase).getLocalization(),
                        newLocalization)
                        && BuildTopology(newLocalization, lastCase);
            }
        } else //if(Case::isAPureLadderCase()) // if(caseID == '=')
        {
            assert(Case::isValidCaseId(previousCase));
            if (Case::isValidCaseId(previousCase)) {
                getCase(previousCase).setNextLadderCase(lastCase);
                getCase(lastCase).setLadderCaseValue(getCase(previousCase).getLadderCaseValue() + 1); //ladder value is incremented for the next ladder
            }
            getCase(lastCase).copyLadderSet(previousCase);
            //Build Topology of the ladder
            return AnalyseAndChooseNextLocalization(true, getCase(lastCase).getLocalization(),
                    getCase(previousCase).getLocalization(),
                    newLocalization)
                    && BuildTopology(newLocalization, lastCase);
        }
    } else if (Case::isAFinishCase(caseID)) //(caseID == '$')
    {
        tCaseId endCase = CASE_ID_UNKNOWN;
        //first we must know if the final case already exist (only one final case is authorized).
        for (const auto& endCaseId : m_endcases) {
            if (Case::isValidCaseId(endCaseId)) {
                endCase = endCaseId;
                break;
            }
        }
        if (!Case::isValidCaseId(endCase)) {//if the final case doesn't exists, we create a new one!
            assert(m_cases.back().getCaseId().id == (int) (m_cases.size() - 1)); //just a sanity check
            endCase.id = (int) m_cases.size();
            m_cases.push_back(Case(this, localization, endCase, caseID)); //the size gives the new id of the new case
            getCase(endCase).setLadderCaseValue(getCase(previousCase).getLadderCaseValue()); //keep the same ladder value
            m_maxladderCaseValue = getCase(previousCase).getLadderCaseValue(); //in a standard game, 6 is the max value
        }
        if (Case::isValidCaseId(previousCase)) {
            for (const auto & player : m_players) {
                if (getCase(previousCase).isALadderForPlayer(&player)) {
                    m_endcases[player.getPlayerNb()] = endCase;
                }
            }
            getCase(endCase).copyLadderSet(previousCase);
            getCase(previousCase).setNextLadderCase(endCase);
        } else {
            return false;
        }
    }
    return true;
}

unsigned int Board::getNumberOfPlayers() {
    return static_cast<unsigned int> (m_players.size());
}

bool Board::AnalyseAndChooseNextLocalization(bool expectingLadderCase,
        const LogicalLocalization &currentLocalization,
        const LogicalLocalization &previousLocalization,
        LogicalLocalization &outputLocalization) {
    //Analyse the four directions (one of them has been already explorated, one is expected, the two remaining possibility are useless)
    for (int direction = 0; direction < 4; direction++) {
        outputLocalization = currentLocalization;

        if ((direction == 0 && outputLocalization.MoveUp())
                || (direction == 1 && outputLocalization.MoveDown(getMaxY()))
                || (direction == 2 && outputLocalization.MoveRight(getMaxX()))
                || (direction == 3 && outputLocalization.MoveLeft())) {
            if (outputLocalization != previousLocalization) {
                char readChar = getCharAt(outputLocalization);
                if (expectingLadderCase) {
                    if (Case::isALadderCase(readChar))//(readChar == '=' || readChar == '$')
                    {
                        return true;
                    }
                } else {
                    if (currentLocalization == previousLocalization) {//it means that it is the primary case, so only '#' are allowed (need in order to initiate recursion)
                        if (Case::isAPureNormalCase(readChar))//(readChar == '#')
                        {
                            return true;
                        }
                    }
                    //if(readChar == '#'
                    //	|| (readChar >= 'a' && readChar <= 'z')
                    //	|| (readChar >= 'A' && readChar <= 'Z'))
                    if (Case::isANormalCase(readChar)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Board::PrepareStartCase() {
    if (m_maxladderCaseValue > 0) {
        for (const auto& startCaseID : m_startcases) {
            if (Case::isValidCaseId(startCaseID)) {
                getCase(startCaseID).setLadderCaseValue(m_maxladderCaseValue);
            } else {
                assert(Case::isValidCaseId(startCaseID));
                return false;
            }
        }
        return true;
    }
    return false;
}

char Board::getCharAt(unsigned int x, unsigned int y) {
    return m_stringlist[y][x];
}

char Board::getCharAt(LogicalLocalization localization) {
    return m_stringlist[localization.getY()][localization.getX()];
}

unsigned int Board::getMaxX() {
    return static_cast<unsigned int> (m_stringlist.front().size());
}

unsigned int Board::getMaxY() {
    return static_cast<unsigned int> (m_stringlist.size());
}

bool Board::displayBoard() {
    tStringList displayTab(getMaxY(), std::string(getMaxX(), ' '));
    for (const auto & caseItem : m_cases) {
        LogicalLocalization loc = caseItem.getLocalization();
        Horse * horse = caseItem.getHorse();
        char c;
        if (horse == nullptr) {
            if (caseItem.isAStartCase())
                c = '_';
            else if (caseItem.isAPureLadderCase())
                c = static_cast<char> ('0' + caseItem.getLadderCaseValue());
            else if (caseItem.isAFinishCase())
                c = '$';
            else if (caseItem.isANormalCase())
                c = '.';
            else
                c = '?';
        } else {
            assert(horse->getPlayer() != nullptr);
            assert(Case::isValidCaseId(horse->getCase()));
            c = static_cast<char> ('a' + horse->getPlayer()->getPlayerNb());
        }
        displayTab[loc.getY()][loc.getX()] = c;
    }
    displayString(displayTab);
    return true;
}

void Board::displayString(tStringList &stringList) {
    for (const auto& line : stringList) {
        std::cout << line << std::endl;
    }
}

unsigned int Board::launchDie() {
    return launchDie(m_maxladderCaseValue);
}

unsigned int Board::launchDie(unsigned int maxValue) {
    return (rand() % maxValue) + 1;
}

bool Board::playOneTurnRequest(Player* currentPlayer, tHorseTargetCaseList &outputMoves, unsigned int &outputDie) {
    assert(currentPlayer != nullptr);
    if (currentPlayer != nullptr) {
        outputDie = launchDie();
        outputMoves.clear(); //must be sure that the list is empty before filling it!
        for (unsigned int iHorse = 0; iHorse < m_nbHorses; iHorse++) {
            Horse* currentHorse = currentPlayer->getHorse(iHorse);

            assert(currentHorse != nullptr);
            if (currentHorse != nullptr) {
                tCaseId currentCase = CASE_ID_UNKNOWN;
                if (currentHorse->isRunning())
                    currentCase = currentHorse->getCase();
                else if (currentHorse->isSleeping())
                    currentCase = m_startcases[currentPlayer->getPlayerNb()];
                else
                    continue;

                assert(Case::isValidCaseId(currentCase));
                tMapDieNumberCase outputPossibleMoves;
                if (getCase(currentCase).getPossibleMoves(currentHorse, outputPossibleMoves)) {
                    tMapDieNumberCase::const_iterator itDieNumberCase = outputPossibleMoves.find(outputDie);
                    if (itDieNumberCase != outputPossibleMoves.end()) {
                        tHorseTargetCase horseCase = {currentHorse, itDieNumberCase->second};
                        outputMoves.push_back(horseCase);
                    }
                }
            } else
                return false;
        }
        return true;
    }
    return false;
}

bool Board::playOneTurnResponse(Horse* horseToMove, tCaseId caseToReach, unsigned int inputDie, Player* &nextPlayer) {
    bool bStatus = true;
    if (horseToMove != nullptr) {
        horseToMove->incrementScore(inputDie);
        if (inputDie != m_maxladderCaseValue)//in a standard game, without a 6 the player don't have the right to relaunch the die
        {
            bStatus = getNextPlayer(nextPlayer);
        }
        if (bStatus && Case::isValidCaseId(caseToReach)) {//chosen move (or only one possibilty)
            getCase(caseToReach).moveFrom(horseToMove->getCase(), horseToMove);
        }
        //else no moves are possible!
    } else {//Player can't move any horses! Now it is the turn to the next player to play!
        if (inputDie != m_maxladderCaseValue)//in a standard game, without a 6 the player don't have the right to relaunch the die
        {
            bStatus = getNextPlayer(nextPlayer);
        }
    }
    return bStatus;
}

Player* Board::getPlayer(unsigned int playerNumber) {
    assert(playerNumber < m_players.size());
    if (playerNumber < m_players.size()) {
        return &m_players[playerNumber];
    }
    return nullptr;
}

unsigned int Board::GetNumberOfHorsesByPLayer() const {
    return m_nbHorses;
}

bool Board::getNextPlayer(Player* &nextPlayer) {
    unsigned int currentPlayerNumber = nextPlayer->getPlayerNb();
    unsigned int nextPlayerNb;
    do {
        nextPlayerNb = (nextPlayer->getPlayerNb() + 1) % m_players.size();
        nextPlayer = &m_players[nextPlayerNb];
        if (!nextPlayer->hasAlreadyWon())//if player has already won the game, we continue with players that have still horses on the board
        {
            return true;
        }
    } while (nextPlayerNb != currentPlayerNumber); //next player can't be the same player than the previous one!
    nextPlayer = nullptr;
    return false;
}

tCaseList& Board::getCasesList() {
    return m_cases;
}

void Board::getpCasesList(tpCaseList &caseListToFill) {
    for (const auto& caseItem : m_cases) {
        caseListToFill.push_back(caseItem.getCaseId());
    }
}

tPlayerList& Board::getPlayersList() {
    return m_players;
}

tCaseId Board::GetCaseFromLocalization(const LogicalLocalization &logicLoc) const {
    tpCaseMap::const_iterator itLogicalLocCase = m_casesLogicalMap.find(logicLoc);
    if (itLogicalLocCase != m_casesLogicalMap.end())
        return itLogicalLocCase->second;
    else
        return CASE_ID_UNKNOWN;
}

bool Board::GetChoiceFromEvents(eUserEventType &userEvent, int &nbHorse, int &nbPlayer) {
    nbHorse = -1;
    nbPlayer = -1;
    userEvent = None;
    while (userEvent == None) {
        std::cout << "Entre le numero du cheval: " << std::endl;
        std::cin >> nbHorse;
        if (nbHorse >= static_cast<int> (GetNumberOfHorsesByPLayer()) || std::cin.fail()) {
            // not a numeric character, probably
            // clear the failure and pull off the non-numeric character
            std::cin.clear();
            char c;
            std::cin >> c;
        } else {
            userEvent = HorseKey;
            return true;
        }
    }
    return false;
}

int Board::GetHorseNumberInTheBox(tHorseTargetCaseList &horseList) {
    int nbHorseInTheBox = 0;
    for (const auto& horseTargetCase : horseList) {
        assert(horseTargetCase.pHorse != nullptr);
        if (horseTargetCase.pHorse != nullptr && horseTargetCase.pHorse->isSleeping()) {
            nbHorseInTheBox++;
        }
    }
    return nbHorseInTheBox;
}

std::vector<int> Board::getScores() const {
    std::vector<int> scores;
    for (const auto& player : m_players) {
        scores.push_back(player.getScore());
    }
    return scores;
}


//TODO try to find an algorithm in order to have an historic of the point without using too much memory.
//Perhaps a time value is necessary in order to know which value to remove.
//T0 initial time
//Tn last time
//wanted time resolution T1-T0
//try to eliminate values without the correct time resolution, by starting by the end of the list, until the number of values in the list is reached
//0 1 2 3 4 5 6 7 8 9
//0 2 4 6 8 reduction
//now accepts only time modulo 2
//0 2* 4 6* 8 10* 12 14* 16 18*
//0 4 8 12 16 reduction
//now accepts only time modulo 4
//0 4 8 12 16 20 24 28 32 36
//0 8 16 24 32 reduction
//no need to store time for each value, because we always remove the same elements!
//0 8 16 24 32 40 48 56
//0 16 32 48 semi reduction: so some value are missing, it will be necessary to adapt the scale in order to have a full screen display

void Board::storeAndReduceScores(unsigned int currentTime, unsigned int nbScoreResolution, std::vector<int> &scoresToAppend) {
    if (currentTime % m_timeScoreResolution == 0) {
        m_historicScores.push_back(scoresToAppend);
        //reduction
        if (m_historicScores.size() >= 4 * nbScoreResolution) {
            scoreReduction();
        }
    }
}

void Board::scoreReduction() {
    for (tHistoricList::iterator it = m_historicScores.begin(); it != m_historicScores.end(); it++) {//When item is erased, iterator points on next item! So if iterator is incremented, iterator skips one item!
        m_historicScores.erase(it++);
        if (it == m_historicScores.end())
            break;
    }
    m_timeScoreResolution *= 2;
}

tHistoricList& Board::getHistoricList() {
    return m_historicScores;
}

void Board::displayHistoric() {
    for (const auto& historicScore : m_historicScores) {//When item is erased, iterator points on next item! So if iterator is incremented, iterator skips one item!
        for (const auto& scoreVal : historicScore) {
            std::cout << scoreVal << '\t';
        }
        std::cout << std::endl;
    }
}

unsigned int Board::getMaxLadderCaseValue() const {
    return m_maxladderCaseValue;
}