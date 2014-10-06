#include "All.h"

// todo: linux

String SharedMemory::_prefixName;

int SharedMemory::dispose( int key )
{
    return 0;
}

// ���������ڴ�
// ���� MS �������������� CHECK. pDACL �� NULL, ��ȫ�������, Ĭ��
// �ο����ϣ�
// http://support.microsoft.com/kb/106387/zh-cn
// http://blog.csdn.net/paradise_lx/article/details/5939988
char* SharedMemory::create( int key, int bufLen )
{
#if __WIN
    // make names
    auto mtxName = String::make( _prefixName, "m", key );
    auto smName = String::make( _prefixName, "sm", key );

    // security
    SECURITY_DESCRIPTOR	sd = { 0 };
    InitializeSecurityDescriptor( &sd, SECURITY_DESCRIPTOR_REVISION );
    SetSecurityDescriptorDacl( &sd, TRUE, NULL, TRUE );
    SECURITY_ATTRIBUTES	sa = { sizeof( SECURITY_ATTRIBUTES ), &sd, FALSE };

    // mutex
    auto mtx = CreateMutexA( &sa, FALSE, mtxName.c_str() );
    ScopeGuard sg_hM( [ &] { ReleaseMutex( mtx ); } );

    // create file mapping
    auto fm = CreateFileMappingA( INVALID_HANDLE_VALUE, &sa, PAGE_READWRITE, 0, bufLen, smName.c_str() );
    if( !fm ) return nullptr;
    // if( GetLastError() == ERROR_ALREADY_EXISTS )   todo

    // map file view
    return (char*)MapViewOfFile( fm, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
#else
    return nullptr;
#endif
}

char* SharedMemory::get( int key, int bufLen )
{
#if __WIN
    // make name
    auto smName = String::make( _prefixName, "sm", key );

    // open file mapping
    auto fm = OpenFileMappingA( FILE_MAP_ALL_ACCESS, false, smName.c_str() );
    if( !fm ) return nullptr;

    // map file view
    return (char*)MapViewOfFile( fm, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
#else
    return nullptr;
#endif
}

