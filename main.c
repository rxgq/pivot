#include <windows.h>

typedef struct {
    const char* title;
    int height;
    int width;
    HWND hwnd;
} RomaWindow;

RomaWindow* create_window(char *title, int height, int width) {
    RomaWindow *window = malloc(sizeof(RomaWindow));
    if (!window) {
        MessageBox(NULL, "Memory Allocation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return NULL;
    }

    window->title = title;
    window->height = height;
    window->width = width;

    HINSTANCE hInstance = GetModuleHandle(NULL);

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

    window->hwnd = CreateWindowEx(
        0,
        window->title,
        window->title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        window->width, window->height,
        NULL, NULL, hInstance, NULL);

    if (window->hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return NULL;
    }

    ShowWindow(window->hwnd, SW_SHOWNORMAL);
    UpdateWindow(window->hwnd);

    return window;
}

void draw_text(RomaWindow *window, char *text) {
    RECT bounds;
    GetClientRect(window->hwnd, &bounds);

    HDC hdc = GetDC(window->hwnd);
    DrawText(hdc, text, -1, &bounds, DT_LEFT);

    ReleaseDC(window->hwnd, hdc);
}

void draw_text_from_top_left(RomaWindow *window, char *text, int t, int l) {
    RECT bounds;
    GetClientRect(window->hwnd, &bounds);

    bounds.top = t;
    bounds.left = l;

    HDC hdc = GetDC(window->hwnd);
    DrawText(hdc, text, -1, &bounds, DT_LEFT);

    ReleaseDC(window->hwnd, hdc);
}

void draw_text_from_top_right(RomaWindow *window, char *text, int t, int r) {
    RECT bounds;
    GetClientRect(window->hwnd, &bounds);

    HDC hdc = GetDC(window->hwnd);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, strlen(text), &textSize);

    bounds.top = t;
    bounds.right = bounds.right - r;
    bounds.left = bounds.right - textSize.cx;

    DrawText(hdc, text, -1, &bounds, DT_LEFT);

    ReleaseDC(window->hwnd, hdc);
}

void draw_text_from_bottom_left(RomaWindow *window, char *text, int b, int l) {
    RECT bounds;
    GetClientRect(window->hwnd, &bounds);

    HDC hdc = GetDC(window->hwnd);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, strlen(text), &textSize);

    bounds.left = l;
    bounds.bottom = bounds.bottom - b;
    bounds.top = bounds.bottom - textSize.cy;

    DrawText(hdc, text, -1, &bounds, DT_LEFT);

    ReleaseDC(window->hwnd, hdc);
}


void draw_text_from_bottom_right(RomaWindow *window, char *text, int b, int r) {
    RECT bounds;
    GetClientRect(window->hwnd, &bounds);

    HDC hdc = GetDC(window->hwnd);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, strlen(text), &textSize);

    bounds.right = bounds.right - r;
    bounds.bottom = bounds.bottom - b;
    bounds.top = bounds.bottom - textSize.cy;
    bounds.left = bounds.right - textSize.cx;

    DrawText(hdc, text, -1, &bounds, DT_LEFT);

    ReleaseDC(window->hwnd, hdc);
}

int main() {
    RomaWindow *window = create_window("Roguelike", 512, 512);
    draw_text_from_bottom_left(window, "MY_TasdasdEXT_STRadsasdasdasING", 100, 10);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
