#include "Lib/All.h"


// �̰߳�ȫ�涨���ڴ��( �ޱ�����һ�������꣬��ѭ��������ѷ�����ĵ�Ԫ ��
// �ڴ���������ڴ�ų��ȣ����䡢�ͷ�ָ�룬ָ�����飬ǰ������������
// ֱ�ӽ� SM �ڴ�ָ��Ӳת��������. ��������� init()
template<int _itemSize, int _capacity>
class SMPool
{
    char _buf[ _capacity ][ _itemSize ];
    int _os[ _capacity ];       // o �� offset
    std::atomic<uint> _ao, _fo; // alloc �ں� free ��ǰ

public:
    void init()
    {
        for( int i = 0; i < _capacity; ++i ) _os[ i ] = i;
        _ao = 0;
        _fo = _capacity;
    }
    char* alloc()
    {
        return pointer( _os[ _ao++ % _capacity ] );
    }
    void free( void* p )
    {
        _os[ _fo++ % _capacity ] = offset( p );
    }
    int size() const
    {
        return _fo - _ao;
    }
    int offset( void* p ) const
    {
        return int( ( (size_t)p - (size_t)_buf ) / _itemSize );
    }
    char* pointer( int o ) const
    {
        return o * _itemSize + (char*)_buf;
    }
};


using namespace std;
using namespace xxx;
int main()
{
    char buf[ 512 * 64 + 4 * 64 + 8 ];
    auto& smp = *( SMPool<512, 64>* )buf;
    smp.init();
    Cout( "size = ", smp.size() );
    Stopwatch sw;
    for( int i = 0; i < 9999999; ++i )
    {
        auto p1 = smp.alloc();
        auto p2 = smp.alloc();
        auto p3 = smp.alloc();
        //Cout( (size_t)p1, (size_t)p2, (size_t)p3, " size = ", smp.size() );
        smp.free( p1 );
        smp.free( p2 );
        smp.free( p3 );
        //Cout( "size = ", smp.size() );
    }
    Cout( sw.ElapsedMillseconds() );
    system( "pause" );
    return 0;
}

