// ͷ�ļ�������
#include "Lib/All.h"


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

        static void fillBuffer( FlatBuffer& fb
                                /*, xxx const& xxx
                                //, xxx const& xxx
                                //, xxx const& xxx*/ );   // todo: parameters
        
        //xxx const& getXxxx();
        //template<typename VT>
        //void setXxxx( VT&& v );
    };
}


namespace SwitchPackets
{
    ushort Ping::getTypeID()
    {
        return 0;
    }

    Ping::Ping( Ping const& other )
        //: _xxxx( other._xxxx )
        //, _xxxx( other._xxxx )
        //, _xxxx( other._xxxx )
    {
    }

    Ping::Ping( Ping&& other )
        //: _xxxx( std::move( other._xxxx ) )
        //, _xxxx( std::move( other._xxxx ) )
        //, _xxxx( std::move( other._xxxx ) )
    {
    }

    Ping& Ping::operator=( Ping const& other )
    {
        // _xxxx = other._xxxx;
        // _xxxx = other._xxxx;
        // _xxxx = other._xxxx;
        return *this;
    }

    Ping& Ping::operator=( Ping&& other )
    {
        // _xxxx = std::move( other._xxxx );
        // _xxxx = std::move( other._xxxx );
        // _xxxx = std::move( other._xxxx );
        return *this;
    }

    //xxx const& Ping::getXxxx()
    //{
    //    return _xxxx;
    //}
    //template<typename VT>
    //void Ping::setXxxx( VT&& v )
    //{
    //    _xxxx = std::forward<VT>( v );
    //}


    void Ping::writeBuffer( FlatBuffer& fb ) const
    {
        //fb.write(
            //_xxxx,
            //_xxxx,
            //_xxxx );
    }

    bool Ping::readBuffer( FlatBuffer& fb )
    {
        //if( !fb.read( _xxxx ) )return false;
        //if( !fb.read( _xxxx ) )return false;
        //if( !fb.read( _xxxx ) )return false;
        return true;
    }
}


int main()
{

    system( "pause" );
    return 0;
}

