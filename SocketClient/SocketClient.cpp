#define WIN32_LEAN_AND_MEAN

#include <Windows.h>   // Библиотека Windows для использования функций Windows API
#include <iostream>    // Библиотека ввода-вывода стандартного потока
#include <WinSock2.h>  // Библиотека для работы с сокетами в Windows
#include <WS2tcpip.h>  // Библиотека для работы с сетевыми функциями Windows

using namespace std;   // Использование пространства имен стандартной библиотеки

int main() {
    SetConsoleCP(1251);  // Установка кодовой страницы консоли для ввода
    setlocale(LC_ALL, "Russian");  // Установка локали для вывода русского текста в консоль
    WSADATA wsaData;  // Структура для хранения информации о использованной версии Winsock
    ADDRINFO hints;   // Структура для указания параметров сокета
    ADDRINFO* addrResult;  // Структура для хранения результатов функции getaddrinfo
    SOCKET ConnectSocket = INVALID_SOCKET;  // Дескриптор сокета
    char recvBuffer[512];  // Буфер для приема данных
    const char* sendBuffer1 = "Хочу закрыть сессию";  // Сообщение от клиента 1
    const char* sendBuffer2 = "Хочу каникулы :((";  // Сообщение от клиента 2

    // Инициализация Winsock
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cout << "Ошибка WSAStartup: " << result << endl;
        return 1;
    }

    // Задание параметров сокета
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // Потоковый сокет
    hints.ai_protocol = IPPROTO_TCP;  // Протокол TCP

    // Получение информации о сервере
    result = getaddrinfo("localhost", "666", &hints, &addrResult);
    if (result != 0) {
        cout << "Ошибка getaddrinfo: " << result << endl;
        WSACleanup();
        return 1;
    }

    // Создание сокета
    ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Ошибка создания сокета" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    // Подключение к серверу
    result = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
    if (result == SOCKET_ERROR) {
        cout << "Не удалось подключиться к серверу" << endl;
        closesocket(ConnectSocket);
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    // Отправка первого сообщения от клиента
    result = send(ConnectSocket, sendBuffer1, (int)strlen(sendBuffer1), 0);
    if (result == SOCKET_ERROR) {
        cout << "Ошибка отправки: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    cout << "Отправлено: " << result << " байт" << endl;

    // Отправка второго сообщения от клиента
    result = send(ConnectSocket, sendBuffer2, (int)strlen(sendBuffer2), 0);
    if (result == SOCKET_ERROR) {
        cout << "Ошибка отправки: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    cout << "Отправлено: " << result << " байт" << endl;

    // Получение сообщения от сервера
    result = recv(ConnectSocket, recvBuffer, sizeof(recvBuffer), 0);
    if (result > 0) {
        recvBuffer[result] = '\0';
        cout << "Сообщение от сервера: " << recvBuffer << endl;
    }

    // Закрытие сокета и освобождение ресурсов
    closesocket(ConnectSocket);
    freeaddrinfo(addrResult);
    WSACleanup();
    return 0;
}
