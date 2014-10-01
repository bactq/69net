#ifndef __NODE_H__
#define __NODE_H__

class Node : public Ref
{
public:
    // return new Node();
    static Node* create();

    // override β�� call Node::Draw �Դ��� childs
    virtual void Draw( int durationTicks );

    // override ͷ�� call Node::Draw �Դ��� add ����
    virtual void Add( Node* child );

    // override ͷ���� bool alive = child->refCount > 1, �� call Node::Remove, ����� if( alive ) {  code ... }
    virtual void Remove( Node* child );

    // remove all childs
    void Clear();

    void RemoveFromParent();

protected:
    Node() = default;
    ~Node();

#ifdef USE_STL
    vector<Node*> childs;
#else
    List<Node*> childs;
#endif
    Node* parent = nullptr;

};

#endif
