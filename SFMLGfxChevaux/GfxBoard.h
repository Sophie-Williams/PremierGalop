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

using tPlayerColor = enum 
{
	red, green, blue, yellow
};

typedef std::vector<sf::Sprite *> tpSpriteList;
typedef std::vector<sf::Text *> tpTextList;
typedef std::vector<tpTextList> tpTextListList;

class GfxBoard : public Board
{
public:
	GfxBoard(U32 nbHorses=100,U16 screensizeX=800, U16 screensizeY=600) noexcept;
	virtual ~GfxBoard(void) noexcept;
	virtual bool ReadBoardTopologyFromTable(unsigned int maxLength) noexcept override;
	virtual bool displayBoard(tpCaseList &caseList) noexcept;
	virtual bool displayBoard() noexcept override;
	virtual void displayLeftPannel(std::string human, std::string nickname, int die, std::vector<int> scores, int horsesInTheBox, int iColor) noexcept;
	virtual bool GetChoiceFromEvents(eUserEventType &userEvent, int &nbHorse, int &nbPlayer) noexcept override;
	virtual void displayHistoric() noexcept override;

private:
	bool LoadHorsesBitmap(const char * file, std::size_t size) noexcept;
	bool BuildStartCaseBitmap() noexcept;
	bool BuildLadderCaseBitmap() noexcept;
	bool ComputeGfxCoordinate() noexcept;
	bool ConvertLogicalToGfxCoordinate(LogicalLocalization &logicLoc, S32 &x, S32 &y, U16 &sizeX, U16 &sizeY) noexcept;
	bool ConvertGfxToLogicalCoordinate(S32 x, S32 y, LogicalLocalization &logicLoc) noexcept;
	
	void blitHumanLabel(std::string &human, int iColor) noexcept;
	void blitNickLabel(std::string &nickname, int iColor) noexcept;
	void blitDieLabel(int die, int iColor) noexcept;
	void blitScoreLabel(std::vector<int> scores) noexcept;
	void blitBoxLabel(int horsesInTheBox, int iColor) noexcept;
	void AdaptRatioForSprite(sf::Sprite *sprite) noexcept;
	void AdaptRatioForText(sf::Text *text) noexcept;
	static float getComputedRatio(float currentSizeX, float currentSizeY, int wantedSizeX, int wantedSizeY) noexcept;
	void drawLine(float x1, float y1, float x2, float y2, const sf::Color *color) noexcept;

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
