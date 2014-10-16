#ifndef _CIRCLEBUFFER_H__
#define _CIRCLEBUFFER_H__

namespace xxx
{

    // ��ʽ���ݵ��Ƚ��ȳ�ѭ����д��( �����ڷ�����ɶ�� )
    class BlockBuffer : Memmoveable
    {
    public:
        explicit BlockBuffer( Pool& _p );
        ~BlockBuffer();
        void Write( char const* _buf, int _len );
        int Read( char* _buf, int _len );           // �Ը���ָ�����ȵ� buf ���ƶ���ָ��. ����ʵ�ʸ��Ƶĳ���
        void Copy( char* _buf, int _len );          // ����ָ�����ȵ� buf, ��� size ��������
        void Clear();
        bool Empty() const;
        int Size() const;                           // δ��֮���ݳ�
    private:
        BlockBuffer( BlockBuffer const& _other ) = delete;
        BlockBuffer& operator=( BlockBuffer const& _other ) = delete;
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
