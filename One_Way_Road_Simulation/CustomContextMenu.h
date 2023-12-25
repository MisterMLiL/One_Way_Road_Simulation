#pragma once

#include <windows.h>

#define contextMenuItemAdd 1
#define contextMenuItemDelete 2
#define contextMenuItemClear 3

class ContextMenu {
public:
    ContextMenu() : hMenu(nullptr) {
        // Создаем контекстное меню
        hMenu = CreatePopupMenu();
        AppendMenu(hMenu, MF_STRING, contextMenuItemAdd, L"Новый автомобиль");
        AppendMenu(hMenu, MF_STRING, contextMenuItemDelete, L"Удалить автомобиль");
        AppendMenu(hMenu, MF_STRING, contextMenuItemClear, L"Очистить очередь");
    }

    ~ContextMenu() {
        // Уничтожаем контекстное меню при завершении
        DestroyMenu(hMenu);
    }

    void ShowContextMenu(HWND hWnd, POINT point) {
        // Отображаем контекстное меню в указанной точке
        TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
    }

private:
    HMENU hMenu;
};