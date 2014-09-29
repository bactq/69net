#include "Logic.h"

/*

��Ƴߴ磺    24576 * 32768
���ӻ��֣�    12 * 16
ÿ��ߴ磺    2048 * 2048
ϸ���ߴ磺    1024 * 1024 ( �� 1/4 �� )

����ߴ磺    768 * 1024    | 1536 * 2048
��Ʊ��ʣ�    32
ϸ������ߴ磺32px          | 64px

���뾫�ȣ�    384 * 512
��Ʊ��ʣ�    64


�������뾫�ȣ���������ǶȺ���С����ֵ:

struct xya
{
byte x;
byte y;
short a;
} data[ 384, 512 ];

// ����ֵΪ 0 ~ 255
// �Ƕ�ֵΪ 0 ~ 65535 ( ��Ӧ PI/2 )

*/

struct xya
{
    double x;
    double y;
    double a;
};
xya _xyas[ 384 ][ 512 ];
xya _xyas2[ 384 ][ 512 ];
//struct xya
//{
//    byte x;
//    byte y;
//    ushort a;
//} _xyas[ 384 ][ 512 ];


Logic::Logic( Video* v, Input* i, Audio* a )
    : _v( v )
    , _i( i )
    , _a( a )
{
    // todo: ��� _xyas( ��ƽ̨һ�� check ����������У��ͣ������Ƿ�һ�� )
    double PI = 3.14159265358979323846264338327950288;
    double PI_2 = PI / 2;
    double PI2 = PI * 2;

    Stopwatch sw;
    for( int i = 0; i < 100; ++i )
    {
        for( int x = 0; x < _countof( _xyas ); ++x )
        {
            for( int y = 0; y < _countof( _xyas[ x ] ); ++y )
            {
                auto X = x * 64;
                auto Y = y * 64;
                auto D = sqrt( X*X + Y*Y );
                //auto frames = D / 64;           // ÿ frame �����ƶ��ľ���
                auto& xya = _xyas[ x ][ y ];
                xya.x = X * 64 / D;
                xya.y = Y * 64 / D; // / frames
                xya.a = atan2( (double)Y, (double)X ) / PI_2;// *0xFFFFu;
            }
        }
    }
    Cout( sw.elapsedMillseconds() );

    sw.reset();
    for( int i = 0; i < 100; ++i )
    {
        for( int x = 0; x < _countof( _xyas ); ++x )
        {
            for( int y = 0; y < _countof( _xyas[ x ] ); ++y )
            {
                auto& xya = _xyas[ x ][ y ];
                auto& xya2 = _xyas2[ x ][ y ];
                xya2 = xya;
            }
        }
    }
    Cout( sw.elapsedMillseconds() );
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
