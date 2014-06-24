#ifndef _STRING_H__
#define _STRING_H__

// ֧�ִ��붨�� buffer ��Ϊ����
// todo:
// ���ָ��ƹ���ɶ��
// ֧�ָ��ֳ��� �ִ� ����
// ֧�� 2���� ƴ�� ��Ϊ
// Utils Ҫ�� 2���� ƴ����غ���
// �Ƶ� Lib ֮��Utils Ҫ�� String ������ĺ���
class String
{
public:
    String( char* buffer, int bufLen, int dataLen = 0 );
    explicit String( int capacity = 64 );

    template<size_t bufLen>
    String( char( &buffer )[ bufLen ], int dataLen = 0 );

    template<size_t len>
    String( char const( &buffer )[ len ] );

    String( String const & other );
    String( String && other );

    String& operator=( String const & other );
    String& operator=( String && other );

    ~String();
    void reserve( int len );
    void clear();
    char* c_str();

    // todo: more operator like ==, > < ...

    template<typename ...TS>
    void append( TS const & ...vs );

    // todo: more util funcs
    void toLower();
    void toUpper();
    void toLowerUnsafe();
    void toUpperUnsafe();

private:
    typedef void ( String::*Disposer )( );
    void disposeIncommingBuffer();
    void disposePoolBuffer();
    void disposeNewBuffer();

    char*       _buffer;
    int         _bufferLength;
    int         _dataLength;
    Disposer    _disposer;
};

#include "String.hpp"


#endif
