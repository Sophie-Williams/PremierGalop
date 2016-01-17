#ifndef  __CHEVAUX_GFXBOARD_H__
#define __CHEVAUX_GFXBOARD_H__

#include <vector>
#include <string>
#include "types.h"
#include "Board.h"
#include "LogicalLocalization.h"
	
struct SDL_Window;
struct SDL_Texture;
struct SDL_Renderer;
typedef struct _TTF_Font TTF_Font;

typedef enum 
{
	red, green, blue, yellow
} tPlayerColor;

const static std::string gfxfontFile = "../gfx/LiberationSansNarrow-Bold.ttf";

const static std::string gfxBackGroundFile = "../gfx/Background.png";

const static std::string gfxNormalCaseFile = "../gfx/NormalCase.png";

#define MAX_LADDER_DIGIT 9

static std::string gfxHorseFiles[]=
{
	"../gfx/RedHorse.png",
	"../gfx/GreenHorse.png",
	"../gfx/BlueHorse.png",
	"../gfx/YellowHorse.png"
};

typedef std::vector<SDL_Texture*> tpTextureList;
typedef std::vector<tpTextureList> tpTextureListList;

typedef struct {int w; int h;} tSize;

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
	void InitSidePanel();
	bool LoadHorsesBitmap(const std::string& file);
	SDL_Texture * LoadTexture(const std::string& file);
	bool BuildStartCaseBitmap();
	bool BuildLadderCaseBitmap();
	//void DrawPixel(int x, int y, U8 R, U8 G, U8 B);
	void ShowBMP(SDL_Texture* image,int x, int y);
	bool ComputeGfxCoordinate();
	bool ConvertLogicalToGfxCoordinate(LogicalLocalization &logicLoc, S32 &x, S32 &y, U16 &sizeX, U16 &sizeY);
	bool ConvertGfxToLogicalCoordinate(S32 x, S32 y, LogicalLocalization &logicLoc);
	
	void blitHumanLabel(std::string &human, int iColor);
	void blitNickLabel(std::string &nickname, int iColor);
	void blitDieLabel(int die, int iColor);
	void blitScoreLabel(std::vector<int> scores);
	void blitBoxLabel(int horsesInTheBox, int iColor);

	static TTF_Font * LoadFont(const std::string& file, int size);

	int getTextureW(SDL_Texture *pTexture);
	int getTextureH(SDL_Texture *pTexture);

private:
	SDL_Window *m_wnd;
	SDL_Renderer *m_renderer;
	tpTextureList m_horseImages;
	SDL_Texture *m_normalCaseImage;
	tpTextureList m_startCaseImages;
	tpTextureListList m_playerladderImages;

	SDL_Texture * m_HumanPanelTexture;
	SDL_Texture * m_NickPanelTexture;
	SDL_Texture * m_DiePanelTexture;
	SDL_Texture * m_ScorePanelTexture;
	SDL_Texture * m_BoxPanelTexture;

	TTF_Font *m_pFont;
	TTF_Font *m_pLittleFont;
	U16 m_screensizeX; //TODO : change to tSize!!!!
	U16 m_screensizeY;
	U16 m_gridCaseSizeX;//TODO : change to tSize!!!!
	U16 m_gridCaseSizeY;

	int m_sidePanelPositionX;
};

#endif
