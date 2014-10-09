#ifndef __CDGRID_H__
#define __CDGRID_H__

namespace xxx
{
    // CD = collision detection
    // ��ǰֻ֧�־��� AABB( ���һ����ʾ�����Ϊ���ӣ���ע�Ἰ��Item���� )

    struct CdPoint
    {
        int x, y;
    };
    struct CdSize
    {
        int w, h;
    };

    struct CdCell;
    struct CdGrid;
    struct CdItem
    {
        CdSize radius;                                                      // �뾶
        CdPoint pos;                                                        // ���ĵ�����
        int groupId;                                                        // 0 ��Ž���������ŷ�����ײ����ͬ��Ų�����
        uint layerMask;                                                     // maskA & maskB ��Ϊ 0 �ķ�����ײ
        uint flag;                                                          // ������ˮ�ţ�����ȥ��
        int ci1, ri1, ci2, ri2;                                             // ���� Index �������֮ ���� ���� λ�ڵ� cell �Լ��� Update
        void* userData;
        CdGrid* parent;                                                     // ���� CdGrid ������( ͬʱҲ�� Init, Destroy ��ʶ�� flag )
        List<CdCell*> cells;                                                // ռ������Щ����

        CdItem();
        ~CdItem();
        void Init( CdSize const& _radius, CdPoint const& _pos, int _groupId = 0, uint _layerMask = 0xFFFFFFFFu, void* _userData = nullptr );
        void Destroy();                                                     // ���� item ���� items �ƶ��� freeItems

        void Index();                                                       // ����������Ϣ
        void Update( CdPoint const& _pos );                                 // ����λ�ü�������Ϣ
        int GetNearItems( List<CdItem*>& _container );                      // ��� _item ��ռ�������ֵ� items, ���ظ���
        int GetCollisionItems( List<CdItem*>& _container );                 // ����� _item ��ײ�� items, ���ظ���
        CdItem* GetCollisionItem();                                         // �����׸���⵽�� �� _item ��ײ�� item
        // todo: ����Ŀ������ ����� item ? �õ���Χ���ɽ���Զ���е� items list?
    };

    struct CdCell
    {
        int ci, ri;
        Hash<CdItem*> items;                                                // ��������Щ item
    };

    // ��������( �ָ���ʾ�����ۿ� )
    struct CdGrid
    {
        Hash<CdItem*> items;
        List<CdItem*> freeItems;
        List<CdCell> cells;

        CdSize gridDiameter;
        CdSize cellRadius;
        CdSize cellDiameter;
        int rowCount;
        int columnCount;

        uint autoFlag;                                                      // ������ˮ�ţ�����ȥ��
        void IncreaseFlag();                                                // ++autoFlag; ѭ�����������в����� 0

        CdGrid();
        ~CdGrid();

        void Init( CdSize _gridDiameter, int _rowCount, int _columnCount ); // ��һ���ܳߴ绮�ָ��Ӳ� Clear
        void Reserve( int _capacity );                                      // �� freeItems ��Ԥ���� capacity �� items
        void Clear();                                                       // Destroy items, �� cells �� items

        CdItem* CreateItem();                                               // ���� CdItem ����( ���ȴ� freeItems �� )��������( ��Ҫ�Լ� Init )

        int GetItems( List<CdItem*>& _container, CdPoint const& _pos );     // ��串����ĳ����� items�����ظ���
        CdItem* GetItem( CdPoint const& _pos );                             // ���ظ�����ĳ����ģ���1��ɨ���� item
        static bool CheckCollision( CdItem* _a, CdItem* _b );               // ������� item �Ƿ��ཻ
        static bool CheckCollision( CdItem* _a, CdPoint _pos );             // ��� pos �Ƿ�λ�� item ��
    };
}

#endif
