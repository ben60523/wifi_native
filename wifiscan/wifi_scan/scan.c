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

unsigned int ifaceNum = 0, scanCounter = 0;

void notificationCallback(PWLAN_NOTIFICATION_DATA pNotifData, PVOID pContext) {
	if (pNotifData != NULL) {
		switch (pNotifData->NotificationSource)
		{
		case WLAN_NOTIFICATION_SOURCE_ACM:
			if (pNotifData->NotificationCode == wlan_notification_acm_scan_complete) {
				PWLAN_BSS_LIST WlanBssList;
				if (WlanGetNetworkBssList(hClient, &pNotifData->InterfaceGuid, pDotSSid, dot11_BSS_type_independent, FALSE, NULL, &WlanBssList) == ERROR_SUCCESS) {
					WCHAR GuidString[40] = { 0 };
					StringFromGUID2(&pNotifData->InterfaceGuid, (LPOLESTR)&GuidString, 39);
					wprintf(L"===========%ws=============\n", GuidString);
					for (int c = 0; c < WlanBssList->dwNumberOfItems; c++) {
						wprintf(L"SSID: %hs\n", WlanBssList->wlanBssEntries[c].dot11Ssid.ucSSID);
						wprintf(L"MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
							WlanBssList->wlanBssEntries[c].dot11Bssid[0],
							WlanBssList->wlanBssEntries[c].dot11Bssid[1],
							WlanBssList->wlanBssEntries[c].dot11Bssid[2],
							WlanBssList->wlanBssEntries[c].dot11Bssid[3],
							WlanBssList->wlanBssEntries[c].dot11Bssid[4],
							WlanBssList->wlanBssEntries[c].dot11Bssid[5]);
						wprintf(L"RSSI: %ld\n", WlanBssList->wlanBssEntries[c].lRssi);
						wprintf(L"---------------------------\n");
					}
					scanCounter++;
				}

			}
			break;
		default:
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
	GUID interfaceGuid;
	DWORD dwPrevNotifType = 0;
	PWLAN_BSS_LIST WlanBssList;


	// variables used for WlanEnumInterfaces


	PWLAN_CONNECTION_ATTRIBUTES pConnectInfo = NULL;
	WLAN_OPCODE_VALUE_TYPE opCode = wlan_opcode_value_type_invalid;

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		return 1;
	}

	dwResult = WlanRegisterNotification(hClient, WLAN_NOTIFICATION_SOURCE_ALL, FALSE, (WLAN_NOTIFICATION_CALLBACK)notificationCallback, NULL, NULL, &dwPrevNotifType);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanRegisterNotification failed with error: %u\n", dwResult);
		return 1;
	}

	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
		return 1;
	}
	else {
		wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems);
		wprintf(L"Current Index: %lu\n", pIfList->dwIndex);
		for (ifaceNum = 0; ifaceNum < (int)pIfList->dwNumberOfItems; ifaceNum++) {
			pIfInfo = (WLAN_INTERFACE_INFO*)&pIfList->InterfaceInfo[ifaceNum];
			PWLAN_RAW_DATA WlanRawData = NULL;
			wprintf(L"%ls Scanning...\n", pIfInfo->strInterfaceDescription);
			dwResult = WlanScan(hClient, &pIfInfo->InterfaceGuid, pDotSSid, WlanRawData, NULL);
			if (dwResult != ERROR_SUCCESS) {
				wprintf(L"WlanScan failed with error: %u\n", dwResult);
				return 1;
			}
		}
		Sleep(4000);
	}
	if (pConnectInfo != NULL) {
		WlanFreeMemory(pConnectInfo);
		pConnectInfo = NULL;
	}

	if (pIfList != NULL) {
		WlanFreeMemory(pIfList);
		pIfList = NULL;
	}
	if (hClient != NULL) {
		//WlanCloseHandle(hClient, NULL);
	}
	if (scanCounter == ifaceNum) {
		wprintf(L"WlanScan_ok\n");
	}
	else
	{
		wprintf(L"scan failed\n");
	}
	return dwRetVal;
}