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
using TTF_Font = struct _TTF_Font;

using tPlayerColor = enum {
    red, green, blue, yellow
};

using tpTextureList = std::vector<SDL_Texture*>;
using tpTextureListList = std::vector<tpTextureList>;
using tSize = struct {
    int w;
    int h;
};

class GfxBoard : public Board {
public:
    GfxBoard(U32 nbHorses = 100, U16 screensizeX = 800, U16 screensizeY = 600) noexcept;
    virtual ~GfxBoard(void) noexcept;
    virtual bool ReadBoardTopologyFromTable(unsigned int maxLength) noexcept override;
    virtual bool displayBoard(tpCaseList &caseList) noexcept;
    virtual bool displayBoard() noexcept override;
    virtual void displayLeftPannel(std::string human, std::string nickname, int die, std::vector<int> scores, int horsesInTheBox, int iColor) noexcept;
    virtual bool GetChoiceFromEvents(eUserEventType &userEvent, int &nbHorse, int &nbPlayer) noexcept override;

private:
    void InitSidePanel() noexcept;
    bool LoadHorsesBitmap(const std::string& file) noexcept;
    SDL_Texture * LoadTexture(const std::string& file) noexcept;
    bool BuildStartCaseBitmap() noexcept;
    bool BuildLadderCaseBitmap() noexcept;
    void ShowBMP(SDL_Texture* image, int x, int y) noexcept;
    bool ComputeGfxCoordinate() noexcept;
    bool ConvertLogicalToGfxCoordinate(LogicalLocalization &logicLoc, S32 &x, S32 &y, U16 &sizeX, U16 &sizeY) noexcept;
    bool ConvertGfxToLogicalCoordinate(S32 x, S32 y, LogicalLocalization &logicLoc) noexcept;

    void blitHumanLabel(std::string &human, int iColor) noexcept;
    void blitNickLabel(std::string &nickname, int iColor) noexcept;
    void blitDieLabel(int die, int iColor) noexcept;
    void blitScoreLabel(std::vector<int> scores) noexcept;
    void blitBoxLabel(int horsesInTheBox, int iColor) noexcept;

    static TTF_Font * LoadFont(const std::string& file, int size) noexcept;

    int getTextureW(SDL_Texture *pTexture) noexcept;
    int getTextureH(SDL_Texture *pTexture) noexcept;

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
    U16 m_gridCaseSizeX; //TODO : change to tSize!!!!
    U16 m_gridCaseSizeY;

    int m_sidePanelPositionX;
};

#endif
