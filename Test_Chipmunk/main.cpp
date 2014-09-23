#include "Lib/All.h"
#include "GLWindow.h"
#include "Video.h"
#include "Input.h"
#include "Audio.h"
#include "Logic.h"
#include "Looper.h"

using namespace std;
using namespace std::chrono;

int main( void )
{
    GLWindow w;
    Video v( &w );
    Input i;
    Audio a;
    Logic logic( &v, &i, &a );
    Looper looper( &logic );

    auto lastTP = system_clock::now();
    w.loop( [ &]
    {
        auto currTP = system_clock::now();
        looper.update( (int)duration_cast<milliseconds>( currTP - lastTP ).count() );
        lastTP = currTP;
    } );

    return 0;
}




//// ���д������� for chipmunk
//template<typename T>
//struct cpPool : Pool
//{
//    cpPool()
//    {
//        Pool::init( sizeof( T ) );
//    }
//    inline T* alloc()
//    {
//#if __DEBUG
//        ++_counter;
//#endif
//        return (T*)Pool::alloc();
//    }
//    inline void free( T* p )
//    {
//#if __DEBUG
//        --_counter;
//#endif
//        Pool::free( p );
//    }
//#if __DEBUG
//    int _counter = 0;
//#endif
//};
//// �����
//cpPool<cpSpace> spacePool;
//cpPool<cpShape> shapePool;
//cpPool<cpCircleShape> circleShapePool;
//cpPool<cpSegmentShape> segmentShapePool;
//cpPool<cpBody> bodyPool;



////glClearColor( 0, 0, 0, 0 );
////glMatrixMode( GL_PROJECTION );
////glLoadIdentity();
////glOrtho( 0, 1, 0, 1, -1, 1 );

//float theta = 0;

//w.run( [ &]
//{
//    //glClear( GL_COLOR_BUFFER_BIT );
//    //glColor3f( 1, 1, 1 );
//    //glBegin( GL_POLYGON );
//    //{
//    //    glVertex3f( 0.25, 0.25, 0.0 );
//    //    glVertex3f( 0.75, 0.25, 0.0 );
//    //    glVertex3f( 0.75, 0.75, 0.0 );
//    //    glVertex3f( 0.25, 0.75, 0.0 );
//    //}
//    //glEnd();

//    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
//    glClear( GL_COLOR_BUFFER_BIT );

//    glPushMatrix();
//    glRotatef( theta, 0.0f, 0.0f, 1.0f );
//    glBegin( GL_TRIANGLES );
//    glColor3f( 1.0f, 0.0f, 0.0f ); glVertex2f( 0.0f, 1.0f );
//    glColor3f( 0.0f, 1.0f, 0.0f ); glVertex2f( 0.87f, -0.5f );
//    glColor3f( 0.0f, 0.0f, 1.0f ); glVertex2f( -0.87f, -0.5f );
//    glEnd();
//    glPopMatrix();

//    theta += 1.0f;

//    cout( ++counter );
//    //std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//    //printf( "." );
//} );



/*
// logic + render ѭ��α����

// double ʱ����:

����ʱ�� = 1 / ÿ�����;
����ʱ������ = ����ʱ�� * 1;              // * 1: ����֡��������ܲ��У���Ϸ�ᱻ����������������

��ʼʱ�� = 0;
����ʱ�� = 0;
�ϴ�ʱ�� = ȡ��ǰʱ��();

while( !�˳� )
{
��ǰʱ�� = ȡ��ǰʱ��();
����ʱ�� = ��ǰʱ�� - �ϴ�ʱ��;
if( ����ʱ�� > ����ʱ������ ) ����ʱ�� = ����ʱ������;
�ϴ�ʱ�� = ��ǰʱ��;

����ʱ�� += ����ʱ��;
while( ����ʱ�� >= ����ʱ�� )
{
�߼�����( ��ʼʱ��, ����ʱ�� );
��ʼʱ�� += ����ʱ��;
����ʱ�� -= ����ʱ��;
}

����();
}


// ������:


�߼�֡ʱ�� = 1 / ÿ�����;         // ʱ������������. ���� 50ms ɶ��
�߼�֡ʱ������ = �߼�֡ʱ�� * 1;   // * 1: ����֡��������ܲ��У���Ϸ�ᱻ�������������쳣

����ʱ�� = 0;
�ϴ�ʱ�� = ȡ��ǰʱ��();

�߼�֡���� = 0;

while( !�˳� )                    // ��δ����п����Ǿ��� engine schedule ֮���� call
{
��ǰʱ�� = ȡ��ǰʱ��();
����ʱ�� = ��ǰʱ�� - �ϴ�ʱ��;
if( ����ʱ�� > ����ʱ������ ) ����ʱ�� = ����ʱ������;
�ϴ�ʱ�� = ��ǰʱ��;

if( ��ͣ ) ��ת������();      // ���� schedule call ���� return ��

����ʱ�� += ����ʱ��;
while( ����ʱ�� >= ����ʱ�� )
{
�߼�����( �߼�֡����++ );
����ʱ�� -= ����ʱ��;
}

����();                       // ���� schedule call ��û���������������
}


*/



//// ÿ 0.2 �����һ���� counter �� ÿ�� count
//std::chrono::milliseconds refreshDuration( 200 );
//std::chrono::seconds durationSec1( 1 );
//std::chrono::time_point<std::chrono::system_clock> lastTime;
//auto firstTime = std::chrono::system_clock::now();
//auto cout = [ &]( long long val )
//{
//    auto now = std::chrono::system_clock::now();
//    if( now - lastTime >= refreshDuration )
//    {
//        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>( now - firstTime ).count();
//        if( !elapsedSeconds ) elapsedSeconds = 1;
//        printf( "%lld ", val / elapsedSeconds );
//        lastTime = now;
//    }
//};
//long long counter = 0;
