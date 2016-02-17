#ifndef  __CHEVAUX_CASE_H__
#define __CHEVAUX_CASE_H__

#include <set>
#include <map>
#include "LogicalLocalization.h"

class Horse;
class Case;
class Player;
class Board;

typedef struct{int id;} tCaseId;
const tCaseId CASE_ID_UNKNOWN ={-1};

typedef std::map<unsigned int, tCaseId> tMapDieNumberCase;

class Case
{
public:
	Case(Board * pBoard, unsigned int x=0, unsigned int y=0, tCaseId id=CASE_ID_UNKNOWN, char caseValue='\0');
	Case(Board * pBoard, const LogicalLocalization &localization, tCaseId id=CASE_ID_UNKNOWN, char caseValue='\0');

	bool getPossibleMoves(Horse* horse, tMapDieNumberCase &outputMoves, unsigned int level=0) const;
	bool moveFrom(tCaseId pCaseSource, Horse* pHorse);

	bool appendBaseLadderForPlayer(Player *player);
	bool isAStartCaseForPlayer(Player* pPlayer) const;
	bool isALadderForPlayer(Player* pPlayer) const;
	bool isAStartCaseForHorse(Horse* pHorse) const;
	bool isALadderForHorse(Horse* pHorse) const;
	bool copyLadderSet(const tCaseId sourceCase);
	bool setNextCase(tCaseId caseToReference);
	bool setNextLadderCase(tCaseId caseToReference);
	bool isALadderCase() const;
	bool isANormalCase() const;
	bool isAPureNormalCase() const;
	bool isAStartCase() const;
	bool isABaseForLadder() const;
	bool isAFinishCase() const;
	bool isAPureLadderCase() const;
	static bool isACase(char charId);
	static bool isALadderCase(char charId);
	static bool isANormalCase(char charId);
	static bool isAPureNormalCase(char charId);
	static bool isAStartCase(char charId);
	static bool isABaseForLadder(char charId);
	static bool isAFinishCase(char charId);
	static bool isAPureLadderCase(char charId);
	const LogicalLocalization& getLocalization() const;
	Horse* getHorse() const;
	tCaseId getCaseId() const {return m_id;};
	static bool isValidCaseId(tCaseId caseId);

	unsigned int getLadderCaseValue() const;
	void setLadderCaseValue(unsigned int ladderValue);

private:
	void setHorse(Horse* horse);

private:
	std::set<Player*> m_PlayerNumberBaseLadder;
	//next case in the normal case
	tCaseId m_pNextCase;
	//next case in the ladder case
	tCaseId m_pNextLadderCase;
	//Horse of player x is occupying the place
	Horse * m_pHorse;
	//if the case is a ladder, it needs a specific value for moving on horse on it.
	unsigned int m_ladderValue;
	//caseValue is the char value read from the board topology file for the case.
	char m_caseValue;
	//a case id should be used for referencing the case 
	tCaseId m_id;
	//keep a pointer on the board
	Board * m_pBoard;
        //logical localization is the localization in the topology file for the case
	LogicalLocalization m_logicalLocalization;
};

#endif