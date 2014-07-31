// ͷ�ļ�������
#include "Lib/All.h"


#define PACKET_CLASS_HEADER( TN )           \
public:                                     \
TN();                                       \
~TN();                                      \
TN( TN const& other );                      \
TN( TN&& other );                           \
TN& operator=( TN const& other );           \
TN& operator=( TN&& other );                \
void writeBuffer( FlatBuffer& fb ) const;   \
bool readBuffer( FlatBuffer& fb );          \
String toString() const;                    \
void toString( String& s ) const;           \
static ushort getTypeID();


// ��������ǰ������: �� namespace
namespace SwitchPackets             // �������ռ�: ģ���� + Packets
{
    class Ping;
    // ... other class
}
// ... other namespace


namespace SwitchPackets
{
    class Ping : Memmoveable
    {
        PACKET_CLASS_HEADER( Ping );

        static void fillBuffer( FlatBuffer& fb );   // todo: parameters
        // todo: getXxxx setXxxx funcs
    };

    ushort Ping::getTypeID()
    {
        return 0;
    }

    void Ping::writeBuffer( FlatBuffer& fb ) const
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

