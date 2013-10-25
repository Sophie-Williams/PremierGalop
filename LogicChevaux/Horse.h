#ifndef  __CHEVAUX_HORSE_H__
#define __CHEVAUX_HORSE_H__

#include "Case.h"

class Player;

class Horse
{
public:
	Horse(Board * pBoard, Player * player);
	Player * getPlayer() const;
	tCaseId getCase() const;
	unsigned int getHorseNumber() const;
	void setCase(tCaseId pCaseToSet);
	bool haveSamePlayerOwner(const Horse *horse) const;

	bool isSleeping() const;
	bool isArrived() const;
	bool isRunning() const;
	bool isRunningOnNormalCase() const;
	bool isRunningOnLadderCase() const;
	bool isRunningOnfirstCaseofLadderCase() const;

	void returnToSleepingBox();
	void setArrivedState();

	bool operator==(const Horse &horse) const;

	int getScore() const;
	void resetScore();
	void incrementScore(int increment);

private:
	Player * m_pPlayer;
	tCaseId m_pCase;
	bool m_bIsArrived;

	int m_score;
	Board * m_pBoard;
};

#endif