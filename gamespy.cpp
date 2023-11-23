#include "main.h"
#include <serverbrowsing/sb_serverbrowsing.h>
#include <qr2/qr2.h>
#include <common/gsAvailable.h>

#define NUM_COLUMNS 5

extern HWND hOptionsDialog;
extern HINSTANCE   g_hInst;

bool g_queryActive = false;
bool g_debugMode = false;

void SBCallback(ServerBrowser sb, SBCallbackReason reason, SBServer server, void* instance);

ServerBrowser g_serverbrowser;  // server browser object initialized with ServerBrowserNew
unsigned char basicFields[] = { HOSTNAME_KEY, GAMEMODE_KEY,  MAPNAME_KEY, NUMPLAYERS_KEY, MAXPLAYERS_KEY, HOSTPORT_KEY, GAMEVARIANT_KEY };
int numFields = sizeof(basicFields) / sizeof(basicFields[0]);

/* ServerBrowserUpdate parameters */
SBBool async = SBTrue;     // we will run the updates asynchronously
SBBool discOnComplete = SBTrue; // disconnect from the master server after completing update 
// (future updates will automatically re-connect)
gsi_char serverFilter[100] = { '\0' };  // filter string for server browser updates

int sort_direction = -1;
int sort_type = 0;

/********
DEFINES
********/
#define GAME_NAME		_T("gmtest")
#define SECRET_KEY		_T("HA6zkS")

/********
DEBUG OUTPUT
********/
#ifdef GSI_COMMON_DEBUG
#if !defined(_MACOSX) && !defined(_IPHONE)
void DebugOut(const char* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	char dbg_out[4096];
	vsprintf_s(dbg_out, fmt, argp);
	va_end(argp);
	OutputDebugStringA(dbg_out);
}

static void DebugCallback(GSIDebugCategory theCat, GSIDebugType theType,
	GSIDebugLevel theLevel, const char* theTokenStr,
	va_list theParamList)
{
	GSI_UNUSED(theLevel);
	DebugOut("[%s][%s] ",
		gGSIDebugCatStrings[theCat],
		gGSIDebugTypeStrings[theType]);

	char dbg_out[4096];
	vsprintf_s(dbg_out, theTokenStr, theParamList);
	OutputDebugStringA(dbg_out);
	
}
#endif
#ifdef GSI_UNICODE
static void AppDebug(const unsigned short* format, ...)
{
	// Construct text, then pass in as ASCII
	unsigned short buf[1024];
	char tmp[2056];
	va_list aList;
	va_start(aList, format);
	_vswprintf(buf, 1024, format, aList);

	UCS2ToAsciiString(buf, tmp);
	gsDebugFormat(GSIDebugCat_App, GSIDebugType_Misc, GSIDebugLevel_Notice,
		"%s", tmp);
}
#else
static void AppDebug(const char* format, ...)
{
	va_list aList;
	va_start(aList, format);
	gsDebugVaList(GSIDebugCat_App, GSIDebugType_Misc, GSIDebugLevel_Notice,
		format, aList);
}
#endif
#else
#define AppDebug _tprintf
#endif

void init_gamespy() {

	/* ServerBrowserNew parameters */
	int version = 0;           // ServerBrowserNew parameter; set to 0 unless otherwise directed by GameSpy
	int maxConcUpdates = 20;	// max number of queries the ServerBrowsing SDK will send out at one time
	SBBool lanBrowse = SBFalse;   // set true for LAN only browsing
	void* userData = NULL;       // optional data that will be passed to the SBCallback function after updates

	// for debug output on these platforms
#ifdef GSI_COMMON_DEBUG
	// Define GSI_COMMON_DEBUG if you want to view the SDK debug output
	// Set the SDK debug log file, or set your own handler using gsSetDebugCallback
	//gsSetDebugFile(stdout); // output to console
	gsSetDebugCallback(DebugCallback);

	// Set debug levels
	gsSetDebugLevel(GSIDebugCat_All, GSIDebugType_All, GSIDebugLevel_Verbose);
#endif

	GSIACResult result;	// used for backend availability check

	// check that the game's backend is available
	GSIStartAvailableCheck(GAME_NAME);
	while ((result = GSIAvailableCheckThink()) == GSIACWaiting)
		msleep(5);
	if (result != GSIACAvailable)
	{
		AppDebug(_T("The backend is not available\n"));
		MessageBox(hMainWindow, "The GameSpy backend is not available.", "Backend not available", MB_ICONERROR);
		PostQuitMessage(0);
		return;
	}

	AppDebug(_T("Creating server browser for %s\n\n"), GAME_NAME);
	// create a new server browser object
	g_serverbrowser = ServerBrowserNew(GAME_NAME, GAME_NAME, SECRET_KEY, version, maxConcUpdates, QVERSION_QR2, lanBrowse, SBCallback, userData);

	sort_type = 1;
	sort_direction = 1;
}

void AddServer(SBServer server) {
	HWND     hwnd_listview = GetDlgItem(hMainWindow, ID_LISTVIEW);

	LV_ITEM lvI;
	ZeroMemory(&lvI, sizeof(LV_ITEM));

	// Initialize LVITEM members that are common to all items.
	lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
	lvI.mask = LVIF_TEXT | LVIF_PARAM;
	lvI.lParam = (LPARAM)server;
	ListView_InsertItem(hwnd_listview, &lvI);
}
void UpdateServer(SBServer server) {
	HWND     hwnd_listview = GetDlgItem(hMainWindow, ID_LISTVIEW);
	LV_ITEM lvI;
	ZeroMemory(&lvI, sizeof(LV_ITEM));
	if (SBServerHasBasicKeys(server)) {
		LVFINDINFOA info;
		ZeroMemory(&info, sizeof(info));
		info.lParam = (LPARAM)server;
		info.flags = LVFI_PARAM;
		int idx = ListView_FindItem(hwnd_listview, -1, &info);
		for (int i = 0; i < NUM_COLUMNS; i++) {
			ListView_SetItemText(hwnd_listview, idx, i, LPSTR_TEXTCALLBACK);
		}		
	}
}

void DeleteServer(SBServer server) {
	HWND     hwnd_listview = GetDlgItem(hMainWindow, ID_LISTVIEW);
	LV_ITEM lvI;
	ZeroMemory(&lvI, sizeof(LV_ITEM));

	LVFINDINFOA info;
	ZeroMemory(&info, sizeof(info));
	info.lParam = (LPARAM)server;
	info.flags = LVFI_PARAM;
	int idx = ListView_FindItem(hwnd_listview, -1, &info);
	ListView_DeleteItem(hwnd_listview, idx);
}

void set_complete_text() {
	char complete_buff[256];
	sprintf_s(complete_buff, sizeof(complete_buff), "Server Browser Update Complete - %d servers", ServerBrowserCount(g_serverbrowser));
	StatusSetText(complete_buff);
}
void set_query_error_text() {
	char complete_buff[256];
	sprintf_s(complete_buff, sizeof(complete_buff), "Query Error: %s", ServerBrowserListQueryError(g_serverbrowser));
	StatusSetText(complete_buff);
}
void SBCallback(ServerBrowser sb, SBCallbackReason reason, SBServer server, void* instance)
{
	switch (reason)
	{
	case sbc_serveradded:  // new SBServer added to the server browser list
		// output the server's IP and port (the rest of the server's basic keys may not yet be available)
		if (SBServerHasBasicKeys(server))
			AddServer(server);
		break;
	case sbc_serverchallengereceived: // received ip verification challenge from server
		// informational, no action required
		break;
	case sbc_serverupdated:  // either basic or full information is now available for this server
		// retrieve and print the basic server fields (specified as a parameter in ServerBrowserUpdate)
		if (SBServerHasBasicKeys(server))
			UpdateServer(server);
		break;
	case sbc_serverupdatefailed:
		AppDebug(_T("Update Failed: %s:%d\n"), SBServerGetPublicAddress(server), SBServerGetPublicQueryPort(server));
		DeleteServer(server);
		break;
	case sbc_updatecomplete: // update is complete; server query engine is now idle (not called upon AuxUpdate completion)
		AppDebug(_T("Server Browser Update Complete\r\n"));
		set_complete_text();
		g_queryActive = false;
		do_sorting();
		//UpdateFinished = gsi_true; // this will let us know to stop calling ServerBrowserThink
		break;
	case sbc_queryerror: // the update returned an error 
		AppDebug(_T("Query Error: %s\n"), ServerBrowserListQueryError(sb));
		set_query_error_text();
		g_queryActive = false;
		//UpdateFinished = gsi_true; // set to true here since we won't get an updatecomplete call
		break;
	default:
		break;
	}

	GSI_UNUSED(instance);
}

void gamespy_think() {
	if (g_serverbrowser == NULL) {
		return;
	}
	ServerBrowserThink(g_serverbrowser);
}
void gamespy_refresh() {
	if (g_serverbrowser == NULL) {
		return;
	}
	if (g_queryActive) {
		MessageBoxA(hMainWindow, "A query is already active. Please wait", "SA:MP Launcher", MB_ICONWARNING);
		return;
	}

	StatusSetText("Querying server list");
	g_queryActive = true;
	HWND     hwnd_listview = GetDlgItem(hMainWindow, ID_LISTVIEW);
	ListView_DeleteAllItems(hwnd_listview);
	ServerBrowserClear(g_serverbrowser);
	//strcpy(serverFilter, "numplayers>0);


	HWND  hwndGroup = GetDlgItem(hMainWindow, ID_FILTER_GROUPBOX);
	
	HWND hwnd_notfull = GetDlgItem(hwndGroup, ID_FILTER_NOT_FULL);
	int  notFullState = SendMessage(hwnd_notfull, BM_GETCHECK, NULL, NULL);

	HWND hwnd_notempty = GetDlgItem(hwndGroup, ID_FILTER_NOT_EMPTY);
	int  notEmptyState = SendMessage(hwnd_notempty, BM_GETCHECK, NULL, NULL);

	HWND hwnd_nopassword = GetDlgItem(hwndGroup, ID_FILTER_NO_PASSWORD);
	int  noPasswordState = SendMessage(hwnd_nopassword, BM_GETCHECK, NULL, NULL);

	int num_filters = 0;
	
	serverFilter[0] = 0;
	if (notFullState) {
		strcat_s(serverFilter, sizeof(serverFilter), "numplayers<maxplayers");
		num_filters++;
	}

	if (notEmptyState) {
		if (num_filters > 0) {
			strcat_s(serverFilter, sizeof(serverFilter), " AND ");
		}
		strcat_s(serverFilter, sizeof(serverFilter), "numplayers>0");
		num_filters++;
	}


	if (noPasswordState) {
		if (num_filters > 0) {
			strcat_s(serverFilter, sizeof(serverFilter), " AND ");
		}
		strcat_s(serverFilter, sizeof(serverFilter), "password==0");
		num_filters++;
	}


	ServerBrowserUpdate(g_serverbrowser, async, discOnComplete, basicFields, numFields, serverFilter);
}

void do_sorting() {
	HWND     hwnd_listview = GetDlgItem(hMainWindow, ID_LISTVIEW);
	ListView_DeleteAllItems(hwnd_listview);

	SBBool ascending = sort_direction != -1 ? SBFalse : SBTrue;
	switch (sort_type) {
	case 0:
		ServerBrowserSort(g_serverbrowser, (SBBool)ascending, "hostname", sbcm_stricase);
		break;
	case 1:
		ServerBrowserSort(g_serverbrowser, (SBBool)ascending, "numplayers", sbcm_int);
		break;
	case 2:
		ServerBrowserSort(g_serverbrowser, (SBBool)ascending, "ping", sbcm_int);
		break;
	case 3:
		ServerBrowserSort(g_serverbrowser, (SBBool)ascending, "gamemode", sbcm_stricase);
		break;
	case 4:
		ServerBrowserSort(g_serverbrowser, (SBBool)ascending, "gamevariant", sbcm_stricase);
		break;
	}

	for (int i = 0; i < ServerBrowserCount(g_serverbrowser); i++)
	{
		// if we got basic info for it, put it back in the list
		SBServer server = ServerBrowserGetServer(g_serverbrowser, i);
		if (SBServerHasBasicKeys(server))
			AddServer(server);
	}
}

void DisplayPlayerListView(SBServer server) {
	HWND     hwndNameEdit = GetDlgItem(hMainWindow, ID_PLAYERS_LISTVIEW);

	ListView_DeleteAllItems(hwndNameEdit);

	if (!SBServerHasFullKeys(server)) {
		return;
	}

	int numplayers = SBServerGetIntValueA(server, "numplayers", 0);
	for (int i = 0; i < numplayers; i++) {
		LV_ITEM lvI;
		ZeroMemory(&lvI, sizeof(LV_ITEM));

		// Initialize LVITEM members that are common to all items.
		lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
		lvI.mask = LVIF_TEXT | LVIF_PARAM;
		lvI.lParam = (LPARAM)server;
		ListView_InsertItem(hwndNameEdit, &lvI);
	}
}

//qr2_registered_key_list[MAX_REGISTERED_KEYS]
void RulesEnumCallback(gsi_char* key, gsi_char* value, void* instance) {
	if (strchr(key, '_') != NULL) {
		return;
	}
	for (int i = 0; i < MAX_REGISTERED_KEYS; i++) {
		if (qr2_registered_key_list[i] == NULL) {
			break;
		}
		if (strcmp(key, qr2_registered_key_list[i]) == 0) {
			return;
		}
	}
	HWND     hwndNameEdit = GetDlgItem(hMainWindow, ID_RULES_LISTVIEW);
	SBServer server = (SBServer)instance;
	LV_ITEM lvI;
	ZeroMemory(&lvI, sizeof(LV_ITEM));

	// Initialize LVITEM members that are common to all items.
	lvI.pszText = (LPSTR)key; // Sends an LVN_GETDISPINFO message.
	lvI.mask = LVIF_TEXT | LVIF_PARAM;
	lvI.lParam = (LPARAM)server;
	ListView_InsertItem(hwndNameEdit, &lvI);

	ListView_SetItemText(hwndNameEdit, lvI.iItem, 1, (LPSTR)value);
}
void DisplayRulesListView(SBServer server) {
	HWND     hwndNameEdit = GetDlgItem(hMainWindow, ID_RULES_LISTVIEW);

	ListView_DeleteAllItems(hwndNameEdit);

	//if (!SBServerHasFullKeys(server)) {
	//	return;
	//}

	SBServerEnumKeys(server, RulesEnumCallback, server);
}
void PlayerInfoListViewNotify(HWND hwnd, LPARAM lParam) {
	SBServer server;
	NMLVDISPINFO* plvdi;
	switch (((LPNMHDR)lParam)->code)
	{
	case LVN_GETDISPINFO:
		plvdi = (NMLVDISPINFO*)lParam;
		server = (SBServer)plvdi->item.lParam;
		if (plvdi->item.mask & LVIF_TEXT)
		{
			switch (plvdi->item.iSubItem)
			{
			case 0:
				plvdi->item.pszText = (LPSTR)SBServerGetPlayerStringValueA(server, plvdi->item.iItem, "name", "NULL");
				break;
			case 1:
				plvdi->item.pszText = (LPSTR)SBServerGetPlayerStringValueA(server, plvdi->item.iItem, "score", "0");
				break;
			}
		}
	}
}

void RulesListViewNotify(HWND hwnd, LPARAM lParam) {
	//SBServer server;
	//NMLVDISPINFO* plvdi;
	//switch (((LPNMHDR)lParam)->code)
	//{
	//case LVN_GETDISPINFO:
	//	plvdi = (NMLVDISPINFO*)lParam;
	//	server = (SBServer)plvdi->item.lParam;
	//	if (plvdi->item.mask & LVIF_TEXT)
	//	{
	//		switch (plvdi->item.iSubItem)
	//		{
	//		case 0:
	//			plvdi->item.pszText = (LPSTR)SBServerGetPlayerStringValueA(server, plvdi->item.iItem, "name", "NULL");
	//			break;
	//		case 1:
	//			plvdi->item.pszText = (LPSTR)SBServerGetPlayerStringValueA(server, plvdi->item.iItem, "score", "0");
	//			break;
	//		}
	//	}
	//}
}
void ListViewNotify(HWND hwnd, LPARAM lParam)
{
	HWND     hwnd_listview = GetDlgItem(hwnd, ID_LISTVIEW);
	const char* defaultString = "NULL";
	NMLVDISPINFO* plvdi;
	LPNMITEMACTIVATE lpnmitem;
	LVITEM iItem;
	ZeroMemory(&iItem, sizeof(iItem));
	int x;
	SBServer server;

	char ipinfo_str[256];
	HWND ipinfo_label;


	LPNMLISTVIEW pnmv;
	
	//DebugOut("list view code: %d\n", (((LPNMHDR)lParam)->code));
	switch (((LPNMHDR)lParam)->code)
	{
	case LVN_ITEMCHANGED:
		pnmv = (LPNMLISTVIEW)lParam;
		iItem.iItem = pnmv->iItem;
		iItem.mask |= LVIF_PARAM;
		x = ListView_GetItem(hwnd_listview, &iItem);
		server = (SBServer)iItem.lParam;

		if (server) {
			ipinfo_label = GetDlgItem(GetDlgItem(hMainWindow, ID_SERVER_INFO_GROUPBOX), ID_SERVER_IP_TEXT);
			sprintf_s(ipinfo_str, sizeof(ipinfo_str), "Address: %s : %d\nPlayers : %d / %d\nPing : %d\nPassword : %s\nMode : %s\nLanguage : %s",
				SBServerGetPublicAddress(server), SBServerGetIntValue(server, "hostport", SAMP_DEFAULT_PORT),
				SBServerGetIntValue(server, "numplayers", 0), SBServerGetIntValue(server, "maxplayers", 0),
				SBServerGetPing(server), SBServerGetBoolValue(server, "password", SBFalse) ? "Yes" : "No",
				SBServerGetStringValue(server, "gamemode", "N/A"), SBServerGetStringValue(server, "gamevariant", "N/A"));
				SendMessage(ipinfo_label, WM_SETTEXT, 0, (LPARAM)ipinfo_str);
				SendMessage(GetDlgItem(hMainWindow, ID_SERVER_INFO_GROUPBOX), WM_SETTEXT, 0, (LPARAM)SBServerGetStringValue(server, "hostname", "N/A"));
				DisplayPlayerListView(server);
				DisplayRulesListView(server);
			
		}
		break;
	case LVN_COLUMNCLICK:
		pnmv = (LPNMLISTVIEW)lParam;
		if (sort_direction == 1) {
			sort_direction = -1;
		}
		else {
			sort_direction = 1;
		}
		sort_type = pnmv->iSubItem;
		do_sorting();
		break;

		case NM_DBLCLK:
		case NM_RETURN:
			do_launch_samp();
			if (hOptionsDialog != NULL) {
				EndDialog(hOptionsDialog, TRUE);
			}
			break;
		case LVN_GETDISPINFO:
			plvdi = (NMLVDISPINFO*)lParam;
			server = (SBServer)plvdi->item.lParam;
			if (plvdi->item.mask & LVIF_TEXT)
			{
				switch (plvdi->item.iSubItem)
				{
				case 0:
					plvdi->item.pszText = (LPSTR)SBServerGetStringValue(server, _T("hostname"), defaultString);
					break;
				case 1:
					sprintf_s(plvdi->item.pszText, 259, "%d/%d", SBServerGetIntValue(server, "numplayers", 0), SBServerGetIntValue(server, "maxplayers", 0));
					break;
				case 2:
					if (SBServerDirectConnect(server)) {
						sprintf_s(plvdi->item.pszText, 259, "%d", SBServerGetPing(server));
					}
					else {
						sprintf_s(plvdi->item.pszText, 259, "N/A");
					}
					
					break;
				case 3:
					plvdi->item.pszText = (LPSTR)SBServerGetStringValue(server, _T("gamemode"), defaultString);
					break;
				case 4:
					plvdi->item.pszText = (LPSTR)SBServerGetStringValue(server, _T("gamevariant"), defaultString);
					break;
				}
			}
	}
}


void do_joinbyip() {
	char temp_buff[256];
	HWND     hwnd_status = GetDlgItem(hOptionsDialog, IDC_IPINPUT);
	GetWindowText(hwnd_status, (LPSTR)&temp_buff, sizeof(temp_buff));

	short port = 7777;
	char* p = strchr(temp_buff, ':');
	if (p != NULL) {
		*p = NULL;
		p++;
		port = atoi(p);
	}
	launch_samp(temp_buff, port, NULL);
}

void init_connect_dialog() {
	char ipinfo_str[256];
	HWND ipinfo_label;

	CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_SERVER_PROPERTIES), hMainWindow, ServerPropertiesDialogProc);

	HWND    hwndHostnameLabel = GetDlgItem(hOptionsDialog, IDC_SERVER_HOSTNAME);

	HWND     hwnd_listview = GetDlgItem(hMainWindow, ID_LISTVIEW);
	int selectedId = SendMessage(hwnd_listview, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

	LVITEM iItem;
	ZeroMemory(&iItem, sizeof(iItem));
	SBServer server;

	if (selectedId != -1) {
		iItem.iItem = selectedId;
		iItem.mask |= LVIF_PARAM;
		int r = ListView_GetItem(hwnd_listview, &iItem);
		server = (SBServer)iItem.lParam;

		sprintf_s(ipinfo_str, sizeof(ipinfo_str), "Name: %s\nAddress: %s:%d\nPlayers: %d / %d\nPing: %d\nPassword: %s\nMode: %s\nLanguage: %s",
			SBServerGetStringValue(server, "hostname", "N/A"), SBServerGetPublicAddress(server), SBServerGetIntValue(server, "hostport", SAMP_DEFAULT_PORT),
			SBServerGetIntValue(server, "numplayers", 0), SBServerGetIntValue(server, "maxplayers", 0),
			SBServerGetPing(server), SBServerGetBoolValue(server, "password", SBFalse) ? "Yes" : "No",
			SBServerGetStringValue(server, "gamemode", "N/A"), SBServerGetStringValue(server, "gamevariant", "N/A"));

		SendMessage(hwndHostnameLabel, WM_SETTEXT, 0, (LPARAM)ipinfo_str);
	}
}

void do_connect_selected_server() {
	HWND     hwnd_listview = GetDlgItem(hMainWindow, ID_LISTVIEW);
	int selectedId = SendMessage(hwnd_listview, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

	HWND hwndPw = GetDlgItem(hOptionsDialog, IDC_SERVER_PASSWORD);

	char password_buff[MAX_PATH];
	GetWindowText(hwndPw, (LPSTR)&password_buff, sizeof(password_buff));

	LVITEM iItem;
	ZeroMemory(&iItem, sizeof(iItem));
	SBServer server;

	if (selectedId != -1) {
		iItem.iItem = selectedId;
		iItem.mask |= LVIF_PARAM;
		int r = ListView_GetItem(hwnd_listview, &iItem);
		server = (SBServer)iItem.lParam;

		if (strlen(password_buff) > 0) {
			launch_samp(SBServerGetPublicAddress(server), SBServerGetIntValue(server, "hostport", SAMP_DEFAULT_PORT), password_buff);
		}
		else {
			launch_samp(SBServerGetPublicAddress(server), SBServerGetIntValue(server, "hostport", SAMP_DEFAULT_PORT), NULL);
		}
		
	}
}