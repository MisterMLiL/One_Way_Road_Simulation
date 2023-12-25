#include <windows.h>
#include <string>
#include <thread>
#include <commctrl.h>
#include <cmath>

#pragma comment(lib,"ComCtl32.Lib")

#include "Resource.h"

#include "Car.h"
#include "RoadSection.h"
#include "CustomListBox.h"
#include "MainMenu.h"
#include "CustomContextMenu.h"
#include "Timers.h"
#include "TimeStack.h"

std::random_device rd;
std::mt19937 gen(rd());

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void EnableButton(bool isEnabled);
void CreateCarQueue();
void SetGreenLightTime();
void MakeRandSituation();
void MakeDefaultSituation();
INT_PTR CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param);

HWND g_hScrollBar;

HWND hwndCheckButton1;
HWND hwndCheckButton2;

HWND hwndCarList1;
HWND hwndCarList2;

HWND hwndCarTimer;

HWND hwndRadioButton1;
HWND hwndRadioButton2;

HWND hwndLightTimer;
HWND hwndListBox;
HWND hwndStartStop;

HWND hwndMainMenu;

HWND hwndStatusWindow;

HWND hwndComboBox;

HWND hwndToolbar;

RoadSection road;
CustomListBox* customListBox;
TimeStack stack;

bool isTimerActive = false;
bool isGreen = false;

const double meanLeftWay = 12.0;
const double meanRightWay = 9.0;

int greenTime = 0;

DWORD startTime = 0;

LRESULT CALLBACK ListBoxSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
HWND CreateListBoxWithContextMenu(HWND hWndParent, ContextMenu* contextMenu, int x, int y, int width, int height);

struct ControlInfo {
    HWND hwndControl; // дескриптор элемента управления
    const char* strTooltip; // текст подсказки
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                            // Optional window styles.
        CLASS_NAME,                   // Window class
        L"Traffic Simulation",         // Window text
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX | WS_VISIBLE,           // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 440,

        NULL,      // Parent window   
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    road = RoadSection(false);

    CreateWindowEx(
        0,                          // Optional window styles.
        L"STATIC",                     // Window class
        L"Светофор 1",                         // Window text
        WS_VISIBLE | WS_CHILD, // Window style

        // Size and position
        10, 35, 80, 25,

        hwnd,     // Parent window  
        (HMENU)NULL, // Control ID
        hInstance, // Instance handle
        NULL      // Additional application data
    );

    // Создание чек-бокса для светофора 1
    hwndCheckButton1 = CreateWindowEx(
        0,                          // Optional window styles.
        L"BUTTON",                   // Window class
        road.getLight1().ToString(),                // Window text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, // Window style

        // Size and position
        110, 35, 100, 25,

        hwnd,     // Parent window  
        (HMENU)IDC_LIGHT1, // Control ID
        hInstance, // Instance handle
        NULL      // Additional application data
    );

    hwndLightTimer = CreateWindowEx(
        0,                          // Optional window styles.
        L"EDIT",                     // Window class
        L"55",                         // Window text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_READONLY, // Window style

        // Size and position
        220, 35, 100, 25,

        hwnd,     // Parent window  
        (HMENU)IDC_LIGHT_TIMER, // Control ID
        hInstance, // Instance handle
        NULL      // Additional application data
    );

    hwndComboBox = CreateWindow(
        L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
        350, 35, 100, 200, hwnd, (HMENU)IDC_COMBOBOX, hInstance, NULL);

    // Добавление элементов в ComboBox
    for (int i = 5; i <= 100; i += 5) {
        wchar_t buffer[10];
        _itow_s(i, buffer, 10);
        SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)buffer);
    }

    CreateWindowEx(
        0,                          // Optional window styles.
        L"STATIC",                     // Window class
        L"Светофор 2",                         // Window text
        WS_VISIBLE | WS_CHILD, // Window style

        // Size and position
        10, 65, 80, 25,

        hwnd,     // Parent window  
        (HMENU)NULL, // Control ID
        hInstance, // Instance handle
        NULL      // Additional application data
    );

    // Создание чек-бокса для светофора 2
    hwndCheckButton2 = CreateWindowEx(
        0,                          // Optional window styles.
        L"BUTTON",                   // Window class
        road.getLight2().ToString(),                // Window text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, // Window style

        // Size and position
        110, 65, 100, 25,

        hwnd,     // Parent window  
        (HMENU)IDC_LIGHT2, // Control ID
        hInstance, // Instance handle
        NULL      // Additional application data
    ); 
    
    CreateWindowEx(
        0,                          // Optional window styles.
        L"STATIC",                     // Window class
        L"Отчет работы симулятора",                         // Window text
        WS_VISIBLE | WS_CHILD, // Window style

        // Size and position
        10, 95, 180, 25,

        hwnd,     // Parent window  
        (HMENU)NULL, // Control ID
        hInstance, // Instance handle
        NULL      // Additional application data
    );

    // Создание радиокнопки для начала движения в первом направлении
    hwndRadioButton1 = CreateWindowEx(
        0,                          // Optional window styles.
        L"BUTTON",                   // Window class
        L"Первое", // Window text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, // Window style

        // Size and position
        240, 95, 100, 25,

        hwnd,     // Parent window  
        (HMENU)IDC_WAY1, // Control ID
        hInstance, // Instance handle
        NULL      // Additional application data
    );

    // Создание радиокнопки для начала движения во втором направлении
    hwndRadioButton2 = CreateWindowEx(
        0,                          // Optional window styles.
        L"BUTTON",                   // Window class
        L"Второе", // Window text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, // Window style

        // Size and position
        350, 95, 100, 25,

        hwnd,     // Parent window  
        (HMENU)IDC_WAY2, // Control ID
        hInstance, // Instance handle
        NULL      // Additional application data
    );

    // Создание таблицы для отчета работы потока
    hwndListBox = CreateWindowEx(
        WS_EX_CLIENTEDGE,                          // Optional window styles.
        L"LISTBOX",                  // Window class
        L"",                         // Window text
        WS_CHILD | WS_VISIBLE | LBS_NOTIFY, // Window style

        // Size and position
        10, 125, 200, 200,

        hwnd,     // Parent window  
        (HMENU)IDC_LISTBOX, // Control ID
        hInstance, // Instance handle
        NULL      // Additional application data
    );

    hwndCarTimer = CreateWindowEx(
        0,                          // Optional window styles.
        L"EDIT",                     // Window class
        L"0",                         // Window text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_READONLY, // Window style

        // Size and position
        240, 325, 210, 25,

        hwnd,     // Parent window  
        (HMENU)IDC_CAR_TIMER, // Control ID
        hInstance, // Instance handle
        NULL      // Additional application data
    );

    // Кнопка для запуска и остановки симулятора
    hwndStartStop = CreateWindowExW(
        0,                          // Optional window styles.
        L"BUTTON",                     // Window class
        L"Запустить",                         // Window text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_READONLY, // Window style

        // Size and position
        480, 325, 100, 25,

        hwnd,     // Parent window  
        (HMENU)IDC_BUTTON, // Control ID
        hInstance, // Instance handle
        NULL      // Additional application data
    );

    const int ImageListID = 0;
    const int numButtons = 3;
    const int bitmapSize = 16;

    const DWORD buttonStyles = BTNS_AUTOSIZE;

    HIMAGELIST g_hImageList = NULL;

    // Create the toolbar.
    hwndToolbar = CreateWindowExW(0, TOOLBARCLASSNAME, NULL,
        WS_CHILD | TBSTYLE_WRAPABLE | WS_DLGFRAME | TBSTYLE_TOOLTIPS, 0, 0, 0, 0,
        hwnd, NULL, hInstance, NULL);

    if (hwndToolbar == NULL)
        return NULL;

    // Create the image list.
    g_hImageList = ImageList_Create(bitmapSize, bitmapSize,   // Dimensions of individual bitmaps.
        ILC_COLOR16 | ILC_MASK,   // Ensures transparent background.
        numButtons, 0);

    // Set the image list.
    SendMessage(hwndToolbar, TB_SETIMAGELIST,
        (WPARAM)ImageListID,
        (LPARAM)g_hImageList);

    // Load the button images.
    SendMessage(hwndToolbar, TB_LOADIMAGES,
        (WPARAM)IDB_STD_SMALL_COLOR,
        (LPARAM)HINST_COMMCTRL);

    TBBUTTON tbButtons[numButtons] =
    {
        { 33, -1, TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L"" },
        { MAKELONG(STD_REPLACE,  ImageListID), IDC_RANDOM,  TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L"" },
        { MAKELONG(STD_UNDO, ImageListID), IDC_DEFAULT, TBSTATE_ENABLED, buttonStyles, {0}, 0, (INT_PTR)L""}
    };

    SendMessage(hwndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(hwndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);

    SendMessage(hwndToolbar, TB_AUTOSIZE, 0, 0);
    ShowWindow(hwndToolbar, TRUE);

    ContextMenu contextMenu;

    hwndCarList1 = CreateListBoxWithContextMenu(hwnd, &contextMenu, 240, 125, 100, 200);
    hwndCarList2 = CreateListBoxWithContextMenu(hwnd, &contextMenu, 350, 125, 100, 200);

    g_hScrollBar = CreateWindowEx(0, L"SCROLLBAR", NULL,
        WS_CHILD | WS_VISIBLE | SBS_VERT,
        210, 125, 20, 195, hwnd, (HMENU)IDC_SCROLLBAR, hInstance, NULL);

    customListBox = new CustomListBox(hwndListBox, g_hScrollBar);

    int widths[5] = { 100, 200, 300, 400, 584 };

    RECT rcClient;
    GetClientRect(hwnd, &rcClient);

    hwndStatusWindow = CreateWindow(STATUSCLASSNAME, L"",
        WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0, hwnd, (HMENU)IDC_STATUSBAR, hInstance, NULL);

    SendMessage(hwndStatusWindow, SB_SETPARTS, 5, (LPARAM)widths);

    SendMessage(hwndStatusWindow, SB_SETTEXT, 0, (LPARAM)L"Время работы");

    SendMessage(hwndStatusWindow, SB_SETTEXT, 1, (LPARAM)L"не запущенно");

    SendMessage(hwndStatusWindow, SB_SETTEXT, 2, (LPARAM)L"СВОА");

    SendMessage(hwndStatusWindow, SB_SETTEXT, 3, (LPARAM)L"0");

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static UINT_PTR timerId = 0;

    static DWORD startTimeLight = 0;

    static DWORD redTime = 10983; // 55 секунд = 55000

    static int msgboxID;


    switch (uMsg)
    {
    case WM_NOTIFY:
    {
        LPTOOLTIPTEXT TTStr;
        TTStr = (LPTOOLTIPTEXT)lParam;
        if (TTStr->hdr.code != TTN_NEEDTEXT)
            break;
        switch (TTStr->hdr.idFrom)
        {
        case IDC_RANDOM:
            TTStr->lpszText = (LPWSTR)L"Рандомное значение";
            break;
        case IDC_DEFAULT:
            TTStr->lpszText = (LPWSTR)L"Стандартные значения";
            break;
        }
    }
    break;
    case WM_CREATE:
        CreateMainMenu(hwnd);
        break;

    case WM_VSCROLL:
        OnVScroll(hwnd, wParam, lParam);
        break;

    case WM_TIMER:
        if (!isTimerActive)
        {
            KillTimer(hwnd, timerId);
            timerId = 0;

            SetWindowText(hwndStartStop, L"Запустить");

            EnableButton(true);

            break;
        }

        if (timerId != 0)
        {
            DWORD elapsedTime = (GetTickCount64() - startTime) / 1000;
            SendMessage(hwndStatusWindow, SB_SETTEXT, 1, (LPARAM)std::to_wstring(elapsedTime).c_str());
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case menuItem1:
            msgboxID = MessageBox(
                NULL,
                L"Автор:\nСтудент группы ПИ-20а\nЛазуренко Е. А.",
                L"Автор",
                MB_OK
            );
            break;

        case menuItem2:
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG), hwnd, DialogProc);

            /*msgboxID = MessageBox(
                hwnd,
                Description,
                L"Краткое описание",
                MB_ICONINFORMATION | MB_OK
            );*/
            break;

        case IDC_BUTTON:
            if (timerId == 0)
            {
                SetGreenLightTime();

                if (greenTime == 0)
                {
                    msgboxID = MessageBox(
                        hwnd,
                        L"Не выбрано время зеленого света!",
                        L"Краткое описание",
                        MB_ICONINFORMATION | MB_OK
                    );
                    break;
                }

                customListBox->ClearItem();

                SetWindowText(hwndStartStop, L"Остановить");
                SendMessage(hwndStatusWindow, SB_SETTEXT, 1, (LPARAM)L"0");
                SendMessage(hwndStatusWindow, SB_SETTEXT, 3, (LPARAM)L"0");

                startTime = GetTickCount64();
                timerId = SetTimer(hwnd, 1, 100, NULL); // 1000 миллисекунд = 1 секунда

                EnableButton(false);
            }
            else
            {
                KillTimer(hwnd, timerId);
                timerId = 0;

                SetWindowText(hwndStartStop, L"Запустить");

                EnableButton(true);
            }

            if (isTimerActive) {
                isTimerActive = false;
            }
            else {
                CreateCarQueue();
                isTimerActive = true;
                std::thread(TrafficLightTimerUpdater).detach();
                std::thread(TrafficTimerUpdater).detach();
                std::thread(AverageWaitingTimeCar).detach();
            }
            break;

        case IDC_LIGHT1:
            switch (HIWORD(wParam)) {
            case BN_CLICKED:
                if (road.getLight1().getColor() == TrafficLight::Color::GREEN)
                {
                    road.setLight1(TrafficLight::Color::RED);
                    SendMessage(hwndRadioButton2, BM_SETCHECK, BST_CHECKED, 0);
                    SendMessage(hwndRadioButton1, BM_SETCHECK, BST_UNCHECKED, 0);
                    road.setSide(true);
                }
                else
                {
                    road.setLight1(TrafficLight::Color::GREEN);
                    SendMessage(hwndRadioButton1, BM_SETCHECK, BST_CHECKED, 0);
                    SendMessage(hwndRadioButton2, BM_SETCHECK, BST_UNCHECKED, 0);
                    isGreen = true;
                    road.setSide(false);
                }

                CheckLight

                SendMessage(hwndRadioButton1, BM_SETCHECK, BST_CHECKED, 0);
                SetWindowText(hwndCheckButton1, road.getLight1().ToString());
                break;
            }
            break;

        case IDC_LIGHT2:
            switch (HIWORD(wParam)) {
            case BN_CLICKED:
                if (road.getLight2().getColor() == TrafficLight::Color::GREEN)
                {
                    road.setLight2(TrafficLight::Color::RED);
                    SendMessage(hwndRadioButton1, BM_SETCHECK, BST_CHECKED, 0);
                    SendMessage(hwndRadioButton2, BM_SETCHECK, BST_UNCHECKED, 0);
                    road.setSide(false);
                }
                else
                {
                    road.setLight2(TrafficLight::Color::GREEN);
                    SendMessage(hwndRadioButton2, BM_SETCHECK, BST_CHECKED, 0);
                    SendMessage(hwndRadioButton1, BM_SETCHECK, BST_UNCHECKED, 0);
                    isGreen = true;
                    road.setSide(true);
                }

                CheckLight

                SetWindowText(hwndCheckButton2, road.getLight2().ToString());
                break;
            }
            break;

        case IDC_WAY1:
            switch (HIWORD(wParam)) {
            case BN_CLICKED:
                road.setLight1(TrafficLight::Color::GREEN);
                road.setLight2(TrafficLight::Color::RED);

                SendMessage(hwndCheckButton1, BM_SETCHECK, BST_CHECKED, 0);
                SendMessage(hwndCheckButton2, BM_SETCHECK, BST_UNCHECKED, 0);

                SetWindowText(hwndCheckButton1, road.getLight1().ToString());
                SetWindowText(hwndCheckButton2, road.getLight2().ToString());

                isGreen = true;

                road.setSide(false);
                break;
            }
            break;

        case IDC_WAY2:
            switch (HIWORD(wParam)) {
            case BN_CLICKED:
                road.setLight1(TrafficLight::Color::RED);
                road.setLight2(TrafficLight::Color::GREEN);

                SendMessage(hwndCheckButton1, BM_SETCHECK, BST_UNCHECKED, 0);
                SendMessage(hwndCheckButton2, BM_SETCHECK, BST_CHECKED, 0);

                SetWindowText(hwndCheckButton1, road.getLight1().ToString());
                SetWindowText(hwndCheckButton2, road.getLight2().ToString());

                isGreen = true;

                road.setSide(true);
                break;
            }
            break;

        case IDC_RANDOM:
            MakeRandSituation();
            break;

        case IDC_DEFAULT:
            MakeDefaultSituation();
            break;
        }
        break;

    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int getRandomValue(int min, int max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(gen);
}

// Генерация случайного булевого значения
bool getRandomBool() {
    std::uniform_int_distribution<int> distribution(0, 1);
    return distribution(gen) == 1;
}

void MakeRandSituation() {
    // 1. Значение сигнала светофора
    bool trafficLightSignal1 = getRandomBool();

    bool trafficLightSignal2 = getRandomBool();

    // 2. Количество автомобилей на первой дороге
    int carsOnFirstRoad = getRandomValue(0, 10);

    // 3. Количество автомобилей на второй дороге
    int carsOnSecondRoad = getRandomValue(0, 10);

    // 4. Время работы зеленого света
    int greenLightTime = getRandomValue(5, 55) / 5 * 5; // Шаг 5

    if (trafficLightSignal1 == trafficLightSignal2)
    {
        road.setLight1(TrafficLight::Color::RED);
        road.setLight2(TrafficLight::Color::RED);
        SendMessage(hwndRadioButton1, BM_SETCHECK, BST_UNCHECKED, 0);
        SendMessage(hwndRadioButton2, BM_SETCHECK, BST_UNCHECKED, 0);
        SendMessage(hwndCheckButton1, BM_SETCHECK, BST_UNCHECKED, 0);
        SendMessage(hwndCheckButton2, BM_SETCHECK, BST_UNCHECKED, 0);
        SetWindowText(hwndCheckButton1, road.getLight1().ToString());
        SetWindowText(hwndCheckButton2, road.getLight2().ToString());
        isGreen = false;
        road.setSide(false);
    }
    else if (trafficLightSignal1)
    {
        road.setLight1(TrafficLight::Color::GREEN);
        road.setLight2(TrafficLight::Color::RED);
        SendMessage(hwndRadioButton1, BM_SETCHECK, BST_CHECKED, 0);
        SendMessage(hwndRadioButton2, BM_SETCHECK, BST_UNCHECKED, 0);
        SendMessage(hwndCheckButton1, BM_SETCHECK, BST_CHECKED, 0);
        SendMessage(hwndCheckButton2, BM_SETCHECK, BST_UNCHECKED, 0);
        SetWindowText(hwndCheckButton1, road.getLight1().ToString());
        SetWindowText(hwndCheckButton2, road.getLight2().ToString());
        isGreen = true;
        road.setSide(false);
    }
    else
    {
        road.setLight1(TrafficLight::Color::RED);
        road.setLight2(TrafficLight::Color::GREEN);
        SendMessage(hwndRadioButton1, BM_SETCHECK, BST_UNCHECKED, 0);
        SendMessage(hwndRadioButton2, BM_SETCHECK, BST_CHECKED, 0);
        SendMessage(hwndCheckButton1, BM_SETCHECK, BST_UNCHECKED, 0);
        SendMessage(hwndCheckButton2, BM_SETCHECK, BST_CHECKED, 0);
        SetWindowText(hwndCheckButton1, road.getLight1().ToString());
        SetWindowText(hwndCheckButton2, road.getLight2().ToString());
        isGreen = true;
        road.setSide(true);
    }

    SendMessage(hwndCarList1, LB_RESETCONTENT, 0, 0);

    for (int i = 0; i < carsOnFirstRoad; i++)
    {
        SendMessage(hwndCarList1, LB_ADDSTRING, 0, (LPARAM)L"Автомобиль");
    }

    SendMessage(hwndCarList2, LB_RESETCONTENT, 0, 0);

    for (int i = 0; i < carsOnSecondRoad; i++)
    {
        SendMessage(hwndCarList2, LB_ADDSTRING, 0, (LPARAM)L"Автомобиль");
    }

    // Находим индекс строки с нужным значением
    int indexToSet = SendMessage(hwndComboBox, CB_FINDSTRINGEXACT, -1, reinterpret_cast<LPARAM>(std::to_wstring(greenLightTime).c_str()));

    // Если строка найдена, устанавливаем текущий выбор
    if (indexToSet != CB_ERR) {
        SendMessage(hwndComboBox, CB_SETCURSEL, indexToSet, 0);
    }
}

void MakeDefaultSituation() {
    road.setLight1(TrafficLight::Color::RED);
    road.setLight2(TrafficLight::Color::RED);
    SendMessage(hwndRadioButton1, BM_SETCHECK, BST_UNCHECKED, 0);
    SendMessage(hwndRadioButton2, BM_SETCHECK, BST_UNCHECKED, 0);
    SendMessage(hwndCheckButton1, BM_SETCHECK, BST_UNCHECKED, 0);
    SendMessage(hwndCheckButton2, BM_SETCHECK, BST_UNCHECKED, 0);
    SetWindowText(hwndCheckButton1, road.getLight1().ToString());
    SetWindowText(hwndCheckButton2, road.getLight2().ToString());
    isGreen = false;
    road.setSide(false);

    SendMessage(hwndCarList1, LB_RESETCONTENT, 0, 0);
    SendMessage(hwndCarList2, LB_RESETCONTENT, 0, 0);

    SendMessage(hwndComboBox, CB_SETCURSEL, CB_ERR, 0);
}

void SetGreenLightTime() {
    int selectedIndex = SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0);

    if (selectedIndex != CB_ERR)
    {
        int textLength = SendMessage(hwndComboBox, CB_GETLBTEXTLEN, selectedIndex, 0);
        wchar_t* buffer = new wchar_t[textLength + 1];
        SendMessage(hwndComboBox, CB_GETLBTEXT, selectedIndex, (LPARAM)buffer);

        int selectedValue = std::stoi(buffer);
        delete[] buffer;

        greenTime = selectedValue;
    }
}

// Функция для диалогового окна
INT_PTR CALLBACK DialogProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param) {
    switch (message) {
    case WM_INITDIALOG: {
        SetWindowText(GetDlgItem(hwnd, IDC_STATIC), Description);
        return TRUE;
    }
    case WM_COMMAND: {
        if (LOWORD(w_param) == IDC_OK || LOWORD(w_param) == IDCANCEL) {
            EndDialog(hwnd, LOWORD(w_param));
            return TRUE;
        }
        break;
    }
    }
    return FALSE;
}

void CreateMainMenu(HWND hwnd) {
    HMENU menu = CreateMenu();
    HMENU subMenu = CreateMenu();

    AppendMenuW(subMenu, MF_STRING, menuItem1, L"Автор");
    AppendMenuW(subMenu, MF_STRING, menuItem2, L"Краткое описание");

    AppendMenuW(menu, MF_POPUP, (UINT_PTR)subMenu, L"Справка");

    SetMenu(hwnd, menu);
}

void TrafficLightTimerUpdater() {
    while (isTimerActive) {
        int timerValue = 10;

        if (isGreen)
        {
            timerValue = greenTime;
        }

        while (isTimerActive && timerValue > 0) {
            // Обновление значения таймера в EditBox
            SetWindowText(hwndLightTimer, std::to_wstring(timerValue).c_str());

            std::this_thread::sleep_for(std::chrono::seconds(1));
            timerValue--;
        }

        if (!isTimerActive)
        {
            return;
        }

        SetWindowText(hwndLightTimer, L"0");

        road.switchLights();

        SetWindowText(hwndCheckButton1, road.getLight1().ToString());
        SetWindowText(hwndCheckButton2, road.getLight2().ToString());

        // Переключение состояния кнопок в зависимости от цвета светофора
        if (road.getLight1().getColor() == TrafficLight::Color::RED &&
            road.getLight2().getColor() == TrafficLight::Color::RED) {
            SendMessage(hwndCheckButton1, BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hwndCheckButton2, BM_SETCHECK, BST_UNCHECKED, 0);

            SendMessage(hwndRadioButton1, BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hwndRadioButton2, BM_SETCHECK, BST_UNCHECKED, 0);

            customListBox->InsertItem(L"Оба крассные");
            //SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)L"Оба крассные");

            isGreen = false;
        }
        else {
            if (road.getLight1().getColor() == TrafficLight::Color::GREEN)
            {
                SendMessage(hwndCheckButton1, BM_SETCHECK, BST_CHECKED, 0);
                customListBox->InsertItem(L"Первый зеленый");
                SendMessage(hwndRadioButton1, BM_SETCHECK, BST_CHECKED, 0);
                //SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)L"Первый зеленый");
            }
            else
            {
                SendMessage(hwndCheckButton2, BM_SETCHECK, BST_CHECKED, 0);
                customListBox->InsertItem(L"Второй зеленый");
                SendMessage(hwndRadioButton2, BM_SETCHECK, BST_CHECKED, 0);
                //SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)L"Второй зеленый");
            }

            isGreen = true;
        }
    }
}

void TrafficTimerUpdater() {
    if (road.carQueueIsEmpty())
    {
        isTimerActive = false;
        return;
    }

    bool isFirstCar = true;

    while (isTimerActive) {
        while (!isGreen)
        {
            isFirstCar = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (road.currentQueueEmpty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        int waitValue = road.getWaitingTime() * 1000;
        bool currentBool = road.getSide();

        while (isTimerActive && waitValue > 0) {
            SetWindowText(hwndCarTimer, std::to_wstring(waitValue).c_str());

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            waitValue -= 100;
        }

        if (road.carQueueIsEmpty())
        {
            isTimerActive = false;
            return;
        }

        if (waitValue <= 0 && !isFirstCar) {
            waitValue = 0;

            SetWindowText(hwndCarTimer, std::to_wstring(waitValue).c_str());

            customListBox->InsertItem(L"Автомобиль подъехал к светофору");
        }

        isFirstCar = false;

        road.setWaitingTime(currentBool, waitValue);

        if (!isTimerActive)
        {
            return;
        }

        int timerValue = 2;

        if (!isGreen)
        {
            continue;
        }

        if (isGreen)
        {
            stack.push(GetTickCount64());

            customListBox->InsertItem(L"Автомобиль начал проезжать");

            while (isTimerActive && timerValue > 0) {
                SetWindowText(hwndCarTimer, std::to_wstring(timerValue).c_str());

                std::this_thread::sleep_for(std::chrono::seconds(1));
                timerValue--;
            }

            if (timerValue == 0)
            {
                customListBox->InsertItem(L"Автомобиль проехал");

                road.deleteCar();

                if (!road.getSide())
                {
                    SendMessage(hwndCarList1, LB_DELETESTRING, 0, (LPARAM)L"Автомобиль");
                }
                else
                {
                    SendMessage(hwndCarList2, LB_DELETESTRING, 0, (LPARAM)L"Автомобиль");
                }
            }
        }

        if (road.carQueueIsEmpty())
        {
            isTimerActive = false;
            return;
        }
    }
}

void AverageWaitingTimeCar() { // Не работает
    int countCar = 0;
    double allWaitTime = 0;

    while (isTimerActive) {
        while (stack.isEmpty())
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }

        DWORD stackFirst = stack.top() - startTime;

        if (stackFirst < 0)
        {
            continue;
        }

        stack.pop();

        double waitTime = stackFirst / static_cast<double>(1000);

        countCar++;
        allWaitTime += waitTime;

        double avg = allWaitTime / countCar;

        SendMessage(hwndStatusWindow, SB_SETTEXT, 3, (LPARAM)std::to_wstring(avg).c_str());
    }
}

#pragma region Кастомный скрол

void OnVScroll(HWND hwnd, WPARAM wParam, LPARAM lParam) {
    int nScrollCode = LOWORD(wParam);
    int nPos = HIWORD(wParam);

    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
    GetScrollInfo(g_hScrollBar, SB_CTL, &si);

    switch (nScrollCode) {
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        si.nPos = nPos;
        break;

    case SB_LINEUP:
        si.nPos = max(0, si.nPos - 1);
        break;

    case SB_LINEDOWN:
        si.nPos = min(si.nMax, si.nPos + 1);
        break;
    }

    si.fMask = SIF_POS;
    SetScrollInfo(g_hScrollBar, SB_CTL, &si, TRUE);

    // Прокрутка ListBox
    SendMessage(hwndListBox, LB_SETTOPINDEX, si.nPos, 0);
}

#pragma endregion

#pragma region Списки автомобилей

LRESULT CALLBACK ListBoxSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    switch (uMsg) {
    case WM_CONTEXTMENU:
    {
        POINT point{};
        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);

        // Показываем контекстное меню
        ((ContextMenu*)dwRefData)->ShowContextMenu(hWnd, point);
        break;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case contextMenuItemAdd:
        {
            SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)L"Автомобиль");
            break;
        }
        case contextMenuItemDelete:
        {
            SendMessage(hWnd, LB_DELETESTRING, 0, (LPARAM)L"Автомобиль");
            break;
        }
        case contextMenuItemClear:
        {
            SendMessage(hWnd, LB_RESETCONTENT, 0, 0);
            break;
        }
        }
    }
    return 0;
    }

    // Вызываем оригинальную процедуру окна для обработки остальных сообщений
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

HWND CreateListBoxWithContextMenu(HWND hWndParent, ContextMenu* contextMenu, int x, int y, int width, int height) {
    // Создаем ListBox
    HWND hListBox = CreateWindow(
        L"LISTBOX",
        L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
        x, y, width, height,
        hWndParent,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    // Привязываем контекстное меню к ListBox
    SetWindowSubclass(hListBox, ListBoxSubclassProc, 0, (DWORD_PTR)contextMenu);

    return hListBox;
}

void CreateCarQueue() {
    road.clearAllCars();

    // Получение количества элементов в ListBox
    double itemCount1 = SendMessage(hwndCarList1, LB_GETCOUNT, 0, 0) - 1;
    double itemCount2 = SendMessage(hwndCarList2, LB_GETCOUNT, 0, 0) - 1;

    if (itemCount1 >= 0)
        road.addCar1(0);

    double center = itemCount1 / 2 + 0.5;
    double part = 1 / itemCount1 / 10;

    // Создание объектов Car и добавление их в очередь
    for (int i = 0; i < itemCount1; i++) {
        // 2.484907
        double time = 0;

        if (i == center)
        {
            time = exp(2.484907);
            road.addCar1(time);
            continue;
        }

        if (i + 1 < center)
        {
            time = exp(2.484907 - part * (i + 1));
        }
        else
        {
            time = exp(2.484907 + part * (i + 1));
        }

        road.addCar1(time);
    }

    if (itemCount2 >= 0)
        road.addCar2(0);

    center = itemCount2 / 2 + 0.5;
    part = 1 / itemCount2 / 100;

    for (int i = 0; i < itemCount2; i++) {
        // 2.197225
        double time = 0;

        if (i + 1 == center)
        {
            time = exp(2.197225);
            road.addCar2(time);
            continue;
        }

        if (i < center)
        {
            time = exp(2.197225 - part * (i + 1));
        }
        else
        {
            time = exp(2.197225 + part * (i + 1));
        }

        road.addCar2(time);
    }
}

#pragma endregion

void EnableButton(bool isEnabled) {
    EnableWindow(hwndCheckButton1, isEnabled);
    EnableWindow(hwndCheckButton2, isEnabled);
    EnableWindow(hwndRadioButton1, isEnabled);
    EnableWindow(hwndRadioButton2, isEnabled);
    EnableWindow(hwndComboBox, isEnabled);
}