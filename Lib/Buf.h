#ifndef __BUF_H__
#define __BUF_H__

namespace xxx
{
    struct BufBase
    {
        // bufs: ������� ticks �� Destroy
        // dots: ������� ticks �� Process
        int activeTicks = 0;

        // ����������� for ��λpool ( �� new ʱ��� )
        int typeId = 0;

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

    template<typename T>
    struct Buf : public BufBase
    {
        static const AutoID<BufBase> typeId;
        Buf()
        {
            this->BufBase::typeId = Buf::typeId.value;
        }
    };
    template<typename T>
    const AutoID<BufBase> Buf<T>::typeId;


    struct BufContainer
    {
        // ָ���õ� buf ��
        BufPool<BufBase>* pool;

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

        BufContainer( BufPool<BufBase>* pool )
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
            rtv->T::bufIdx = bufs.Size();
            bufs.Push( rtv );
            bufActiveTicks = 0;     // �൱�ڱ��࣬ȫ������
            return rtv;
        }

        template<typename T, typename ...PTS>
        inline T* CreateDot( PTS&& ...ps )
        {
            auto rtv = pool->Alloc<T>( std::forward<PTS>( ps )... );
            rtv->T::bufIdx = dots.Size();
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
