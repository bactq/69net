#include "Precompile.h"

Game1::~Game1()
{
    delete G::node; G::node = nullptr;
    G::logic = nullptr;
}
Game1::Game1()
{
    G::logic = this;
    G::node = new Node();

    // code here
    /*
    ��Ƴߴ磺    768 * 1024
    ���ӻ��֣�    12 * 16
    ÿ��ߴ磺    64 * 64
    ϸ���ߴ磺    32 * 32 ( �� 1/4 �� )
    */


    G::window->Init( L"test", 768, 1024, 0, 0, true );
    //G::_glwindow->setVsync( false );

    G::window->resizeCallback = []
    {
        glViewport( 0, 0, G::window->width, G::window->height );          // ռ����������
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluOrtho2D( 0, G::window->width, 0, G::window->height );
    };

    G::node->beforeDraw = []( int durationTicks )
    {
        glLineWidth( 3 );
        glEnable( GL_LINE_STIPPLE );
        glLineStipple( 2, 0x3f07 );
        glBegin( GL_LINE_LOOP );
        {
            glVertex2s( 191, 594 );
            glVertex2s( 390, 21 );
            glVertex2s( 594, 577 );
            glVertex2s( 12, 236 );
            glVertex2s( 767, 262 );
        }
        glEnd();
    };
}

void Game1::Update()
{
    // todo: code here
    printf( "." );
}
