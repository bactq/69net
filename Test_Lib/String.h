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
    // prepare
    explicit String( int capacity = 64 );

    // ref
    String( char* buf, int bufLen, int dataLen = 0 );

    // copy  // todo
    String( char const* s );

    // copy  // todo
    String( String const & other );

    // move
    String( String && other );

    // copy
    String& operator=( String const & other );

    // move
    String& operator=( String && other );

    // del buf
    ~String();

    // prepare memory
    void reserve( int len );

    // set _dataLen to 0
    void clear();

    // return _buf
    char* c_str();


    void toLower();
    void toUpper();

    // faster than toLower() 4x speed
    // careful these chars��   @[\]^_` 
    void toLowerUnsafe();

    template<typename ...TS>
    void append( TS const & ...vs );

    // todo: more util funcs
    // todo: more operator like ==, > < ...

private:
    typedef void ( String::*Disposer )( );
    void disposeIncommingBuffer();
    void disposePoolBuffer();
    void disposeNewBuffer();

    char*       _buf;
    int         _bufLen;
    int         _dataLen;
    Disposer    _disposer;
};

#include "String.hpp"


#endif
