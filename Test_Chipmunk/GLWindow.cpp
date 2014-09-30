#include "Precompile.h"


std::function<void()> defaultTimerProc;

// x, y
std::pair<LONG, LONG> getTaskbarOffset()
{
    auto rtv = std::make_pair( 0, 0 );
    RECT rect;
    HWND taskBar = FindWindow( L"Shell_traywnd", NULL );
    if( taskBar && GetWindowRect( taskBar, &rect ) )
    {
        if( rect.left <= 0 && rect.top <= 0 )
        {
            if( rect.right > rect.bottom )
            {
                //printf( "top" );
                rtv.second = rect.bottom;
            }
            else
            {
                //printf( "left" );
                rtv.first = rect.right;
            }
        }
        //else if( rect.left == 0 ) printf( "bottom" );
        //else printf( "right" );
    }
    return rtv;
}



LRESULT CALLBACK defaultProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uiMsg )
    {
    case WM_ACTIVATE:
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SIZE:
        return 0;

    case WM_CLOSE:
        PostQuitMessage( 0 );
        return 0;

    case WM_TIMER:
        if( defaultTimerProc ) defaultTimerProc();
        return 0;

        // ����Ч����ʵҲ����á�����ڱ��������²����� 0.5 ������ʱ���ڻᶨס
    case WM_ENTERMENULOOP:
    case WM_ENTERSIZEMOVE:
        // http://stackoverflow.com/questions/3102074/win32-my-application-freezes-while-the-user-resizes-the-window
        SetTimer( hWnd,                 // handle to main window 
                  1,                    // timer identifier 
                  1,                    // 1 ms interval 
                  (TIMERPROC)NULL ); // no timer callback 
        break;

    case WM_EXITMENULOOP:
    case WM_EXITSIZEMOVE:
        KillTimer( hWnd, 1 );
        break;
    }

    return DefWindowProc( hWnd, uiMsg, wParam, lParam );
}

GLWindow::GLWindow()
{
    G::glwindow = this;
}

GLWindow::~GLWindow()
{
    if( this->wnd )
    {
        wglMakeCurrent( nullptr, nullptr );
        ReleaseDC( this->wnd, this->dc );
        wglDeleteContext( this->rc );
        DestroyWindow( this->wnd );
        UnregisterClass( this->className, this->app );
    }
    G::glwindow = nullptr;
}

bool GLWindow::Init( wchar_t* className, int width, int height, int x /*= 0*/, int y /*= 0*/, bool doubleBuffer /*= true*/, wchar_t* title /*= L""*/, WNDPROC wndProc /*= &defaultProc*/ )
{
    assert( !this->wnd );    // ���ظ�����

    this->className = className;
    this->wndProc = wndProc;
    this->autoSwap = this->doubleBuffer = doubleBuffer;

    RECT rect;
    rect.left = 0L;
    rect.top = 0L;
    rect.right = this->width = width;
    rect.bottom = this->height = height;

    this->wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    this->wndClass.lpfnWndProc = wndProc;
    this->wndClass.cbClsExtra = 0;
    this->wndClass.cbWndExtra = 0;
    this->wndClass.hInstance = this->app;
    this->wndClass.hIcon = LoadIcon( NULL, IDI_WINLOGO );
    this->wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
    this->wndClass.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );  // nullptr
    this->wndClass.lpszMenuName = nullptr;
    this->wndClass.lpszClassName = className;
    if( !RegisterClass( &this->wndClass ) )
    {
        printf( "GetLastError() = %d", GetLastError() );
        return false;
    }

    AdjustWindowRectEx( &rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE );
    auto offset = getTaskbarOffset();

    this->wnd = CreateWindowEx(
        WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
        className, title,
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        x + offset.first, y + offset.second,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL,
        this->app,
        NULL
        );

    if( !this->wnd )
    {
        printf( "GetLastError() = %d", GetLastError() );
        UnregisterClass( this->className, this->app );
        return false;
    }


    this->dc = GetDC( this->wnd );

    PIXELFORMATDESCRIPTOR pfd;
    memset( &pfd, 0, sizeof( PIXELFORMATDESCRIPTOR ) );
    pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    if( doubleBuffer ) pfd.dwFlags |= PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = (BYTE)GetDeviceCaps( this->dc, BITSPIXEL );

    pfd.cRedBits = 8;
    pfd.cGreenBits = 8;
    pfd.cBlueBits = 8;
    pfd.cAlphaBits = 8;

    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    pfd.iLayerType = PFD_MAIN_PLANE;

    int iFormat = ChoosePixelFormat( this->dc, &pfd );
    SetPixelFormat( this->dc, iFormat, &pfd );

    this->rc = wglCreateContext( this->dc );
    wglMakeCurrent( this->dc, this->rc );

    ShowWindow( this->wnd, SW_SHOW );
    SetForegroundWindow( this->wnd );
    SetFocus( this->wnd );

    return true;
}

typedef BOOL( APIENTRY *PFNWGLSWAPINTERVALEXTPROC )( int );
bool GLWindow::SetVsync( bool enable /*= true */ )
{
    if( !this->wnd ) return false;
    char* extensions = (char*)glGetString( GL_EXTENSIONS );
    if( strstr( extensions, "WGL_EXT_swap_control" ) )
    {
        auto wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
        wglSwapIntervalEXT( enable ? 1 : 0 );
        return true;
    }
    return false;
}

void GLWindow::SwapBuffer()
{
    assert( this->doubleBuffer );
    SwapBuffers( this->dc );
}

void GLWindow::Loop( std::function<void()> updater )
{
    if( this->wndProc == &defaultProc )
    {
        if( this->autoSwap )
        {
            defaultTimerProc = [=]
            {
                this->SwapBuffer();
                updater();
            };
        }
        else
        {
            defaultTimerProc = updater;
        }
    }

    MSG msg;
    bool running = true;
    while( running )
    {
        /* Do Windows stuff */
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            if( msg.message == WM_QUIT )
            {
                running = false;
            }
            else
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
        updater();
        if( this->autoSwap ) this->SwapBuffer();
    }
}

void GLWindow::SetAutoSwapBuffer( bool enable /*= true */ )
{
    this->autoSwap = enable;
}
