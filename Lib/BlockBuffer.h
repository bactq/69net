#ifndef _CIRCLEBUFFER_H__
#define _CIRCLEBUFFER_H__

namespace xxx
{

    // ��ʽ���ݵ��Ƚ��ȳ�ѭ����д��( �����ڷ�����ɶ�� )
    class BlockBuffer : Memmoveable
    {
    public:
        explicit BlockBuffer( Pool& p );
        ~BlockBuffer();
        void Write( char const* buf, int len );
        int Read( char* buf, int len );             // �Ը���ָ�����ȵ� buf ���ƶ���ָ��. ����ʵ�ʸ��Ƶĳ���
        void Copy( char* buf, int len );            // ����ָ�����ȵ� buf, ��� size ��������
        void Clear();
        bool Empty() const;
        int Size() const;                           // δ��֮���ݳ�
    private:
        BlockBuffer( BlockBuffer const& o ) = delete;
        BlockBuffer& operator=( BlockBuffer const& o ) = delete;
        struct Page
        {
            Page*   next;
            char    data[ 1 ];
        };
        Page*       wp;                             // ������д��ҳ
        Page*       rp;                             // �����ڶ���ҳ
        int         wpLen;                          // ������д��ҳ����д����
        int         rpLen;                          // �����ڶ���ҳ���Ѷ�����
        int         size;                           // ����ҳʣ�೤�� + ��дҳ��д���� + �м�����ҳ�� * ҳsize
        Pool*       pool;
    };

}

#endif
