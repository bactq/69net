#ifndef _SMBUFFER_H__
#define _SMBUFFER_H__

// ���ṹ�弰�ڴ�ȫ�������� SharedMemory �ϵ�һ��ѭ�� buffer
// ʹ�÷�����ֱ�ӽ� SM �ڴ�ָ�� Ӳת��������ָ��
// SM �����߻������ init ������ʼ��
class SMBuffer
{
public:
    void init( int bufLen );
    int write( char const* buf, int len );  // ����ʵ��д�볤��
    int read( char* buf, int len );         // ����ʵ�ʶ�ȡ����
    int copy( char* buf, int len );         // ����ʵ�ʸ��Ƴ���
    int skip( int len );                    // ����ʵ����������
    void clear();
    bool empty() const;
    int size() const;
    int space() const;

private:
    SMBuffer() = delete;
    SMBuffer( SMBuffer const& other ) = delete;
    SMBuffer& operator=( SMBuffer const& other ) = delete;

    std::atomic<int> _size;
    int _bufLen, _ro, _wo;
    char _buf[ 1 ];     // point to buffer space
};

#endif
