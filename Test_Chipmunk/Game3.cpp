#include "Precompile.h"

/*
��Ƴߴ磺    768 * 1024
���ӻ��֣�    12 * 16
ÿ��ߴ磺    64 * 64
ϸ���ߴ磺    32 * 32 ( �� 1/4 �� )

��ʵ�����λ�� ( 16 ~ 767 - 16, 16 ~ 1023 - 16 ) ��ʾ N �����飬���������
*/

static int dw = 768, dh = 1024, rowCount = 16, columnCount = 12;

Game3::Game3()
{
    scene.designSize = { (float)dw, (float)dh };
    G::window->Init( L"Game3", dw, dh );
    //G::window->setVsync( false );
}

void Game3::Loaded()
{
}

void Game3::Update()
{
}

Game3::~Game3()
{
}
