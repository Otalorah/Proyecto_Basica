#ifndef UNICODE
#define UNICODE
#endif 

#include <string>
#include <windows.h>
// #include "functions.h"

using namespace std;

// Se definen las constantes
# define NEW_ATTEMPT 1
# define NUM_DIGITS 5
# define MAX_ATTEMPTS 10

// Se declaran las funciones
void createRow(HWND hwnd);
void displayDialog(HWND hwnd);
void initializeInterface(HWND hwnd);
void registerDialogClass(HINSTANCE hInstance);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DialogProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Se declaran variables globales
HWND attempText;
HWND contentRows;
int numAttemps = 1;
HBRUSH hBrushStatic;
char numInput[NUM_DIGITS + 1];
COLORREF colorRows = RGB(225, 225, 225);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Se registra el nombre de la clase
    const wchar_t CLASS_NAME[] = L"JUEGO PICAS Y FIJAS";

    WNDCLASS wc = { };

    // Se le asignan las propiedades al objeto wc
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);
    registerDialogClass(hInstance);

    // Se crea la ventana principal
    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"PICAS Y FIJAS", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 760, 600, // La posición y tamaño de la ventana
        NULL, NULL, hInstance, NULL
    );

    // En caso de no poder crear la ventana
    if (hwnd == NULL) return 0;
    ShowWindow(hwnd, nCmdShow);

    // Ciclo de mensajes de la ventana
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Función para manejar los mensajes de la ventana principal
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Se pinta la pantalla de la ventana
        HBRUSH brush = CreateSolidBrush(RGB(225, 160, 0));
        FillRect(hdc, &ps.rcPaint, brush);
        DeleteObject(brush);

        EndPaint(hwnd, &ps);

        return 0;
    }
    case WM_CREATE:
    {
        // Se muestra el modal
        displayDialog(hwnd);
        // Se inicializa la interfaz
        initializeInterface(hwnd);
        return 0;
    }
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        SetBkColor(hdcStatic, colorRows);
        return (INT_PTR)hBrushStatic;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
            case NEW_ATTEMPT:
            {
                if (numAttemps <= MAX_ATTEMPTS) {
                    // Se crea la fila
                    createRow(hwnd);
                    numAttemps++;
                }
            }
        }
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Función para crear las ventanas iniciales de la interfaz
void initializeInterface(HWND hwnd)
{
    // Se crean los títulos de las columnas
    CreateWindowEx(WS_EX_LTRREADING, L"Static", L"INTENTO #", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 0, 0, 190, 43, hwnd, NULL, NULL, NULL);
    CreateWindowEx(WS_EX_LTRREADING, L"Static", L"NÚMERO", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 190, 0, 190, 43, hwnd, NULL, NULL, NULL);
    CreateWindowEx(WS_EX_LTRREADING, L"Static", L"PICAS", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 380, 0, 190, 43, hwnd, NULL, NULL, NULL);
    CreateWindowEx(WS_EX_LTRREADING, L"Static", L"FIJAS", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 570, 0, 190, 43, hwnd, NULL, NULL, NULL);

    // Se crea el cuadro para ingresar el número
    attempText = CreateWindowEx(WS_EX_LTRREADING, L"Edit", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 348, 507, 68, 15, hwnd, NULL, NULL, NULL);

    // Establecer el límite máximo de caracteres
    SendMessage(attempText, EM_SETLIMITTEXT, NUM_DIGITS, 0);

    // Se crea el botón
    CreateWindowEx(WS_EX_LTRREADING, L"Button", L"Adivinar", WS_CHILD | WS_VISIBLE | WS_BORDER, 348, 525, 68, 30, hwnd, (HMENU)NEW_ATTEMPT, NULL, NULL);
}

// Función para mostrar el resultado de cada intento
void createRow(HWND hwnd)
{
    // Se calcula la altura de la fila
    int height = 43 * numAttemps;

    // Se obtiene el número ingresado
    GetWindowTextA(attempText, numInput, NUM_DIGITS + 1);

    // Convertir datos a wstring
    wstring attemptStr = to_wstring(numAttemps);
    wstring textNum = wstring(numInput, numInput + NUM_DIGITS);

    // Crear la ventana con el número de intento
    CreateWindowEx(WS_EX_LTRREADING, L"Static", attemptStr.c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 0, height, 190, 43, hwnd, NULL, NULL, NULL);

    // Crear la ventana con el número ingresado
    CreateWindowEx(WS_EX_LTRREADING, L"Static", textNum.c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 190, height, 190, 43, hwnd, NULL, NULL, NULL);

    // Crear la ventana con el número de picas
    CreateWindowEx(WS_EX_LTRREADING, L"Static", L"No se sabe", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 380, height, 190, 43, hwnd, NULL, NULL, NULL);

    // Crear la ventana con el número de fijas
    CreateWindowEx(WS_EX_LTRREADING, L"Static", L"Tampoco se sabe", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 570, height, 190, 43, hwnd, NULL, NULL, NULL);

    // Crear el pincel para el color de fondo
    hBrushStatic = CreateSolidBrush(colorRows);

    // Eliminar el texto ingresado por el usuario
    SetWindowText(attempText, L"");
}

void registerDialogClass(HINSTANCE hInstance)
{
    WNDCLASS dialog = { };

    // Se le asignan las propiedades al objeto wc
    dialog.hInstance = hInstance;
    dialog.lpfnWndProc = DialogProcedure;
    dialog.lpszClassName = L"DialogClass";

    RegisterClass(&dialog);
}

LRESULT CALLBACK DialogProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void displayDialog(HWND hwnd)
{
    CreateWindowW(L"DialogClass", L"Bienvenido", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 400, 400, 200, 200, hwnd, NULL, NULL, NULL);
}
