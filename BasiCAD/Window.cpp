/**********************************************************************************
// Window (C�digo Fonte)
// 
// Cria��o:     19 Mai 2007
// Atualiza��o: 19 Mai 2025
// Compilador:	Visual C++ 2022
//
// Descri��o:   Abstrai os detalhes de configura��o de uma janela
//
**********************************************************************************/

#include "Window.h"

// -------------------------------------------------------------------------------
// inicializa��o de membros est�ticos da classe

void (*Window::inFocus)()     = nullptr;                                    // nenhuma a��o ao ganhar foco
void (*Window::lostFocus)()   = nullptr;                                    // nenhuma a��o ao perder foco
int Window::screenWidth       = 0;                                          // largura da tela
int Window::screenHeight      = 0;                                          // altura da tela

// -------------------------------------------------------------------------------
// construtor

Window::Window()
{
    // adiciona suporte a monitores com alto dpi
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    monitorX          = 0;                                                  // posi��o x do monitor no desktop virtual
    monitorY          = 0;                                                  // posi��o y do monitor no desktop virtual
    screenDpi         = GetDpiForSystem();                                  // dpi da tela
    windowScale       = screenDpi / float(USER_DEFAULT_SCREEN_DPI);         // escala da janela
    
    hdc               = { 0 };                                              // contexto do dispositivo  
    screenWidth       = GetSystemMetricsForDpi(SM_CXSCREEN, screenDpi);     // largura da tela
    screenHeight      = GetSystemMetricsForDpi(SM_CYSCREEN, screenDpi);     // altura da tela
    clientWidth       = screenWidth;                                        // janela inicia em tela cheia
    clientHeight      = screenHeight;                                       // janela inicia em tela cheia
    clientCenterX     = clientWidth/2;                                      // centro da �rea cliente no eixo x
    clientCenterY     = clientHeight/2;                                     // centro da �rea cliente no eixo y
    clientAspectRatio = clientWidth / float(clientHeight);                  // aspect ratio da �rea cliente
    clientRect        = { 0, 0, 0, 0 };                                     // �rea cliente da janela
                                                                            
    windowId          = 0;                                                  // id nulo porque a janela ainda n�o existe
    windowIcon        = LoadIcon(NULL, IDI_APPLICATION);                    // �cone padr�o de uma aplica��o
    windowCursor      = LoadCursor(NULL, IDC_ARROW);                        // cursor padr�o de uma aplica��o
    windowColor       = RGB(0,0,0);                                         // cor de fundo padr�o � preta
    windowTitle       = string("Windows App");                              // t�tulo padr�o da janela
    windowStyle       = WS_POPUP | WS_VISIBLE;                              // estilo para tela cheia
    windowMode        = FULLSCREEN;                                         // modo padr�o � tela cheia
    windowPosX        = 0;                                                  // posi��o inicial da janela no eixo x
    windowPosY        = 0;                                                  // posi��o inicial da janela no eixo y
    windowWidth       = screenWidth;                                        // largura da janela
    windowHeight      = screenHeight;                                       // altura da janela
    windowAspectRatio = windowWidth / float(windowHeight);                  // aspect ratio da janela
    windowMinWidth    = windowWidth / 2;                                    // largura m�nima da janela
    windowMinHeight   = windowHeight / 2;                                   // altura m�nima da janela
                                                                            
    bordersWidth      = 0;                                                  // largura das bordas e barras da janela
    bordersHeight     = 0;                                                  // altura das bordas e barras da janela
}

// -------------------------------------------------------------------------------

Window::~Window()
{
    // libera contexto do dispositivo
    if (hdc) ReleaseDC(windowId, hdc);
}

// -------------------------------------------------------------------------------

void Window::Mode(int mode)
{
    windowMode = mode;

	switch (windowMode)
	{
	case FULLSCREEN:
		windowStyle = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE;
		break;
	case UNLOCKED:
		windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		break;
	case LOCKED:
		windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;
		break;
	case ASPECTRATIO:
		windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		windowStyle -= WS_MAXIMIZEBOX;
		break;
	}
}

// -------------------------------------------------------------------------------

void Window::Size(int width, int height)
{ 
    // tamanho da �rea cliente
    clientWidth = int(width * windowScale);
    clientHeight = int(height * windowScale);

    // posi��o central da �rea cliente
    clientCenterX = clientWidth / 2;
    clientCenterY = clientHeight / 2;

    // aspect ratio da �rea cliente
    clientAspectRatio = clientWidth / float(clientHeight);

    // tamanho da �rea cliente e da janela iniciam iguais
    // janela ser� redimensionada no momento da sua cria��o
    windowWidth = clientWidth;
    windowHeight = clientHeight;

    // tamanho m�nimo da janela
    windowMinWidth = windowWidth / 2;
    windowMinHeight = windowHeight / 2;

    // aspect ratio da janela
    windowAspectRatio = windowWidth / float(windowHeight);

    // ajusta a posi��o da janela para o centro da tela
    windowPosX = (screenWidth/2) - (windowWidth /2);
    windowPosY = (screenHeight/2) - (windowHeight /2);
}

// -------------------------------------------------------------------------------

void Window::ClientResize(int width, int height)
{
    // tamanho da �rea cliente
    clientWidth = width;
    clientHeight = height;

    // posi��o do central da �rea cliente
    clientCenterX = clientWidth / 2;
    clientCenterY = clientHeight / 2;

    // tamanho da janela
    windowWidth = width + bordersWidth;
    windowHeight = height + bordersHeight;

    // atualiza aspect ratio se necess�rio
    if (windowMode != ASPECTRATIO)
    {
         // aspect ratio da �rea cliente
        clientAspectRatio = clientWidth / float(clientHeight);

        // aspect ratio da janela
        windowAspectRatio = windowWidth / float(windowHeight);
    }
}

// -------------------------------------------------------------------------------

void Window::ChangeDpi(uint dpi, RECT* rect)
{
    // escala para novo dpi
    screenDpi = dpi; 
    windowScale = screenDpi / float(USER_DEFAULT_SCREEN_DPI); 

    // tamanho da janela
    windowWidth = rect->right - rect->left;
    windowHeight = rect->bottom - rect->top;

    // tamanho m�nimo da janela
    windowMinWidth = windowWidth / 2;
    windowMinHeight = windowHeight / 2;

    // aspect ratio da janela
    windowAspectRatio = windowWidth / float(windowHeight);

    // redimensiona janela com uma chamada a MoveWindow
    MoveWindow(
        windowId,       // identificador da janela
        rect->left,     // posi��o x
        rect->top,      // posi��o y
        windowWidth,    // largura
        windowHeight,   // altura
        TRUE);          // repintar

    // captura informa��es do monitor que cont�m a janela
    HMONITOR hMonitor = MonitorFromWindow(windowId, MONITOR_DEFAULTTONEAREST);
    MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
    GetMonitorInfo(hMonitor, &monitorInfo);

    // atualiza tamanho da tela
    screenWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
    screenHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

    // atualiza coordenadas de origem do monitor 
    monitorX = monitorInfo.rcMonitor.left;
    monitorY = monitorInfo.rcMonitor.top;
}

// -------------------------------------------------------------------------------

void Window::Sizing(RECT * rect, WPARAM wParam)
{
    // manter as propor��es originais da janela
    if (windowMode == ASPECTRATIO)
    {
        // novas dimens�es da janela
        uint width = rect->right - rect->left;
        uint height = rect->bottom - rect->top;

        // qual borda est� sendo usada para redimensionar
        switch (wParam)
        {
        case WMSZ_TOPLEFT:
        case WMSZ_TOPRIGHT:
            // calcula altura para manter a propor��o da janela
            height = uint(width / windowAspectRatio);
            rect->top = rect->bottom - height;

            // se a coordenada saiu da tela
            if (rect->top - monitorY < 0)
            {
                // limita o topo
                rect->top = monitorY + 0;

                // calcula altura com o novo limite
                height = rect->bottom - rect->top;

                // calcula largura para manter a propor��o da janela
                width = uint(height * windowAspectRatio);

                // ajusta largura de acordo com o lado modificado
                if (wParam == WMSZ_TOPLEFT)
                    rect->left = rect->right - width;
                else
                    rect->right = rect->left + width;
            }
            break;

        case WMSZ_BOTTOMLEFT:
        case WMSZ_BOTTOMRIGHT:
        case WMSZ_LEFT:
        case WMSZ_RIGHT:
            // calcula altura para manter a propor��o da janela
            height = uint(width / windowAspectRatio);
            rect->bottom = rect->top + height;

            // se a coordenada saiu da tela
            if (rect->bottom - monitorY > screenHeight)
            {
                // limita a coordenada inferior
                rect->bottom = monitorY + screenHeight;

                // calcula altura com o novo limite
                height = rect->bottom - rect->top;

                // calcula largura para manter a propor��o da janela
                width = uint(height * windowAspectRatio);

                // ajusta largura de acordo com o lado modificado
                if (wParam == WMSZ_LEFT || wParam == WMSZ_BOTTOMLEFT)
                    rect->left = rect->right - width;
                else
                    rect->right = rect->left + width;
            }
            break;

        case WMSZ_BOTTOM:
        case WMSZ_TOP:
            // calcula largura para manter a propor��o da janela
            width = uint(height * windowAspectRatio);
            rect->right = rect->left + width;

            // se a coordenada saiu da tela
            if (rect->right - monitorX > screenWidth)
            {
                // limita a coordenada direita
                rect->right = monitorX + screenWidth;

                // calcula largura com o novo limite
                width = rect->right - rect->left;

                // calcula altura para manter a propor��o da janela
                height = uint(width / windowAspectRatio);

                // ajusta altura de acordo com o lado modificado
                if (wParam == WMSZ_TOP)
                    rect->top = rect->bottom - height;
                else
                    rect->bottom = rect->top + height;
            }
            break;
        }
    }
}

// -------------------------------------------------------------------------------

bool Window::Create()
{
    // identificador da aplica��o
    HINSTANCE appId = GetModuleHandle(NULL);
    
    // definindo uma classe de janela
    WNDCLASSEX wndClass;     
    wndClass.cbSize        = sizeof(WNDCLASSEX);
    wndClass.style         = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc   = Window::WinProc;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = 0;
    wndClass.hInstance     = appId;
    wndClass.hIcon         = windowIcon;
    wndClass.hCursor       = windowCursor; 
    wndClass.hbrBackground = (HBRUSH) CreateSolidBrush(windowColor);
    wndClass.lpszMenuName  = NULL;
    wndClass.lpszClassName = "AppWindow";
    wndClass.hIconSm       = windowIcon;

    // registrando classe "AppWindow"
    if (!RegisterClassEx(&wndClass))
        return false;

    // criando uma janela baseada na classe "AppWindow" 
    windowId = CreateWindowEx(
        NULL,                           // estilos extras
        "AppWindow",                    // nome da classe da janela
        windowTitle.c_str(),            // t�tulo da janela
        windowStyle,                    // estilo da janela
        windowPosX, windowPosY,         // posi��o (x,y) inicial
        clientWidth, clientHeight,      // largura e altura da janela
        NULL,                           // identificador da janela pai
        NULL,                           // identificador do menu
        appId,                          // identificador da aplica��o
        NULL);                          // par�metros de cria��o

    // Ao usar o modo em janela � preciso levar em conta que as barras 
    // e bordas ocupam espa�o na janela. O c�digo abaixo ajusta o tamanho
    // da janela de forma que a �rea cliente fique com o tamanho requisitado
    // na fun��o principal

    if (windowMode != FULLSCREEN)
    {
        // ret�ngulo com o tamanho da �rea cliente desejada
        RECT rect = {0, 0, clientWidth, clientHeight};

        // ajusta o tamanho do ret�ngulo
        AdjustWindowRectEx(&rect,
            GetWindowStyle(windowId),
            GetMenu(windowId) != NULL,
            GetWindowExStyle(windowId));

        // novas dimens�es da janela
        windowWidth = rect.right - rect.left;
        windowHeight = rect.bottom - rect.top;

        // dimens�es das bordas e barras da janela
        bordersWidth = windowWidth - clientWidth;
        bordersHeight = windowHeight - clientHeight;

        // ajusta a posi��o da janela para o centro da tela
        windowPosX = (screenWidth/2) - (windowWidth/2);
        windowPosY = (screenHeight/2) - (windowHeight/2);

        // aspect ratio da janela
        windowAspectRatio = windowWidth / float(windowHeight);

        // redimensiona janela com uma chamada a MoveWindow
        MoveWindow(
            windowId,       // identificador da janela
            windowPosX,     // posi��o x
            windowPosY,     // posi��o y
            windowWidth,    // largura
            windowHeight,   // altura
            TRUE);          // repintar
    }

    // captura contexto do dispositivo
    hdc = GetDC(windowId);

    // pega tamanho da �rea cliente
    GetClientRect(windowId, &clientRect);

    // retorna estado da inicializa��o (bem sucedida ou n�o)
    return (windowId ? true : false);
}

// -------------------------------------------------------------------------------

LRESULT CALLBACK Window::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    // janela perdeu o foco
    case WM_KILLFOCUS:
        if (lostFocus)
            lostFocus();
        return 0;

    // janela recebeu o foco
    case WM_SETFOCUS:
        if (inFocus)
            inFocus();
        return 0;

    // a janela foi destruida
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// -----------------------------------------------------------------------------
