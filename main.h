#pragma once

#include <Windows.h>
#include <CommCtrl.h>

#include <windowsx.h>
#include <tchar.h>
#include <stdio.h>

#include "resource.h"

enum {
	ID_MAIN_WINDOW = 1,
	ID_LISTVIEW,
	ID_STATUSBAR,
	ID_REFRESH,
	ID_SERVER_IP_TEXT,
	ID_FILTER_GROUPBOX,
	ID_FILTER_NOT_FULL,
	ID_FILTER_NOT_EMPTY,
	ID_FILTER_NO_PASSWORD,
	ID_SERVER_INFO_GROUPBOX,
	ID_PLAYERS_LISTVIEW,
	ID_RULES_LISTVIEW,
	ID_QUERY_FAVOURITES,
	ID_QUERY_OPENSPY,
	ID_QUERY_OPENSPY_HOSTED
};

extern "C" {
	extern HWND hMainWindow;
}

#define SAMP_DEFAULT_PORT 7777
#define MAX_SAMP_NAME 24

void init_gamespy();
void gamespy_think();
void gamespy_refresh();
BOOL InsertListViewItems(HWND hWndListView, int cItems);
void ListViewNotify(HWND hwnd, LPARAM lParam);
void PlayerInfoListViewNotify(HWND hwnd, LPARAM lParam);
void RulesListViewNotify(HWND hwnd, LPARAM lParam);
void StatusSetText(const TCHAR* szText);
void ResizeStatusText(UINT width);

void ResizeServerDetailsListViews(HWND parent);
BOOL InitPlayerInfoListView(HWND listView);
BOOL InitRulesListView(HWND listView);

void do_launch_samp();
void launch_samp(const char* ip, SHORT port, const char* password);
void DebugOut(const char* fmt, ...);

void init_prefs_text();
void on_click_save_prefs();

void init_connect_dialog();

void do_connect_selected_server();
void do_sorting();
void do_joinbyip();

INT_PTR CALLBACK ServerPropertiesDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);