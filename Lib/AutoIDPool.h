#ifndef __AUTOIDPOOL_H__
#define __AUTOIDPOOL_H__

namespace xxx
{
    /*
ʾ����


// ���࣬����߱����нӿ�Ҫ��
struct FooBase
{
    int typeId = 0;                     // ��������id. ���������� new ʱ���
    CorCore() = default;                // Ĭ�Ϲ��캯��
    virtual ~FooBase();                 // �������������
    // void Init( ... );                // ����������ʵ��. ����Ϊ���캯��
    inline virtual void Destroy() {}    // ����Ϊ��������
}


// Ϊ�˸��������͸���������id, ʵ��һ��ģ���Լ̳�. Ҳ���Ը��ƴ�����ú�ɶ��
template<typename T>
struct Foo : public FooBase
{
    static const AutoID<FooBase> typeId;
    Foo()
    {
        FooBase::typeId = this->typeId.value;
    }
};
template<typename T>
const AutoID<FooBase> Foo<T>::typeId;


// ����Լ�����ض���ÿ�γ�ʼ���� Init �����������ɱ�
struct Foo1 : Foo<Foo1> 
{
    void Init( ... );
}
struct Foo2 : Foo<Foo2>
{
    void Init( ... );
    void Destroy() override {}
}
...


BufPool<Foo> p;
auto f1 = p.Alloc<Foo1>( ... );
auto f2 = p.Alloc<Foo2>( ... );
...
p.Free( f1 );
p.Free( f2 );
    */


    // ��Ϊ��̬��Ա���� class, �ṩ���� type id �����ɹ���
    template<typename T>
    struct AutoID
    {
        int value;
        static int maxValue;
        AutoID() { value = maxValue++; }
    };
    template<typename T>
    int AutoID<T>::maxValue = 0;


    // T �Ľӿ�����ο�ʾ��
    template<typename T>
    struct BufPool
    {
        List<List<T*>> data;

        BufPool()
        {
            data.Resize( AutoID<T>::maxValue + 1 );
        }
        ~BufPool()
        {
            Clear();
        }

        template<typename CT, typename ...PTS>
        CT* Alloc( PTS&& ...ps )
        {
            CT* rtv;
            auto& os = data[ CT::typeId.value ];
            if( os.Size() )
                rtv = (CT*)os.TopPop();
            else
                rtv = new CT();
            rtv->Init( std::forward<PTS>( ps )... );
            return rtv;
        }

        void Free( T* o )
        {
            o->Destroy();
            data[ o->typeId ].Push( o );
        }

        template<typename CT>
        void Prepare( int count )
        {
            auto& os = data[ CT::typeId.value ];
            for( int i = 0; i < data.Size(); ++i )
            {
                os.Push( new CT() );
            }
        }

        void Clear()
        {
            for( int i = 0; i < data.Size(); ++i )
            {
                auto& os = data[ i ];
                for( int j = 0; j < os.Size(); ++j )
                {
                    delete os[ j ];
                }
                os.Clear();
            }
        }
    };

}

#endif
