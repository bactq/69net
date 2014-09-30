#include "Precompile.h"

Video::Video( GLWindow* w )
    : _w( w )
{
    w->init( L"test", 768, 1024, 0, 0, true );
    //w->setVsync( false );

    // ���ԭ���� resize ʱ call
    glViewport( 0, 0, _w->_w, _w->_h );          // ռ����������
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, _w->_w, 0, _w->_h );
}

Video::~Video()
{
}

void Video::update( int durationTicks )
{

}
