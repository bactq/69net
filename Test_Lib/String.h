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
    explicit String( int capacity = 64 );
    template<size_t len>
    String( char( &buffer )[ len ] );
    String( char* buffer, int len );
    // todo: more copy constructor
    // todo: more operator
    ~String();
    void reserve( int len );
    char* c_str();

    template<typename ...TS>
    void append( TS const & ...vs );

    // todo: more util funcs

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
