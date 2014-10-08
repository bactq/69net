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
    struct CdItem
    {
        CdSize radius;                  // �뾶
        CdPoint pos;                    // ���ĵ�����
        int groupId;                    // 0 ��Ž���������ŷ�����ײ����ͬ��Ų�����
        uint layerMask;                 // maskA & maskB ��Ϊ 0 �ķ�����ײ
        uint flag;                      // ������ˮ�ţ�����ȥ��
        void* userData;
        List<CdCell*> cells;            // ռ������Щ����
    };

    struct CdCell
    {
        CdPoint index;                  // x: columnIndex, y: rowIndex
        Hash<CdItem*> items;            // ��������Щ item
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

        uint autoFlag;  // ������ˮ�ţ�����ȥ��( ��ĳ����ʱ +1�����ٴε���0ʱ������ cell �� item �� flag ��� 0, �� 1 )
        void IncreaseFlag();                                                // ++autoFlag; ѭ�����������в����� 0

        CdGrid();
        ~CdGrid();

        void Init( CdSize _gridDiameter, int _rowCount, int _columnCount ); // ��һ���ܳߴ绮�ָ��Ӳ� Clear
        void Reserve( int _capacity );                                      // �� freeItems ��Ԥ���� capacity �� items
        CdItem* CreateItem();                                               // �����ڴ�( ���ȴ� freeItems �� )��������
        void DestroyItem( CdItem* _item );                                  // ���� item ���� items �ƶ��� freeItems
        void Clear();                                                       // Destroy items, �� cells �� items
        void Index( CdItem* _item );                                          // ����������Ϣ
        void Update( CdItem* _item, CdPoint const& _pos );                  // ����λ�ü�������Ϣ
        int GetNearItems( List<CdItem*>& _container, CdItem* _item );       // ��� _item ��ռ�������ֵ� items, ���ظ���
        int GetCollisionItems( List<CdItem*>& _container, CdItem* _item );  // ����� _item ��ײ�� items, ���ظ���
        CdItem* GetCollisionItem( CdItem* _item );                          // �����׸���⵽�� �� _item ��ײ�� item
        int GetItems( List<CdItem*>& _container, CdPoint const& _pos );     // ��串����ĳ����� items�����ظ���
        CdItem* GetItem( CdPoint const& _pos );                             // ���ظ�����ĳ����ģ���1��ɨ���� item
        static bool CheckCollision( CdItem* _a, CdItem* _b );               // ������� item �Ƿ��ཻ
        static bool CheckCollision( CdItem* _a, CdPoint _pos );             // ��� pos �Ƿ�λ�� item ��
        // todo: ����Ŀ������ ����� item ? �õ���Χ���ɽ���Զ���е� items list?
    };
}

#endif
