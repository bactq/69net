#ifndef __LOOPER_H__
#define __LOOPER_H__
#include "Lib/All.h"
#include "Logic.h"
#include "Render.h"

class Looper
{
public:
    Looper( Logic* logic, Render* render = nullptr );
    ~Looper();
    void update( int durationTicks );
private:
    Logic* _logic;
    Render* _render;

    // todo: ��������
    int _logicFrameTicks = 1000 / 60;                           // ÿ֡������, ��ǰΪ 1 tick = 1 ms
    int _logicFrameTickLimit = _logicFrameTicks * 1;            // * 1: ����֡

    int _accumulatTicks = 0;                                    // ����ʱ��
    int _renderTicks = 0;                                       // ��Ⱦ�Ĵ���ʱ��( ��ִ���˶��ٴ�֡�߼���� )
};

#endif
