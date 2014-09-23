#ifndef __LOOPER_H__
#define __LOOPER_H__
#include "Lib/All.h"
#include "Logic.h"

class Looper
{
public:
    Looper( Logic* logic );
    ~Looper();
    void update( int durationTicks );
//private:
    Logic* _logic;
    int _logicFrameTicks = 1000 / 60;                           // ÿ֡ tick ��( 1 tick = 1 ms )
    int _logicFrameTicksLimit = _logicFrameTicks * 1;           // ֡���ʱ���޶����������ʱ��Ҳֻ�� 1 ֡
    int _accumulatTicks = 0;                                    // ����ʱ��
};

#endif
