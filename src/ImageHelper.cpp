#include "ImageHelper.h"
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <memory>

#pragma comment(lib, "gdiplus.lib")

static HWND g_hImageWnd = NULL;
static Gdiplus::GdiplusStartupInput g_gdiplusStartupInput;
static ULONG_PTR g_gdiplusToken = 0;
static int g_gdiplusRefCount = 0;  // Счётчик ссылок

// Вспомогательная функция для конвертации UTF-8 в UTF-16
std::wstring Utf8ToUtf16(const std::string& utf8) {
    if (utf8.empty()) return L"";

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), (int)utf8.size(), NULL, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), (int)utf8.size(), &wstr[0], size_needed);
    return wstr;
}

LRESULT CALLBACK ImageWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Используем std::unique_ptr для автоматического управления памятью
        std::unique_ptr<std::string>* pPathPtr = reinterpret_cast<std::unique_ptr<std::string>*>(
            GetWindowLongPtr(hwnd, GWLP_USERDATA));

        if (pPathPtr && *pPathPtr && !(*pPathPtr)->empty()) {
            std::wstring wpath = Utf8ToUtf16(*(*pPathPtr));
            Gdiplus::Image image(wpath.c_str());

            if (image.GetLastStatus() == Gdiplus::Ok) {
                Gdiplus::Graphics graphics(hdc);

                RECT rc;
                GetClientRect(hwnd, &rc);
                int winW = rc.right - rc.left;
                int winH = rc.bottom - rc.top;

                int imgW = static_cast<int>(image.GetWidth());
                int imgH = static_cast<int>(image.GetHeight());

                if (imgW > 0 && imgH > 0) {
                    // Масштабирование с сохранением пропорций
                    float scaleX = static_cast<float>(winW) / imgW;
                    float scaleY = static_cast<float>(winH) / imgH;
                    float scale = min(scaleX, scaleY);

                    int drawW = static_cast<int>(imgW * scale);
                    int drawH = static_cast<int>(imgH * scale);
                    int drawX = (winW - drawW) / 2;
                    int drawY = (winH - drawH) / 2;

                    graphics.DrawImage(&image, drawX, drawY, drawW, drawH);
                }
            }
        }

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_SIZE:
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    case WM_DESTROY: {
        // Очищаем данные
        std::unique_ptr<std::string>* pPathPtr = reinterpret_cast<std::unique_ptr<std::string>*>(
            GetWindowLongPtr(hwnd, GWLP_USERDATA));
        delete pPathPtr;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
        g_hImageWnd = NULL;
        return 0;
    }
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

// Функция для инициализации GDI+
void InitGdiplus() {
    if (g_gdiplusRefCount == 0) {
        Gdiplus::GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);
    }
    g_gdiplusRefCount++;
}

// Функция для деинициализации GDI+
void ShutdownGdiplus() {
    if (g_gdiplusRefCount > 0) {
        g_gdiplusRefCount--;
        if (g_gdiplusRefCount == 0) {
            Gdiplus::GdiplusShutdown(g_gdiplusToken);
            g_gdiplusToken = 0;
        }
    }
}

void showImage(std::string path) {
    if (path.empty()) return;

    // Инициализируем GDI+
    InitGdiplus();

    // Закрываем предыдущее окно
    closeImage();

    HINSTANCE hInstance = GetModuleHandle(NULL);

    // Регистрируем класс окна (только один раз)
    static bool classRegistered = false;
    if (!classRegistered) {
        WNDCLASSEX wc = { 0 };
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = ImageWndProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszClassName = L"QuizImageViewer";  // Используем широкую строку!

        if (!RegisterClassEx(&wc)) {
            ShutdownGdiplus();
            return;
        }
        classRegistered = true;
    }

    // Создаём окно
    g_hImageWnd = CreateWindowExW(
        WS_EX_CLIENTEDGE,
        L"QuizImageViewer",
        L"Картинка к вопросу",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,  // WS_OVERLAPPEDWINDOW включает системное меню и возможность изменения размера
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL,
        hInstance,
        NULL
    );

    if (g_hImageWnd) {
        // Сохраняем путь в userdata окна (используем указатель на unique_ptr)
        auto* pPathPtr = new std::unique_ptr<std::string>(new std::string(path));
        SetWindowLongPtr(g_hImageWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pPathPtr));

        ShowWindow(g_hImageWnd, SW_SHOW);
        UpdateWindow(g_hImageWnd);
    }
    else {
        ShutdownGdiplus();
    }
}

void closeImage() {
    if (g_hImageWnd && IsWindow(g_hImageWnd)) {
        DestroyWindow(g_hImageWnd);
        g_hImageWnd = NULL;

        // Уменьшаем счётчик GDI+ при закрытии последнего окна
        if (g_hImageWnd == NULL) {
            ShutdownGdiplus();
        }
    }
}

// Добавьте эту функцию для очистки при завершении программы
void cleanupImageHelper() {
    closeImage();
    while (g_gdiplusRefCount > 0) {
        ShutdownGdiplus();
    }
}