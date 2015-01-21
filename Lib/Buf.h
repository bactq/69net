#ifndef __BUF_H__
#define __BUF_H__

namespace xxx
{
    // sample ��������

    struct BufBase
    {
        // bufs: ������� ticks �� Destroy
        // dots: ������� ticks �� Process
        int activeTicks = 0;

        int typeId = 0, versionNumber = 0;                  // for Pool

        // λ��������ʱ��λ�� for ��ɾ
        int bufIdx = 0;

        // ��Ӧ�����߼���ش���
        BufBase() = default;

        // �������� -- ��ʼ��/ ����( �������Լ�ʵ�� )
        // void Init( ... );

        // �������� -- ִ��
        inline virtual bool Process( int ticks ) = 0;

        // �������� -- ����
        inline virtual void Destroy() {}

        // ��Ӧ�����߼���ش���
        virtual ~BufBase() {};
    };


    struct BufContainer
    {
        // ָ���õ� buf ��
        AutoIDPool<BufBase>* pool;

        // �� buf/dot ��С������ ticks for ��ʱ���������� Process �Ĺ���
        int bufActiveTicks;
        int dotActiveTicks;

        // bufsDirty Ϊ true ʱ�� call ���� �������� bufs
        std::function<void()> BeforeBufProcess;

        // bufsDirty Ϊ true ʱ �������� bufs �� call ��
        std::function<void()> AfterBufProcess;

        // ������Ч�ģ��Ը� Properties buf ϵ��Ϊ���� buf
        // ����Ա����( ����ɾ )�仯( lazyDirty Ϊ true )ʱ����������
        List<BufBase*> bufs;

        // ��������Ч�ģ��� call ��������( �γ�ָ����, Process ����������ִ�� ) Ϊ����( dot ϵ )
        // ÿ�� Process ����������
        List<BufBase*> dots;

        BufContainer( AutoIDPool<BufBase>* pool )
            : pool( pool )
            , bufActiveTicks( INT_MAX )
            , dotActiveTicks( INT_MAX )
        {
        }
        ~BufContainer()
        {
            Clear();
        }

        template<typename T, typename ...PTS>
        inline T* CreateBuf( PTS&& ...ps )
        {
            auto rtv = pool->Alloc<T>( std::forward<PTS>( ps )... );
            rtv->bufIdx = bufs.Size();
            bufs.Push( rtv );
            bufActiveTicks = 0;     // �൱�ڱ��࣬ȫ������
            return rtv;
        }

        template<typename T, typename ...PTS>
        inline T* CreateDot( PTS&& ...ps )
        {
            auto rtv = pool->Alloc<T>( std::forward<PTS>( ps )... );
            rtv->bufIdx = dots.Size();
            dots.Push( rtv );
            if( rtv->activeTicks < dotActiveTicks )
            {
                dotActiveTicks = rtv->activeTicks;
            }
            return rtv;
        }

        inline void DestroyBuf( BufBase* buf )
        {
            bufs.Top()->bufIdx = buf->bufIdx;
            bufs.EraseFast( buf->bufIdx );
            pool->Free( buf );
            bufActiveTicks = 0;     // �൱�ڱ��࣬ȫ������
        }

        inline void DestroyDot( BufBase* buf )
        {
            dots.Top()->bufIdx = buf->bufIdx;
            dots.EraseFast( buf->bufIdx );
            pool->Free( buf );
        }

        inline void Clear()
        {
            // �п��ܴ����岻������������ Destroy ��ʱ���ִ������µ�
            while( bufs.Size() )
            {
                for( int i = bufs.Size() - 1; i >= 0; --i )
                {
                    DestroyBuf( bufs[ i ] );
                }
            }
            while( dots.Size() )
            {
                for( int i = dots.Size() - 1; i >= 0; --i )
                {
                    DestroyDot( dots[ i ] );
                }
            }
            bufActiveTicks = INT_MAX;
            dotActiveTicks = INT_MAX;
        }

        inline bool Process( int ticks = 0 )
        {
            if( bufActiveTicks <= ticks )
            {
                bufActiveTicks = INT_MAX;
                if( BeforeBufProcess )
                {
                    BeforeBufProcess();
                }
                for( int i = bufs.Size() - 1; i >= 0; --i )
                {
                    auto& o = bufs[ i ];
                    if( !o->Process( ticks ) || o->activeTicks <= ticks )
                    {
                        DestroyBuf( o );
                    }
                    else
                    {
                        // ֻɨ�������ŵ� buf ���´�ִ�� ticks
                        if( o->activeTicks < bufActiveTicks )
                        {
                            bufActiveTicks = o->activeTicks;
                        }
                    }
                }
                if( AfterBufProcess )
                {
                    AfterBufProcess();
                }
            }

            if( dotActiveTicks <= ticks )
            {
                dotActiveTicks = INT_MAX;
                for( int i = dots.Size() - 1; i >= 0; --i )
                {
                    auto& o = dots[ i ];
                    // ��ɨ���´�ִ�е���С���
                    // �����ɱ���ĸպ�����С��� Ҳû��ϵ ���ϸ�, ���Ǹ� ticks ����ɨ����ȷֵ
                    // �о������࣬��ʡ���� ��� ������ɨ�� ��С���������
                    if( o->activeTicks < dotActiveTicks )
                    {
                        dotActiveTicks = o->activeTicks;
                    }
                    if( o->activeTicks <= ticks )
                    {
                        if( !o->Process( ticks ) )
                        {
                            DestroyDot( o );
                        }
                    }
                }
                // ��ǰ�㷨��ÿ��һ��ִ�е㣬��Ӳɨ���ʺ϶��������ٵ����( ��ֵ����ȷ���²��� 10 ������ )��
                // ���Ԫ���ٶ�㣬����Ҫ��ɨ��ͬʱ���´�ִ�е�� dot �Ŷ�����
                // ����ּ���ɨ����С�ļ��������ǰ�Ķ����е�Ԫ�����ˣ��ٷ������
            }

            return true;
        }
    };

}

#endif


/*
#include "Lib/All.h"
using namespace std;
using namespace xxx;

struct Buf_MaxHP_Enhance100Percent;
struct Buf_MaxHP_Enhance10Point;
struct Dot_HP_Recover1PointPerTicks;

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
    Foo( BufPool<BufBase>* bp )
        : bc( bp )
    {
        bc.BeforeBufProcess = [ this ] { ResetBufProperties(); };
        bc.AfterBufProcess = [ this ] { CalcBufProperties(); };
        Init();
    }

    inline void Process( int ticks )
    {
        bc.Process( ticks );
        // todo: foreach call funcs
    }

    void CreateBuf_���Ѫ����10��( int ticks, int life );
    void CreateBuf_���Ѫ���Ӱٷ�֮100( int ticks, int life );
    void CreateDot_Ѫ���ָ�1��( int ticks, int cd );
};

struct Buf_MaxHP_Enhance100Percent : Buf<Buf_MaxHP_Enhance100Percent>
{
    Foo* owner;
    inline void Init( Foo* owner, int ticks, int life )
    {
        this->owner = owner;
        this->activeTicks = ticks + life;
    }
    inline bool Process( int ticks ) override
    {
        owner->cur_maxHPMultiple += 1;
        return true;
    }
};

struct Buf_MaxHP_Enhance10Point : Buf<Buf_MaxHP_Enhance10Point>
{
    Foo* owner;
    inline void Init( Foo* owner, int ticks, int life )
    {
        this->owner = owner;
        this->activeTicks = ticks + life;
    }
    inline bool Process( int ticks ) override
    {
        owner->cur_maxHPIncrease += 10;
        return true;
    }
};

struct Dot_HP_Recover1PointPerTicks : Buf<Dot_HP_Recover1PointPerTicks>
{
    Foo* owner;
    int cd;
    inline void Init( Foo* owner, int ticks, int cd )
    {
        this->owner = owner;
        this->activeTicks = ticks + cd;
        this->cd = cd;
    }
    inline bool Process( int ticks ) override
    {
        // �����Ͻ������� dot ��Ҫ�����ַ�ʽ������: owner->funcs.Push( .... )
        // ���൱�ڰ�����ŵ� Process ֮������. 
        // ִ�й����У��п���Ŀ������ɶ�ģ�����Ҫ����Ӧ����
        owner->cur_HP += 1;
        if( owner->cur_HP > owner->buf_maxHP )
        {
            owner->cur_HP = owner->buf_maxHP;
        }
        activeTicks = ticks + cd;
        //Cout( "owner->cur_HP = ", owner->cur_HP );
        return true;
    }
};

void Foo::CreateBuf_���Ѫ����10��( int ticks, int life )
{
    bc.CreateBuf<Buf_MaxHP_Enhance10Point>( this, ticks, life );
}
void Foo::CreateBuf_���Ѫ���Ӱٷ�֮100( int ticks, int life )
{
    bc.CreateBuf<Buf_MaxHP_Enhance100Percent>( this, ticks, life );
}
void Foo::CreateDot_Ѫ���ָ�1��( int ticks, int cd )
{
    bc.CreateDot<Dot_HP_Recover1PointPerTicks>( this, ticks, cd );
}


typedef xxx::Singleton<BufPool<BufBase>> FooBufPool;
int main()
{
    FooBufPool::InitInstance();

    Stopwatch sw;
    for( int j = 0; j < 10000000; ++j )
    {
        Foo foo( FooBufPool::GetInstance() );

        // ��һ��֡����ֵ
        int ticks = 123;

        // ������ buf( �� Ѫ���� �ٷֱȺ͵���, ���ʱ����ͬ )
        foo.CreateBuf_���Ѫ����10��( ticks, 10 );
        foo.CreateBuf_���Ѫ���Ӱٷ�֮100( ticks, 9 );
        foo.CreateBuf_���Ѫ����10��( ticks, 8 );
        foo.CreateBuf_���Ѫ���Ӱٷ�֮100( ticks, 7 );
        foo.CreateBuf_���Ѫ����10��( ticks, 6 );

        // ������ dot( ���ϵĻָ�Ѫ, ÿ�����ʱ����ͬ )
        foo.CreateDot_Ѫ���ָ�1��( ticks, 1 );
        foo.CreateDot_Ѫ���ָ�1��( ticks, 2 );
        foo.CreateDot_Ѫ���ָ�1��( ticks, 3 );

        for( int i = 0; i < 11; ++i )
        {
            ticks++;
            foo.Process( ticks );
            //Cout( "buf_maxHP = ", foo.buf_maxHP, ", cur_HP = ", foo.cur_HP );
        }
    }
    Cout( "ms = ", sw.ElapsedMillseconds() );

    Cout( "FooBufPool::GetInstance()->data[ 0 ].Size() = ", FooBufPool::GetInstance()->data[ 0 ].Size() );
    Cout( "FooBufPool::GetInstance()->data[ 1 ].Size() = ", FooBufPool::GetInstance()->data[ 1 ].Size() );
    Cout( "FooBufPool::GetInstance()->data[ 2 ].Size() = ", FooBufPool::GetInstance()->data[ 2 ].Size() );

    system( "pause" );
    return 0;
}

*/