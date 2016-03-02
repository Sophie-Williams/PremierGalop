#ifndef  __CHEVAUX_LOGICALLOCALIZATION_H__
#define __CHEVAUX_LOGICALLOCALIZATION_H__

#include <iostream>

class LogicalLocalization {
public:
    LogicalLocalization(unsigned int x = 0, unsigned int y = 0);

    void setLocalization(unsigned int x, unsigned int y);
    unsigned int getX() const;
    unsigned int getY() const;
    bool MoveLeft(unsigned int step = 1);
    bool MoveRight(unsigned int maxX, unsigned int step = 1);
    bool MoveDown(unsigned int maxY, unsigned int step = 1);
    bool MoveUp(unsigned int step = 1);
    bool operator==(const LogicalLocalization &rightParameter) const;
    bool operator!=(const LogicalLocalization &rightParameter) const;
    bool operator<(const LogicalLocalization &rightParameter) const;

    friend std::ostream& operator<<(std::ostream& os, LogicalLocalization loc) {
        os << '[' << loc.getX() << "," << loc.getY() << ']';
        return os;
    }


private:
    unsigned int m_x;
    unsigned int m_y;
};

#endif