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
        float w, h;
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
        Point   pos;                // ʵ�ʻ����õ�ȫ������( draw ʱ dirty ��� )

#ifdef USE_STL
        vector<Node*> childs;
#else
        List<Node*> childs;
#endif
        Node* parent = nullptr;

        // return new Node();
        static Node* Create();

        virtual void Draw( int _durationTicks );    // override β�� call Node::Draw �Դ��� childs

        virtual void Add( Node* _child );
        virtual void Added();       // �� parent Add �����øú���

        virtual void Remove( Node* _child );
        virtual void Removed();     // �� parent Remove �����øú���( ����������ü����Ļ� )
        void RemoveFromParent();
        virtual void Clear();       // remove all childs

    protected:
        Node();
        ~Node();
    };
}

#endif
