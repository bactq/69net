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
