#include "Lib/All.h"

// todo: buf / debuf calc system
namespace xxx
{
    struct BufBaseCore
    {
        // λ��������ʱ��λ�� for ��ɾ
        int bufIdx = 0;

        // ������������� for ��λ��pool
        virtual int GetTypeId() = 0;

        // ��Ӧ�����߼���ش���
        BufBaseCore() = default;

        // �������� -- ��ʼ��/ ����( �������Լ�ʵ�� )
        // void Init( ... );

        // �������� -- ִ��
        inline virtual bool Process( int ticks ) = 0;

        // �������� -- ����
        inline virtual void Destroy() {}

        // ��Ӧ�����߼���ش���
        virtual ~BufBaseCore() {};

        // todo: ����������ֵ��¶���� �Ա� BufContainer Destroy
        // 
    };

    template<typename T>
    struct BufBase : public BufBaseCore
    {
        static const AutoID<BufBaseCore> typeId;
        int GetTypeId() override { return typeId.value; }
    };
    template<typename T>
    const AutoID<BufBaseCore> BufBase<T>::typeId;

    struct BufPool
    {
        List<List<BufBaseCore*>> data;
        BufPool()
        {
            data.Resize( AutoID<BufBaseCore>::maxValue + 1 );
        }
        ~BufPool()
        {
            Clear();
        }
        template<typename T, typename ...PTS>
        T* Alloc( PTS&& ...ps )
        {
            T* rtv;
            auto& bufs = data[ T::typeId.value ];
            if( bufs.Size() )
                rtv = (T*)bufs.TopPop();
            else
                rtv = new T();
            rtv->Init( std::forward<PTS>( ps )... );
            return rtv;
        }
        inline void Free( BufBaseCore* buf )
        {
            buf->Destroy();
            data[ buf->GetTypeId() ].Push( buf );
        }
        template<typename T>
        inline void Prepare( int count )
        {
            auto& bufs = data[ T::typeId.value ];
            for( int i = 0; i < data.Size(); ++i )
            {
                bufs.Push( new T() );
            }
        }
        inline void Clear()
        {
            for( int i = 0; i < data.Size(); ++i )
            {
                auto& bufs = data[ i ];
                for( int j = 0; j < bufs.Size(); ++j )
                {
                    delete bufs[ j ];
                }
                bufs.Clear();
            }
        }
    };

    struct BufContainer
    {
        // ָ���õ� buf ��
        BufPool* pool;

        // Create/DestroyLazy ��� true ���� Process ʱ �������� bufs
        bool bufsDirty;

        // bufsDirty Ϊ true ʱ�� call ���� �������� bufs
        std::function<void()> ResetBufProps;

        // bufsDirty Ϊ true ʱ �������� bufs �� call ��
        std::function<void()> CalcBufProps;

        // ������Ч�ģ��Ը� Properties buf ϵ��Ϊ���� buf
        // ����Ա����( ����ɾ )�仯( lazyDirty Ϊ true )ʱ����������
        List<BufBaseCore*> bufs;

        // ��������Ч�ģ��� call ��������( �γ�ָ����, Process ����������ִ�� ) Ϊ����( dot ϵ )
        // ÿ�� Process ����������
        List<BufBaseCore*> dots;

        BufContainer( BufPool* pool )
            : pool( pool )
            , bufsDirty( false )
        {
        }

        template<typename T, typename ...PTS>
        inline T* CreateBuf( PTS&& ...ps )
        {
            auto rtv = pool->Alloc<T>( std::forward<PTS>( ps )... );
            rtv->T::bufIdx = bufs.Size();
            bufs.Push( rtv );
            bufsDirty = true;
            return rtv;
        }

        template<typename T, typename ...PTS>
        inline T* CreateDot( PTS&& ...ps )
        {
            auto rtv = pool->Alloc<T>( std::forward<PTS>( ps )... );
            rtv->T::bufIdx = dots.Size();
            dots.Push( rtv );
            return rtv;
        }

        inline void DestroyBuf( BufBaseCore* buf )
        {
            bufs.Top()->bufIdx = buf->bufIdx;
            bufs.EraseFast( buf->bufIdx );
            pool->Free( buf );
            bufsDirty = true;
        }

        inline void DestroyDot( BufBaseCore* buf )
        {
            dots.Top()->bufIdx = buf->bufIdx;
            dots.EraseFast( buf->bufIdx );
            pool->Free( buf );
        }

        inline void Clear()
        {
            for( int i = bufs.Size() - 1; i >= 0; --i )
            {
                DestroyBuf( bufs[ i ] );
            }
            for( int i = dots.Size() - 1; i >= 0; --i )
            {
                DestroyDot( dots[ i ] );
            }
            bufsDirty = true;
        }

        inline bool Process( int ticks = 0 )
        {
            if( bufsDirty )
            {
                bufsDirty = false;  // ������ģ���һ Process ���������иı� bufs, ������ȷ�ı� true
                if( ResetBufProps )
                {
                    ResetBufProps();
                }
                for( int i = bufs.Size() - 1; i >= 0; --i )
                {
                    auto& o = bufs[ i ];
                    if( !o->Process( ticks ) )
                    {
                        DestroyBuf( o );
                    }
                }
                if( CalcBufProps )
                {
                    CalcBufProps();
                }
            }

            for( int i = dots.Size() - 1; i >= 0; --i )
            {
                auto& o = dots[ i ];
                if( !o->Process( ticks ) )
                {
                    DestroyDot( o );
                }
            }

            return true;
        }
    };

    /*
    sample:
    */
}

using namespace std;
using namespace xxx;



struct Foo
{
    // ԭ��ϵ��( �����Ͻ�����һ��ԭ���� )
    int ori_maxHP;                  // ԭʼ���Ѫ��
    float ori_maxHPMultiple;        // ԭʼ���Ѫ���ٷֱ� buf ֵ
    int ori_maxHPIncrease;          // ԭʼ���Ѫ������ buf ֵ
    int ori_HP;

    // ��ǰϵ��( ԭʼֵ�� buf ϵ�� )
    int cur_maxHP;
    float cur_maxHPMultiple;
    int cur_maxHPIncrease;
    int cur_HP;

    // buf ��ϵ��
    int buf_maxHP;

    // Process ����Ҫ����ִ�еĺ���, ִ����� clear
    List<std::function<void()>> funcs;

    // ��ʼ��ԭ������
    inline void InitOriginal()
    {
        ori_maxHP = 100;
        ori_maxHPMultiple = 1;
        ori_maxHPIncrease = 0;
        ori_HP = 50;
    }

    inline void InitCurrent()
    {
        cur_maxHP = ori_maxHP;
        cur_HP = ori_HP;
    }

    // Ϊ���� buf ������ buf ϵ��
    inline void ResetBufProperties()
    {
        cur_maxHPMultiple = ori_maxHPMultiple;
        cur_maxHPIncrease = ori_maxHPIncrease;
    }

    // ���� buf Ӱ�������( ���ȼ���ʽ�ڴ� )
    inline void CalcBufProperties()
    {
        buf_maxHP = int( ori_maxHP * cur_maxHPMultiple ) + cur_maxHPIncrease;
    }

    inline void CallFuncs()
    {
        for( int i = 0; i < funcs.Size(); ++i )
        {
            funcs[ i ]();
        }
        funcs.Clear();
    }

    // �ۺϳ�ʼ��
    inline void Init()
    {
        InitOriginal();
        InitCurrent();
        ResetBufProperties();
    }

    BufContainer bc;
    Foo( BufPool* bp )
        : bc( bp )
    {
        bc.ResetBufProps = [ this ] { ResetBufProperties(); };
        bc.CalcBufProps = [ this ] { CalcBufProperties(); };
        Init();
    }
};

struct Buf_maxHP_Enhance100Percent : BufBase<Buf_maxHP_Enhance100Percent>
{
    Foo* owner;
    inline void Init( Foo* owner )
    {
        this->owner = owner;
    }
    inline bool Process( int ticks ) override
    {
        owner->cur_maxHPMultiple += 1;
        return true;
    }
};

struct Buf_maxHP_Enhance10Point : BufBase<Buf_maxHP_Enhance10Point>
{
    Foo* owner;
    inline void Init( Foo* owner )
    {
        this->owner = owner;
    }
    inline bool Process( int ticks ) override
    {
        owner->cur_maxHPIncrease += 10;
        return true;
    }
};

struct Dot_HP_Recover1PointPerTicks : BufBase<Dot_HP_Recover1PointPerTicks>
{
    Foo* owner;
    inline void Init( Foo* owner )
    {
        this->owner = owner;
    }
    inline bool Process( int ticks ) override
    {
        owner->cur_HP += 1;
        if( owner->cur_HP > owner->buf_maxHP )
        {
            owner->cur_HP = owner->buf_maxHP;
        }
        return true;
    }
};


int main()
{
    BufPool p;
    Foo foo( &p );

    // ������ buf( �� Ѫ���� �ٷֱȺ͵��� )
    foo.bc.CreateBuf<Buf_maxHP_Enhance10Point>( &foo );
    foo.bc.CreateBuf<Buf_maxHP_Enhance100Percent>( &foo );
    foo.bc.CreateBuf<Buf_maxHP_Enhance10Point>( &foo );
    foo.bc.CreateBuf<Buf_maxHP_Enhance100Percent>( &foo );
    foo.bc.CreateBuf<Buf_maxHP_Enhance10Point>( &foo );

    // ������ dot( ÿ���ڲ��ϵĻָ�Ѫ )
    foo.bc.CreateDot<Dot_HP_Recover1PointPerTicks>( &foo );
    foo.bc.CreateDot<Dot_HP_Recover1PointPerTicks>( &foo );
    foo.bc.CreateDot<Dot_HP_Recover1PointPerTicks>( &foo );

    Stopwatch sw;
    for( int i = 0; i < 9999999; ++i )
    {
        foo.bc.Process();
    }
    Cout( "ms = ", sw.ElapsedMillseconds() );

    // shoud be 330, 330
    Cout( "buf_maxHP = ", foo.buf_maxHP, ", cur_HP = ", foo.cur_HP );

    //foo.CallFuncs();

    system( "pause" );
    return 0;
}
