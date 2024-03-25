#include <windows.h>
#include <stdio.h>

 void getMouse() {
    UINT deviceCount;
    PRAWINPUTDEVICELIST pRawInputDeviceList;

    if (GetRawInputDeviceList(NULL, &deviceCount, sizeof(RAWINPUTDEVICELIST)) != 0) {
        printf("Ошибка при получении списка устройств\n");
        return;
    }

    if ((pRawInputDeviceList = (PRAWINPUTDEVICELIST)malloc(sizeof(RAWINPUTDEVICELIST) * deviceCount)) == NULL) {
        printf("Ошибка при выделении памяти\n");
        return;
    }

    if (GetRawInputDeviceList(pRawInputDeviceList, &deviceCount, sizeof(RAWINPUTDEVICELIST)) == -1) {
        printf("Ошибка при записи списка устройств\n");
        free(pRawInputDeviceList);
        return;
    }

    for (UINT i = 0; i < deviceCount; ++i) {
        if (pRawInputDeviceList[i].dwType == RIM_TYPEMOUSE) {
            void *deviceHandle;
            deviceHandle = (void*) pRawInputDeviceList[i].hDevice;

            UINT dataSize;
            GetRawInputDeviceInfo(deviceHandle, RIDI_DEVICENAME, NULL, &dataSize);
            char* deviceName = (char*)malloc(dataSize);

            if (GetRawInputDeviceInfo(deviceHandle, RIDI_DEVICENAME, deviceName, &dataSize) < 0)
                printf("Ошибка при получении имени устройства\n");

            else {
                printf("Vendor ID и Product ID мыши: ");
                for (int j = 8; j < 25; ++j)
                    printf("%c", deviceName[j] == '&' ? ' ' : deviceName[j]);

                printf("\n");
            }

            free(deviceName);
            free(pRawInputDeviceList);
            return;
        }
    }

}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    getMouse();

    int buttons = GetSystemMetrics(SM_CMOUSEBUTTONS);
    boolean mouseWheel = GetSystemMetrics(SM_MOUSEWHEELPRESENT);

    printf("Количество кнопок: %d\n", buttons);
    printf("Колесо мыши: %s\n\n", mouseWheel ? "Да" : "Нет");

    while(1) {
        SHORT leftButtonState = GetAsyncKeyState(VK_LBUTTON);
        if (leftButtonState != 0)
            printf("Левая кнопка мыши нажата!\n");

        SHORT rightButtonState = GetAsyncKeyState(VK_RBUTTON);
        if (rightButtonState != 0)
            printf("Правая кнопка мыши нажата!\n");

        SHORT middleButtonState = GetAsyncKeyState(VK_MBUTTON);
        if (middleButtonState != 0)
            printf("Средняя кнопка мыши нажата!\n");

        SHORT xButton1State = GetAsyncKeyState(VK_XBUTTON1);
        if (xButton1State != 0)
            printf("Кнопка X1 мыши нажата!\n");

        SHORT xButton2State = GetAsyncKeyState(VK_XBUTTON2);
        if (xButton2State != 0)
            printf("Кнопка X2 мыши нажата!\n");

        POINT cursorPos;
        if (GetCursorPos(&cursorPos))
            printf("Координаты курсора: x = %ld, y = %ld\n", cursorPos.x, cursorPos.y);

        if (GetAsyncKeyState(VK_ESCAPE)) {
            printf("Выход из программы\n");
            break;
        }

        Sleep(100);
    }

    return 0;
}
