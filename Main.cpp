#include "main.h"

LRESULT Wndproc(
    HWND hwnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam);

BOOL CALLBACK OptionsDialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ConnectByIPDialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

HWND DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE
    hinst, int cParts);

void ResizeRefreshButton();

extern "C" {
    HWND hMainWindow;
}

HWND hRefeshButton;
HWND hwndListView;
HWND hOptionsDialog;
HWND hwndStatus;
HWND hIPLabel;
HINSTANCE   g_hInst;

extern bool g_debugMode;

HICON hMainIcon = NULL;


#define ITEM_COUNT   100000

const char CLASS_NAME[] = "SAMPLauncherMain";
WNDCLASS mainWindowClass = { };

enum Controls {
    RefreshButtonCtrl = 1,
    ListViewCtrl
};

BOOL InitApplication(HINSTANCE hInstance)
{

    hMainIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDM_MAIN_ICON));
    WNDCLASSEX  wcex;
    ATOM        aReturn;

    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = 0;
    wcex.lpfnWndProc = (WNDPROC)Wndproc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDM_MAIN_MENU);
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIcon = hMainIcon;
    wcex.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDM_MAIN_ICON), IMAGE_ICON, 16, 16, 0);
    RegisterClassEx(&wcex);

    return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow)
{
    g_hInst = hInstance;
    InitApplication(hInstance);

    INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);


    hMainWindow = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        "SAMP Launcher",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 990, 710,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    HWND button = CreateWindow("BUTTON",
        TEXT("Refresh"),                        // dummy text
        WS_VISIBLE | WS_CHILD,                   // style
        0,                         // x position
        0,                         // y position
        0,                         // width
        0,                         // height
        hMainWindow,                // parent
        (HMENU)ID_REFRESH,        // ID
        hInstance,                   // instance
        NULL);                     // no extra data


    HWND filterGroup = CreateWindow("BUTTON", TEXT("Filter"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, 0, 0, 0, hMainWindow, (HMENU)ID_FILTER_GROUPBOX, hInstance, NULL);
    CreateWindow(
        "BUTTON",  // Predefined class; Unicode assumed 
        "Not Full",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,  // Styles 
        10,         // x position 
        25,         // y position 
        105,        // Button width
        15,        // Button height
        filterGroup,     // Parent window
        (HMENU)ID_FILTER_NOT_FULL,       // No menu.
        (HINSTANCE)hInstance,
        NULL);      // Pointer not needed.

    CreateWindow(
        "BUTTON",  // Predefined class; Unicode assumed 
        "Not Empty",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,  // Styles 
        10,         // x position 
        45,         // y position 
        105,        // Button width
        15,        // Button height
        filterGroup,     // Parent window
        (HMENU)ID_FILTER_NOT_EMPTY,       // No menu.
        (HINSTANCE)hInstance,
        NULL);      // Pointer not needed.

    CreateWindow(
        "BUTTON",  // Predefined class; Unicode assumed 
        "No Password",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,  // Styles 
        10,         // x position 
        65,         // y position 
        105,        // Button width
        15,        // Button height
        filterGroup,     // Parent window
        (HMENU)ID_FILTER_NO_PASSWORD,       // No menu.
        (HINSTANCE)hInstance,
        NULL);      // Pointer not needed.

    HWND hwndFavourites = CreateWindow(
        "BUTTON",  // Predefined class; Unicode assumed 
        "Favourites",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  // Styles 
        120,         // x position 
        25,         // y position 
        105,        // Button width
        15,        // Button height
        filterGroup,     // Parent window
        (HMENU)ID_QUERY_FAVOURITES,       // No menu.
        (HINSTANCE)hInstance,
        NULL);      // Pointer not needed.

    CreateWindow(
        "BUTTON",  // Predefined class; Unicode assumed 
        "Internet",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  // Styles 
        120,         // x position 
        45,         // y position 
        105,        // Button width
        15,        // Button height
        filterGroup,     // Parent window
        (HMENU)ID_QUERY_OPENSPY,       // No menu.
        (HINSTANCE)hInstance,
        NULL);      // Pointer not needed.

    CreateWindow(
        "BUTTON",  // Predefined class; Unicode assumed 
        "OpenSpy",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,  // Styles 
        120,         // x position 
        65,         // y position 
        105,        // Button width
        15,        // Button height
        filterGroup,     // Parent window
        (HMENU)ID_QUERY_OPENSPY_HOSTED,       // No menu.
        (HINSTANCE)hInstance,
        NULL);      // Pointer not needed.

    HWND serverInfoGroup = CreateWindow("BUTTON", TEXT("Selected Server"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 0, 0, 0, 0, hMainWindow, (HMENU)ID_SERVER_INFO_GROUPBOX, hInstance, NULL);

    hIPLabel = CreateWindow("STATIC",
        TEXT(""),                        // dummy text
        WS_VISIBLE | WS_CHILD,                   // style
        15,                         // x position
        20,                         // y position
        300,                         // width
        100,                         // height
        serverInfoGroup,                // parent
        (HMENU)ID_SERVER_IP_TEXT,        // ID
        hInstance,                   // instance
        NULL);                     // no extra data


    SendMessage(hwndFavourites, BM_SETCHECK, BST_CHECKED, NULL);

    ResizeRefreshButton();

    DoCreateStatusBar(hMainWindow, ID_STATUSBAR, hInstance, 1);

    ShowWindow(hMainWindow, nCmdShow);
    UpdateWindow(hMainWindow);

    init_gamespy();
    gamespy_refresh();

    SetTimer(hMainWindow, WM_USER + 3, 2000, NULL); //UI loop update timer

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        gamespy_think();
        if (hOptionsDialog != NULL) {
            if (IsDialogMessage(hOptionsDialog, &msg))
                continue;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

BOOL InsertListViewItems(HWND hwndListView)
{
    //empty the list
    ListView_DeleteAllItems(hwndListView);

    //set the number of items in the list
    //ListView_SetItemCount(hwndListView, ITEM_COUNT);

    return TRUE;
}
BOOL InitPlayerInfoListView(HWND listView) {
    LV_COLUMN   lvColumn;
    int         i;
    TCHAR       szString[2][20] = { TEXT("Player"), TEXT("Score") };

    //empty the list
    ListView_DeleteAllItems(listView);

    //initialize the columns
    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvColumn.fmt = LVCFMT_LEFT;
    for (i = 0; i < 2; i++)
    {
        if (i == 0) {
            lvColumn.cx = 220;
        }
        else {
            lvColumn.cx = 50;
        }

        lvColumn.pszText = szString[i];
        ListView_InsertColumn(listView, i, &lvColumn);
    }

    InsertListViewItems(listView);
    return TRUE;
}

BOOL InitRulesListView(HWND listView) {
    LV_COLUMN   lvColumn;
    int         i;
    TCHAR       szString[2][20] = { TEXT("Key"), TEXT("Value") };

    //empty the list
    ListView_DeleteAllItems(listView);

    //initialize the columns
    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvColumn.fmt = LVCFMT_LEFT;
    for (i = 0; i < 2; i++)
    {
        if (i == 1) {
            lvColumn.cx = 180;
        }
        else {
            lvColumn.cx = 90;
        }

        lvColumn.pszText = szString[i];
        ListView_InsertColumn(listView, i, &lvColumn);
    }

    InsertListViewItems(listView);
    return TRUE;
}

BOOL InitListView(HWND hwndListView)
{
    LV_COLUMN   lvColumn;
    int         i;
    TCHAR       szString[5][20] = {TEXT("Server Name"), TEXT("Players"), TEXT("Ping"), TEXT("Gamemode"), TEXT("Language")};

    //empty the list
    ListView_DeleteAllItems(hwndListView);

    //initialize the columns
    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvColumn.fmt = LVCFMT_LEFT;
    for (i = 0; i < 5; i++)
    {
        if (i == 0) {
            lvColumn.cx = 280;
        } 
        else if (i == 1 || i == 2) {
            lvColumn.cx = 55;
        }
        else {
            lvColumn.cx = 120;
        }
        lvColumn.pszText = szString[i];
        ListView_InsertColumn(hwndListView, i, &lvColumn);
    }

    InsertListViewItems(hwndListView);

    return TRUE;
}

void ResizeRefreshButton() {
    HWND     hwnd_button = GetDlgItem(hMainWindow, ID_REFRESH);
    RECT  rc;
    const int X_OFFSET = 75;
    const int Y_OFFSET = -5;
    GetClientRect(hMainWindow, &rc);

    rc.top = rc.bottom - X_OFFSET;
    rc.left = rc.left - Y_OFFSET;

    MoveWindow(hwnd_button,
        rc.left,
        rc.top,
        100,
        25,
        TRUE);


    HWND filtergroup_label = GetDlgItem(hMainWindow, ID_FILTER_GROUPBOX);
    GetClientRect(hMainWindow, &rc);

    const int FILTER_X_OFFSET = 175;
    const int FILTER_Y_OFFSET = -5;

    rc.top = rc.bottom - FILTER_X_OFFSET;
    rc.left = rc.left - FILTER_Y_OFFSET;

    MoveWindow(filtergroup_label,
        rc.left,
        rc.top,
        250,
        95,
        TRUE);

    HWND serverinfo_label = GetDlgItem(hMainWindow, ID_SERVER_INFO_GROUPBOX);
    GetClientRect(hMainWindow, &rc);

    const int SERVERINFO_X_OFFSET = 180;
    const int SERVERINFO_Y_OFFSET = -275;

    rc.top = rc.bottom - SERVERINFO_X_OFFSET;
    rc.left = rc.left - SERVERINFO_Y_OFFSET;

    MoveWindow(serverinfo_label,
        rc.left,
        rc.top,
        370,
        135,
        TRUE);

    //HWND ipinfo_label = GetDlgItem(serverinfo_label, ID_SERVER_IP_TEXT);

    //GetClientRect(hMainWindow, &rc);

    //const int SELECTED_INFO_X_OFFSET = 0;
    //const int SELECTED_INFO_Y_OFFSET = 0;

    //rc.top = rc.bottom - SELECTED_INFO_X_OFFSET;
    //rc.left = rc.left - SELECTED_INFO_Y_OFFSET;

    //MoveWindow(ipinfo_label,
    //    rc.left,
    //    rc.top,
    //    350,
    //    120,
    //    TRUE);


    
}

void ResizeListView(HWND hwndListView, HWND hwndParent)
{
    RECT  rc;

    const int X_OFFSET = -180;
    const int Y_OFFSET = -320;
    GetClientRect(hwndParent, &rc);

    rc.bottom += X_OFFSET;
    rc.right += Y_OFFSET;

    MoveWindow(hwndListView,
        rc.left,
        rc.top,
        rc.right - rc.left,
        rc.bottom - rc.top,
        TRUE);

    //only call this if we want the LVS_NOSCROLL style
    //PositionHeader(hwndListView);
}

HWND CreateListView(HINSTANCE hInstance, HWND hwndParent)
{
    DWORD       dwStyle;
    HWND        hwndListView;
    HIMAGELIST  himlSmall;
    HIMAGELIST  himlLarge;
    BOOL        bSuccess = TRUE;

    dwStyle = WS_TABSTOP |
        WS_CHILD |
        WS_BORDER |
        WS_VISIBLE |
        LVS_AUTOARRANGE |
        LVS_REPORT | LVS_SINGLESEL;

    hwndListView = CreateWindowEx(WS_EX_CLIENTEDGE,          // ex style
        WC_LISTVIEW,               // class name - defined in commctrl.h
        TEXT(""),                        // dummy text
        dwStyle,                   // style
        0,                         // x position
        0,                         // y position
        0,                         // width
        0,                         // height
        hwndParent,                // parent
        (HMENU)ID_LISTVIEW,        // ID
        g_hInst,                   // instance
        NULL);                     // no extra data

    if (!hwndListView)
        return NULL;

    ResizeListView(hwndListView, hwndParent);

    HWND playersListView = CreateWindowEx(WS_EX_CLIENTEDGE,          // ex style
        WC_LISTVIEW,               // class name - defined in commctrl.h
        TEXT(""),                        // dummy text
        dwStyle,                   // style
        0,                         // x position
        0,                         // y position
        0,                         // width
        0,                         // height
        hwndParent,                // parent
        (HMENU)ID_PLAYERS_LISTVIEW,        // ID
        g_hInst,                   // instance
        NULL);                     // no extra data

    HWND rulesListView = CreateWindowEx(WS_EX_CLIENTEDGE,          // ex style
        WC_LISTVIEW,               // class name - defined in commctrl.h
        TEXT(""),                        // dummy text
        dwStyle,                   // style
        0,                         // x position
        0,                         // y position
        0,                         // width
        0,                         // height
        hwndParent,                // parent
        (HMENU)ID_RULES_LISTVIEW,        // ID
        g_hInst,                   // instance
        NULL);                     // no extra data


    ResizeServerDetailsListViews(hwndParent);
    InitPlayerInfoListView(playersListView);
    InitRulesListView(rulesListView);


    return hwndListView;
}

void ResizeServerDetailsListViews(HWND parent) {

    RECT  rc;

    HWND hwndMainlistView = GetDlgItem(parent, ID_LISTVIEW);

    const int X_OFFSET = 0;
    const int Y_OFFSET = -310;
    GetClientRect(parent, &rc);

    //ShowWindow(hwndMainlistView, 0);

    //rc.bottom += X_OFFSET;
    rc.right += Y_OFFSET;

    HWND hwndListView = GetDlgItem(parent, ID_PLAYERS_LISTVIEW);

    MoveWindow(hwndListView,
        rc.right,
        0,
        300,
        rc.bottom - 195,
        TRUE);

    hwndListView = GetDlgItem(parent, ID_RULES_LISTVIEW);
    GetClientRect(parent, &rc);
    rc.right += Y_OFFSET;
    rc.top += 360;

    MoveWindow(hwndListView,
        rc.right,
        rc.bottom - 185,
        300,
        140,
        TRUE);


    

}
void on_click_browse_button() {

    OPENFILENAME ofn;       // common dialog box structure
    char szFile[260];       // buffer for file name
    HWND hwnd;              // owner window
    HANDLE hf;              // file handle

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hOptionsDialog;
    ofn.lpstrFile = szFile;
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "GTA SA exe\0*.exe\0All\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        HWND     hwndNameEdit = GetDlgItem(hOptionsDialog, IDC_BROWSE_EDIT);
        SendMessage(hwndNameEdit, WM_SETTEXT, 0, (LPARAM)szFile);
    }
}


INT_PTR CALLBACK ServerPropertiesDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hwndOwner = NULL;
    RECT rc, rcDlg, rcOwner;


    switch (uMsg)
    {
    case WM_COMMAND:
        switch (GET_WM_COMMAND_ID(wParam, lParam))
        {
        case IDOK:
            do_connect_selected_server();
            EndDialog(hOptionsDialog, TRUE);
            break;
        case IDCANCEL:
            EndDialog(hOptionsDialog, TRUE);
            break;
        }

    case WM_INITDIALOG:
        hOptionsDialog = hwndDlg;
        SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hMainIcon);
        SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)hMainIcon);

        // Get the owner window and dialog box rectangles. 

        if ((hwndOwner = GetParent(hwndDlg)) == NULL)
        {
            hwndOwner = GetDesktopWindow();
        }

        GetWindowRect(hwndOwner, &rcOwner);
        GetWindowRect(hwndDlg, &rcDlg);
        CopyRect(&rc, &rcOwner);

        // Offset the owner and dialog box rectangles so that right and bottom 
        // values represent the width and height, and then offset the owner again 
        // to discard space taken up by the dialog box. 

        OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
        OffsetRect(&rc, -rc.left, -rc.top);
        OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);

        // The new position is the sum of half the remaining space and the owner's 
        // original position. 

        SetWindowPos(hwndDlg,
            hMainWindow,
            rcOwner.left + (rc.right / 2),
            rcOwner.top + (rc.bottom / 2),
            0, 0,          // Ignores size arguments. 
            SWP_NOSIZE);
        init_prefs_text();
        if (GetDlgCtrlID((HWND)wParam) != IDM_OPTIONS_DIALOG)
        {
            SetFocus(GetDlgItem(hwndDlg, IDM_OPTIONS_DIALOG));
            return FALSE;
        }

        break;
    case WM_DESTROY:
        hOptionsDialog = NULL;
    default:
        DefWindowProc(hwndDlg, uMsg, wParam, lParam);
        break;
    }
    return FALSE;
}

INT_PTR CALLBACK ConnectByIPDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hwndOwner = NULL;
    RECT rc, rcDlg, rcOwner;


    switch (uMsg)
    {
    case WM_COMMAND:
        switch (GET_WM_COMMAND_ID(wParam, lParam))
        {
        case IDOK:
            do_joinbyip();
            EndDialog(hOptionsDialog, TRUE);
            break;
        }
        break;
    case WM_INITDIALOG:
        hOptionsDialog = hwndDlg;
        SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hMainIcon);
        SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)hMainIcon);

        // Get the owner window and dialog box rectangles. 

        if ((hwndOwner = GetParent(hwndDlg)) == NULL)
        {
            hwndOwner = GetDesktopWindow();
        }

        GetWindowRect(hwndOwner, &rcOwner);
        GetWindowRect(hwndDlg, &rcDlg);
        CopyRect(&rc, &rcOwner);

        // Offset the owner and dialog box rectangles so that right and bottom 
        // values represent the width and height, and then offset the owner again 
        // to discard space taken up by the dialog box. 

        OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
        OffsetRect(&rc, -rc.left, -rc.top);
        OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);

        // The new position is the sum of half the remaining space and the owner's 
        // original position. 

        SetWindowPos(hwndDlg,
            hMainWindow,
            rcOwner.left + (rc.right / 2),
            rcOwner.top + (rc.bottom / 2),
            0, 0,          // Ignores size arguments. 
            SWP_NOSIZE);
        init_prefs_text();
        if (GetDlgCtrlID((HWND)wParam) != IDM_OPTIONS_DIALOG)
        {
            SetFocus(GetDlgItem(hwndDlg, IDM_OPTIONS_DIALOG));
            return FALSE;
        }

        break;
    case WM_DESTROY:
        hOptionsDialog = NULL;
    default:
        DefWindowProc(hwndDlg, uMsg, wParam, lParam);
        break;
    }
    return FALSE;
}

INT_PTR CALLBACK OptionsDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hwndOwner = NULL;
    RECT rc, rcDlg, rcOwner;

    HWND hwndDebugModeChk = GetDlgItem(hwndDlg, IDC_DEBUG_MODE);
    int  debugMode = SendMessage(hwndDebugModeChk, BM_GETCHECK, NULL, NULL);

    switch (uMsg)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_REFRESH) {
            switch (HIWORD(wParam)) {
            case BN_CLICKED:
                gamespy_refresh();
                break;
            }

            break;
        }
        switch (GET_WM_COMMAND_ID(wParam, lParam))
        {
            case IDC_BROWSE_BUTTON:
                on_click_browse_button();
                break;
            case IDC_SAVE_BUTTON:
                on_click_save_prefs();
                break;
            case IDC_DEBUG_MODE:
                g_debugMode = debugMode;
            break;
        }
    break;
    case WM_INITDIALOG:
        hOptionsDialog = hwndDlg;
        SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hMainIcon);
        SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)hMainIcon);

        // Get the owner window and dialog box rectangles. 

        if ((hwndOwner = GetParent(hwndDlg)) == NULL)
        {
            hwndOwner = GetDesktopWindow();
        }

        GetWindowRect(hwndOwner, &rcOwner);
        GetWindowRect(hwndDlg, &rcDlg);
        CopyRect(&rc, &rcOwner);

        // Offset the owner and dialog box rectangles so that right and bottom 
        // values represent the width and height, and then offset the owner again 
        // to discard space taken up by the dialog box. 

        OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
        OffsetRect(&rc, -rc.left, -rc.top);
        OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);

        // The new position is the sum of half the remaining space and the owner's 
        // original position. 

        SetWindowPos(hwndDlg,
            hMainWindow,
            rcOwner.left + (rc.right / 2),
            rcOwner.top + (rc.bottom / 2),
            0, 0,          // Ignores size arguments. 
            SWP_NOSIZE);
        init_prefs_text();
        if (GetDlgCtrlID((HWND)wParam) != IDM_OPTIONS_DIALOG)
        {
            SetFocus(GetDlgItem(hwndDlg, IDM_OPTIONS_DIALOG));
            return FALSE;
        }
        break;
    case WM_DESTROY:
        hOptionsDialog = NULL;
        //intentional fallthrough
    default:
        DefWindowProc(hwndDlg, uMsg, wParam, lParam);
        break;
    }
    return FALSE;
}

void ResizeStatusText() {
    SendMessage(hwndStatus, WM_SIZE, 0, 0);
}
LRESULT Wndproc(
    HWND hwnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{
    CREATESTRUCT* createStruct;

    NMHDR* notifyMsg;

    switch (uMsg)
    {
    case WM_CREATE:
        createStruct = (CREATESTRUCT*)lParam;
        if (createStruct->hwndParent == NULL) { //parent window is null, must be main window
            // create the TreeView control
            hwndListView = CreateListView(g_hInst, hwnd);

            //initialize the TreeView control
            InitListView(hwndListView);
        }

        break;
    case WM_EXITSIZEMOVE:
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    case WM_SIZE:
        ResizeListView(hwndListView, hwnd);
        ResizeRefreshButton();
        ResizeStatusText();
        ResizeServerDetailsListViews(hwnd);
        //InvalidateRect(hwnd, NULL, TRUE);
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_REFRESH) {
            switch (HIWORD(wParam)) {
            case BN_CLICKED:
                gamespy_refresh();
                break;
            }

            break;
        }
        if (hOptionsDialog != NULL) {
            EndDialog(hOptionsDialog, TRUE);
        }
        switch (GET_WM_COMMAND_ID(wParam, lParam))
        {
        case ID_OPTIONS_PREFERENCES:
            CreateDialog(g_hInst, MAKEINTRESOURCE(IDM_OPTIONS_DIALOG), hMainWindow, OptionsDialogProc);
            break;
        case ID_OPTIONS_CONNECTBYIP:
            CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_CONNECTIP_DIALOG), hMainWindow, ConnectByIPDialogProc);
            break;
        case ID_FILE_EXIT:
            DestroyWindow(hwnd);
            break;

        }
        break;
    case WM_NOTIFY:
        notifyMsg = (NMHDR*)lParam;
            if (notifyMsg->hwndFrom == GetDlgItem(hwnd, ID_LISTVIEW)) {
                ListViewNotify(hwnd, lParam);
            }
            else if (notifyMsg->hwndFrom == GetDlgItem(hwnd, ID_PLAYERS_LISTVIEW)) {
                PlayerInfoListViewNotify(hwnd, lParam);
            }
            else if (notifyMsg->hwndFrom == GetDlgItem(hwnd, ID_RULES_LISTVIEW)) {
                RulesListViewNotify(hwnd, lParam);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE
    hinst, int cParts)
{
    RECT rcClient;
    HLOCAL hloc;
    PINT paParts;
    int i, nWidth;

    // Ensure that the common control DLL is loaded.
    InitCommonControls();

    // Create the status bar.
    hwndStatus = CreateWindowEx(
        0,                       // no extended styles
        STATUSCLASSNAME,         // name of status bar class
        (PCTSTR)NULL,           // no text when first created
        SBARS_SIZEGRIP |         // includes a sizing grip
        WS_CHILD | WS_VISIBLE,   // creates a visible child window
        0, 0, 0, 0,              // ignores size and position
        hwndParent,              // handle to parent window
        (HMENU)idStatus,       // child window identifier
        hinst,                   // handle to application instance
        NULL);                   // no window creation data

    return hwndStatus;
}


BOOL StatusSetText(HWND hwnd, int iPart, const TCHAR* szText, BOOL bNoBorders = FALSE, BOOL bPopOut = FALSE)
{
    UINT flags = 0;
    if (bNoBorders)
    {
        flags |= SBT_NOBORDERS;
    }
    if (bPopOut)
    {
        flags |= SBT_POPOUT;
    }

    return (BOOL)SendMessage(hwnd, SB_SETTEXT, (WPARAM)(iPart | flags), (LPARAM)szText);
}



void StatusSetText(const TCHAR* szText) {
    HWND     hwnd_status = GetDlgItem(hMainWindow, ID_STATUSBAR);
    StatusSetText(hwnd_status, 0, szText);
}


void init_prefs_text() {
    HWND     hwndBrowseEdit = GetDlgItem(hOptionsDialog, IDC_BROWSE_EDIT);
    HWND     hwndNameEdit = GetDlgItem(hOptionsDialog, IDC_PLAYER_EDIT);
    // Get the user's gta_sa location
    char exeLocation[MAX_PATH], name[MAX_SAMP_NAME];
    DWORD buffer = sizeof(exeLocation);

    // Open registry key
    HKEY hKey;
    long lError = RegOpenKeyEx(HKEY_CURRENT_USER,
        "Software\\SAMP",
        0,
        KEY_READ,
        &hKey);

    // Get value
    DWORD dwRet = RegQueryValueEx(hKey, "gta_sa_exe", NULL, NULL, (LPBYTE)&exeLocation, &buffer);

    // Make sure we got a good value for the gta_sa path
    if (dwRet == ERROR_SUCCESS)
    {
        SendMessage(hwndBrowseEdit, WM_SETTEXT, 0, (LPARAM)exeLocation);
    }

    dwRet = RegQueryValueEx(hKey, "PlayerName", NULL, NULL, (LPBYTE)&exeLocation, &buffer);

    // Make sure we got a good value for the gta_sa path
    if (dwRet == ERROR_SUCCESS)
    {
        SendMessage(hwndNameEdit, WM_SETTEXT, 0, (LPARAM)exeLocation);
    }

    RegCloseKey(hKey);


}

void on_click_save_prefs() {
    HWND     hwndBrowseEdit = GetDlgItem(hOptionsDialog, IDC_BROWSE_EDIT);
    HWND     hwndNameEdit = GetDlgItem(hOptionsDialog, IDC_PLAYER_EDIT);

    char temp_buff[MAX_PATH];
    // Open registry key
    HKEY hKey;
    long lError = RegOpenKeyEx(HKEY_CURRENT_USER,
        "Software\\SAMP",
        0,
        KEY_WRITE,
        &hKey);


    GetWindowText(hwndBrowseEdit, (LPSTR)&temp_buff, sizeof(temp_buff));
    RegSetKeyValue(hKey, NULL, "gta_sa_exe", REG_SZ, temp_buff, strlen(temp_buff));

    GetWindowText(hwndNameEdit, (LPSTR)&temp_buff, sizeof(temp_buff));
    RegSetKeyValue(hKey, NULL, "PlayerName", REG_SZ, temp_buff, strlen(temp_buff));

    RegCloseKey(hKey);

    EndDialog(hOptionsDialog, TRUE);
}