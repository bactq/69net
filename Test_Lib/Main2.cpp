#include "Lib/All.h"
#include <conio.h>
using namespace std;
using namespace xxx;

/*

todo:

���ֵ��͹ֵ� AI:
1. warrior
2. archer
3. priest

ְҵ�ص㣺
warrior ��ս������������Χ��С( ������ )
archer Զ�̣�����������Χ�ϴ󣬼�ʸ����ɳ�( ������ )
priest Զ�̣��������Ʒ�Χ�еȣ����÷�ʽΪ������Ŀ�����ϵ����Ʒ���Ч��( ˲���� )

�ϸ���˵����Ŀ�����þ�����˵���ǲ���ʲôԶ�̽�ս�ģ���սҲ�п��ܲ���Ͷ����Ϊ
����ʲô paladin ����̹����ְҵ�����ڸ����ͣ���ǰ�Ȳ���
�����趨����Ϊ���������ͣ�����Ҳֻ��һ�֣��Է���ʵ���ر�� AI Ч��

�����ԣ�
�뾶: ��ɫռ�����, �����Ͻ�Ҳ�������ҽӵ�ļ�������           ��λ����       ����ֵ��0.5
Ѫ��: ����������ֵ                                             ��λ����       ����ֵ��100
����: ��Ϊ����ǿ������ֵ                                       ��λ����       ����ֵ��100
��Ұ: ���䷶Χ��������Ϊ��һ�����ɫ�ص���Բ�İ뾶             ��λ����       ����ֵ��10
����: ÿ���ƶ�����                                             ��λ����       ����ֵ��3.5

�������ԣ�
��ǿ: ��Ϊ��������ֵ( ����ǿ�ȹ�ʽ�� ��ǿ * ����/100 )         ��λ����       ����ֵ��20
����: ÿ���Ӳ��� ��Ϊ�Ĵ���                                    ��λ����       ����ֵ��1
���: ��Ϊ������Ŀ���������( ͨ���� ��Ұ С���� )           ��λ����       ����ֵ��5

AI��أ�
���У���������/������Ϊ�ľ�����ֵ( ��̵İٷֱ� )              ��λ���ٷֱ�   ����ֵ��80%
���У�����������Ϊ�ı���Ѫ���ٷֱ�( �ݶ� )                     ��λ���ٷֱ�   ����ֵ��20%
���У�����������Ϊ�� ���Ŀ�� ��һ���˺�Ѫ���ٷֱ�( �ݶ� )     ��λ���ٷֱ�   ����ֵ��10%

����"����"��
�����ϵ�� AI ��ѡ�����Ϊʱ����
��Ŀ�������̵�˲�� ��ֹ����ʱ��AI �������� ��Զ�����չ����Ϊ��
���ǻ�Ҫ�ٽӽ�һЩ���Ծ���ȷ�� ����Ŀ����ʱ���Ȼ�������ˣ�Ҳ���ǹ����š�
Ϊȷ��������Ч�ԣ�������������Ա��Ե� AI �� "Ԥ��"��

���絶�� 1 �ף�Ŀ�����ĵ� ����λ�����Լ������ҽӵ�� 1�� + Ŀ��뾶  ���ڣ�
��ʱ��� ����( ���и�����ӿ��Ĺ��� )���ȵ�������������Ч�ж��׶Σ�ʱ��
Ŀ������Ѿ� �Ƴ��� ���Ĺ������룬���Ǳ㿳����Ŀ���ˣ�����Ŀ�겻���������Լ��ƶ���û���ܵ��ҽӵ㱳��

��һ�����Ϊ����Ŀ�������̵�˲�� �������ƶ��ģ���ȡ��Ŀ����ٶȣ�
��ʱ�򣬿ɽ�� ���� �������������ǰ�������� "����"��
��Ȼ��͵���������ǣ�ÿ�ζ����� "����" ��������Ϊ�������������Ҳ���ñ����ȶ�������������

����"����"��
AI �ж�Ѫ�ٵ�һ���̶ȵ� �Ա���Ϊ����Ҳ�п��ܳ��� "�Ӳ���" ���������ʱ��ɴ಻���ˡ�
����һ������ǣ�����Է��� "��һ��" ����Ҫ���Լ������������Ͻ�Ҳ����ѡ����
�����ӻ��ӣ����ܻ�Ҫ�ٽ�һ���Ļ��ֳ� �Ը� ɶ�����Գ�������һ����ָ��ѡ��

����"����"��
AI �ж� ���Լ���Ŀ�꣬��һ�� �����Լ���������Ѫ����һ���ٷֱ�ֵ��
���ӵõ�������£� AI ѡ�� ����
Ӱ�� ���� ѡ������أ�����������Ϊ�������� ����ֵ

���� archer ������ �ŷ��ݣ�����ԭ��Ӳƴ����ͺ� ����ֵ �Լ�������û���� ���Լ��Է��Ĺ����� ���й�ϵ

��ν"�ӵõ�":
�����߽����Լ�����Ұ���Լ���δ���빥���ߵ����ʱ
�����Լ������ٿ��������ʱ


ע������ֵ�Ķ�����Χ��ǰ������

*/


struct Foo;
struct FooState1 : public CorStateBase
{
    Foo* owner;
    String name;
    int sleepTicks = 0;
    FooState1( Foo* owner );
    void Init( String const& name, int sleepTicks );
    bool Process( int ticks ) override;
};
struct FooState2 : public CorStateBase
{
    Foo* owner;
    String name;
    int sleepTicks = 0;
    FooState2( Foo* owner );
    void Init( String const& name, int sleepTicks );
    bool Process( int ticks ) override;
};
struct Foo : public CorBase, public AutoIDAttacher < Foo, CorBase >
{
    CorStateBase* s = nullptr;
    String name;
    FooState1 s1;
    FooState2 s2;
    Foo();
    void Init( String const& name, int sleepTicks );
    bool Process( int ticks ) override;
};

struct Bar : public CorBase, public AutoIDAttacher < Bar, CorBase >
{
    void Init( String const& name );
    bool Process( int ticks ) override;
    String name;
    List<PoolPtr<Foo>> childs;
    void AddChild( Foo* foo );
};




Foo::Foo()
    : s1( this )
    , s2( this )
{
}
void Foo::Init( String const& name, int sleepTicks )
{
    this->name = name;
    s1.Init( String::Make( name, "_state_1" ), sleepTicks );
}
bool Foo::Process( int ticks )
{
    CoutLine( name, " Process" );
    s->Process( ticks );
    return s != nullptr;
}




FooState1::FooState1( Foo* owner )
{
    this->owner = owner;
}
void FooState1::Init( String const& name, int sleepTicks )
{
    this->name = name;
    this->sleepTicks = sleepTicks;
    owner->s = this;
}
bool FooState1::Process( int ticks )
{
    COR_BEGIN;
    CoutLine( name, " Process: before sleep( ", sleepTicks, " )" );
    COR_SLEEP( sleepTicks );
    CoutLine( name, " Process: sleeped" );
    owner->s2.Init( String::Make( name, "_state_2" ), sleepTicks );
    COR_END;
}




FooState2::FooState2( Foo* owner )
{
    this->owner = owner;
}
void FooState2::Init( String const& name, int sleepTicks )
{
    this->name = name;
    this->sleepTicks = sleepTicks;
    owner->s = this;
}
bool FooState2::Process( int ticks )
{
    COR_BEGIN;
    CoutLine( name, " Process: before sleep( ", sleepTicks, " )" );
    COR_SLEEP( sleepTicks );
    CoutLine( name, " Process: sleeped" );
    owner->s = nullptr;
    COR_END;
}






void Bar::Init( String const& name )
{
    this->name = name;
}

bool Bar::Process( int ticks )
{
    CoutLine( name, " Process begin" );
    Cout( "alive childs: {" );
    for( int i = childs.Size() - 1; i >= 0; --i )
    {
        auto& c = childs[ i ];
        if( auto p = c.Ptr() )
        {
            Cout( p->name, ", " );
        }
        else
        {
            childs.EraseFast( i );
        }
    }
    CoutLine( "}" );
    CoutLine( name, " Process end" );
    return true;
}

void Bar::AddChild( Foo* foo )
{
    childs.Push( foo );
}




int main()
{
    CorManager cm;
    auto b = cm.CreateItem<Bar>( "bar" );
    b->AddChild( cm.CreateItem<Foo>( "foo1", 1 ) );
    b->AddChild( cm.CreateItem<Foo>( "foo2", 2 ) );
    b->AddChild( cm.CreateItem<Foo>( "foo3", 3 ) );
    while( cm.Process() )
    {
        _getch();
        CoutLine();
    };
    system( "pause" );
    return 0;
}
