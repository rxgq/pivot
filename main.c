#include <windows.h>

typedef struct {
    const char* title;
    int height;
    int width;
} RomaWindow;

HWND create_window(HINSTANCE hInstance, const RomaWindow *window) {
    WNDCLASSEX wc = {0};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = DefWindowProc;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = window->title;

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return NULL;
    }

    HWND hwnd = CreateWindowEx(
        0,
        window->title,
        window->title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        window->width, window->height,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return NULL;
    }

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    return hwnd;
}

void draw_text(HWND hwnd, char *text) {
    RECT bounds;
    GetClientRect(hwnd, &bounds);

    HDC hdc = GetDC(hwnd);

    DrawText(hdc, text, -1, &bounds, DT_CENTER | DT_VCENTER);

    ReleaseDC(hwnd, hdc);
}

int main() {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    RomaWindow window = {"roguelike", 512, 512};
    HWND hwnd = create_window(hInstance, &window);
    draw_text(hwnd, "Hello World!");


    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
