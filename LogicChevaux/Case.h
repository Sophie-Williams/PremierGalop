#ifndef  __CHEVAUX_CASE_H__
#define __CHEVAUX_CASE_H__

#include <set>
#include <map>
#include "LogicalLocalization.h"

class Horse;
class Case;
class Player;
class Case;

typedef std::map<unsigned int, Case*> tMapDieNumberCase;

class Case
{
public:
	Case(unsigned int x=0, unsigned int y=0, char caseValue='\0');
	Case(const LogicalLocalization &localization, char caseValue='\0');

	bool getPossibleMoves(Horse* horse, tMapDieNumberCase &outputMoves, unsigned int level=0);
	bool moveFrom(Case* pCaseSource, Horse* pHorse);

	bool appendBaseLadderForPlayer(Player *player);
	bool isAStartCaseForPlayer(Player* pPlayer) const;
	bool isALadderForPlayer(Player* pPlayer) const;
	bool isAStartCaseForHorse(Horse* pHorse) const;
	bool isALadderForHorse(Horse* pHorse) const;
	bool copyLadderSet(const Case* sourceCase);
	bool setNextCase(Case* caseToReference);
	bool setNextLadderCase(Case* caseToReference);
	bool isALadderCase();
	bool isANormalCase();
	bool isAPureNormalCase();
	bool isAStartCase();
	bool isABaseForLadder();
	bool isAFinishCase();
	bool isAPureLadderCase();
	static bool isACase(char charId);
	static bool isALadderCase(char charId);
	static bool isANormalCase(char charId);
	static bool isAPureNormalCase(char charId);
	static bool isAStartCase(char charId);
	static bool isABaseForLadder(char charId);
	static bool isAFinishCase(char charId);
	static bool isAPureLadderCase(char charId);
	LogicalLocalization& getLocalization();
	Horse* getHorse();

	unsigned int getLadderCaseValue();
	void setLadderCaseValue(unsigned int ladderValue);

private:
	void setHorse(Horse* horse);

private:
	std::set<Player*> m_PlayerNumberBaseLadder;
	//next case in the normal case
	Case * m_pNextCase;
	//next case in the ladder case
	Case * m_pNextLadderCase;
	//Horse of player x is occupying the place
	Horse * m_pHorse;
	//if the case is a ladder, it needs a specific value for moving on horse on it.
	unsigned int m_ladderValue;
	//caseValue is the char value read from the board topology file for the case.
	char m_caseValue;
	//logical localization is the localization in the topology file for the case
	LogicalLocalization m_logicalLocalization;
};

#endif