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
    G::scene->designSize = { (float)dw, (float)dh };
    G::window->Init( L"test", dw, dh );
    //G::window->setVsync( false );
}

void Game1::Loaded()
{
    auto mb = new MarginBox();
    mb->AutoRelease();
    mb->margin = { 5, 5, 5, 5 };
    mb->size = { G::scene->size.w - 10, G::scene->size.h - 10 };
    G::scene->Add( mb );
    
    for( auto i = 0; i < 9999; ++i )
    {
        auto o = new Object();
        objs.push_back( o );
        o->size = { 32, 32 };
        o->offset = { 
            rand() % ( 768 - 5 - 5 - 32 ) + 16, 
            rand() % ( 1024 - 5 - 5 - 32 ) + 16
        };
        o->xyInc = {
            rand() % 2 == 1 ? 1 : -1, 
            rand() % 2 == 1 ? 1 : -1
        };
        mb->Add( o );
    }
}

void Game1::Update()
{
    for( auto o : objs )
    {
        o->Update();
    }
}

Game1::~Game1()
{
    for( auto o : objs )
    {
        o->Release();
    }
}
