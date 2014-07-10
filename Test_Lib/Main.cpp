#include "Lib/All.h"
using namespace std;



// ��ʽ���ݵ��Ƚ��ȳ���( �����ڷ�����ɶ�� )
class LinkedBuffer
{
public:
    explicit LinkedBuffer( Pool& p );
    ~LinkedBuffer();
    void write( char const* buf, int len );
    int read( char* buf, int bufLen );         // �Ը���ָ�����ȵ� buf ���ƶ���ָ��. ����ʵ�ʸ��Ƶĳ���
    void copy( char* buf, int len );            // ����ָ�����ȵ� buf, ��� size ��������
    void clear();
    bool empty();
    int size();                 // δ��֮���ݳ�
private:
    struct Page
    {
        Page*   next;
        char    data[ 1 ];
    };
    Page*       _wp;            // ������д��ҳ
    Page*       _rp;            // �����ڶ���ҳ
    int         _wpLen;         // ������д��ҳ����д����
    int         _rpLen;         // �����ڶ���ҳ���Ѷ�����
    int         _size;          // ����ҳʣ�೤�� + ��дҳ��д���� + �м�����ҳ�� * ҳsize
    Pool&       _pool;
};

LinkedBuffer::LinkedBuffer( Pool& p )
    : _wp( nullptr )
    , _rp( nullptr )
    , _wpLen( 0 )
    , _rpLen( 0 )
    , _size( 0 )
    , _pool( p )
{
}

LinkedBuffer::~LinkedBuffer()
{
    clear();
}

void LinkedBuffer::clear()
{
    Page* p = _wp;
    while( p )
    {
        _pool.free( p );
        p = p->next;
    }
    _wp = _rp = nullptr;
    _wpLen = _rpLen = _size = 0;
}

void LinkedBuffer::write( char const* buf, int len )
{
    assert( buf );
    if( !len ) return;
    _size += len;

    int ps = _pool.pageBufLen() - sizeof( Page* );
    if( !_wp ) _wp = (Page*)_pool.alloc();
    if( len <= ps ) goto CopyLast;

CopyPage:
    memcpy( _wp->data, buf, ps );
    len -= ps;
    buf += ps;
    if( len >= ps )
    {
        _wp->next = (Page*)_pool.alloc();
        _wp = _wp->next;
        goto CopyPage;
    }
CopyLast:
    _wpLen = len;
    _wp->next = nullptr;
    if( len ) memcpy( _wp->data, buf, len );
}

void LinkedBuffer::copy( char* buf, int len )
{
    assert( len <= _size );

}

int LinkedBuffer::read( char* buf, int bufLen )
{
    // ...
    return 0;
}







template<typename KT, typename VT>
class LRUCache;


template<typename KT, typename VT>
class LRUCacheItem
{
public:
    template<typename PKT, typename PVT>
    LRUCacheItem( PKT && key, PVT && value )        // ��������֮���� link. ��Ȼ head �� prev next ָ��ĵ�ַ���ܲ���ȷ
        : _next( nullptr )                          // ������ָ��Ϊ��, ��Ϊ���ƺ�( ����������� ) ��Ҫ link �������������
        , _prev( nullptr )
        , _key( forward<PKT>( key ) )
        , _value( forward<PVT>( value ) )
    {
    }

    LRUCacheItem( LRUCacheItem const & other )
        : _next( nullptr )
        , _prev( nullptr )
        , _key( other._key )
        , _value( other._value )
    {
    }

    LRUCacheItem( LRUCacheItem && other )
        : _next( nullptr )
        , _prev( nullptr )
        , _key( move( other._key ) )
        , _value( move( other._value ) )
    {
    }

    LRUCacheItem & operator=( LRUCacheItem const & other )
    {
        _next = nullptr;
        _prev = nullptr;
        _key = other._key;
        _value = other._value;
    }

    LRUCacheItem & operator=( LRUCacheItem && other )
    {
        _next = nullptr;
        _prev = nullptr;
        _key = move( other._key );
        _value = move( other._value );
    }

    friend LRUCache < KT, VT > ;
private:
    LRUCacheItem( LRUCacheItem * next, LRUCacheItem * prev )
        : _next( next )
        , _prev( prev )
        //, _key( KT() )            // �ƺ����س�ʼ��
        //, _value( VT() )
    {
    }

    void link( LRUCacheItem & head )
    {
        _next = &head;
        _prev = head._prev;
        head._prev->_next = this;
        head._prev = this;
    }

    void unlink()
    {
        _prev->_next = _next;
        _next->_prev = _prev;
    }

    void moveTo( LRUCacheItem & head )
    {
        unlink();
        this->_next = &head;
        this->_prev = head._prev;
        head._prev->_next = this;
        head._prev = this;
    }

    LRUCacheItem *  _next;
    LRUCacheItem *  _prev;
    KT              _key;
    VT              _value;
};


template<typename KT, typename VT>
class LRUCache
{
public:
    typedef LRUCacheItem<KT, VT> ITEM_T;

    // limit Ϊ�����޶�. ���޽��Ƴ�
    LRUCache( int limit = 100 )
        : _limit( limit )
        , _head( &_head, &_head )
    {
        _data.reserve( limit + 1 );
    }

    // add. ����ָ�� _data �д洢 value �ĵ�ַ ��  true( add�ɹ� ), false( �Ѵ��� )
    template<typename PKT, typename PVT>
    pair<VT*, bool> add( PKT && key, PVT && value, bool override = true )
    {
        pair<VT*, bool> rtv;
        auto it = _data.insert( make_pair( forward<PKT>( key ), ITEM_T( key, forward<PVT>( value ) ) ) );
        if( it.second )
        {
            it.first->second.link( _head );
            if( (int)_data.size() > _limit )
            {
                evict();
            }
            rtv.second = false;
        }
        else if( override ) // ����
        {
            it.first->second._value = forward<PVT>( value );
            rtv.second = true;
        }
        rtv.first = &it.first->second._value;
        return rtv;
    }

    // get. ˳����������, ���Ƴ�
    VT * get( KT const & key )
    {
        auto it = _data.find( key );
        if( it == _data.end() )
        {
            return nullptr;
        }
        it->second.moveTo( _head );
        return &it->second._value;
    }

    // clear
    void clear()
    {
        _data.clear();
    }

    // dump
    void dump()
    {
        for( auto &o : _data ) cout << o.first << endl;
        cout << endl;
    }

    // todo: ȱ�����Ƴ�? ȱ _data ��¶?

private:
    // �Ƴ����һ��
    void evict()
    {
        auto lastItem = _head._next;
        lastItem->unlink();
        _data.erase( lastItem->_key );
    }

    int                         _limit;
    ITEM_T                      _head;
    unordered_map<KT, ITEM_T>   _data;
};






int main()
{

    return 0;
}
