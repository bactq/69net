#include "Lib/All.h"
#include <chipmunk.h>
#include "vld.h"

using namespace std;

// ���д������� for chipmunk
template<typename T>
struct cpPool : Pool
{
    cpPool()
    {
        Pool::init( sizeof( T ) );
    }
    inline T* alloc()
    {
#if __DEBUG
        ++_counter;
#endif
        return (T*)Pool::alloc();
    }
    inline void free( T* p )
    {
#if __DEBUG
        --_counter;
#endif
        Pool::free( p );
    }
#if __DEBUG
    int _counter = 0;
#endif
};


int main( void )
{
    // �����
    cpPool<cpSpace> spacePool;
    cpPool<cpShape> shapePool;
    cpPool<cpCircleShape> circleShapePool;
    cpPool<cpSegmentShape> segmentShapePool;
    cpPool<cpBody> bodyPool;


    {
        // ����һ���հ׵���������

        auto space = spacePool.alloc();
        cpSpaceInit( space );
        ScopeGuard sgSpace( [ &]
        {
            cpSpaceDestroy( space );
            spacePool.free( space );
        } );
        cpSpaceSetIterations( space, 1 );               // ������������
        cpSpaceSetGravity( space, cpv( 0, -100 ) );     // ��������


        // Ϊ���洴��һ����̬�߶���״
        // ������΢��б�߶��Ա�����Թ���ȥ
        // ���ǽ���״������space��Ĭ�Ͼ�̬�����ϣ�����Chipmunk����״�ǲ����ƶ���

        auto ground_ = segmentShapePool.alloc();
        auto ground = (cpShape*)ground_;
        cpSegmentShapeInit( ground_, space->staticBody, cpv( -20, 5 ), cpv( 20, -5 ), 0 );
        ScopeGuard sgGround( [ &]
        {
            cpSpaceRemoveShape( space, ground );
            cpShapeDestroy( ground );
            segmentShapePool.free( ground_ );
        } );
        cpShapeSetFriction( ground, 1 );
        cpSpaceAddShape( space, ground );


        // ����������������һ�������䵽���ϲ�����ȥ
        // ����������Ҫ����һ�� cpBody �����ɶ������������
        // ���������������λ�á��ٶȡ��Ƕȵ�
        // Ȼ�����ǽ���ײ��״������cpBody���Ը���һ���ߴ����״

        cpFloat radius = 5;
        cpFloat mass = 1;

        // ת��������������������תһ��
        // ʹ�� cpMomentFor*() �����Ƽ�����
        cpFloat moment = cpMomentForCircle( mass, 0, radius, cpvzero );


        auto ballBody = bodyPool.alloc();
        cpBodyInit( ballBody, mass, moment );
        cpSpaceAddBody( space, ballBody );
        ScopeGuard sgBallBody( [ &]
        {
            cpSpaceRemoveBody( space, ballBody );
            cpBodyDestroy( ballBody );
            bodyPool.free( ballBody );
        } );

        cpBodySetPos( ballBody, cpv( 0, 15 ) ); // ���Զ����� cpBodyActivate �� sleep �� idle ��������( ������ )

        // cpGroup: ��ײ��, �����Ͻ���Ҫ�õ���������ҷ�����ӵ�֮�䲻�ж�������Ϊͬһ group


        // �������ǻ����崴����ײ��״
        // �����Ϊͬһ�����崴�������ײ��״
        // ���ǽ��ḽ�Ź����������ϲ��ƶ�����

        auto ballShape_ = circleShapePool.alloc();
        auto ballShape = (cpShape*)ballShape_;
        cpCircleShapeInit( ballShape_, ballBody, radius, cpvzero );
        cpSpaceAddShape( space, ballShape );
        ScopeGuard sgBallShape( [ &]
        {
            cpSpaceRemoveShape( space, ballShape );
            cpShapeDestroy( ballShape );
            circleShapePool.free( ballShape_ );
        } );

        cpShapeSetFriction( ballShape, 0.7 );           // Ħ��ϵ��

        // ����һ�ж����������ˣ�����ͨ������ʱ�䲽��С����ʱ������������ģ��ռ��е���������
        // *�߶�*�Ƽ�ʹ�ù̶�����ʱ�䲽
        cpFloat timeStep = 1.0 / 60.0;
        for( cpFloat time = 0; time < 2; time += timeStep )
        {
            cpVect pos = cpBodyGetPos( ballBody );
            cpVect vel = cpBodyGetVel( ballBody );
            printf(
                "Time is %5.2f. ballBody is at (%5.2f, %5.2f). It's velocity is (%5.2f, %5.2f)\n",
                time, pos.x, pos.y, vel.x, vel.y
                );

            cpSpaceStep( space, timeStep );
        }

        //// �������ǵĶ����˳�
        //cpShapeFree( ballShape );
        //cpSpaceRemoveBody( space, ballBody );
        //cpBodyFree( ballBody );
        //cpShapeFree( ground_ );
        ////cpSpaceFree( space );
    }

#if __DEBUG
    Cout( spacePool._counter, ", ", shapePool._counter, ", ", circleShapePool._counter, ", ", segmentShapePool._counter, ", ", bodyPool._counter, ", " );
#endif

    return 0;
}
