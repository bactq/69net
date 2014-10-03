#include "Precompile.h"

/*
��Ƴߴ磺    768 * 1024
���ӻ��֣�    12 * 16
ÿ��ߴ磺    64 * 64
ϸ���ߴ磺    32 * 32 ( �� 1/4 �� )
*/

Game1::Game1()
{
    int dw = 768, dh = 1024;
    scene.designSize = { (float)dw, (float)dh };
    G::window->Init( L"test", dw, dh );
    //G::window->setVsync( false );
}

// ��Ŷ�����Ӧ�� xy ����
Point angleXyIncs[ 360 ];

void Game1::Loaded()
{
    for( int i = 0; i < 360; ++i )
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

void Game1::Update()
{
    // ��������������¼�
    input.touchEvents.clear();

    if( input.touching )
    {
        createObj( input.touchPos.x - mb->offset.x, input.touchPos.y - mb->offset.y );
    }

    List<Object*> dead;
    for( auto o : objs )
    {
        if( !o->Update() )
        {
            dead.push( o );
        }
    }
    for( int i = 0; i < dead.size(); ++i )
    {
        auto& o = dead[ i ];
        objs.erase( o );
        o->RemoveFromParent();
        o->Release();
    }
}

Game1::~Game1()
{
    for( auto o : objs )
    {
        o->Release();
    }
}

void Game1::createObj( float x, float y )
{
    auto o = new Object();
    objs.insert( o );
    o->size = { 1, 1 };
    o->offset = { x, y, };
    o->xyInc = angleXyIncs[ rand() % 360 ];
    mb->Add( o );
}

//for( auto i = 0; i < 9999; ++i )
//{
//    auto o = new Object();
//    objs.push_back( o );
//    o->size = { 32, 32 };
//    o->offset = { 
//        rand() % ( 768 - 5 - 5 - 32 ) + 16, 
//        rand() % ( 1024 - 5 - 5 - 32 ) + 16
//    };
//    o->xyInc = {
//        rand() % 2 == 1 ? 1 : -1, 
//        rand() % 2 == 1 ? 1 : -1
//    };
//    mb->Add( o );
//}


//while( !es.empty() )
//{
//    auto& e = es.top();
//    if( e.type == TouchEventType::Down )
//    {
//        createObj( e.x - mb->offset.x, e.y - mb->offset.y );
//    }
//    es.pop();
//}
