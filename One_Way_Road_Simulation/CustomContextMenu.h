#pragma once

#include <windows.h>

#define contextMenuItemAdd 1
#define contextMenuItemDelete 2
#define contextMenuItemClear 3

class ContextMenu {
public:
    ContextMenu() : hMenu(nullptr) {
        // ������� ����������� ����
        hMenu = CreatePopupMenu();
        AppendMenu(hMenu, MF_STRING, contextMenuItemAdd, L"����� ����������");
        AppendMenu(hMenu, MF_STRING, contextMenuItemDelete, L"������� ����������");
        AppendMenu(hMenu, MF_STRING, contextMenuItemClear, L"�������� �������");
    }

    ~ContextMenu() {
        // ���������� ����������� ���� ��� ����������
        DestroyMenu(hMenu);
    }

    void ShowContextMenu(HWND hWnd, POINT point) {
        // ���������� ����������� ���� � ��������� �����
        TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
    }

private:
    HMENU hMenu;
};