#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstring>
#include <cstdlib>

#include "FileUtility.h"
#include "GfxBoard.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_endian.h> /* Used for the endian-dependent 24 bpp mode */

const static int MAX_DISPLAYED_HORSE = 8;
const static int MAX_LADDER_DIGIT = 9;

const static std::string gfxfontFile = "../gfx/LiberationSansNarrow-Bold.ttf";
const static std::string gfxBackGroundFile = "../gfx/Background.png";
const static std::string gfxNormalCaseFile = "../gfx/NormalCase.png";
const static std::vector<std::string> gfxHorseFiles{
    "../gfx/RedHorse.png",
    "../gfx/GreenHorse.png",
    "../gfx/BlueHorse.png",
    "../gfx/YellowHorse.png"
};

const static SDL_Color colors[] ={
    {0xFF, 0x00, 0x00, 0xFF}, //red
    {0x00, 0xFF, 0x00, 0xFF}, //green
    {0x00, 0x00, 0xFF, 0xFF}, //blue
    {0xFF, 0xFF, 0x00, 0xFF}, //yellow
};

GfxBoard::GfxBoard(U32 nbHorses, U16 screensizeX, U16 screensizeY) noexcept : Board(nbHorses), m_screensizeX(screensizeX), m_screensizeY(screensizeY), m_sidePanelPositionX(screensizeY) {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
        std::cerr << "Impossible d'initialiser SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }
    atexit(SDL_Quit);

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        std::cerr << "Impossible d'initialiser SDL image: " << IMG_GetError() << std::endl;
        exit(2);
    }
    atexit(IMG_Quit);

    if (TTF_Init() < 0) {
        std::cerr << "Impossible d'initialiser SDL ttf: " << TTF_GetError() << std::endl;
        exit(3);
    }
    atexit(TTF_Quit);

    if ((m_pFont = LoadFont(gfxfontFile, 40)) == nullptr
            || (m_pLittleFont = LoadFont(gfxfontFile, 15)) == nullptr) {
        std::cerr << "Can't open ttf: " << TTF_GetError() << std::endl;
        exit(4);
    }

    m_wnd = SDL_CreateWindow("Plateau",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            screensizeX, screensizeY,
            SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (m_wnd == nullptr) {
        fprintf(stderr, "Impossible de créer la fenetre\n");
        exit(6);
    }

    m_renderer = SDL_CreateRenderer(m_wnd, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr) {
        fprintf(stderr, "Impossible de créer le gestionnaire de rendu\n");
        exit(7);
    }

    InitSidePanel();

    //Draw black screen
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);

    //permit to simulate dimensions
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); // better rendering of dimension
    SDL_RenderSetLogicalSize(m_renderer, m_screensizeX, m_screensizeY);

    for (const auto& gfxHorseFile : gfxHorseFiles) {
        if (!LoadHorsesBitmap(gfxHorseFile)) {
            std::cerr << "Impossible de charger l'image: " << gfxHorseFile << "  raison: %s" << SDL_GetError() << std::endl;
            exit(10);
        }
    }
    if ((m_normalCaseImage = LoadTexture(gfxNormalCaseFile)) == nullptr) {
        std::cerr << "Impossible de charger l'image: " << gfxNormalCaseFile << "  raison: %s" << SDL_GetError() << std::endl;
        exit(20);
    }
    BuildStartCaseBitmap();
    BuildLadderCaseBitmap();
}

TTF_Font * GfxBoard::LoadFont(const std::string& file, int size) noexcept {
    TTF_Font* pOutputFont = TTF_OpenFont(file.c_str(), size);
    if ((pOutputFont == nullptr)
            && (file.length() > STR_PARENT_DIRECTORY.length())) {
        pOutputFont = TTF_OpenFont(file.substr(STR_PARENT_DIRECTORY.length()).c_str(), size);
    }
    return pOutputFont;
}

GfxBoard::~GfxBoard() noexcept {
    for (const auto& horseImage : m_horseImages) {
        SDL_DestroyTexture(horseImage);
    }
    m_horseImages.clear();
    for (const auto& startCaseImage : m_startCaseImages) {
        SDL_DestroyTexture(startCaseImage);
    }
    m_startCaseImages.clear();
    for (const auto& playerladderImages : m_playerladderImages) {
        for (auto& ladderImage : playerladderImages) {
            SDL_DestroyTexture(ladderImage);
        }
    }

    //Destroy window
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_wnd);
    m_wnd = nullptr;
    m_renderer = nullptr;

    m_playerladderImages.clear();
    TTF_CloseFont(m_pFont);
    TTF_CloseFont(m_pLittleFont);
}

SDL_Texture * GfxBoard::LoadTexture(const std::string& file) noexcept {
    /* Charger une image BMP dans une surface*/
    //SDL_Surface *image = SDL_LoadBMP(file);//Can only load bmp files!
    SDL_Surface *image = IMG_Load(file.c_str()); //Can load many type of files!
    SDL_Texture *texture = nullptr;
    if ((image == nullptr)
            && (file.length() > STR_PARENT_DIRECTORY.length())) {
        image = IMG_Load(file.substr(STR_PARENT_DIRECTORY.length()).c_str());
        if (image == nullptr) {
            std::cerr << "Impossible de charger " << file.c_str() << ":" << SDL_GetError() << std::endl;
        } else {
            texture = SDL_CreateTextureFromSurface(m_renderer, image);
            SDL_FreeSurface(image);
        }
    }
    return texture;
}

int GfxBoard::getTextureW(SDL_Texture *pTexture) noexcept {
    int w, h;
    if (pTexture == nullptr || SDL_QueryTexture(pTexture, nullptr, nullptr, &w, &h) != 0) {
        w = 0;
        h = 0;
    }
    return w;
}

int GfxBoard::getTextureH(SDL_Texture *pTexture) noexcept {
    int w, h;
    if (pTexture == nullptr || SDL_QueryTexture(pTexture, nullptr, nullptr, &w, &h) != 0) {
        w = 0;
        h = 0;
    }
    return h;
}

bool GfxBoard::LoadHorsesBitmap(const std::string& file) noexcept {
    SDL_Texture* texture = LoadTexture(file);
    if (texture != nullptr) {
        m_horseImages.push_back(texture);
    }
    return (texture != nullptr);
}

bool GfxBoard::ReadBoardTopologyFromTable(unsigned int maxLength) noexcept {
    return Board::ReadBoardTopologyFromTable(maxLength)
            && ComputeGfxCoordinate();
}

bool GfxBoard::ComputeGfxCoordinate() noexcept {
    //compute the size of a grid case
    m_gridCaseSizeX = static_cast<U16> (m_screensizeY / getMaxX());
    m_gridCaseSizeY = static_cast<U16> (m_screensizeY / getMaxY());
    return true;
}

void GfxBoard::ShowBMP(SDL_Texture* texture, int x, int y) noexcept {
    SDL_Rect dest;
    /* Copie à l'écran.
        La surface ne doit pas être bloquée maintenant
     */
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h); //TODO test if w and h are valid!

    dest.x = static_cast<S16> (x);
    dest.y = static_cast<S16> (y);
    dest.w = static_cast<U16> (w);
    dest.h = static_cast<U16> (h);
    SDL_RenderCopy(m_renderer, texture, nullptr /* take all the texture*/, &dest);
}

bool GfxBoard::ConvertLogicalToGfxCoordinate(LogicalLocalization &logicLoc, S32 &x, S32 &y, U16 &sizeX, U16 &sizeY) noexcept {
    x = static_cast<U16> (logicLoc.getX() * m_gridCaseSizeX);
    y = static_cast<U16> (logicLoc.getY() * m_gridCaseSizeY);
    sizeX = m_gridCaseSizeX;
    sizeY = m_gridCaseSizeY;
    return true;
}

bool GfxBoard::ConvertGfxToLogicalCoordinate(S32 x, S32 y, LogicalLocalization &logicLoc) noexcept {
    if (x >= 0 && x < m_screensizeY //only the left square part of the screen is using LogicalLocalization!
            && y >= 0 && y < m_screensizeY) {
        logicLoc.setLocalization(x / m_gridCaseSizeX, y / m_gridCaseSizeY);
        return true;
    } else {
        return false;
    }
}

bool GfxBoard::displayBoard() noexcept {
    tpCaseList caseList;
    getpCasesList(caseList);
    return displayBoard(caseList);
}

bool GfxBoard::displayBoard(tpCaseList & /*caseList*/) noexcept {
    int w, h;
    // =================
    // =         =human= 1/10
    // =         =nick = 1/10
    // =         = die = 1/10
    // =         =score= 5/10
    // =         =box  = 2/10
    // =================
    //compute a grid coordinate
    //Clear screen
    SDL_RenderClear(m_renderer);

    tStringList displayTab(getMaxY(), std::string(getMaxX(), ' '));
    for (const auto& caseItem : getCasesList()) {
        LogicalLocalization loc = caseItem.getLocalization();

        Horse * horse = caseItem.getHorse();
        SDL_Texture* texture = nullptr;
        S32 offsetX = 0;
        S32 offsetY = 0;

        if (caseItem.isAStartCase()) {
            //c = '_';
            for (const auto& player : this->getPlayersList()) {
                if (caseItem.isAStartCaseForPlayer(&player)) {
                    texture = m_startCaseImages[player.getPlayerNb()];
                    break;
                }
            }
            //center offset adjustment
            SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
            offsetX = (m_gridCaseSizeX - w) / 2;
            offsetY = (m_gridCaseSizeY - h) / 2;
        } else if (caseItem.isAPureLadderCase()) {
            //c = static_cast<char>('0' + caseItem.getLadderCaseValue());
            for (const auto& player : this->getPlayersList()) {
                if (caseItem.isALadderForPlayer(&player)) {
                    texture = m_playerladderImages[player.getPlayerNb()][caseItem.getLadderCaseValue() - 1];
                    break;
                }
            }
            SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
            offsetX = (m_gridCaseSizeX - w) / 2;
            offsetY = (m_gridCaseSizeY - h) / 2;
        } else if (caseItem.isAFinishCase()) {
            //c = '$';
            texture = nullptr;
        } else if (caseItem.isANormalCase()) {
            //c = '.';
            texture = m_normalCaseImage;
            //center offset adjustment
            SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
            offsetX = (m_gridCaseSizeX - w) / 2;
            offsetY = (m_gridCaseSizeY - h);
        } else {
            //c = '?';
            texture = nullptr;
        }
        ////////////////////
        S32 x = 0, y = 0;
        U16 sizeX = 0, sizeY = 0;
        if (texture != nullptr) {
            if (ConvertLogicalToGfxCoordinate(loc, x, y, sizeX, sizeY)) {
                x += offsetX;
                y += offsetY;
                ShowBMP(texture, x, y);
            }
        }

        ////////////////////
        if (horse != nullptr) {
            assert(horse->getPlayer() != nullptr);
            assert(Case::isValidCaseId(horse->getCase()));
            //c = static_cast<char>('a' + horse->getPlayer()->getPlayerNb());
            texture = m_horseImages[horse->getPlayer()->getPlayerNb()];

            ////////////////////
            S32 x = 0, y = 0;
            U16 sizeX = 0, sizeY = 0;
            if (texture != nullptr) {
                if (ConvertLogicalToGfxCoordinate(loc, x, y, sizeX, sizeY)) {
                    //center offset adjustment
                    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
                    offsetX = (m_gridCaseSizeX - w) / 2;
                    offsetY = (m_gridCaseSizeY - h) / 2;
                    x += offsetX;
                    y += offsetY;
                    ShowBMP(texture, x, y);
                }
            }
            ////////////////////
        }
        //displayTab[loc.getY()][loc.getX()]=c;
    }
    return true;
}

//Creation of the texture

void GfxBoard::InitSidePanel() noexcept {
    m_HumanPanelTexture = SDL_CreateTexture(m_renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            m_sidePanelPositionX, m_screensizeY / 10);
    m_NickPanelTexture = SDL_CreateTexture(m_renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            m_sidePanelPositionX, m_screensizeY / 10);
    m_DiePanelTexture = SDL_CreateTexture(m_renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            m_sidePanelPositionX, 3 * m_screensizeY / 10);
    m_ScorePanelTexture = SDL_CreateTexture(m_renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            m_sidePanelPositionX, 3 * m_screensizeY / 10);
    m_BoxPanelTexture = SDL_CreateTexture(m_renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            m_sidePanelPositionX, 2 * m_screensizeY / 10);
}

void GfxBoard::displayLeftPannel(std::string human, std::string nickname, int die, std::vector<int> scores, int horsesInTheBox, int iColor) noexcept {
    // =================
    // =         =human= 1/10
    // =         =nick = 1/10
    // =         = die = 3/10
    // =         =score= 3/10
    // =         =box  = 2/10
    // =================

    blitHumanLabel(human, iColor);
    blitNickLabel(nickname, iColor);
    blitDieLabel(die, iColor);
    blitScoreLabel(scores);
    blitBoxLabel(horsesInTheBox, iColor);
    SDL_RenderPresent(m_renderer);
}

void GfxBoard::blitHumanLabel(std::string &human, int iColor) noexcept {
    int LeftPannelPositionX = m_screensizeY;
    int HumanPositionY = 0;

    SDL_Surface *text_surface = TTF_RenderText_Solid(m_pLittleFont, human.c_str(), colors[iColor]);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
    ShowBMP(text_texture, LeftPannelPositionX, HumanPositionY);
    SDL_FreeSurface(text_surface);
}

void GfxBoard::blitNickLabel(std::string &nickname, int iColor) noexcept {
    int LeftPannelPositionX = m_screensizeY;
    int nickPositionY = 1 * m_screensizeY / 10;

    std::string nicknameLabel("Nom du joueur: ");
    nicknameLabel += nickname;

    SDL_Surface *text_surface = TTF_RenderText_Solid(m_pLittleFont, nicknameLabel.c_str(), colors[iColor]);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
    ShowBMP(text_texture, LeftPannelPositionX, nickPositionY);
    SDL_FreeSurface(text_surface);
}

void GfxBoard::blitDieLabel(int die, int iColor) noexcept {
    int LeftPannelPositionX = m_screensizeY;
    int diePositionY = 2 * m_screensizeY / 10;
    std::string dieStr;
    std::stringstream ss;
    ss << die;
    dieStr = ss.str();

    SDL_Surface *text_surface = TTF_RenderText_Solid(m_pFont, dieStr.c_str(), colors[iColor]);
    int offsetX = (m_screensizeX - LeftPannelPositionX) / 2;
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
    ShowBMP(text_texture, LeftPannelPositionX + offsetX, diePositionY);
    SDL_FreeSurface(text_surface);
}

void GfxBoard::blitScoreLabel(std::vector<int> scores) noexcept {
    int LeftPannelPositionX = m_screensizeY;
    int scorePositionY = 3 * m_screensizeY / 10;
    int iColor = 0;
    std::string scoreStr;
    std::stringstream ss;

    SDL_Color whiteColor = {255, 255, 255, 255};
    SDL_Surface *text_surface = TTF_RenderText_Solid(m_pFont, "Score:", whiteColor);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
    ShowBMP(text_texture, LeftPannelPositionX, scorePositionY);
    scorePositionY += text_surface->h * 8 / 10;
    SDL_FreeSurface(text_surface);

    for (const auto& score : scores) {
        ss << score;
        scoreStr = ss.str();
        text_surface = TTF_RenderText_Solid(m_pFont, scoreStr.c_str(), colors[iColor]);
        text_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
        ShowBMP(text_texture, LeftPannelPositionX, scorePositionY);
        scorePositionY += text_surface->h * 8 / 10;
        SDL_FreeSurface(text_surface);
        ss.str("");
        iColor++;
    }
}

void GfxBoard::blitBoxLabel(int horsesInTheBox, int iColor) noexcept {
    int LeftPannelPositionX = m_screensizeY;
    int boxPositionY = 8 * m_screensizeY / 10;

    if (horsesInTheBox > MAX_DISPLAYED_HORSE) {
        ShowBMP(m_horseImages[iColor], LeftPannelPositionX, boxPositionY);

        std::stringstream ss;
        ss << std::setw(3) << std::setfill('0') << horsesInTheBox;
        std::string str = ss.str();
        SDL_Surface *text_surface = TTF_RenderText_Solid(m_pFont, str.c_str(), colors[iColor]);
        SDL_Texture *text_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
        ShowBMP(text_texture, LeftPannelPositionX + getTextureW(m_horseImages[iColor]), boxPositionY);
        SDL_FreeSurface(text_surface);
    } else {
        SDL_Texture * pHorseTexture = m_horseImages[iColor];
        int horseSize_w = getTextureW(pHorseTexture);
        for (int i = 0; i < horsesInTheBox; i++) {
            ShowBMP(pHorseTexture, LeftPannelPositionX, boxPositionY);
            LeftPannelPositionX += horseSize_w + 5;
        }
    }
}

bool GfxBoard::BuildStartCaseBitmap() noexcept {
    for (const auto& color : colors) {
        SDL_Surface *text_surface = TTF_RenderText_Solid(m_pFont, "D", color);
        if (text_surface == nullptr) {
            std::cerr << "Can't render glif from ttf: " << TTF_GetError() << std::endl;
            return false;
        } else {
            SDL_Texture *text_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
            m_startCaseImages.push_back(text_texture);
            SDL_FreeSurface(text_surface);
        }
    }
    return true;
}

bool GfxBoard::BuildLadderCaseBitmap() noexcept {
    m_playerladderImages.reserve(sizeof (colors));
    for (unsigned int iColor = 0; iColor<sizeof (colors); iColor++) {
        m_playerladderImages.push_back(tpTextureList());
        m_playerladderImages[iColor].reserve(MAX_LADDER_DIGIT);
        for (char iDigit = 0; iDigit < MAX_LADDER_DIGIT; iDigit++) {
            char digitChar[] = {static_cast<char> ('1' + iDigit), '\0'};
            SDL_Surface *text_surface = TTF_RenderText_Solid(m_pFont, digitChar, colors[iColor]);

            if (text_surface == nullptr) {
                std::cerr << "Can't render glif from ttf: " << TTF_GetError() << std::endl;
                return false;
            } else {
                SDL_Texture *text_texture = SDL_CreateTextureFromSurface(m_renderer, text_surface);
                m_playerladderImages[iColor].push_back(text_texture);
                SDL_FreeSurface(text_surface);
            }
        }
    }
    return true;
}

bool GfxBoard::GetChoiceFromEvents(eUserEventType &userEvent, int &nbHorse, int &nbPlayer) noexcept {
    SDL_Event test_event;
    nbHorse = -1;
    nbPlayer = -1;
    userEvent = None;
    while (SDL_WaitEvent(&test_event)) {
        switch (test_event.type) {
            case SDL_MOUSEBUTTONUP:
            {
                LogicalLocalization logicLoc;
                if (ConvertGfxToLogicalCoordinate(test_event.button.x, test_event.button.y, logicLoc)) {
                    //need to compute LogicalLocalization from position, then determine Horse number!
                    tCaseId pointedCase = GetCaseFromLocalization(logicLoc);
                    if (Case::isValidCaseId(pointedCase)) {
                        Horse * pointedHorse = getCase(pointedCase).getHorse();
                        if (pointedHorse != nullptr) {
                            nbPlayer = pointedHorse->getPlayer()->getPlayerNb();
                            nbHorse = pointedHorse->getHorseNumber();
                            userEvent = ClickedCase;
                        }
                    }
                } else {
                    userEvent = ClickedPanel;
                }
            }
                break;
            case SDL_KEYUP:
                switch (test_event.key.keysym.sym) {
                    case SDLK_0:
                    case SDLK_KP_0:
                        userEvent = HorseKey;
                        nbHorse = 0;
                        break;
                    case SDLK_1:
                    case SDLK_KP_1:
                        userEvent = HorseKey;
                        nbHorse = 1;
                        break;
                    case SDLK_2:
                    case SDLK_KP_2:
                        userEvent = HorseKey;
                        nbHorse = 2;
                        break;
                    case SDLK_3:
                    case SDLK_KP_3:
                        userEvent = HorseKey;
                        nbHorse = 3;
                        break;
                    case SDLK_4:
                    case SDLK_KP_4:
                        userEvent = HorseKey;
                        nbHorse = 4;
                        break;
                    case SDLK_5:
                    case SDLK_KP_5:
                        userEvent = HorseKey;
                        nbHorse = 5;
                        break;
                    case SDLK_6:
                    case SDLK_KP_6:
                        userEvent = HorseKey;
                        nbHorse = 6;
                        break;
                    case SDLK_7:
                    case SDLK_KP_7:
                        userEvent = HorseKey;
                        nbHorse = 7;
                        break;
                    case SDLK_8:
                    case SDLK_KP_8:
                        userEvent = HorseKey;
                        nbHorse = 8;
                        break;
                    case SDLK_9:
                    case SDLK_KP_9:
                        userEvent = HorseKey;
                        nbHorse = 9;
                        break;
                    case SDLK_ESCAPE:
                        userEvent = CloseEvent;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        if (userEvent != None) {
            break;
        }
    }
    return userEvent != None;
}