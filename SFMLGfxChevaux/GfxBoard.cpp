#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdlib>

#include "FileUtility.h"
#include "GfxBoard.h"
#include "../gfx/BlueHorse.png.h"
#include "../gfx/RedHorse.png.h"
#include "../gfx/GreenHorse.png.h"
#include "../gfx/YellowHorse.png.h"
#include "../gfx/NormalCase.png.h"
#include "../gfx/LiberationSansNarrow-Bold.ttf.h"

#include "SFML/Graphics.hpp"

typedef struct{
	const char * file;
	int size;
} t_file_size;

static const t_file_size gfxHorseFiles[]=
{
	{RedHorse_png,sizeof(RedHorse_png)},
	{GreenHorse_png,sizeof(GreenHorse_png)},
	{BlueHorse_png,sizeof(BlueHorse_png)},
	{YellowHorse_png,sizeof(YellowHorse_png)},
};

#define MAX_DISPLAYED_HORSE 8

#define BIG_FONT_SIZE         40
#define SMALL_FONT_SIZE       15

static const sf::Color *colors[]=
{
	&sf::Color::Red,
	&sf::Color::Green,
	&sf::Color::Blue,
	&sf::Color::Yellow,
};

GfxBoard::GfxBoard(U32 nbHorses, U16 screensizeX, U16 screensizeY) : Board(nbHorses), m_screensizeX(screensizeX), m_screensizeY(screensizeY)
{
	// Create the main rendering window
	m_pApp = new sf::RenderWindow(sf::VideoMode(screensizeX, screensizeY), "SFML Graphics");
	m_pApp->clear();
	//m_pApp->SetFramerateLimit(60);
	
	m_pFont = new sf::Font();
	//if(!m_pFont->LoadFromFile(gfxfontFile,30))
	if(!m_pFont->loadFromMemory(LiberationSansNarrow_Bold_ttf,sizeof(LiberationSansNarrow_Bold_ttf)))
	{
		std::cerr << "Can't open ttf: " /*<< gfxfontFile */<< std::endl;
		exit(4);
	}

	for(unsigned int i = 0; i<sizeof(gfxHorseFiles)/sizeof(gfxHorseFiles[0]); i++)
	{
		LoadHorsesBitmap(gfxHorseFiles[i].file, gfxHorseFiles[i].size);
	}
	
	sf::Texture * texture = new sf::Texture();
	//if(texture->LoadFromFile(gfxNormalCaseFile))
	if(texture->loadFromMemory(NormalCase_png,sizeof(NormalCase_png)))
	{
		m_normalCaseSprite = new sf::Sprite(*texture);
	}

	BuildStartCaseBitmap();
	BuildLadderCaseBitmap();
}

GfxBoard::~GfxBoard()
{
	for(tpSpriteList::iterator it=m_horseSprites.begin(); it != m_horseSprites.end(); it++)
	{
		//const sf::Texture * pTexture = (*it)->GetTexture();
		delete *it;
		//delete pTexture;
	}
	m_horseSprites.clear();

	delete m_startCaseText;

	for(tpTextList::iterator it=m_playerladderText.begin(); it != m_playerladderText.end(); it++)
	{
		delete *it;
	}
	m_playerladderText.clear();

	delete m_pFont;
	delete m_pApp;
}

bool GfxBoard::LoadHorsesBitmap(const char *file, std::size_t size)
{
    //TODO Check if it is a good idea to store Image pointer inside sprite!
	sf::Texture * texture = new sf::Texture();
	//m_horseImages.push_back(image);
	//if(texture->LoadFromFile(file))
	if(texture->loadFromMemory(file,size))
	{
		m_horseSprites.push_back(new sf::Sprite(*texture));
	}
	return false;
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
	AdaptRatioForSprite(m_normalCaseSprite);
	for(tpTextList::iterator it = m_playerladderText.begin(); it != m_playerladderText.end(); it++)
	{
		AdaptRatioForText(*it);
	}
	AdaptRatioForText(m_startCaseText);
	for(tpSpriteList::iterator it = m_horseSprites.begin(); it != m_horseSprites.end(); it++)
	{
		AdaptRatioForSprite(*it);
	}
	
	return true;
}

void GfxBoard::AdaptRatioForSprite(sf::Sprite *sprite)
{
	assert(sprite!=NULL);
	if(sprite!=NULL)
	{
		const sf::FloatRect rectLocal = sprite->getLocalBounds();
		float computedRatio = GfxBoard::getComputedRatio(static_cast<float>(rectLocal.width),
			static_cast<float>(rectLocal.height),m_gridCaseSizeX,m_gridCaseSizeY);
		sprite->setOrigin(rectLocal.left,rectLocal.top);
		sprite->setScale(computedRatio,computedRatio);
		//sf::FloatRect rect = text->getGlobalBounds();
		//int fake = 0;
	}
}

void GfxBoard::AdaptRatioForText(sf::Text *text)
{
	assert(text!=NULL);
	if(text!=NULL)
	{
		sf::FloatRect rectLocal = text->getGlobalBounds();
		float computedRatio = GfxBoard::getComputedRatio(rectLocal.width,rectLocal.height,m_gridCaseSizeX,m_gridCaseSizeY);
		text->setOrigin(rectLocal.left,rectLocal.top);
		text->setScale(computedRatio,computedRatio);
		//sf::FloatRect rect = text->getGlobalBounds();
		//int fake = 0;
	}
}

float GfxBoard::getComputedRatio(float currentSizeX, float currentSizeY, int wantedSizeX, int wantedSizeY)
{
	float ratioY= static_cast<float>(wantedSizeX) / currentSizeX;
	float ratioX= static_cast<float>(wantedSizeY) / currentSizeY;
	float computedRatio;
	if(ratioX<1.f)
	{
		if(ratioY<1.f)
		{
			computedRatio = (ratioX<ratioY)? ratioY : ratioX; //reduce
		}
		else
		{
			computedRatio = ratioX; //reduce
		}
	}
	else //ratioX>1.f
	{
		 if(ratioY<1.f)
		 {
			computedRatio = ratioY; //reduce
		 }
		 else
		 {
			computedRatio = (ratioX<ratioY)? ratioX : ratioY; //increase
		 }
	}
	return computedRatio;
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

bool GfxBoard::displayBoard(tpCaseList &)
{
	return displayBoard();
}

bool GfxBoard::displayBoard()
{ 
	// partial update is not supported, so we refresh all
	// =================
	// =         =human= 1/10
	// =         =nick = 1/10
	// =         = die = 1/10
	// =         =score= 5/10
	// =         =box  = 2/10
	// =================
	//compute a grid coordinate
	//display background picture

	m_pApp->clear();

	for(int x=0; x < m_screensizeX; x+=m_gridCaseSizeX)
	{
		float fx = static_cast<float>(x);
		drawLine(fx,0,fx,m_screensizeY,&sf::Color::White);
	}
	for(int y=0; y < m_screensizeY; y+=m_gridCaseSizeY)
	{
		float fy = static_cast<float>(y);
		drawLine(0,fy,m_screensizeX,fy,&sf::Color::White);
	}

	tStringList displayTab(getMaxY(),std::string(getMaxX(),' '));
	for(tCaseList::iterator itCase=getCasesList().begin(); itCase != getCasesList().end(); itCase++)
	{
		LogicalLocalization loc = itCase->getLocalization();

		Horse * horse = itCase->getHorse();
		sf::Text * pText = NULL;
		sf::Sprite * pSprite = NULL;
		S32 offsetX = 0;
		S32 offsetY = 0;

		if(itCase->isAStartCase())
		{
			//c = '_';
			for(tPlayerList::iterator itPlayer=this->getPlayersList().begin(); itPlayer!=this->getPlayersList().end(); itPlayer++)
			{
				if(itCase->isAStartCaseForPlayer(&*itPlayer))
				{
					pText = m_startCaseText;
					pText->setColor(*colors[itPlayer->getPlayerNb()]);
					break;
				}
			}
			//center offset adjustment
			offsetX = (m_gridCaseSizeX)/5;
			offsetY = 0;//-(m_gridCaseSizeY)/4;
		}
		else if(itCase->isAPureLadderCase())
		{
			//c = static_cast<char>('0' + itCase->getLadderCaseValue());
			for(tPlayerList::iterator itPlayer=this->getPlayersList().begin(); itPlayer!=this->getPlayersList().end(); itPlayer++)
			{
				if(itCase->isALadderForPlayer(&*itPlayer))
				{
					pText = m_playerladderText[itCase->getLadderCaseValue()-1];
					pText->setColor(*colors[itPlayer->getPlayerNb()]);
					break;
				}
			}
			offsetX = (m_gridCaseSizeX)/4;
			offsetY = 0;//-(m_gridCaseSizeY)/4;
		}
		else if(itCase->isAFinishCase())
		{
			//c = '$';
			//drawable = NULL;
		}
		else if(itCase->isANormalCase())
		{
			//c = '.';
			pSprite = m_normalCaseSprite;
			//center offset adjustment
			offsetX = 0;
			offsetY = (m_gridCaseSizeY - static_cast<U16>(m_normalCaseSprite->getGlobalBounds().height));
		}
		else
		{
			//c = '?';
			//drawable = NULL;
		}
		////////////////////
		S32 x = 0, y = 0;
		U16 sizeX = 0, sizeY = 0;
		if(pText!=NULL || pSprite!=NULL)
		{
			if(ConvertLogicalToGfxCoordinate(loc,x,y,sizeX,sizeY))
			{
				x+=offsetX;
				y+=offsetY;
				if(pText!=NULL)
				{
					pText->setPosition(static_cast<float>(x),static_cast<float>(y));
					m_pApp->draw(*pText);
				}
				if(pSprite!=NULL)
				{
					pSprite->setPosition(static_cast<float>(x),static_cast<float>(y));
					m_pApp->draw(*pSprite);
				}
			}
		}

		////////////////////
		if(horse!=NULL)
		{
			assert(horse->getPlayer()!=NULL);
			assert(Case::isValidCaseId(horse->getCase()));
			//c = static_cast<char>('a' + horse->getPlayer()->getPlayerNb());
			pSprite = m_horseSprites[horse->getPlayer()->getPlayerNb()];

			////////////////////
			S32 x = 0, y = 0;
			U16 sizeX = 0, sizeY = 0;
			if(pSprite!=NULL)
			{
				if(ConvertLogicalToGfxCoordinate(loc,x,y,sizeX,sizeY))
				{
					pSprite->setPosition(static_cast<float>(x),static_cast<float>(y));
					m_pApp->draw(*pSprite);
				}
			}
			////////////////////
		}
		//displayTab[loc.getY()][loc.getX()]=c;
	}
	//m_pApp->Display();//Display should not be done here because it needs
    
	sf::Event event;
	while (m_pApp->pollEvent(event))
    {
        // Close window : exit
		if (event.type == event.Closed)
            m_pApp->close();
    }
	return m_pApp->isOpen();
}

//bool GfxBoard::clearCase(LogicalLocalization & logicLoc)
//{
//	S32 x=0,y=0;
//	U16 sizeX=0, sizeY=0;
//	if(this->ConvertLogicalToGfxCoordinate(logicLoc,x,y,sizeX, sizeY))
//	{
//		sf::Shape blackRectangle = sf::Shape::Rectangle(0,0,m_gridCaseSizeX,m_gridCaseSizeY, sf::Color::Black);
//		blackRectangle.SetPosition(x,y);
//		m_pApp->Draw(blackRectangle);
//		return true;
//	}
//	return false;
//}

void GfxBoard::displayLeftPannel(std::string human, std::string nickname, int die, std::vector<int> scores, int horsesInTheBox, int iColor)
{
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
	m_pApp->display();
}

void GfxBoard::blitHumanLabel(std::string &human, int iColor)
{
	float LeftPannelPositionX = static_cast<float>(m_screensizeY);
	float HumanPositionY = 0.f;

	sf::Text aiTxt(human.c_str(), *m_pFont, SMALL_FONT_SIZE);
	aiTxt.setColor(*colors[iColor]);
	aiTxt.setPosition(LeftPannelPositionX,HumanPositionY);
	m_pApp->draw(aiTxt);
}

void GfxBoard::blitNickLabel(std::string &nickname, int iColor)
{
	float LeftPannelPositionX = static_cast<float>(m_screensizeY);
	float nickPositionY = static_cast<float>(1 * m_screensizeY/10);

	std::string nicknameLabel("Nom du joueur: ");
	nicknameLabel += nickname;

	sf::Text nicknameTxt(nicknameLabel.c_str(), *m_pFont, SMALL_FONT_SIZE);
	nicknameTxt.setColor(*colors[iColor]);
	nicknameTxt.setPosition(LeftPannelPositionX,nickPositionY);
	m_pApp->draw(nicknameTxt);
}

void GfxBoard::blitDieLabel(int die, int iColor)
{
	float LeftPannelPositionX = static_cast<float>(m_screensizeY);
	float diePositionY  = static_cast<float>(2 * m_screensizeY/10);
	std::string dieStr;
	std::stringstream ss;
	ss << die;
	dieStr = ss.str();

	sf::Text dieTxt(dieStr.c_str(), *m_pFont, BIG_FONT_SIZE);
	dieTxt.setColor(*colors[iColor]);
	float offsetX = (m_screensizeX - LeftPannelPositionX) / 2;
	dieTxt.setPosition(LeftPannelPositionX + offsetX,diePositionY);
	m_pApp->draw(dieTxt);
}

void GfxBoard::blitScoreLabel(std::vector<int> scores)
{
	float LeftPannelPositionX = static_cast<float>(m_screensizeY);
	float scorePositionY  = static_cast<float>(3 * m_screensizeY/10);
	int iColor = 0;
	std::string scoreStr;
	std::stringstream ss;
	
	sf::Text scoreTxt("Score:",*m_pFont,BIG_FONT_SIZE);
	scoreTxt.setColor(sf::Color::White);
	scoreTxt.setPosition(LeftPannelPositionX,scorePositionY);
	m_pApp->draw(scoreTxt);
	scorePositionY+= scoreTxt.getGlobalBounds().height;

	for(std::vector<int>::iterator it= scores.begin(); it != scores.end(); it++, iColor++)
	{
		ss << *it;
		scoreStr = ss.str();
		scoreTxt.setString(scoreStr.c_str());
		scoreTxt.setColor(*colors[iColor]);
		scoreTxt.setPosition(LeftPannelPositionX, scorePositionY);
		m_pApp->draw(scoreTxt);
		scorePositionY+= scoreTxt.getGlobalBounds().height;
		ss.str("");
	}
}

void GfxBoard::blitBoxLabel(int horsesInTheBox, int iColor)
{
	float LeftPannelPositionX = static_cast<float>(m_screensizeY);
	float boxPositionY  = static_cast<float>(8 * m_screensizeY/10);

	sf::Sprite horseSprite = *m_horseSprites[iColor];//Does a copy, in order to not modify the horseSprites original
	horseSprite.scale(1.f,1.f);//reset scale
	if(horsesInTheBox>MAX_DISPLAYED_HORSE)
	{
		horseSprite.setPosition(LeftPannelPositionX, boxPositionY);
		m_pApp->draw(horseSprite);

		std::stringstream ss;
		ss << std::setw(3) << std::setfill('0') <<horsesInTheBox;
		std::string str = ss.str();
		sf::Text boxTxt(str.c_str(),*m_pFont, BIG_FONT_SIZE);
		boxTxt.setColor(*colors[iColor]);
		boxTxt.setPosition(LeftPannelPositionX + horseSprite.getGlobalBounds().width, boxPositionY);
		m_pApp->draw(boxTxt);
	}
	else
	{
		for(int i = 0; i < horsesInTheBox; i++)
		{
			horseSprite.setPosition(LeftPannelPositionX, boxPositionY);
			m_pApp->draw(horseSprite);
			LeftPannelPositionX += + horseSprite.getGlobalBounds().width + 5;//
		}
	}
}

bool GfxBoard::BuildStartCaseBitmap()
{
	m_startCaseText = new sf::Text("D",*m_pFont,BIG_FONT_SIZE);
	return true;
}

bool GfxBoard::BuildLadderCaseBitmap()
{
	m_playerladderText.reserve(MAX_LADDER_DIGIT);
	for(char iDigit = 0; iDigit<MAX_LADDER_DIGIT; iDigit++)
	{
		char digitChar[] = {'1' + iDigit, '\0' };
		m_playerladderText.push_back(new sf::Text(digitChar,*m_pFont,BIG_FONT_SIZE));
	}
	return true;
}

bool GfxBoard::GetChoiceFromEvents(eUserEventType &userEvent, int &nbHorse, int &nbPlayer)
{
	userEvent=None;
    // Process events
    sf::Event event;
    while (userEvent==None)
    {
		if(m_pApp->waitEvent(event))
		{
			// Close window : exit
			if(event.type == event.Closed)
			{
				m_pApp->close();
				userEvent = CloseEvent;
			}
			else if(event.type == event.MouseButtonReleased)
			{
				LogicalLocalization logicLoc;
				//TODO check if we need convert pixel coordinate to view coordinate
				if(ConvertGfxToLogicalCoordinate(event.mouseButton.x, event.mouseButton.y, logicLoc))
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
			else if(event.type == event.KeyPressed)
			{
				if(event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
				{
					userEvent = HorseKey;
					nbHorse=event.key.code - sf::Keyboard::Num0;
				}
				else if(event.key.code >= sf::Keyboard::Numpad0 && event.key.code <= sf::Keyboard::Numpad9)
				{
					userEvent = HorseKey;
					nbHorse=event.key.code - sf::Keyboard::Numpad0;
				}
				else if(event.key.code == sf::Keyboard::Escape)
				{
					m_pApp->close();
					userEvent = CloseEvent;
				}
			}
		}
	}
	return userEvent != None;
}

void GfxBoard::displayHistoric()
{
	m_pApp->clear();
	int xPos = 0;
	int yMaxPos = 0;
	int iPlayer = 0;
	for(std::vector<int>::iterator it2=getHistoricList().back().begin(); it2 != getHistoricList().back().end(); it2++, iPlayer++)
	{
		if(yMaxPos<*it2)
		{
			yMaxPos = *it2;
		}
		std::string strLegend = getPlayer(iPlayer)->getNickname() + " : " + getPlayer(iPlayer)->getTypeOfPlayer();
		sf::Text str(strLegend,*m_pFont,SMALL_FONT_SIZE);
		str.setColor(*colors[iPlayer]);
		str.setPosition(static_cast<float>(m_screensizeX-400),static_cast<float>(iPlayer*30));
		m_pApp->draw(str);
	}

	for(tHistoricList::iterator it=getHistoricList().begin();
		it!=getHistoricList().end();
		it++, xPos++)
	{//When item is erased, iterator points on next item! So if iterator is incremented, iterator skips one item!
		tHistoricList::iterator itNext = it;
		itNext++;
		if(itNext!=getHistoricList().end())
		{
			for(unsigned int i=0; i < it->size(); i++)
			{
				float x0 =static_cast<float>(xPos* m_screensizeX / getHistoricList().size());
				float y0 =static_cast<float>((*it)[i] * m_screensizeY / yMaxPos);
				float x1 =static_cast<float>((xPos+1)* m_screensizeX / getHistoricList().size());
				float y1 =static_cast<float>((*itNext)[i] * m_screensizeY / yMaxPos);
				const sf::Color *color=colors[i];
				drawLine(x0,y0,x1,y1,color);
			}
		}
	}
	m_pApp->display();
	sf::Image result=m_pApp->capture();
	result.saveToFile("Result.png");
	sf::sleep(sf::milliseconds(1000));
	m_pApp->clear();
	m_pApp->display();
	result=m_pApp->capture();
	result.saveToFile("Result2.png");
}

void GfxBoard::drawLine(float x1, float y1, float x2, float y2, const sf::Color *color)
{
	sf::Vector2f a(x1,y1), b(x2,y2);
	sf::Vertex line[] = {a, b};
	line[0].color=*color;
	line[1].color=*color;
	m_pApp->draw(line, sizeof(line)/sizeof(sf::Vertex), sf::Lines);
}
