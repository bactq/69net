#include "Precompile.h"

Game1::~Game1()
{
    delete G::node; G::node = nullptr;
    delete G::audio; G::audio = nullptr;
    delete G::input; G::input = nullptr;
    G::logic = nullptr;
}
Game1::Game1()
{
    G::logic = this;
    G::input = new Input();
    G::audio = new Audio();
    G::node = new Node();

    /*
    ��Ƴߴ磺    768 * 1024
    ���ӻ��֣�    12 * 16
    ÿ��ߴ磺    64 * 64
    ϸ���ߴ磺    32 * 32 ( �� 1/4 �� )
    */

    G::glwindow->Init( L"test", 768, 1024, 0, 0, true );
    //G::_glwindow->setVsync( false );

    // ���ԭ���� resize ʱ call
    // todo: �� window resize ʱ������ timer, �����Ͻ������ٴ�һ�� timer �׶εĻص������������
    // ͬʱ����������ѭ��֮ǰ��Ҳ��ִ��һ�� timer �׶εĻص�
    glViewport( 0, 0, G::glwindow->width, G::glwindow->height );          // ռ����������
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, G::glwindow->width, 0, G::glwindow->height );

    // todo: code here
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
