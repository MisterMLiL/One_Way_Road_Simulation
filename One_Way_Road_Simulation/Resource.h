//{{NO_DEPENDENCIES}}
// Включаемый файл, созданный в Microsoft Visual C++.
// Используется One_Way_Road_Simulation.rc

#define IDS_APP_TITLE			103

#define IDR_MAINFRAME			128
#define IDD_ONEWAYROADSIMULATION_DIALOG	102
#define IDD_ABOUTBOX			103
#define IDM_ABOUT				104
#define IDM_EXIT				105
#define IDI_ONEWAYROADSIMULATION			107
#define IDI_SMALL				108
#define IDC_ONEWAYROADSIMULATION			109
#define IDC_MYICON				2
#ifndef IDC_STATIC
#define IDC_STATIC				-1
#endif

// Идентификаторы ресурсов
#define IDC_LIGHT1 1001
#define IDC_LIGHT2 1002
#define IDC_WAY1 1003
#define IDC_WAY2 1004
#define IDC_LISTBOX 1005
#define IDC_BUTTON 1007
#define IDC_CARSLEFT 1008
#define IDC_CARSRIGHT 1009
#define IDC_COMBOBOX 1010
#define IDC_COMBOBOX 1010

#define IDD_DIALOG 11010
#define IDC_OK 11011

#define IDC_WORK_TIME 2001
#define IDC_LIGHT_TIMER 2002
#define IDC_CAR_TIMER 2003

#define IDC_SCROLLBAR 3001
#define IDC_STATUSBAR 3002

#define IDC_RANDOM 4002
#define IDC_DEFAULT 4003

const wchar_t* Description = L"Описание:\n\
- Дорога с двусторонним движением имеет участок с односторонним движением длиной 500 м из - за ремонтных работ.\n\
- Светофоры находятся на обоих концах этого участка.\n\
- Светофоры регулируют движение по участку, позволяя движение в одном направлении на заданный интервал времени.\n\
- Когда загорается зеленый свет, автомобили проезжают участок.\n\
- Автомобиль, подъезжающий к участку, проезжает его без задержек при горящем зеленом свете, если перед светофором нет других машин.";

#define CheckLight if ((road.getLight1().getColor() == TrafficLight::Color::GREEN &&\
road.getLight2().getColor() == TrafficLight::Color::GREEN) ||\
(road.getLight1().getColor() == TrafficLight::Color::RED &&\
    road.getLight2().getColor() == TrafficLight::Color::RED))\
{\
    road.setLight1(TrafficLight::Color::RED);\
    road.setLight2(TrafficLight::Color::RED);\
SendMessage(hwndRadioButton1, BM_SETCHECK, BST_UNCHECKED, 0); \
SendMessage(hwndRadioButton2, BM_SETCHECK, BST_UNCHECKED, 0); \
SendMessage(hwndCheckButton1, BM_SETCHECK, BST_UNCHECKED, 0); \
SendMessage(hwndCheckButton2, BM_SETCHECK, BST_UNCHECKED, 0); \
SetWindowText(hwndCheckButton1, road.getLight1().ToString()); \
SetWindowText(hwndCheckButton2, road.getLight2().ToString()); \
isGreen = false;\
road.setSide(false);\
break; \
}

// Следующие стандартные значения для новых объектов
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NO_MFC					130
#define _APS_NEXT_RESOURCE_VALUE	129
#define _APS_NEXT_COMMAND_VALUE		32771
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		110
#endif
#endif
