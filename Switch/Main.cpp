// ͷ�ļ�������
#include "Lib/All.h"

// ��Ŀ���������ռ�
namespace SwitchPackets
{
    // ǰ��������
    class Ping;

    class Ping : Memmoveable
    {
    public:
        static int getTypeID();
    private:
        friend FlatBuffer;
        int getBufferSize() const;
        void writeBuffer( FlatBuffer& fb ) const;
        void writeBufferDirect( FlatBuffer& fb ) const;
        bool readBuffer( FlatBuffer& fb );

        // todo: fields
    public:
        // ���ֹ��캯����
        Ping();
        Ping( FlatBuffer& fb );
        ~Ping();
        Ping( Ping const& other );
        Ping( Ping&& other );
        Ping& operator = ( Ping const& other );
        Ping& operator=( Ping&& other );

        // todo: getXxxx setXxxx funcs

        static void fillBuffer( FlatBuffer& fb );   // todo: parameters
    };

    int Ping::getTypeID()
    {
        return 0;
    }

    int Ping::getBufferSize() const
    {
        return 0;
    }

    void Ping::writeBuffer( FlatBuffer& fb ) const
    {

    }

    void Ping::writeBufferDirect( FlatBuffer& fb ) const
    {

    }

    bool Ping::readBuffer( FlatBuffer& fb )
    {
        return true;
    }

}


int main()
{
    system( "pause" );
    return 0;
}

