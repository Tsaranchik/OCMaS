#include <windows.h>
#include <stdio.h>

int main() {
    SetConsoleOutputCP(CP_UTF8); //Устанавливаем кодировку вывода

    UINT deviceCount; //Количество устройств ввода
    PRAWINPUTDEVICELIST pRawInputDeviceList; //Список устройств ввода

    //Получение количества (если первый аргумент фукнции = NULL) устройств ввода, если функция возвращает 0,
    //то функция выполнена успешно, иначе выводится сообщение об ошибке
    if (GetRawInputDeviceList(NULL, &deviceCount, sizeof(RAWINPUTDEVICELIST)) != 0) {
        printf("Ошибка при получении списка устройств\n");
        return 1;
    }

    //Выделение памяти под список устройств ввода, если память не выделена (NULL), то выводится сообщение об ошибке
    if ((pRawInputDeviceList = (PRAWINPUTDEVICELIST)malloc(sizeof(RAWINPUTDEVICELIST) * deviceCount)) == NULL) {
        printf("Ошибка при выделении памяти\n");
        return 1;
    }

    //Запись списка устройств ввода и проверка на ошибки при записи, если функция возвращает -1, то функция сработала
    //неверно и выводится сообщение об ошибке
    if (GetRawInputDeviceList(pRawInputDeviceList, &deviceCount, sizeof(RAWINPUTDEVICELIST)) == -1) {
        printf("Ошибка при записи списка устройств\n");
        free(pRawInputDeviceList);
        return 1;
    }

    //Проходимся по списку устройств ввода
    for (UINT i = 0; i < deviceCount; ++i) {
        //Если устройство является мышью
        if (pRawInputDeviceList[i].dwType == RIM_TYPEMOUSE) {
            void *deviceHandle; //Указатель на устройство или же его дескриптор
            deviceHandle = (void*) pRawInputDeviceList[i].hDevice; //Присваиваем дескриптор устройства

            UINT dataSize; // Размер строки-информации об устройстве

            //Получаем информацию об устройстве и выделяем память под строку-информацию об устройстве
            GetRawInputDeviceInfo(deviceHandle, RIDI_DEVICENAME, NULL, &dataSize);
            char* deviceName = (char*)malloc(dataSize);

            //Если функция GetRawInputDeviceInfo c аргументом deviceName (имя устройства) возвращает отрицательное
            // число, то выводится сообщение об ошибке
            //Если вместо deviceName указать NULL, как выше, то мы получим как раз таки размер
            // строки-информации об устройстве
            if (GetRawInputDeviceInfo(deviceHandle, RIDI_DEVICENAME, deviceName, &dataSize) < 0)
                printf("Ошибка при получении имени устройства\n");

                //Выводим информацию об устройстве, т.е. Vendor ID и Product ID
            else {
                printf("Полный ID мыши: %s\n", deviceName);

                printf("Vendor ID и Product ID мыши: ");
                for (int j = 8; j < 25; ++j)
                    printf("%c", deviceName[j] == '&' ? ' ' : deviceName[j]);

                printf("\n");
            }

            //Освобождаем память, которую мы выделяли
            free(deviceName);
            free(pRawInputDeviceList);
        }
    }

    //Получаем системную информацию о мыши, в данном случае количество кнопок мыши и наличие колеса мыши
    //GetSystemMetrics - функция, которая получает информацию из системного реестра Windows, однако
    //функция не делает это напрямую, она использует API Windows, чтобы получить информацию
    int buttons = GetSystemMetrics(SM_CMOUSEBUTTONS);
    boolean mouseWheel = GetSystemMetrics(SM_MOUSEWHEELPRESENT);

    //Вывод информацию, которую мы получили выше
    printf("Количество кнопок: %d\n", buttons);
    printf("Колесо мыши: %s\n\n", mouseWheel ? "Да" : "Нет");

//Бесконечный цикл, который проверяет состояние кнопок мыши и координаты курсора
    while(1) {
        //GetAsyncKeyState - функция, которая возвращает состояние клавиши, если клавиша нажата, то функция возвращает
        //ненулевое число, иначе 0
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

        //GetCursorPos - функция, которая получает координаты курсора
        POINT cursorPos;
        if (GetCursorPos(&cursorPos))
            printf("Координаты курсора: x = %ld, y = %ld\n", cursorPos.x, cursorPos.y);

        // GetAsyncKeyState(VK_ESCAPE) - функция, которая проверяет нажата ли клавиша ESCAPE, если нажата, то
        // программа завершается
        if (GetAsyncKeyState(VK_ESCAPE)) {
            printf("Выход из программы\n");
            break;
        }

        //Задержка в 200 миллисекунд, чтобы не перегружать процессор
        Sleep(200);
    }

    return 0;
}
