#include <windows.h>
#include <stdio.h>

typedef struct {
    const char* title;
    int height;
    int width;
    HWND hwnd;
} RunicView;

typedef enum {
    BLACK,
    WHITE,
    RED,
    BLUE,
    GREEN
} RunicColor;

typedef struct {
    const char *body;
    const RunicColor color;
    int x;
    int y;
} Player;

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

COLORREF get_color(RunicColor color) {
    switch (color) {
        case BLACK: return RGB(0,   0,   0);
        case WHITE: return RGB(255, 255, 255);
        case RED:   return RGB(255, 0,   0);
        case BLUE:  return RGB(0,   0,   255);
        case GREEN: return RGB(0,   255, 0);
    }
}

void draw_char(RunicView *view, char *text, RunicColor color) {
    RECT bounds;
    GetClientRect(view->hwnd, &bounds);
    HDC hdc = GetDC(view->hwnd);

    COLORREF oldColor = SetTextColor(hdc, get_color(color));
    DrawText(hdc, text, -1, &bounds, DT_LEFT);
    SetTextColor(hdc, oldColor);

    ReleaseDC(view->hwnd, hdc);
}

void draw_char_at(RunicView *view, char *text, RunicColor color, int x, int y) {
    HDC hdc = GetDC(view->hwnd);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, strlen(text), &textSize);

    RECT bounds;
    bounds.left = x;
    bounds.top = y;
    bounds.right = bounds.left + textSize.cx;
    bounds.bottom = bounds.top + textSize.cy;

    COLORREF oldColor = SetTextColor(hdc, get_color(color));
    DrawText(hdc, text, -1, &bounds, DT_LEFT | DT_TOP);
    SetTextColor(hdc, oldColor);

    ReleaseDC(view->hwnd, hdc);
}


void draw_char_from_top_left(RunicView *view, char *text, RunicColor color, int t, int l) {
    RECT bounds;
    GetClientRect(view->hwnd, &bounds);

    bounds.top = t;
    bounds.left = l;

    HDC hdc = GetDC(view->hwnd);

    COLORREF oldColor = SetTextColor(hdc, get_color(color));
    DrawText(hdc, text, -1, &bounds, DT_LEFT);
    SetTextColor(hdc, oldColor);

    ReleaseDC(view->hwnd, hdc);
}

void draw_char_from_top_right(RunicView *view, char *text, RunicColor color, int t, int r) {
    RECT bounds;
    GetClientRect(view->hwnd, &bounds);

    HDC hdc = GetDC(view->hwnd);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, strlen(text), &textSize);

    bounds.top = t;
    bounds.right = bounds.right - r;
    bounds.left = bounds.right - textSize.cx;

    COLORREF oldColor = SetTextColor(hdc, get_color(color));
    DrawText(hdc, text, -1, &bounds, DT_LEFT);
    SetTextColor(hdc, oldColor);

    ReleaseDC(view->hwnd, hdc);
}

void draw_char_from_bottom_left(RunicView *view, char *text, RunicColor color, int b, int l) {
    RECT bounds;
    GetClientRect(view->hwnd, &bounds);

    HDC hdc = GetDC(view->hwnd);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, strlen(text), &textSize);

    bounds.left = l;
    bounds.bottom = bounds.bottom - b;
    bounds.top = bounds.bottom - textSize.cy;

    COLORREF oldColor = SetTextColor(hdc, get_color(color));
    DrawText(hdc, text, -1, &bounds, DT_LEFT);
    SetTextColor(hdc, oldColor);

    ReleaseDC(view->hwnd, hdc);
}

void draw_text_from_bottom_right(RunicView *view, char *text, RunicColor color, int b, int r) {
    RECT bounds;
    GetClientRect(view->hwnd, &bounds);

    HDC hdc = GetDC(view->hwnd);

    SIZE textSize;
    GetTextExtentPoint32(hdc, text, strlen(text), &textSize);

    bounds.right = bounds.right - r;
    bounds.bottom = bounds.bottom - b;
    bounds.top = bounds.bottom - textSize.cy;
    bounds.left = bounds.right - textSize.cx;

    COLORREF oldColor = SetTextColor(hdc, get_color(color));
    DrawText(hdc, text, -1, &bounds, DT_LEFT);
    SetTextColor(hdc, oldColor);

    ReleaseDC(view->hwnd, hdc);
}

void draw_my_box(RunicView *view) {
    draw_char_from_top_right(view, "---------------", BLACK, 200, 200);
    draw_char_from_top_right(view, "| . . . . . . |", BLACK, 210, 200);
    draw_char_from_top_right(view, "| . . . . . . |", BLACK, 226, 200);
    draw_char_from_top_right(view, "| . . . . . . |", BLACK, 242, 200);
    draw_char_from_top_right(view, "---------------", BLACK, 258, 200);
}

void draw_box_from_x_y(RunicView *view, int width, int height, int x, int y) {
    HDC hdc = GetDC(view->hwnd);
    Rectangle(hdc, x, y, x + width, y + height);
    ReleaseDC(view->hwnd, hdc);
}

void draw_player(RunicView *view, Player *p) {
    HDC hdc = GetDC(view->hwnd);

    RECT bounds;
    bounds.left = p->x;
    bounds.top = p->y;

    COLORREF oldColor = SetTextColor(hdc, get_color(p->color));
    DrawText(hdc, p->body, -1, &bounds, DT_LEFT);
    SetTextColor(hdc, oldColor);

    ReleaseDC(view->hwnd, hdc);
}

void move_player(RunicView *view, Player *p, int dx, int dy) {
    draw_char_at(view, " ", BLACK, p->x, p->y);

    p->x += dx;
    p->y += dy;

    draw_player(view, p);
}

void handle_input(RunicView *view, Player *p) {
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        move_player(view, p, -4, 0);
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        move_player(view, p, 4, 0);
    }
    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        move_player(view, p, 0, -4);
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        move_player(view, p, 0, 4);
    }
}

int main() {
    RunicView *view = create_window("Roguelike", 512, 512);

    Player player = {"@", BLUE, 250, 240};
    draw_player(view, &player);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        handle_input(view, &player);
    }

    return 0;
}
