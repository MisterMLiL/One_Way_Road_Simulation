#include "CustomListBox.h"

CustomListBox::CustomListBox(HWND hListBox, HWND hScrollBar) : m_hListBox(hListBox), m_hScrollBar(hScrollBar) {
    UpdateScrollBar();
}

void CustomListBox::InsertItem(const wchar_t* item) {
    SendMessage(m_hListBox, LB_ADDSTRING, 0, (LPARAM)item);
    UpdateScrollBar();
}

void CustomListBox::ClearItem() {
    SendMessage(m_hListBox, LB_RESETCONTENT, 0, 0);
    UpdateScrollBar();
}

void CustomListBox::UpdateScrollBar() {
    int itemCount = SendMessage(m_hListBox, LB_GETCOUNT, 0, 0);

    // Установка диапазона полосы прокрутки
    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE;
    si.nMin = 0;
    si.nMax = max(0, itemCount - 13);
    si.nPage = 1;

    SetScrollInfo(m_hScrollBar, SB_CTL, &si, TRUE);
}