#include "Precompile.h"

/*

��Ƴߴ磺    768 * 1024
���ӻ��֣�    12 * 16
ÿ��ߴ磺    64 * 64
ϸ���ߴ磺    32 * 32 ( �� 1/4 �� )

*/





Logic::Logic( Video* v, Input* i, Audio* a )
    : _v( v )
    , _i( i )
    , _a( a )
{
}

Logic::~Logic()
{
}

void Logic::update()
{
    _i->update( _f );
    printf( "." );

    ++_f;
}
