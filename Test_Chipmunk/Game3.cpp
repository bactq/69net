#include "Precompile.h"

/*
��Ƴߴ磺    768 * 1024
���ӻ��֣�    24 * 32
ÿ��ߴ磺    32 * 32
ϸ���ߴ磺    16 * 16

*/

static const int dw = 768, dh = 1024, rowCount = 32, columnCount = 24;

Game3::Game3()
{
    scene.designSize = { (float)dw, (float)dh };
    G::window->Init( L"Game3", dw, dh );
    //G::window->SetVsync( false );

    cdgrid = new CdGrid();
    cdgrid->Init( { dw * 3, dh * 3 }, rowCount * 3, columnCount * 3 );  // 3*3 ���飬���м�Ϊ��Ļӳ��
}

static Node* createPlane()
{
    auto b = Create<Node>();
    auto b1 = Create<Box>();    // -
    auto b2 = Create<Box>();    // |
    auto b3 = Create<Box>();    // -
    b->Add( b1 );
    b->Add( b2 );
    b->Add( b3 );

    b1->size = { 16 * 8, 16 * 2 };
    b2->size = { 16 * 2, 16 * 5 };
    b2->pos = { 16 * 0, 16 * -0.5 };
    b3->size = { 16 * 4, 16 * 1 };
    b3->pos = { 16 * 0, 16 * -3.5 };

    b1->color = b2->color = b3->color = { 111, 111, 255, 0 };
    return b;
}

void Game3::Loaded()
{
    fillxya();

    plane = createPlane();
    plane->pos = { scene.size.w / 2, scene.size.h / 2 };
    scene.Add( plane );
}

void Game3::Update()
{
    input.touchEvents.Clear();
    if( input.touching )
    {
        // �������
        auto xya = getxya( plane->pos, input.touchPos );
        auto xi = xya->x / _xyam * 15;
        auto yi = xya->y / _xyam * 15;
        auto maxxi = input.touchPos.x - plane->pos.x;
        auto maxyi = input.touchPos.y - plane->pos.y;
        if( xi < 0 && xi < maxxi ) xi = maxxi;
        else if( xi > 0 && xi > maxxi ) xi = maxxi;
        if( yi < 0 && yi < maxyi ) yi = maxyi;
        else if( yi > 0 && yi > maxyi ) yi = maxyi;
        plane->pos.x += xi;
        plane->pos.y += yi;
        plane->dirty = true;
    }

    // �����ӵ�
    for( int i = 0; i < 100; ++i )
    {
        auto bullet = Bullet::Create( &scene, cdgrid, { plane->pos.x, plane->pos.y + 16 * 2 } );
    }

    // ������
    for( int i = 0; i < 40; ++i )
    {
        auto monster = Monster::Create( &scene, cdgrid, { rand() % dw, dh } );
    }

    // ��ǰ��
    for( int i = Monster::objs.Size() - 1; i >= 0; --i )
    {
        auto& o = Monster::objs[ i ];
        if( !o->Update() )
        {
            o->Destroy();
        }
    }

    // �ӵ�ǰ��
    for( int i = Bullet::objs.Size() - 1; i >= 0; --i )
    {
        auto& o = Bullet::objs[ i ];
        if( !o->Update() )
        {
            o->Destroy();
        }
    }

    // ÿ������ʾһ���ܵĹֺ͵���
    static int counter = 0;
    if( ++counter >= 60 )
    {
        counter = 0;
        Cout( "total bullets:", Bullet::objs.Size(), "\ntotal monsters:", Monster::objs.Size() );
    }
}

Game3::~Game3()
{
    delete cdgrid;
    Monster::FreeObjs();
    Bullet::FreeObjs();
}



//struct Action : Ref { };
//struct Sequence : public Action { List<Action*> childs; };
//struct Repeater : public Action { Action* action; int count; };
//struct MoveTo : public Action { float x, y; };
//struct CallFunc : public Action { std::function<void()> func; };
//struct Delay : public Action { float sec; };
