#ifndef  __CHEVAUX_GFXBOARD_H__
#define __CHEVAUX_GFXBOARD_H__

#include <vector>
#include <string>
#include "types.h"
#include "Board.h"
#include "LogicalLocalization.h"
	
struct SDL_Surface;
typedef struct _TTF_Font TTF_Font;

typedef enum 
{
	red, green, blue, yellow
} tPlayerColor;

static char gfxfontFile[] = "../gfx/LiberationSansNarrow-Bold.ttf";

static char gfxBackGroundFile[] = "../gfx/Background.png";

static char gfxNormalCaseFile[] = "../gfx/NormalCase.png";

#define MAX_LADDER_DIGIT 9

static char *gfxHorseFiles[]=
{
	"../gfx/RedHorse.png",
	"../gfx/GreenHorse.png",
	"../gfx/BlueHorse.png",
	"../gfx/YellowHorse.png"
};

typedef std::vector<SDL_Surface*> tpSurfaceList;
typedef std::vector<tpSurfaceList> tpSurfaceListList;

class GfxBoard : public Board
{
public:
	GfxBoard(U32 nbHorses=100, U16 screensizeX=800, U16 screensizeY=600);
	virtual ~GfxBoard(void);
	virtual bool ReadBoardTopologyFromTable(unsigned int maxLength);
	virtual bool displayBoard(tpCaseList &caseList);
	virtual bool displayBoard();
	virtual void displayLeftPannel(std::string human, std::string nickname, int die, std::vector<int> scores, int horsesInTheBox, int iColor);
	virtual bool GetChoiceFromEvents(eUserEventType &userEvent, int &nbHorse, int &nbPlayer);

private:
	bool LoadHorsesBitmap(char * file);
	bool BuildStartCaseBitmap();
	bool BuildLadderCaseBitmap();
	void DrawPixel(int x, int y, U8 R, U8 G, U8 B);
	void ShowBMP(SDL_Surface* image,int x, int y);
	void FillRect(U16 x, U16 y, U16 sizeX, U16 sizeY, U8 R, U8 G, U8 B, U8 A);
	bool ComputeGfxCoordinate();
	bool ConvertLogicalToGfxCoordinate(LogicalLocalization &logicLoc, S32 &x, S32 &y, U16 &sizeX, U16 &sizeY);
	bool ConvertGfxToLogicalCoordinate(S32 x, S32 y, LogicalLocalization &logicLoc);
	
	void blitHumanLabel(std::string &human, int iColor);
	void blitNickLabel(std::string &nickname, int iColor);
	void blitDieLabel(int die, int iColor);
	void blitScoreLabel(std::vector<int> scores);
	void blitBoxLabel(int horsesInTheBox, int iColor);

	static TTF_Font * LoadFont(const char * file, int size);
	static SDL_Surface * LoadBitmap(char *file);

private:
	SDL_Surface *m_screen;
	tpSurfaceList m_horseImages;
	SDL_Surface *m_normalCaseImage;
	tpSurfaceList m_startCaseImages;
	tpSurfaceListList m_playerladderImages;
	TTF_Font *m_pFont;
	TTF_Font *m_pLittleFont;
	U16 m_screensizeX;
	U16 m_screensizeY;
	U16 m_gridCaseSizeX;
	U16 m_gridCaseSizeY;
};

#endif
