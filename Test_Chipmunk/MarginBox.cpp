#include "Precompile.h"


MarginBox::MarginBox()
{
    anchor = { 0, 0 };
}

void MarginBox::Draw( int _durationTicks )
{
    if( dirty || parent->dirty )
    {
        size =
        {
            parent->size.w - margin.left - margin.right,
            parent->size.h - margin.top - margin.bottom,
        };
        offset = { margin.left, margin.bottom };
    }
    Box::Draw( _durationTicks );
}
