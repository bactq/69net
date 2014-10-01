#ifndef __NODEBASE_H__
#define __NODEBASE_H__

namespace xxx
{
    class NodeBase : public Ref
    {
    public:

#ifdef USE_STL
        vector<NodeBase*> childs;
#else
        List<NodeBase*> childs;
#endif
        NodeBase* parent = nullptr;

        // return new Node();
        static NodeBase* Create();

        // override β�� call Node::Draw �Դ��� childs
        virtual void Draw( int durationTicks );

        // override ͷ�� call Node::Draw �Դ��� add ����
        virtual void Add( NodeBase* child );

        // override ͷ���� bool alive = child->refCount > 1, �� call Node::Remove, ����� if( alive ) {  code ... }
        virtual void Remove( NodeBase* child );

        // remove all childs
        void Clear();

        void RemoveFromParent();

    protected:
        NodeBase() = default;
        ~NodeBase();
    };
}

#endif
