#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstring>
#include <cstdlib>

#include "FileUtility.h"
#include "GfxBoard.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <SDL_endian.h> /* Used for the endian-dependent 24 bpp mode */

#define MAX_DISPLAYED_HORSE 8

static SDL_Color colors[]=
{
	{0xFF,0x00,0x00}, //red
	{0x00,0xFF,0x00}, //green
	{0x00,0x00,0xFF}, //blue
	{0xFF,0xFF,0x00}, //yellow
};

GfxBoard::GfxBoard(U32 nbHorses, U16 screensizeX, U16 screensizeY) : Board(nbHorses), m_screensizeX(screensizeX), m_screensizeY(screensizeY)
{
	if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 )
	{
		std::cerr << "Impossible d'initialiser SDL: " << SDL_GetError() << std::endl;
		exit(1);
	}
	atexit(SDL_Quit);

	if(IMG_Init(IMG_INIT_PNG) < 0)
	{
		std::cerr << "Impossible d'initialiser SDL image: " << IMG_GetError() << std::endl;
		exit(2);
	}
	atexit(IMG_Quit);

	if(TTF_Init() < 0)
	{
		std::cerr << "Impossible d'initialiser SDL ttf: " << TTF_GetError() << std::endl;
		exit(3);
	}
	atexit(TTF_Quit);

	if((m_pFont = LoadFont(gfxfontFile, 40)) == NULL
		|| (m_pLittleFont = LoadFont(gfxfontFile, 15)) == NULL)
	{
		std::cerr << "Can't open ttf: " << TTF_GetError() << std::endl;
		exit(4);
	}

	m_screen = SDL_SetVideoMode(screensizeX, screensizeY, 16, SDL_SWSURFACE);
	if ( m_screen == NULL ) {
		fprintf(stderr, "Impossible de passer en %dx%d en 16 bpp: %s\n", screensizeX, screensizeY,  SDL_GetError());
		exit(6);
	}

	for(unsigned int i = 0; i<sizeof(gfxHorseFiles)/sizeof(char *); i++)
	{
		if(!LoadHorsesBitmap(gfxHorseFiles[i]))
		{
			std::cerr << "Impossible de charger l'image: " << gfxHorseFiles[i] << "  raison: %s" << SDL_GetError() << std::endl;
			exit(7);
		}
	}
	if((m_normalCaseImage = LoadBitmap(gfxNormalCaseFile)) == NULL)
	{
		std::cerr << "Impossible de charger l'image: " << gfxNormalCaseFile << "  raison: %s" << SDL_GetError() << std::endl;
		exit(8);
	}
	BuildStartCaseBitmap();
	BuildLadderCaseBitmap();
}

TTF_Font * GfxBoard::LoadFont(const char * file, int size)
{
	TTF_Font* pOutputFont = TTF_OpenFont(file,size);
	if((pOutputFont == NULL)
		&& (strnlen(file,255) + 1 > sizeof(STR_PARENT_DIRECTORY)))
	{
		pOutputFont = TTF_OpenFont(file + sizeof(STR_PARENT_DIRECTORY) - 1,size);
	}
	return pOutputFont;
}

GfxBoard::~GfxBoard()
{
	for(tpSurfaceList::iterator it=m_horseImages.begin(); it != m_horseImages.end(); it++)
	{
		SDL_FreeSurface(*it);
	}
	m_horseImages.clear();
	for(tpSurfaceList::iterator it=m_startCaseImages.begin(); it != m_startCaseImages.end(); it++)
	{
		SDL_FreeSurface(*it);
	}
	m_startCaseImages.clear();
	for(tpSurfaceListList::iterator it1=m_playerladderImages.begin(); it1 != m_playerladderImages.end(); it1++)
	{
		for(tpSurfaceList::iterator it2=it1->begin(); it2 != it1->end(); it2++)
		{
			SDL_FreeSurface(*it2);
		}
	}
	m_playerladderImages.clear();
	TTF_CloseFont(m_pFont);
	TTF_CloseFont(m_pLittleFont);
}


SDL_Surface * GfxBoard::LoadBitmap(char *file)
{
	/* Charger une image BMP dans une surface*/
	//SDL_Surface *image = SDL_LoadBMP(file);//Can only load bmp files!
	SDL_Surface *image = IMG_Load(file);//Can load many type of files!
	if((image == NULL)
		&& (strnlen(file,255) + 1 > sizeof(STR_PARENT_DIRECTORY)))
	{
		image = IMG_Load(file + sizeof(STR_PARENT_DIRECTORY) - 1);
		if(image == NULL)
		{
			fprintf(stderr, "Impossible de charger %s: %s\n", file, SDL_GetError());
		}
	}
	return image;
}


bool GfxBoard::LoadHorsesBitmap(char *file)
{
	SDL_Surface* image = LoadBitmap(file);
	if(image!=NULL)
	{
		m_horseImages.push_back(image);
	}
	return (image!=NULL);
}

bool GfxBoard::ReadBoardTopologyFromTable(unsigned int maxLength)
{
	return Board::ReadBoardTopologyFromTable(maxLength)
		&& ComputeGfxCoordinate();
}

bool GfxBoard::ComputeGfxCoordinate()
{
	//compute the size of a grid case
	m_gridCaseSizeX = static_cast<U16>(m_screensizeY/getMaxX());
	m_gridCaseSizeY = static_cast<U16>(m_screensizeY/getMaxY());
	return true;
}

void GfxBoard::DrawPixel(int x, int y, U8 R, U8 G, U8 B)
{
    U32 color = SDL_MapRGB(m_screen->format, R, G, B);

    if ( SDL_MUSTLOCK(m_screen) ) {
        if ( SDL_LockSurface(m_screen) < 0 ) {
            return;
        }
    }
    switch (m_screen->format->BytesPerPixel) {
        case 1: { /*On gère le mode 8bpp */
            U8 *bufp;

            bufp = static_cast<U8 *>(m_screen->pixels) + y*m_screen->pitch + x;
            *bufp = static_cast<U8>(color);
        }
        break;

        case 2: { /* Certainement 15 ou 16 bpp */
            U16 *bufp;

            bufp = static_cast<Uint16 *>(m_screen->pixels) + y*m_screen->pitch/2 + x;
            *bufp = static_cast<Uint16>(color);
        }
        break;

        case 3: { /* 24 bpp lent et généralement pas utilisé */
            U8 *bufp;

            bufp = static_cast<Uint8 *>(m_screen->pixels) + y*m_screen->pitch + x * 3;
            if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
                bufp[0] = static_cast<Uint8>(color);
                bufp[1] = static_cast<Uint8>(color >> 8);
                bufp[2] = static_cast<Uint8>(color >> 16);
            } else {
                bufp[2] = static_cast<Uint8>(color);
                bufp[1] = static_cast<Uint8>(color >> 8);
                bufp[0] = static_cast<Uint8>(color >> 16);
            }
        }
        break;

        case 4: { /* Probablement 32 bpp alors */
            U32 *bufp;

            bufp = static_cast<U32*>(m_screen->pixels) + y*m_screen->pitch/4 + x;
            *bufp = color;
        }
        break;
    }
    if ( SDL_MUSTLOCK(m_screen) ) {
        SDL_UnlockSurface(m_screen);
    }
    SDL_UpdateRect(m_screen, x, y, 1, 1);
}

void GfxBoard::ShowBMP(SDL_Surface* image,int x, int y)
{
	SDL_Rect dest;
    /* Copie à l'écran.
	La surface ne doit pas être bloquée maintenant
     */
    dest.x = static_cast<S16>(x);
    dest.y = static_cast<S16>(y);
    dest.w = static_cast<U16>(image->w);
    dest.h = static_cast<U16>(image->h);
    SDL_BlitSurface(image, NULL, m_screen, &dest);

    /*Mise à jour de la portion qui a changé */
    //SDL_UpdateRects(m_screen, 1, &dest);
}

bool GfxBoard::ConvertLogicalToGfxCoordinate(LogicalLocalization &logicLoc, S32 &x, S32 &y, U16 &sizeX, U16 &sizeY)
{
	x = static_cast<U16>(logicLoc.getX() * m_gridCaseSizeX);
	y = static_cast<U16>(logicLoc.getY() * m_gridCaseSizeY);
	sizeX = m_gridCaseSizeX;
	sizeY = m_gridCaseSizeY;
	return true;
}

bool GfxBoard::ConvertGfxToLogicalCoordinate(S32 x, S32 y, LogicalLocalization &logicLoc)
{
	if( x>=0  && x < m_screensizeY //only the left square part of the screen is using LogicalLocalization!
		&&  y>=0  && y < m_screensizeY)
	{
		logicLoc.setLocalization(x/m_gridCaseSizeX,y/m_gridCaseSizeY);
		return true;
	}
	else
	{
		return false;
	}
}

bool GfxBoard::displayBoard()
{
	tpCaseList caseList;
	getpCasesList(caseList);
	return displayBoard(caseList);
}

bool GfxBoard::displayBoard(tpCaseList &caseList)
{
	// =================
	// =         =human= 1/10
	// =         =nick = 1/10
	// =         = die = 1/10
	// =         =score= 5/10
	// =         =box  = 2/10
	// =================
	//compute a grid coordinate
	//display background picture
	bool bPartialUpdate = (caseList.size() <= 2) ;
	if(!bPartialUpdate)
	{//if there is too much case to update, we delete all the zone
		FillRect(0,0,m_screensizeY,m_screensizeY,0,0,0,255);//fill zone with a black rectangle
	}
	//display cases (normal case and ladder) and horses
	/*for(int x=0; x < m_screensizeX; x+=m_gridCaseSizeY)
	{
		for(int y=0; y < m_screensizeY; y+=m_gridCaseSizeY)
		{
			U32 *bufp = static_cast<U32*>(m_screen->pixels) + y*m_screen->pitch/4 + x;
			*bufp = SDL_MapRGB(m_screen->format, 255, 255, 255);
		}
	}*/

	tStringList displayTab(getMaxY(),std::string(getMaxX(),' '));
	for(tpCaseList::iterator itCase=caseList.begin(); itCase != caseList.end(); itCase++)
	{
		tCaseId pItCase = *itCase;
		LogicalLocalization loc = getCase(pItCase).getLocalization();
		if(bPartialUpdate)
		{
			FillRect(loc.getX()*m_gridCaseSizeX,loc.getY()*m_gridCaseSizeY
				,m_gridCaseSizeX,m_gridCaseSizeY,0,0,0,255);//fill case with a black rectangle
		}

		Horse * horse = getCase(pItCase).getHorse();
		SDL_Surface* image = NULL;
		S32 offsetX = 0;
		S32 offsetY = 0;

		if(getCase(pItCase).isAStartCase())
		{
			//c = '_';
			for(tPlayerList::iterator itPlayer=this->getPlayersList().begin(); itPlayer!=this->getPlayersList().end(); itPlayer++)
			{
				if(getCase(pItCase).isAStartCaseForPlayer(&*itPlayer))
				{
					image = m_startCaseImages[itPlayer->getPlayerNb()];
					break;
				}
			}
			//center offset adjustment
			offsetX = (m_gridCaseSizeX - image->w)/2;
			offsetY = (m_gridCaseSizeY - image->h)/2;
		}
		else if(getCase(pItCase).isAPureLadderCase())
		{
			//c = static_cast<char>('0' + itCase->getLadderCaseValue());
			for(tPlayerList::iterator itPlayer=this->getPlayersList().begin(); itPlayer!=this->getPlayersList().end(); itPlayer++)
			{
				if(getCase(pItCase).isALadderForPlayer(&*itPlayer))
				{
					image = m_playerladderImages[itPlayer->getPlayerNb()][getCase(pItCase).getLadderCaseValue()-1];
					break;
				}
			}
			offsetX = (m_gridCaseSizeX - image->w)/2;
			offsetY = (m_gridCaseSizeY - image->h)/2;
		}
		else if(getCase(pItCase).isAFinishCase())
		{
			//c = '$';
			image = NULL;
		}
		else if(getCase(pItCase).isANormalCase())
		{
			//c = '.';
			image = m_normalCaseImage;
			//center offset adjustment
			offsetX = (m_gridCaseSizeX - image->w)/2;
			offsetY = (m_gridCaseSizeY - image->h);
		}
		else
		{
			//c = '?';
			image = NULL;
		}
		////////////////////
		S32 x = 0, y = 0;
		U16 sizeX = 0, sizeY = 0;
		if(image!=NULL)
		{
			if(ConvertLogicalToGfxCoordinate(loc,x,y,sizeX,sizeY))
			{
				x+=offsetX;
				y+=offsetY;
				ShowBMP(image,x,y);
			}
		}

		////////////////////
		if(horse!=NULL)
		{
			assert(horse->getPlayer()!=NULL);
			assert(Case::isValidCaseId(horse->getCase()));
			//c = static_cast<char>('a' + horse->getPlayer()->getPlayerNb());
			image = m_horseImages[horse->getPlayer()->getPlayerNb()];

			////////////////////
			S32 x = 0, y = 0;
			U16 sizeX = 0, sizeY = 0;
			if(image!=NULL)
			{
				if(ConvertLogicalToGfxCoordinate(loc,x,y,sizeX,sizeY))
				{
					//center offset adjustment
					offsetX = (m_gridCaseSizeX - image->w)/2;
					offsetY = (m_gridCaseSizeY - image->h)/2;
					x+=offsetX;
					y+=offsetY;
					ShowBMP(image,x,y);
				}
			}
			////////////////////
		}
		//displayTab[loc.getY()][loc.getX()]=c;
	}
	//displayString(displayTab);
	SDL_Rect dest;
    /* Copie à l'écran.
	La surface ne doit pas être bloquée maintenant
     */
    dest.x = 0;
    dest.y = 0;
	dest.w = m_screensizeY;
	dest.h = m_screensizeY;
	SDL_UpdateRects(m_screen, 1, &dest);
	return true;
}

void GfxBoard::displayLeftPannel(std::string human, std::string nickname, int die, std::vector<int> scores, int horsesInTheBox, int iColor)
{
	// =================
	// =         =human= 1/10
	// =         =nick = 1/10
	// =         = die = 3/10
	// =         =score= 3/10
	// =         =box  = 2/10
	// =================
	//display background picture
	SDL_Rect dest;
    /* Copie à l'écran.
	La surface ne doit pas être bloquée maintenant
     */
    dest.x = m_screensizeY;
    dest.y = 0;
	dest.w = m_screensizeX-m_screensizeY;
	dest.h = m_screensizeY;

	FillRect(m_screensizeY,0,m_screensizeX-m_screensizeY,m_screensizeY,0,0,0,255);

	blitHumanLabel(human, iColor);
	blitNickLabel(nickname, iColor);
	blitDieLabel(die, iColor);
	blitScoreLabel(scores);
	blitBoxLabel(horsesInTheBox, iColor);

	SDL_UpdateRects(m_screen, 1, &dest);
}

void GfxBoard::blitHumanLabel(std::string &human, int iColor)
{
	int LeftPannelPositionX = m_screensizeY;
	int HumanPositionY = 0;

	SDL_Surface *text_surface = TTF_RenderText_Solid(m_pLittleFont,human.c_str(),colors[iColor]);
	ShowBMP(text_surface, LeftPannelPositionX, HumanPositionY);
	SDL_FreeSurface(text_surface);
}

void GfxBoard::blitNickLabel(std::string &nickname, int iColor)
{
	int LeftPannelPositionX = m_screensizeY;
	int nickPositionY = 1 * m_screensizeY/10;

	std::string nicknameLabel("Nom du joueur: ");
	nicknameLabel += nickname;

	SDL_Surface *text_surface = TTF_RenderText_Solid(m_pLittleFont,nicknameLabel.c_str(),colors[iColor]);
	ShowBMP(text_surface, LeftPannelPositionX, nickPositionY);
	SDL_FreeSurface(text_surface);
}

void GfxBoard::blitDieLabel(int die, int iColor)
{
	int LeftPannelPositionX = m_screensizeY;
	int diePositionY  = 2 * m_screensizeY/10;
	std::string dieStr;
	std::stringstream ss;
	ss << die;
	dieStr = ss.str();

	SDL_Surface *text_surface = TTF_RenderText_Solid(m_pFont,dieStr.c_str(),colors[iColor]);
	int offsetX = (m_screensizeX - LeftPannelPositionX) / 2;
	ShowBMP(text_surface, LeftPannelPositionX + offsetX, diePositionY);
	SDL_FreeSurface(text_surface);
}

void GfxBoard::blitScoreLabel(std::vector<int> scores)
{
	int LeftPannelPositionX = m_screensizeY;
	int scorePositionY  = 3 * m_screensizeY/10;
	int iColor = 0;
	std::string scoreStr;
	std::stringstream ss;
	
	SDL_Color whiteColor = {255,255,255};
	SDL_Surface *text_surface = TTF_RenderText_Solid(m_pFont,"Score:",whiteColor);
	ShowBMP(text_surface, LeftPannelPositionX, scorePositionY);
	scorePositionY+= text_surface->h * 8 /10;
	SDL_FreeSurface(text_surface);

	for(std::vector<int>::iterator it= scores.begin(); it != scores.end(); it++, iColor++)
	{
		ss << *it;
		scoreStr = ss.str();
		SDL_Surface *text_surface = TTF_RenderText_Solid(m_pFont,scoreStr.c_str(),colors[iColor]);
		ShowBMP(text_surface, LeftPannelPositionX, scorePositionY);
		scorePositionY+= text_surface->h * 8 /10;
		SDL_FreeSurface(text_surface);
		ss.str("");
	}
}

void GfxBoard::blitBoxLabel(int horsesInTheBox, int iColor)
{
	int LeftPannelPositionX = m_screensizeY;
	int boxPositionY  = 8 * m_screensizeY/10;

	if(horsesInTheBox>MAX_DISPLAYED_HORSE)
	{
		ShowBMP(m_horseImages[iColor], LeftPannelPositionX, boxPositionY);

		std::stringstream ss;
		ss << std::setw(3) << std::setfill('0') <<horsesInTheBox;
		std::string str = ss.str();
		SDL_Surface *text_surface = TTF_RenderText_Solid(m_pFont,str.c_str(),colors[iColor]);
		ShowBMP(text_surface, LeftPannelPositionX + m_horseImages[iColor]->w, boxPositionY);
		SDL_FreeSurface(text_surface);
	}
	else
	{
		SDL_Surface * horseSurface = m_horseImages[iColor];
		for(int i = 0; i < horsesInTheBox; i++)
		{
			ShowBMP(horseSurface, LeftPannelPositionX, boxPositionY);
			LeftPannelPositionX += horseSurface->w + 5;
		}
	}
}

void GfxBoard::FillRect(U16 x, U16 y, U16 sizeX, U16 sizeY, U8 R, U8 G, U8 B, U8 A)
{
	U32 color = SDL_MapRGBA(m_screen->format, R, G, B, A);
	SDL_Rect dest;
    /* Copie à l'écran.
	La surface ne doit pas être bloquée maintenant
     */
    dest.x = static_cast<S16>(x);
    dest.y = static_cast<S16>(y);
    dest.w = static_cast<U16>(sizeX);
    dest.h = static_cast<U16>(sizeY);
	SDL_FillRect(m_screen, &dest, color);

    /*Mise à jour de la portion qui a changé */
    //SDL_UpdateRects(m_screen, 1, &dest);
}

bool GfxBoard::BuildStartCaseBitmap()
{
	for(int iColor=0; iColor<sizeof(colors); iColor++)
	{
		SDL_Surface *text_surface = TTF_RenderText_Solid(m_pFont,"D",colors[iColor]);
		if(text_surface==NULL)
		{
			std::cerr << "Can't render glif from ttf: " << TTF_GetError() << std::endl;
			return false;
		}
		else
		{
			m_startCaseImages.push_back(text_surface);
		}
	}
	return true;
}

bool GfxBoard::BuildLadderCaseBitmap()
{
	m_playerladderImages.reserve(sizeof(colors));
	for(int iColor=0; iColor<sizeof(colors); iColor++)
	{
		m_playerladderImages.push_back(tpSurfaceList());
		m_playerladderImages[iColor].reserve(MAX_LADDER_DIGIT);
		for(char iDigit = 0; iDigit<MAX_LADDER_DIGIT; iDigit++)
		{
			char digitChar[] = {'1' + iDigit, '\0' };
			SDL_Surface *text_surface = TTF_RenderText_Solid(m_pFont,digitChar,colors[iColor]);

			if(text_surface==NULL)
			{
				std::cerr << "Can't render glif from ttf: " << TTF_GetError() << std::endl;
				return false;
			}
			else
			{
				m_playerladderImages[iColor].push_back(text_surface);
			}
		}
	}
	return true;
}

bool GfxBoard::GetChoiceFromEvents(eUserEventType &userEvent, int &nbHorse, int &nbPlayer)
{
	SDL_Event test_event;
	nbHorse = -1;
	nbPlayer = -1;
	userEvent = None;
	while(SDL_WaitEvent(&test_event))
	{
		switch(test_event.type)
		{
		case SDL_MOUSEBUTTONUP:
			{
				LogicalLocalization logicLoc;
				if(ConvertGfxToLogicalCoordinate(test_event.button.x, test_event.button.y, logicLoc))
				{
					//need to compute LogicalLocalization from position, then determine Horse number!
					tCaseId pointedCase = GetCaseFromLocalization(logicLoc);
					if(Case::isValidCaseId(pointedCase))
					{
						Horse * pointedHorse = getCase(pointedCase).getHorse();
						if(pointedHorse!=NULL)
						{
							nbPlayer = pointedHorse->getPlayer()->getPlayerNb();
							nbHorse = pointedHorse->getHorseNumber();
							userEvent = ClickedCase;
						}
					}
				}
				else
				{
					userEvent = ClickedPanel;
				}
			}
		break;
		case SDL_KEYUP:
			switch(test_event.key.keysym.sym)
			{
				case SDLK_0:
				case SDLK_KP0:
					userEvent = HorseKey; nbHorse=0;
				break;
				case SDLK_1:
				case SDLK_KP1:
					userEvent = HorseKey; nbHorse=1;
				break;
				case SDLK_2:
				case SDLK_KP2:
					userEvent = HorseKey; nbHorse=2;
				break;
				case SDLK_3:
				case SDLK_KP3:
					userEvent = HorseKey; nbHorse=3;
				break;
				case SDLK_4:
				case SDLK_KP4:
					userEvent = HorseKey; nbHorse=4;
				break;
				case SDLK_5:
				case SDLK_KP5:
					userEvent = HorseKey; nbHorse=5;
				break;
				case SDLK_6:
				case SDLK_KP6:
					userEvent = HorseKey; nbHorse=6;
				break;
				case SDLK_7:
				case SDLK_KP7:
					userEvent = HorseKey; nbHorse=7;
				break;
				case SDLK_8:
				case SDLK_KP8:
					userEvent = HorseKey; nbHorse=8;
				break;
				case SDLK_9:
				case SDLK_KP9:
					userEvent = HorseKey; nbHorse=9;
				break;
				default:
				break;
			}
		break;
		default:
		break;
		}
		if(userEvent!=None)
		{
			break;
		}
	}
	return userEvent != None;
}