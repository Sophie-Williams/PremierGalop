#include "LogicalLocalization.h"

LogicalLocalization::LogicalLocalization(unsigned int x, unsigned int y) : m_x(x), m_y(y)
{
}

void LogicalLocalization::setLocalization(unsigned int x, unsigned int y)
{
	m_x = x;
	m_y = y;
}

unsigned int LogicalLocalization::getX() const
{
	return m_x;
}

unsigned int LogicalLocalization::getY() const
{
	return m_y;
}

bool LogicalLocalization::MoveLeft(unsigned int step)
{
	if(m_x >= step)
	{
		m_x -= step;
		return true;
	}
	return false;
}

bool LogicalLocalization::MoveRight(unsigned int maxX, unsigned int step)
{
	if(m_x + step < maxX)
	{
		m_x += step;
		return true;
	}
	return false;
}

bool LogicalLocalization::MoveDown(unsigned int maxY, unsigned int step)
{
	if(m_y + step < maxY)
	{
		m_y += step;
		return true;
	}
	return false;
}

bool LogicalLocalization::MoveUp(unsigned int step)
{
	if(m_y >= step)
	{
		m_y -= step;
		return true;
	}
	return false;
}

bool LogicalLocalization::operator==(const LogicalLocalization &rightParameter) const
{
	return (m_x == rightParameter.m_x && m_y == rightParameter.m_y);
}

bool LogicalLocalization::operator!=(const LogicalLocalization &rightParameter) const
{
	return ! (*this == rightParameter);
}

bool LogicalLocalization::operator<(const LogicalLocalization &rightParameter) const
{
	if(m_x == rightParameter.m_x)
		return (m_y < rightParameter.m_y);
	else
		return (m_x < rightParameter.m_x);
}