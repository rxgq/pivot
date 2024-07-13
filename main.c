#include <windows.h>

typedef struct {
    const char* title;
    int height;
    int width;
    HWND hwnd;
} RunicView;

RunicView* create_window(char *title, int height, int width) {
    RunicView *view = malloc(sizeof(RunicView));
    if (!view) {
        MessageBox(NULL, "Memory Allocation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return NULL;
    }

    view->title = title;
    view->height = height;
    view->width = width;

    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASSEX wc = {0};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = DefWindowProc;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = view->title;

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return NULL;
    }

    view->hwnd = CreateWindowEx(
        0,
        view->title,
        view->title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        view->width, view->height,
        NULL, NULL, hInstance, NULL);

    if (view->hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return NULL;
    }

    ShowWindow(view->hwnd, SW_SHOWNORMAL);
    UpdateWindow(view->hwnd);

    return view;
}

void draw_text(RunicView *view, char *text) {
    RECT bounds;
    GetClientRect(view->hwnd, &bounds);

    HDC hdc = GetDC(view->hwnd);
    DrawText(hdc, text, -1, &bounds, DT_LEFT);

    ReleaseDC(view->hwnd, hdc);
}

void draw_text_from_top_left(RunicView *view, char *text, int t, int l) {
    RECT bounds;
    GetClientRect(view->hwnd, &bounds);

    bounds.top = t;
    bounds.left = l;

    HDC hdc = GetDC(view->hwnd);
    DrawText(hdc, text, -1, &bounds, DT_LEFT);

    ReleaseDC(view->hwnd, hdc);
}

void draw_text_from_top_right(RunicView *view, char *text, int t, int r) {
    RECT bounds;
    GetClientRect(view->hwnd, &bounds);

    HDC hdc = GetDC(view->hwnd);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, strlen(text), &textSize);

    bounds.top = t;
    bounds.right = bounds.right - r;
    bounds.left = bounds.right - textSize.cx;

    DrawText(hdc, text, -1, &bounds, DT_LEFT);

    ReleaseDC(view->hwnd, hdc);
}

void draw_text_from_bottom_left(RunicView *view, char *text, int b, int l) {
    RECT bounds;
    GetClientRect(view->hwnd, &bounds);

    HDC hdc = GetDC(view->hwnd);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, strlen(text), &textSize);

    bounds.left = l;
    bounds.bottom = bounds.bottom - b;
    bounds.top = bounds.bottom - textSize.cy;

    DrawText(hdc, text, -1, &bounds, DT_LEFT);

    ReleaseDC(view->hwnd, hdc);
}


void draw_text_from_bottom_right(RunicView *view, char *text, int b, int r) {
    RECT bounds;
    GetClientRect(view->hwnd, &bounds);

    HDC hdc = GetDC(view->hwnd);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, strlen(text), &textSize);

    bounds.right = bounds.right - r;
    bounds.bottom = bounds.bottom - b;
    bounds.top = bounds.bottom - textSize.cy;
    bounds.left = bounds.right - textSize.cx;

    DrawText(hdc, text, -1, &bounds, DT_LEFT);

    ReleaseDC(view->hwnd, hdc);
}

int main() {
    RunicView *view = create_window("Roguelike", 512, 512);
    draw_text_from_bottom_left(view, "MY_TasdasdEXT_STRadsasdasdasING", 100, 10);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}