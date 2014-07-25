#include "Lib/All.h"
template<typename ...TS>
void cout( TS const& ...parms )
{
    String s;
    s.append( parms... );
    puts( s.c_str() );
}
template<typename ...TS>
void coutn( TS const& ...parms )
{
    cout( parms..., '\n' );
}
#include <windows.h>
template<typename ...TS>
void coutPos( short x, short y, TS const& ...parms )
{
    static auto h = GetStdHandle( STD_OUTPUT_HANDLE );
    static COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition( h, pos );
    cout( parms... );
}

#include <enet/enet.h>
int test()
{
    // ���������ǰ�ú������ʼ���ͷ���ʼ�����ŵ�����������ﲻһ�����ʣ������Ͻ�Ӧ�����ڳ���Ŀ�ʼ�ͽ���ʱ�ֱ��һ��
    if( auto rtv = enet_initialize() ) return rtv;
    ScopeGuard sg_enet_deinitialize( [] { enet_deinitialize(); } );

    // ����һ����ַ����, ���������� IP �� 1234 �Ͻ�������
    ENetAddress address = { ENET_HOST_ANY, 1234 };
    // �������������о����ָ�� IP
    // enet_address_set_host( &address, "x.x.x.x" );

    // ���� host ����( ������ʱ���� )
    auto server = enet_host_create( &address, 32, 2, 0, 0 );    // num of clients, num of channels, incoming bandwidth, outgoing bandwidth
    if( !server ) return -1;
    ScopeGuard sg_server( [ = ] { enet_host_destroy( server ); } );

    // ���� counter ������ʾ ������ ѭ���ٶ�
    int64 counter = 0;
    // timer ���
    std::chrono::milliseconds refreshDuration( 200 );
    std::chrono::seconds durationSec1( 1 );
    std::chrono::time_point<std::chrono::system_clock> lastTime;
    auto firstTime = std::chrono::system_clock::now();


    while( true )
    {
        ENetEvent event;
        if( enet_host_service( server, &event, 0 ) )
        {
            switch( event.type )
            {
            case ENET_EVENT_TYPE_CONNECT:
            {
                printf( "A new client connected from %x:%u.\n",
                        event.peer->address.host,
                        event.peer->address.port );
                /* Store any relevant client information here. */
                event.peer->data = "cccccc";
                auto packet = enet_packet_create( "go", 3, ENET_PACKET_FLAG_RELIABLE );
                enet_peer_send( event.peer, 0, packet );
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE:
            {
                //printf( "A packet of length %u containing %s was received from %s on channel %u.\n",
                //        event.packet->dataLength,
                //        event.packet->data,
                //        event.peer->data,
                //        event.channelID );
                /* Clean up the packet now that we're done using it. */
                // todo: counter += packet len
                int len = event.packet->dataLength;
                counter += event.packet->dataLength;
                enet_packet_destroy( event.packet );
                auto packet = enet_packet_create( &len, 4, ENET_PACKET_FLAG_RELIABLE );
                enet_peer_send( event.peer, 0, packet );
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT:
            {
                printf( "%s disconnected.\n", event.peer->data );
                /* Reset the peer's client information. */
                event.peer->data = NULL;
            }
            }
        }
        //std::this_thread::sleep_for( std::chrono::milliseconds( 0 ) );
        //++counter;    // counter ֱ�����ڼ��յ����ֽ���


        // ��Ƶ��� now ��ÿ���̨����� cpu ̫��
        auto now = std::chrono::system_clock::now();
        if( now - lastTime >= refreshDuration )
        {
            auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>( now - firstTime ).count();
            if( !elapsedSeconds ) elapsedSeconds = 1;
            coutPos( 0, 0, counter, ", ", counter / elapsedSeconds );
            lastTime = now;
        }

        // other logic here
    }
    return 0;
}

int main()
{
    if( auto rtv = test() )
    {
        cout( rtv );
    }

    system( "pause" );
    return 0;
}
