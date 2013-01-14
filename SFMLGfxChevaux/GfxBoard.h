#ifndef  __CHEVAUX_GFXBOARD_H__
#define __CHEVAUX_GFXBOARD_H__

#include <vector>
#include <string>
#include "types.h"
#include "Board.h"
#include "LogicalLocalization.h"

namespace sf
{
	class RenderWindow;
	class Font;
	class Image;
	class Sprite;
	class Text;
	class Color;
}

typedef enum 
{
	red, green, blue, yellow
} tPlayerColor;

#define MAX_LADDER_DIGIT 9

typedef std::vector<sf::Sprite *> tpSpriteList;
typedef std::vector<sf::Text *> tpTextList;
typedef std::vector<tpTextList> tpTextListList;

class GfxBoard : public Board
{
public:
	GfxBoard(U32 nbHorses=100,U16 screensizeX=800, U16 screensizeY=600);
	virtual ~GfxBoard(void);
	virtual bool ReadBoardTopologyFromTable(unsigned int maxLength);
	virtual bool displayBoard(tpCaseList &caseList);
	virtual bool displayBoard();
	virtual void displayLeftPannel(std::string human, std::string nickname, int die, std::vector<int> scores, int horsesInTheBox, int iColor);
	virtual bool GetChoiceFromEvents(eUserEventType &userEvent, int &nbHorse, int &nbPlayer);
	virtual void displayHistoric();

private:
	bool LoadHorsesBitmap(const char * file, std::size_t size);
	bool GfxBoard::BuildStartCaseBitmap();
	bool BuildLadderCaseBitmap();
	bool ComputeGfxCoordinate();
	bool ConvertLogicalToGfxCoordinate(LogicalLocalization &logicLoc, S32 &x, S32 &y, U16 &sizeX, U16 &sizeY);
	bool ConvertGfxToLogicalCoordinate(S32 x, S32 y, LogicalLocalization &logicLoc);
	
	void blitHumanLabel(std::string &human, int iColor);
	void blitNickLabel(std::string &nickname, int iColor);
	void blitDieLabel(int die, int iColor);
	void blitScoreLabel(std::vector<int> scores);
	void blitBoxLabel(int horsesInTheBox, int iColor);
	void AdaptRatioForSprite(sf::Sprite *sprite);
	void AdaptRatioForText(sf::Text *text);
	static float getComputedRatio(float currentSizeX, float currentSizeY, int wantedSizeX, int wantedSizeY);
	void drawLine(float x1, float y1, float x2, float y2, const sf::Color *color);

private:
	sf::RenderWindow * m_pApp;
	sf::Font * m_pFont;
	tpSpriteList m_horseSprites;
	sf::Sprite *m_normalCaseSprite;
	sf::Text *m_startCaseText;
	tpTextList m_playerladderText;
	U16 m_screensizeX;
	U16 m_screensizeY;
	U16 m_gridCaseSizeX;
	U16 m_gridCaseSizeY;
};

#endif