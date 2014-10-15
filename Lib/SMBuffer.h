#ifndef _SMBUFFER_H__
#define _SMBUFFER_H__

namespace xxx
{


    // ���ṹ�弰�ڴ�ȫ�������� SharedMemory �ϵ�һ��ѭ�� buffer
    // ʹ�÷�����ֱ�ӽ� SM �ڴ�ָ�� Ӳת��������ָ��
    // SM �����߻������ Init ������ʼ��
    // ����һ��һд��������/�߳�ͬʱʹ��
    class SMBuffer
    {
    public:
        void Init( int bufLen );
        int Write( char const* buf, int len );  // ����ʵ��д�볤��
        int Read( char* buf, int len );         // ����ʵ�ʶ�ȡ����
        int Copy( char* buf, int len );         // ����ʵ�ʸ��Ƴ���
        int Skip( int len );                    // ����ʵ����������
        void Clear();
        bool Empty() const;
        int Size() const;
        int Space() const;

    private:
        SMBuffer() = delete;
        SMBuffer( SMBuffer const& other ) = delete;
        SMBuffer& operator=( SMBuffer const& other ) = delete;

        std::atomic<int> _size;
        int _bufLen, _ro, _wo;
        char _buf[ 1 ];     // point to buffer space
    };


}

#endif
