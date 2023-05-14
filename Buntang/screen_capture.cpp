#include "screen_capture.h"

namespace win {

    //T1113
    DWORD ScreenCaptureMainMonitor()
    {
        DWORD result = ERROR_SUCCESS;

        HMONITOR hMonitor = ::MonitorFromWindow(::GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);

        MONITORINFO monitorInfo;
        monitorInfo.cbSize = sizeof(monitorInfo);
        GetMonitorInfoW(hMonitor, &monitorInfo);
        int monitorWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
        int monitorHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

        HDC hdcScreen = ::GetDC(nullptr);
        HDC hdcMem = ::CreateCompatibleDC(hdcScreen);

        BITMAPINFOHEADER bi = { };
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = monitorWidth;
        bi.biHeight = monitorHeight;
        bi.biPlanes = 1;
        bi.biBitCount = 24;
        bi.biCompression = BI_RGB;
        void* pBits = nullptr;

        HBITMAP hBitmap = ::CreateDIBSection(hdcScreen, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &pBits, NULL, 0);
        if (hBitmap == nullptr)
            return ::GetLastError();
        HGDIOBJ hOldBitmap = ::SelectObject(hdcMem, hBitmap);

        ::BitBlt(hdcMem, 0, 0, monitorWidth, monitorHeight, hdcScreen, monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, SRCCOPY);

        // Save the bitmap to a file (change the filename as desired)
        std::wstring filename = L"screenshot.bmp";
        HANDLE hFile = ::CreateFileW(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            return ::GetLastError();
        }

        BITMAP bitmap = { 0 };
        bitmap.bmType = 0;
        bitmap.bmWidth = monitorWidth;
        bitmap.bmHeight = monitorHeight;
        bitmap.bmWidthBytes = ((monitorWidth * 24 + 31) / 32) * 4;
        bitmap.bmPlanes = 1;
        bitmap.bmBitsPixel = 24;
        bitmap.bmBits = pBits;
        BITMAPFILEHEADER bfh = { 0 };
        bfh.bfType = 'MB';
        bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        bfh.bfSize = bfh.bfOffBits + bitmap.bmWidthBytes * bitmap.bmHeight;
        BITMAPINFOHEADER bih = { 0 };
        bih.biSize = sizeof(BITMAPINFOHEADER);
        bih.biWidth = monitorWidth;
        bih.biHeight = monitorHeight;
        bih.biPlanes = 1;
        bih.biBitCount = 24;
        bih.biCompression = BI_RGB;
        DWORD bytesWritten = 0;
        if (!::WriteFile(hFile, &bfh, sizeof(bfh), &bytesWritten, NULL) || bytesWritten != sizeof(bfh) ||
            !::WriteFile(hFile, &bih, sizeof(bih), &bytesWritten, NULL) ||
            bytesWritten != sizeof(bih) ||
            !::WriteFile(hFile, bitmap.bmBits, bitmap.bmWidthBytes * bitmap.bmHeight, &bytesWritten, NULL) ||
            bytesWritten != bitmap.bmWidthBytes * bitmap.bmHeight)
        {
            ::CloseHandle(hFile);
            return ::GetLastError();
        }
        ::CloseHandle(hFile);
        ::SelectObject(hdcMem, hOldBitmap);
        ::DeleteObject(hBitmap);
        ::DeleteDC(hdcMem);
        ::ReleaseDC(NULL, hdcScreen);

        return ERROR_SUCCESS;
    }


} //namespace win