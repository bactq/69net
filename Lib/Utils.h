#ifndef __UTILS_H__
#define __UTILS_H__

namespace Utils
{
    // ȡ����ϵ�У�ֵΪ toString ������. 10 ���ơ�
    int getToStringMaxLength( uint8   v );
    int getToStringMaxLength( uint16  v );
    int getToStringMaxLength( uint    v );
    int getToStringMaxLength( uint64  v );
    int getToStringMaxLength( int8    v );
    int getToStringMaxLength( int16   v );
    int getToStringMaxLength( int     v );
    int getToStringMaxLength( int64   v );
    int getToStringMaxLength( double  v );
    int getToStringMaxLength( float   v );
    int getToStringMaxLength( bool    v );
    int getToStringMaxLength( char const* v );
    int getToStringMaxLength( String const & v );
    int getToStringMaxLength( std::string const & v );

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
    // todo: more type here




    // �õ��պ�С�� n ������ �������ڴ����
    int getPrime( int n );

}


#endif
