#include "Precompile.h"

/*
��Ƴߴ磺    768 * 1024
���ӻ��֣�    12 * 16
ÿ��ߴ磺    64 * 64
ϸ���ߴ磺    32 * 32 ( �� 1/4 �� )
*/

Game2::Game2()
{
    int dw = 768, dh = 1024;
    scene.designSize = { (float)dw, (float)dh };
    G::window->Init( L"test", dw, dh );
    //G::window->setVsync( false );
}


static Point angleXyIncs[ 360 ];           // ��Ŷ�����Ӧ�� xy ����

void Game2::Loaded()
{
    for( int i = 0; i < 360; ++i )  // ��� 360 �ȵ� xy ������
    {
        auto a = M_PI * 2 / 360 * i;
        angleXyIncs[ i ] = { sin( a ), cos( a ) };
    }

    mb = new MarginBox();
    mb->AutoRelease();
    mb->margin = { 5, 5, 5, 5 };
    mb->size = { scene.size.w - 10, scene.size.h - 10 };
    scene.Add( mb );
}

void Game2::Update()
{
    input.touchEvents.clear();      // ��������������¼���ֻ��ע״̬
    if( input.touching )
    {
        createObj( input.touchPos.x - mb->offset.x, input.touchPos.y - mb->offset.y );
    }

    List<Hash<Object*>::Node*> dead;
    for( int i = 0; i < objs.size(); ++i )
    {
        auto o = objs[ i ];
        if( !o->key->Update() )
        {
            dead.push( o );
        }
    }
    for( int i = 0; i < dead.size(); ++i )
    {
        auto& o = dead[ i ]->key;   // ref = 2
        o->RemoveFromParent();      // ref = 1
        objs.erase( o );            // ref = 1
        o->Release();               // ref = 0 ( delete )
    }
}

Game2::~Game2()
{
    for( int i = 0; i < objs.size(); ++i )
    {
        auto& o = objs[ i ]->key;   // ref = 2
        o->Release();               // ref = 1 ( ���� scene.Childs ɱ )
    }
}

void Game2::createObj( float x, float y )
{
    auto o = new Object();          // ref = 1
    objs.insert( o );               // ref = 1
    mb->Add( o );                   // ref = 2
    o->size = { 1, 1 };
    o->offset = { x, y, };
    o->xyInc = angleXyIncs[ rand() % 360 ];
}
