#ifndef __XYA_H__
#define __XYA_H__


// ������ýṹ
struct Xya
{
    signed char x;
    signed char y;
    signed char a;
};
extern float _xyam;
void fillxya();
Xya* getxya( Point const& a, Point const& b );


#endif
