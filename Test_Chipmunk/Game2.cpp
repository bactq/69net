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
    G::window->Init( L"Game2", dw, dh );
    //G::window->setVsync( false );
}

void Game2::Loaded()
{
}

void Game2::Update()
{
}

Game2::~Game2()
{
}

void Game2::createObj( float x, float y )
{
    //auto o = new Object();          // ref = 1
    //objs.insert( o );               // ref = 1
    //mb->Add( o );                   // ref = 2
    //o->size = { 1, 1 };
    //o->offset = { x, y, };
    //o->xyInc = angleXyIncs[ rand() % 360 ];
}
