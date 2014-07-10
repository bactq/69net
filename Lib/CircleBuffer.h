#ifndef _CIRCLEBUFFER_H__
#define _CIRCLEBUFFER_H__

// ��ʽ���ݵ��Ƚ��ȳ�ѭ����д��( �����ڷ�����ɶ�� )
class CircleBuffer
{
public:
    explicit CircleBuffer( Pool& p );
    ~CircleBuffer();
    void write( char const* buf, int len );
    int read( char* buf, int bufLen );          // �Ը���ָ�����ȵ� buf ���ƶ���ָ��. ����ʵ�ʸ��Ƶĳ���
    void copy( char* buf, int len );            // ����ָ�����ȵ� buf, ��� size ��������
    void clear();
    bool empty() const;
    int size() const;                           // δ��֮���ݳ�
private:
    struct Page
    {
        Page*   next;
        char    data[ 1 ];
    };
    Page*       _wp;                            // ������д��ҳ
    Page*       _rp;                            // �����ڶ���ҳ
    int         _wpLen;                         // ������д��ҳ����д����
    int         _rpLen;                         // �����ڶ���ҳ���Ѷ�����
    int         _size;                          // ����ҳʣ�೤�� + ��дҳ��д���� + �м�����ҳ�� * ҳsize
    Pool&       _pool;
};


#endif
