#ifndef __UTILS_H__
#define __UTILS_H__


namespace Utils
{

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

    // һЩ����дģ��Ĳ���
    int toString( char * dstBuf, std::string const & v );
    template<int len>
    int toString( char * dstBuf, char const ( &v )[ len ] )
    {
        memcpy( dstBuf, v, len - 1 );
        return len - 1;
    }

    

    template<typename T>
    void fillCore( char * & buf, int & offset, T const & v )
    {
        offset += toString( buf + offset, v );
    }

    template<typename T, typename ...TS>
    void fillCore( char * & buf, int & offset, T const & v, TS const & ...vs )
    {
        offset += toString( buf + offset, v );
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
        offset += toHexString( buf + offset, v );
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



}


#endif
