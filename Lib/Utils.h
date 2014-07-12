#ifndef __UTILS_H__
#define __UTILS_H__

namespace Utils
{
    // ȡ����ϵ�У�ֵΪ toString ������. 10 ���ơ�
    inline int getToStringMaxLength( uint8   v ) { return 3; }
    inline int getToStringMaxLength( uint16  v ) { return 5; }
    inline int getToStringMaxLength( uint    v ) { return 10; }
    inline int getToStringMaxLength( uint64  v ) { return 19; }
    inline int getToStringMaxLength( int8    v ) { return 4; }
    inline int getToStringMaxLength( int16   v ) { return 6; }
    inline int getToStringMaxLength( int     v ) { return 11; }
    inline int getToStringMaxLength( int64   v ) { return 20; }
    inline int getToStringMaxLength( double  v ) { return 20; }
    inline int getToStringMaxLength( float   v ) { return 20; }
    inline int getToStringMaxLength( bool    v ) { return 5; }
    inline int getToStringMaxLength( char    v ) { return 1; }
    inline int getToStringMaxLength( std::string const & v ) { return (int)v.size(); }
    inline int getToStringMaxLength( char const* v ) { return (int)strlen( v ); }
    inline int getToStringMaxLength( String const & v ) { return v.size(); }

    template<typename T>
    void getFillMaxLengthCore( int & len, T const & v )
    {
        len += getToStringMaxLength( v );
    }

    template<typename T, typename ...TS>
    void getFillMaxLengthCore( int & len, T const & v, TS const & ...vs )
    {
        getFillMaxLengthCore( len, v );
        getFillMaxLengthCore( len, vs... );
    }

    template<typename ...TS>
    int getFillMaxLength( TS const & ...vs )
    {
        int len = 0;
        getFillMaxLengthCore( len, vs... );
        return len;
    }






    // ��ֵ����ת��Ϊ�ַ��� ����ת����Ĵ��� (��ҪΪ Append, Write �Ⱥ�������)
    int toString( char * dstBuf, uint8   v );
    int toString( char * dstBuf, uint16  v );
    int toString( char * dstBuf, uint    v );
    int toString( char * dstBuf, uint64  v );
    int toString( char * dstBuf, int8    v );
    int toString( char * dstBuf, int16   v );
    int toString( char * dstBuf, int     v );
    int toString( char * dstBuf, int64   v );
    int toString( char * dstBuf, double  v, int maxlen = 20, int precision = 10, bool delEndZeros = true );
    int toString( char * dstBuf, float   v, int maxlen = 20, int precision = 10, bool delEndZeros = true );
    int toString( char * dstBuf, bool    v );
    int toString( char * dstBuf, char    v );

    // һЩ����дģ��Ĳ���
    int toString( char * dstBuf, char const* v );
    int toString( char * dstBuf, std::string const & v );
    int toString( char * dstBuf, String const & v );
    //template<int len>
    //int toString( char * dstBuf, char const ( &v )[ len ] )
    //{
    //    memcpy( dstBuf, v, len - 1 );
    //    return len - 1;
    //}



    template<typename T>
    void fillCore( char * & buf, int & offset, T const & v )
    {
        offset += toString( buf + offset, v );
    }

    template<typename T, typename ...TS>
    void fillCore( char * & buf, int & offset, T const & v, TS const & ...vs )
    {
        fillCore( buf, offset, v );
        fillCore( buf, offset, vs... );
    }

    // fill �����ڸ������ char * buffer ƴ�ִ�, ������ǰԤ���㳤��. �� assert ����ֵ < len
    template<typename ...TS>
    int fill( char * buf, TS const & ...vs )
    {
        int offset = 0;
        fillCore( buf, offset, vs... );
        buf[ offset ] = '\0';
        return offset;
    }



    // HEX ��
    //

    int toHexString( char * dstBuf, uint8   v );
    int toHexString( char * dstBuf, uint16  v );
    int toHexString( char * dstBuf, uint    v );
    int toHexString( char * dstBuf, uint64  v );
    // todo: byte[], string to hex
    int toHexString( char * dstBuf, int8   v );
    int toHexString( char * dstBuf, int16  v );
    int toHexString( char * dstBuf, int    v );
    int toHexString( char * dstBuf, int64  v );


    template<typename T>
    void fillHexCore( char * & buf, int & offset, T const & v )
    {
        offset += toHexString( buf + offset, v );
    }

    template<typename T, typename ...TS>
    void fillHexCore( char * & buf, int & offset, T const & v, TS const & ...vs )
    {
        fillHexCore( buf, offset, v );
        fillHexCore( buf, offset, vs... );
    }

    template<typename ...TS>
    int fillHex( char * buf, TS const & ...vs )
    {
        int offset = 0;
        fillHexCore( buf, offset, vs... );
        buf[ offset ] = '\0';
        return offset;
    }











    // �ִ�תֵ��ֵ������Ҫ�� 0 Ϊ��β�Ҳ��������߰���� �ո�, TAB ���Ű����
    void fromString( uint8   & dstVar, char const * s );
    void fromString( uint16  & dstVar, char const * s );
    void fromString( uint32  & dstVar, char const * s );
    void fromString( uint64  & dstVar, char const * s );
    void fromString( int8    & dstVar, char const * s );
    void fromString( int16   & dstVar, char const * s );
    void fromString( int     & dstVar, char const * s );
    void fromString( int64   & dstVar, char const * s );
    void fromString( double  & dstVar, char const * s );
    void fromString( float   & dstVar, char const * s );
    void fromString( bool    & dstVar, char const * s );






    // calc lead zero ���� 2 ��������λͷ�ϵ� 0 �ĸ���
#ifdef __GNUC__
    INLINE int clz( size_t x )
    {
        return __builtin_clz( x );
    }
#elif defined(__IA) && defined(_MSC_VER)
#include <intrin.h>
    INLINE int clz( size_t x )
    {
        unsigned long r = 0;
#ifdef __X64
        _BitScanReverse64( &r, x );
        return 63 - r;
#else
        _BitScanReverse( &r, x );
        return 31 - r;
#endif
    }
#else
#ifdef __X64
#error not support
#endif
    INLINE int popcnt( size_t x )
    {
        x -= ( ( x >> 1 ) & 0x55555555 );
        x = ( ( ( x >> 2 ) & 0x33333333 ) + ( x & 0x33333333 ) );
        x = ( ( ( x >> 4 ) + x ) & 0x0f0f0f0f );
        x += ( x >> 8 );
        x += ( x >> 16 );
        return x & 0x0000003f;
    }
    //INLINE int ctz( size_t x ) {
    //    return popcnt((x & -x) - 1);
    //}
    INLINE int clz( size_t x )
    {
        x |= ( x >> 1 );
        x |= ( x >> 2 );
        x |= ( x >> 4 );
        x |= ( x >> 8 );
        x |= ( x >> 16 );
        return 32 - popcnt( x );
    }
#endif


    // ���ظպô��� x �� 2^n ��ֵ�����ڴ����
    // ��� x ���� 2^n ϵ �򷵻�ԭֵ
    INLINE size_t round2n( size_t len )
    {
#ifdef __X64
        int bits = 63 - clz( len );
#else
        int bits = 31 - clz( len );
#endif
        size_t rtv = size_t( 1 ) << bits;
        if( rtv == len ) return len;
        return rtv << 1;
    }

    // ��һ������ 2 �ļ��η�
    INLINE size_t calc2n( size_t len )
    {
#ifdef __X64
        return 63 - clz( len );
#else
        return 31 - clz( len );
#endif
    }






    template<typename T>
    bool isValueType()    // ���������㣬ö�٣�ָ��
    {
        return std::is_enum<T>::value || std::is_integral<T>::value || std::is_pointer<T>::value || std::is_floating_point<T>::value;
    }







    int getHash_CS( byte const* buf, int len );             // �� x64 �� 8 �ֽ� �����ڴ� �ϳ���( ���ƾ��� 8+ ) ���ܸ���
    int getHash_Lua( byte const* buf, int len );            // 32 λ �� ��С�ִ�ʱ����

    template<typename T>
    INLINE int getHashCode( T const &in )
    {
        if( sizeof( T ) == 1 )
            return ( (byte*)&in )[ 0 ];
        if( sizeof( T ) == 2 )
            return ( (byte*)&in )[ 0 ] || ( ( (byte*)&in )[ 1 ] << 8 );
        if( sizeof( T ) == 3 )
            return ( (byte*)&in )[ 0 ] || ( ( (byte*)&in )[ 1 ] << 8 ) || ( ( (byte*)&in )[ 2 ] << 16 );
        if( sizeof( T ) == 4 )
            return *(int*)&in;
        return getHash_Lua( (byte const*)&in, sizeof( T ) );
    }
    INLINE int getHashCode( String const & in )
    {
        if( sizeof( size_t ) == 8 && in.size() >= 8 && ( (size_t)in.c_str() % 8 == 0 ) )
            return getHash_CS( (byte const*)in.c_str(), (int)in.size() );
        else
            return getHash_Lua( (byte const*)in.c_str(), (int)in.size() );
    }
    INLINE int getHashCode( std::string const & in )
    {
        if( sizeof( size_t ) == 8 && in.size() >= 8 && ( (size_t)in.c_str() % 8 == 0 ) )
            return getHash_CS( (byte const*)in.c_str(), (int)in.size() );
        else
            return getHash_Lua( (byte const*)in.c_str(), (int)in.size() );
    }
    INLINE int getHashCode( HashString const & in )
    {
        return in._h;
    }
    INLINE int getHashCode( HashString* const & in )
    {
        return in->_h;
    }
    // todo: more type here



    // תָ��Ƚ�Ϊָ��ָ������ݱȽ�
    template<typename T>
    INLINE bool equalsTo( T const& a, T const& b )
    {
        return a == b;
    }
    template<>
    INLINE bool equalsTo( HashString* const& a, HashString* const& b )
    {
        return *a == *b;
    }
    template<>
    INLINE bool equalsTo( String* const& a, String* const& b )
    {
        return *a == *b;
    }



    // �õ��պ�С�� n ������ �������ڴ����
    int getPrime( int n );












    template<typename T>
    int getSizeCore( T const& v ) { return sizeof( T ); }
    inline int getSizeCore( String const& v ) { return v.size() + sizeof( int ); };
    inline int getSizeCore( FlatBuffer const& v ) { return v.size() + sizeof( int ); };

    template<typename T>
    void getSizeCore( int& len, T const& v )
    {
        len += getSizeCore( v );
    }

    template<typename T, typename ...TS>
    void getSizeCore( int& len, T const & v, TS const & ...vs )
    {
        getSizeCore( len, v );
        getSizeCore( len, vs... );
    }

    template<typename ...TS>
    int getSize( TS const & ...vs )         // ����ͳ�� binary ����
    {
        int len = 0;
        getSize( len, vs... );
        return len;
    }







    template<typename T>
    INLINE void binaryWrite( char* dest, T const& src )
    {
#ifdef __IA
        *(T*)dest = src;
#else
        auto p = (char*)&src;
        if( sizeof( T ) == 1 )
        {
            dest[ 0 ] = p[ 0 ];
        }
        if( sizeof( T ) == 2 )
        {
            dest[ 1 ] = p[ 1 ];
        }
        if( sizeof( T ) == 4 )
        {
            dest[ 2 ] = p[ 2 ];
            dest[ 3 ] = p[ 3 ];
        }
        if( sizeof( T ) == 8 )
        {
            dest[ 4 ] = p[ 4 ];
            dest[ 5 ] = p[ 5 ];
            dest[ 6 ] = p[ 6 ];
            dest[ 7 ] = p[ 7 ];
        }
#endif
    }
    void binaryWrite( char* dest, String const & src );
    void binaryWrite( char* dest, FlatBuffer const & src );



    template<typename T>
    INLINE bool binaryRead( T& dest, char const* src, int len )
    {
        if( len < sizeof( T ) ) return false;
#ifdef __IA
        dest = *(T*)src;
#else
        auto p = (char*)&dest;
        if( sizeof( T ) == 1 )
        {
            p[ 0 ] = src[ 0 ];
        }
        if( sizeof( T ) == 2 )
        {
            p[ 1 ] = src[ 1 ];
        }
        if( sizeof( T ) == 4 )
        {
            p[ 2 ] = src[ 2 ];
            p[ 3 ] = src[ 3 ];
        }
        if( sizeof( T ) == 8 )
        {
            p[ 4 ] = src[ 4 ];
            p[ 5 ] = src[ 5 ];
            p[ 6 ] = src[ 6 ];
            p[ 7 ] = src[ 7 ];
        }
#endif
        return true;
    }
    bool binaryRead( String& dest, char const* src, int len );
    bool binaryRead( FlatBuffer& dest, char const* src, int len );







    void binaryDumpCore( String & s, char const * buf, int len );
    void binaryDump( String & s, char const * buf, int len );


}


#endif
