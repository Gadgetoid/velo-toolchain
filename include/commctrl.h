#ifndef VELO_COMMCTRL_H
#define VELO_COMMCTRL_H

#include <windows.h>

#define HINST_COMMCTRL ((HINSTANCE)-1)
#define IDB_STD_SMALL_COLOR 0
#define STD_CUT 0
#define STD_COPY 1
#define STD_PASTE 2
#define STD_UNDO 3
#define STD_REDOW 4
#define STD_DELETE 5
#define STD_FILENEW 6
#define STD_FILEOPEN 7
#define STD_FILESAVE 8
#define STD_PRINTPRE 9
#define STD_PROPERTIES 10
#define STD_HELP 11
#define STD_FIND 12
#define STD_REPLACE 13
#define STD_PRINT 14

#define CMDBAR_HELP 0x0001
#define CMDBAR_OK 0x0002

#define TBSTATE_CHECKED 0x01
#define TBSTATE_ENABLED 0x04
#define TBSTYLE_BUTTON 0x00
#define TBSTYLE_SEP 0x01
#define TBSTYLE_CHECK 0x02

#define TB_ENABLEBUTTON (WM_USER + 1)
#define TB_CHECKBUTTON (WM_USER + 2)
#define TB_ADDBUTTONS (WM_USER + 20)
#define SB_SETTEXT (WM_USER + 11)
#define SB_SETPARTS (WM_USER + 4)
#define SBT_NOBORDERS 0x0100

#ifndef RC_INVOKED
typedef struct {
    int iBitmap;
    int idCommand;
    BYTE fsState;
    BYTE fsStyle;
    BYTE bReserved[2];
    DWORD dwData;
    int iString;
} TBBUTTON;

void WINAPI InitCommonControls(void);
HWND WINAPI CommandBar_Create(HINSTANCE instance, HWND parent, int id);
BOOL WINAPI CommandBar_InsertMenubar(HWND command_bar, HINSTANCE instance, WORD menu, WORD button);
HMENU WINAPI CommandBar_GetMenu(HWND command_bar, WORD button);
int WINAPI CommandBar_AddBitmap(HWND command_bar, HINSTANCE instance, int bitmap, int count, int width, int height);
BOOL WINAPI CommandBar_AddAdornments(HWND command_bar, DWORD flags, DWORD reserved);
int WINAPI CommandBar_Height(HWND command_bar);
BOOL WINAPI CommandBar_Show(HWND command_bar, BOOL show);
HWND WINAPI CommandBar_InsertComboBox(HWND command_bar, HINSTANCE instance, int width, UINT style, WORD id, WORD button);
HWND WINAPI CreateStatusWindowW(LONG style, LPCWSTR text, HWND parent, UINT id);

#define CommandBar_AddButtons(command_bar, count, buttons) ((BOOL)SendMessageW((command_bar), TB_ADDBUTTONS, (WPARAM)(count), (LPARAM)(buttons)))
#define CommandBar_Destroy(command_bar) DestroyWindow(command_bar)
#endif

#endif
