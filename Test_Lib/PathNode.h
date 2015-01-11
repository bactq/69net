#ifndef __PATHNODE_H__
#define __PATHNODE_H__

#include <type_traits>

template<typename T>
struct PathNode
{
    T userContext;
    int x = -1, y = -1;
    int pqIdx = -1;
    double g = 0;
    double h = 0;
    double f = 0;

    bool operator<( PathNode const& other )
    {
        return this->f < other.f;
    }

    template<typename VT>
    void Assign( int x, int y, VT&& userContext )
    {
        this->userContext = std::forward<VT>( userContext );
        this->x = x;
        this->y = y;
    }
};

#endif
