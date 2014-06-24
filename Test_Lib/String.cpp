#include "../Lib/All.h"
#include "String.h"


String::String( char* buffer, int bufLen, int dataLen )
    : _buffer( buffer )
    , _bufferLength( bufLen )
    , _dataLength( dataLen )
    , _disposer( nullptr )
{
}

// todo: �����أ� ע���滻ÿ���� 1 �� new ��λ��
// todo: if capacity <= pool.item.buffer.size ��� pool ����, _disposer ָ�� disposePoolBuffer
String::String( int capacity /*= 64 */ )
{
    if( capacity <= 64 ) _bufferLength = 64;
    else _bufferLength = (int)Utils::round2n( capacity );
    _disposer = &String::disposeNewBuffer;
    _dataLength = 0;
    //std::cout << "String::String( int capacity ) new char[ " << _bufferLength << " ]\n";
    _buffer = new char[ _bufferLength ];
}

String::String( String const & other )
    : String( other._buffer, (int)Utils::round2n( other._dataLength ), other._dataLength )
{
}

String::String( String && other )
    : _buffer( other._buffer )
    , _bufferLength( other._bufferLength )
    , _dataLength( other._dataLength )
    , _disposer( other._disposer )
{
    //other._buffer = nullptr;  // �����в���Ҫ
    //other._bufferLength = 0;
    //other._dataLength = 0;
    other._disposer = nullptr;
}

String::~String()
{
    if( _disposer ) ( this->*_disposer )( );
}

void String::reserve( int len )
{
    if( len < _bufferLength ) return;
    len = (int)Utils::round2n( len + 1 );
    _bufferLength = len;
    //std::cout << "void String::reserve( int len ) new char[ " << len << " ]\n";
    auto newBuffer = new char[ len ];
    memcpy( newBuffer, _buffer, _dataLength + 1 );
    if( _disposer ) ( this->*_disposer )( );
    _disposer = &String::disposeNewBuffer;
    _buffer = newBuffer;
}

void String::clear()
{
    _dataLength = 0;
    _buffer[ 0 ] = '\0';
}

char* String::c_str()
{
    return _buffer;
}

void String::disposeIncommingBuffer() {}

void String::disposePoolBuffer()
{
    // todo
}

void String::disposeNewBuffer()
{
    //std::cout << "delete[] _buffer\n";
    delete[] _buffer;
}

String& String::operator=( String const & other )
{
    if( _bufferLength > other._dataLength )
    {
        _dataLength = other._dataLength;
        memcpy( _buffer, other._buffer, other._dataLength + 1 );
    }
    else
    {
        if( _disposer ) ( this->*_disposer )( );
        _bufferLength = (int)Utils::round2n( other._dataLength + 1 );
        _disposer = &String::disposeNewBuffer;
        //std::cout << "String& String::operator=( String const & other ) new char[ " << _bufferLength << " ]\n";
        _buffer = new char[ _bufferLength ];
        _dataLength = other._dataLength;
        memcpy( _buffer, other._buffer, other._dataLength + 1 );
    }
    return *this;
}

String& String::operator=( String && other )
{
    if( _disposer ) ( this->*_disposer )( );
    _buffer = other._buffer;
    _bufferLength = other._bufferLength;
    _dataLength = other._dataLength;
    _disposer = other._disposer;
    //other._buffer = nullptr;  // �����в���Ҫ
    //other._bufferLength = 0;
    //other._dataLength = 0;
    other._disposer = nullptr;
    return *this;
}


static byte const lowerchars[] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};
static byte const upperchars[] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

void String::toLower()
{
    for( size_t i = 0; i < _dataLength; ++i ) _buffer[ i ] = lowerchars[ _buffer[ i ] ];
}

void String::toUpper()
{
    for( size_t i = 0; i < _dataLength; ++i ) _buffer[ i ] = upperchars[ _buffer[ i ] ];
}

void String::toLowerUnsafe()
{
    // todo
}

void String::toUpperUnsafe()
{
    // todo
}
