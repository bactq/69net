#ifndef __LOOPER_H__
#define __LOOPER_H__

namespace xxx
{
    class Looper
    {
    public:
        void Update( int _durationTicks );
    //private:
        int logicFrameTicks = 1000 / 60;                        // ÿ֡ tick ��( 1 tick = 1 ms )
        int logicFrameTicksLimit = logicFrameTicks * 10;        // ֡���ʱ���޶����������ʱ��Ҳֻ�� 1 ֡
        int accumulatTicks = 0;                                 // ����ʱ��
    };
}

#endif