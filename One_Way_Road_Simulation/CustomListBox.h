#pragma once

#include <windows.h>

class CustomListBox {
public:
    CustomListBox(HWND hListBox, HWND hScrollBar);

    void InsertItem(const wchar_t* item);

    void ClearItem();

private:
    HWND m_hListBox;
    HWND m_hScrollBar;

    void UpdateScrollBar();
};

void OnVScroll(HWND hwnd, WPARAM wParam, LPARAM lParam);