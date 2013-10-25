#ifndef  __CHEVAUX_BOARD_H__
#define __CHEVAUX_BOARD_H__

#include <string>
#include <vector>
#include <map>

#include "types.h"
#include "Case.h"
#include "Player.h"
#include "LogicalLocalization.h"

typedef std::vector<std::string> tStringList;
typedef std::vector<Case> tCaseList;
typedef std::map<LogicalLocalization,tCaseId> tpCaseMap;
typedef std::vector<tCaseId> tpCaseList;
typedef std::vector<Player> tPlayerList;
typedef std::list<std::vector<int> > tHistoricList;


typedef enum
{
	None,
	ClickedCase,
	ClickedPanel,
	HorseKey,
	CloseEvent,
} eUserEventType;


class Board
{
public:
	Board(U32 nbHorse = 100);

	bool ReadBoardTopologyFromFile(const std::string &file);
	bool ReadBoardTopologyFromTable(tStringList &stringlist, unsigned int maxLength);
	static int getPlayerNumber(char letter);
	static char getPlayerLetter(int playerNumber);
	static char getPlayerCapitalLetter(int playerNumber);
	unsigned int getNumberOfPlayers();
	Player * getPlayer(unsigned int playerNumber);
	unsigned int getMaxLadderCaseValue() const;
	Case& getCase(tCaseId caseId);

	unsigned int getMaxX();
	unsigned int getMaxY();

	virtual bool displayBoard();
	static void displayString(tStringList &stringList);
	unsigned int launchDie();
	static unsigned int launchDie(unsigned int maxValue);
	unsigned int GetNumberOfHorsesByPLayer() const;
	tCaseId GetCaseFromLocalization(const LogicalLocalization &logicLoc) const;
	virtual bool GetChoiceFromEvents(eUserEventType &userEvent, int &nbHorse, int &nbPlayer);

	bool playOneTurnRequest(Player* currentPlayer, tHorseTargetCaseList &outputMoves, unsigned int &outputDie);
	bool playOneTurnResponse(Horse* horseToMove, tCaseId caseToReach, unsigned int inputDie, Player* &nextPlayer);

	static int GetHorseNumberInTheBox(tHorseTargetCaseList &horseList);
	std::vector<int> getScores() const;
	void getpCasesList(tpCaseList &caseListToFill);
	void storeAndReduceScores(unsigned int currentTime, unsigned int nbScoreResolution, std::vector<int> &scoresToAppend);
	void scoreReduction();
	tHistoricList& getHistoricList();
	virtual void displayHistoric();
	tPlayerList& getPlayersList();

protected:
	virtual bool ReadBoardTopologyFromTable(unsigned int maxLength);
	tCaseList& getCasesList();

private:
	bool BuildPlayers(int nbPlayers);
	bool BuildTopology(const LogicalLocalization &localization, tCaseId previousCase = CASE_ID_UNKNOWN);
	bool PrepareStartCase();
	bool AnalyseAndChooseNextLocalization(bool expectingLadderCase,
							   const LogicalLocalization &currentLocalization,
							   const LogicalLocalization &previousLocalization,
							   LogicalLocalization &outputLocalization);
	char getCharAt(unsigned int x, unsigned int y);
	char getCharAt(LogicalLocalization localization);
	bool getNextPlayer(Player* &currentPlayer);

private:
	U32 m_nbHorses;
	//use for building and filling classes
	tStringList m_stringlist;

	//list of all cases (allocation of the cases are done here!) 
	tCaseList m_cases;
	tpCaseMap m_casesLogicalMap;

	//reference to the start cases for each player startcases[0] is the start case of player 0
	tpCaseList m_startcases;
	//reference to the end cases for each player endcases[0] is the end case of player 0
	tpCaseList m_endcases;
	//list of all players
	tPlayerList m_players;
	unsigned int m_maxladderCaseValue;

	tHistoricList m_historicScores;
	unsigned int m_timeScoreResolution;
};

#endif