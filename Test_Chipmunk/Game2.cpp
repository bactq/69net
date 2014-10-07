#include "Precompile.h"

/*
��Ƴߴ磺    768 * 1024
���ӻ��֣�    12 * 16
ÿ��ߴ磺    64 * 64
ϸ���ߴ磺    32 * 32 ( �� 1/4 �� )

��ʵ�����λ�� ( 16 ~ 767 - 16, 16 ~ 1023 - 16 ) ��ʾ N �����飬���������
*/

static int dw = 1024, dh = 768, rowCount = 12, columnCount = 16;
//static int dw = 768, dh = 1024, rowCount = 16, columnCount = 12;

Game2::Game2()
{
    scene.designSize = { (float)dw, (float)dh };
    G::window->Init( L"Game2", dw, dh );
    //G::window->setVsync( false );
}

void Game2::createBox( int _x, int _y )
{
    auto box = Create<Box>();
    box->offset = { _x, _y };
    box->size = { 32, 32 };
    scene.Add( box );

    auto item = cd.CreateItem();
    //box->userData = item;
    item->userData = box;
    item->pos = { _x, _y };
    item->radius = { 16, 16 };

    cd.Index( item );
}

void Game2::Loaded()
{
    cd.Init( { dw, dh }, rowCount, columnCount );
    for( int i = 0; i < 99999; ++i )
    {
        int x = 16 + rand() % ( dw - 1 - 16 - 16 );
        int y = 16 + rand() % ( dh - 1 - 16 - 16 );
        createBox( x, y );
    }
}

void Game2::Update()
{
    while( input.touchEvents.size() )
    {
        auto& e = input.touchEvents.top();
        //if( e.type == TouchEventType::Down )
        //{
            Cout( "clicked on ( ", e.x, ", ", e.y, " )" );
            cd.GetItems( touchedItems, { e.x, e.y } );
            for( int i = 0; i < touchedItems.size(); ++i )
            {
                auto& item = touchedItems[ i ];
                Cout( item->pos.x, ", ", item->pos.y );

                scene.Remove( (Node*)item->userData );
                cd.DestroyItem( item );
            }
        //}
        input.touchEvents.pop();
    }
}

Game2::~Game2()
{
}