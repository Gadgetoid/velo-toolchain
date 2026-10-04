#ifndef VELO_EXTRAS_NOTIFY_H
#define VELO_EXTRAS_NOTIFY_H

#ifndef RC_INVOKED

typedef CE_USER_NOTIFICATION PEG_USER_NOTIFICATION;
typedef PCE_USER_NOTIFICATION PPEG_USER_NOTIFICATION;

#if VELO_CE >= 2
#define PegClearUserNotification CeClearUserNotification
#define PegGetUserNotificationPreferences CeGetUserNotificationPreferences
#define PegHandleAppNotifications CeHandleAppNotifications
#define PegRunAppAtEvent CeRunAppAtEvent
#define PegRunAppAtTime CeRunAppAtTime
#define PegSetUserNotification CeSetUserNotification
#else
BOOL PegClearUserNotification(HANDLE hNotification);
BOOL PegGetUserNotificationPreferences(HWND hWndParent, PPEG_USER_NOTIFICATION lpNotification);
BOOL PegHandleAppNotifications(WCHAR *pwszAppName);
BOOL PegRunAppAtEvent(WCHAR *pwszAppName, LONG lWhichEvent);
BOOL PegRunAppAtTime(WCHAR *pwszAppName, SYSTEMTIME *lpTime);
HANDLE PegSetUserNotification(HANDLE hNotification, WCHAR *pwszAppName, SYSTEMTIME *lpTime, PPEG_USER_NOTIFICATION lpUserNotification);
#define CeClearUserNotification PegClearUserNotification
#define CeGetUserNotificationPreferences PegGetUserNotificationPreferences
#define CeHandleAppNotifications PegHandleAppNotifications
#define CeRunAppAtEvent PegRunAppAtEvent
#define CeRunAppAtTime PegRunAppAtTime
#define CeSetUserNotification PegSetUserNotification
#endif

#endif

#endif
