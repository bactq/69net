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
        CdSize radius;                  // ���
        CdPoint pos;                    // ���꣨ê��λ�����½ǣ�
        int groupId;                    // 0 ��Ž���������ŷ�����ײ����ͬ��Ų�����
        uint layerMask;                 // maskA & maskB ��Ϊ 0 �ķ�����ײ
        uint flag;                      // ������ˮ�ţ�����ȥ��
        void* userData;
        List<CdCell*> cells;            // ռ������Щ����
    };

    struct CdCell
    {
        CdPoint index;                  // x: columnIndex, y: rowIndex
        Set<CdItem*> items;             // ��������Щ item
    };

    // ��������( �ָ���ʾ�����ۿ� )
    struct CdGrid
    {
        Set<CdItem*> items;
        List<CdItem*> freeItems;
        List<CdCell> cells;

        CdSize cellRadius;
        CdSize cellDiameter;
        int rowCount = 0;
        int columnCount = 0;
        CdSize gridDiameter;


        uint autoFlag = 0;  // ������ˮ�ţ�����ȥ��( ��ĳ����ʱ +1�����ٴε���0ʱ������ cell �� item �� flag ��� 0, �� 1 )

        ~CdGrid();

        void Init( CdSize _gridDiameter, int _rowCount, int _columnCount ); // ��һ���ܳߴ绮�ָ��Ӳ� Clear
        void Reserve( int capacity );                                       // �� freeItems ��Ԥ���� capacity �� items
        CdItem* CreateItem();                                               // �����ڴ�( ���ȴ� freeItems �� )��������
        void DestroyItem( CdItem* _item );                                  // ���� item ���� items �ƶ��� freeItems
        void Clear();                                                       // Destroy items, �� cells �� items
        void Reindex( CdItem* _item );                                      // ����������Ϣ
        void Reindex( CdItem* _item, CdPoint const& _pos );                 // ����λ�ü�������Ϣ
        int GetNearItems( List<CdItem*> container, CdItem* _item );         // ��� _item ��ռ�������ֵ� items, ���ظ���
        int GetCollisionItems( List<CdItem*> container, CdItem* _item );    // ����� _item ��ײ�� items, ���ظ���
        int GetCellIndex( CdPoint const& _pos );                            // �ж�һ������λ���ĸ�( �������� -1 )
        void ResetAllFlag();                                                // ������ items, freeItems �� flag Ϊ 0
    };
}

#endif
