#include <windows.h>

// Window Procedure: handles messages for the overlay window.
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_CREATE:
        {
            // Set the window's layered attributes:
            // This call makes the window use a specified alpha value.
            SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Enable transparent background for text.
            SetBkMode(hdc, TRANSPARENT);

            // Create a bold, 32-point Arial font.
            HFONT hFont = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                     DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                     CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                     VARIABLE_PITCH, TEXT("Arial"));
            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

            // Set text color (red in this example).
            SetTextColor(hdc, RGB(255, 0, 0));

            // Display the squat jump result.
            const wchar_t* text = L"Squat Jump: 50 cm";
            TextOut(hdc, 50, 50, text, lstrlen(text));

            // Clean up GDI objects.
            SelectObject(hdc, hOldFont);
            DeleteObject(hFont);

            EndPaint(hwnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Entry point: registers the window class and creates the overlay window.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    // Define and register a window class.
    WNDCLASS wc = {0};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = TEXT("OverlayWindowClass");
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

    if(!RegisterClass(&wc))
    {
        MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error"),
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create a borderless window with extended styles for layered, transparent, and topmost.
    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST,
        wc.lpszClassName,
        TEXT("Squat Jump Overlay"),
        WS_POPUP, // No border, title, etc.
        100, 100, 400, 200, // x, y, width, height; adjust as needed
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error"),
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Set the transparency level (255 = fully opaque for drawn text).
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);

    // Display the window.
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Main message loop.
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

