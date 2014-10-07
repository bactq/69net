#include "Precompile.h"

Game1::Game1()
{
    int dw = 768, dh = 1024;
    scene.designSize = { (float)dw, (float)dh };
    G::window->Init( L"Game1", dw, dh );
    //G::window->setVsync( false );
}

static Point angleXyIncs[ 360 ];           // ��Ŷ�����Ӧ�� xy ����

void Game1::Loaded()
{
    for( int i = 0; i < 360; ++i )  // ��� 360 �ȵ� xy ������
    {
        auto a = M_PI * 2 / 360 * i;
        angleXyIncs[ i ] = { sin( a ), cos( a ) };
    }

    mb = Create<MarginBox>();       // ������ windows ��Ե 5 �ı߿�
    mb->margin = { 5, 5, 5, 5 };
    mb->size = { scene.size.w - 10, scene.size.h - 10 };
    scene.Add( mb );
}

void Game1::Update()
{
    input.touchEvents.clear();      // ��������������¼���ֻ��ע״̬
    if( input.touching )
    {
        createObj( input.touchPos.x - mb->offset.x, input.touchPos.y - mb->offset.y );
    }

    for( int i = objs.size() - 1; i >= 0; --i )
    {
        auto n = objs[ i ];
        auto& o = n->key;
        if( !o->Update() )     // �������� object, �����ļ� kill list
        {
            o->RemoveFromParent();      // ref = 1
            o->Release();               // ref = 0 ( delete )
            objs.erase( n );
        }
    }
}

Game1::~Game1()
{
    for( int i = 0; i < objs.size(); ++i )
    {
        auto& o = objs[ i ]->key;       // ref = 2
        o->Release();                   // ref = 1 ( scene.childs ��ɱ )
    }
}

void Game1::createObj( float x, float y )
{
    auto o = new Object();              // ref = 1
    objs.insert( o );                   // ref = 1
    mb->Add( o );                       // ref = 2
    o->size = { 1, 1 };
    o->offset = { x, y, };
    o->xyInc = angleXyIncs[ rand() % 360 ];
}
