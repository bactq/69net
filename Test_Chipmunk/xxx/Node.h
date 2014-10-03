#ifndef __NODE_H__
#define __NODE_H__

namespace xxx
{
    struct Point
    {
        float x, y;
    };
    struct Size
    {
        float width, height;
    };
    struct Color3b
    {
        GLubyte r, g, b;
    };
    struct Color4b
    {
        GLubyte r, g, b, a;
    };


    struct Node : public Ref
    {
        Size    size;               // ����( �߼�����������������Ҫ����� )
        Point   dock;               // ������( 0 ~ 1 �ٷֱȣ������ parent size )
        Point   anchor;             // ê��( 0 ~ 1 �ٷֱ�, ����� size )
        Point   offset;             // ������������ê��ƫ������
        Color4b color;
        // todo: scale, angle  (matrix?) shader? blend?

        bool    dirty;              // ����( Ĭ�ϻ�Ӱ�쵽�� )
        Point   position;           // ʵ�ʻ����õ�ȫ������( draw ʱ dirty ��� )

#ifdef USE_STL
        vector<Node*> childs;
#else
        List<Node*> childs;
#endif
        Node* parent = nullptr;

        // return new Node();
        static Node* Create();

        // override β�� call Node::Draw �Դ��� childs
        virtual void Draw( int durationTicks );

        virtual void Add( Node* child );
        virtual void Remove( Node* child );
        void RemoveFromParent();
        virtual void Clear();       // remove all childs

        // �� parent Add �����øú���
        virtual void Added();
        // �� parent Remove �����øú���( ����������ü����Ļ� )
        virtual void Removed();

    protected:
        Node();
        ~Node();
    };
}

#endif
