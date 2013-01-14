#ifndef  __CHEVAUX_HORSE_H__
#define __CHEVAUX_HORSE_H__

class Player;
class Case;

class Horse
{
public:
	Horse(Player * player);
	Player * getPlayer() const;
	Case * getCase() const;
	unsigned int getHorseNumber() const;
	void setCase(Case* pCaseToSet);
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
	Case * m_pCase;
	bool m_bIsArrived;

	int m_score;
};

#endif