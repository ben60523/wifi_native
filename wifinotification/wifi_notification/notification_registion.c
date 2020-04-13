#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <wlanapi.h>
#include <Windot11.h>
#include <objbase.h>
#include <stdio.h>
#include <stdlib.h>

HANDLE hClient = NULL;
PWLAN_INTERFACE_INFO pIfInfo = NULL;
PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
PDOT11_SSID pDotSSid = NULL;


void notificationCallback(PWLAN_NOTIFICATION_DATA pNotifData, PVOID pContext) {
	if (pNotifData != NULL) {
		switch (pNotifData->NotificationSource)
		{
		case WLAN_NOTIFICATION_SOURCE_ACM:
			switch (pNotifData->NotificationCode)
			{
			case wlan_notification_acm_scan_complete:
				wprintf(L"wlan scan completed.\n");
				break;
			case wlan_notification_acm_scan_fail:
				wprintf(L"wlan scan failed.\n");
				break;
			case wlan_notification_acm_scan_list_refresh:
				wprintf(L"wlan scan list refresh.\n");
				break;
			case wlan_notification_acm_connection_start:
				wprintf(L"wlan start connect.\n");
				break;
			case wlan_notification_acm_connection_complete:
				wprintf(L"wlan connected.\n");
				break;
			case wlan_notification_acm_connection_attempt_fail:
				wprintf(L"wlan connect failed.\n");
				break;
			case wlan_notification_acm_disconnecting:
				wprintf(L"wlan disconnecting.\n");
				break;
			case wlan_notification_acm_disconnected:
				wprintf(L"wlan discconected.\n");
				break;

			default:
				wprintf(L"Unknow code: %d\n", (int)pNotifData->NotificationCode);
				break;
			}
			break;
		default:
			wprintf(L"Unknow code: %d, source: %d.\n", (int)pNotifData->NotificationCode, (int)pNotifData->NotificationSource);
			break;
		}
	}
}

int wmain()
{

	DWORD dwMaxClient = 2;
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;
	DWORD dwRetVal = 0;
	DWORD dwPrevNotifType = 0;


	// variables used for WlanEnumInterfaces


	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		return 1;
	}
	else {
		wprintf(L"handle opened\n");
	}

	dwResult = WlanRegisterNotification(hClient, WLAN_NOTIFICATION_SOURCE_ALL, FALSE, (WLAN_NOTIFICATION_CALLBACK)notificationCallback, NULL, NULL, &dwPrevNotifType);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanRegisterNotification failed with error: %u\n", dwResult);
		return 1;
	}
	else {
		wprintf(L"Notification registered. Start to listen wlan events, enter q to quit.\n");
		while (1) {
			if (getchar() == 'q') {
				break;
			}
		}
	}
	return dwRetVal;
}