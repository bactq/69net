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
        void Init( int _bufLen );
        int Write( char const* _buf, int len );     // ����ʵ��д�볤��
        int Read( char* _buf, int len );            // ����ʵ�ʶ�ȡ����
        int Copy( char* _buf, int len );            // ����ʵ�ʸ��Ƴ���
        int Skip( int len );                        // ����ʵ����������
        void Clear();
        bool Empty() const;
        int Size() const;
        int Space() const;

    private:
        SMBuffer() = delete;
        SMBuffer( SMBuffer const& o ) = delete;
        SMBuffer& operator=( SMBuffer const& o ) = delete;

        std::atomic<int> size;
        int bufLen, ro, wo;
        char buf[ 1 ];                              // point to buffer space
    };


}

#endif
