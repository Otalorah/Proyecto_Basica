#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <string>

using namespace std;

# define CREATE_ATTEMPT 1
# define MAX_CHAR_LIMIT 5 // Define el máximo de caracteres permitidos

// Se declaran las funciones a utilizar
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Se declaran variables globales
HWND attempText;
LPSTR textsaved1;
int numAttemps = 1;
HBRUSH hBrushStatic;
COLORREF colorRows = RGB(225, 225, 225);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
   // Se registra el nombre de la clase
   const wchar_t CLASS_NAME[]  = L"JUEGO PICAS Y FIJAS";
   
   WNDCLASS wc = { };

   // Se le asignan las propiedades al objeto wc
   wc.lpfnWndProc   = WindowProc;
   wc.hInstance     = hInstance;
   wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

   // Se crea la ventana principal
   HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"PICAS Y FIJAS", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 760, 500, // La posición y tamaño de la ventana
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
            // Se crean los títulos de las columnas
            CreateWindowEx(WS_EX_LTRREADING, L"Static", L"INTENTO #", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 0, 0, 190, 43, hwnd, NULL, NULL, NULL);
            CreateWindowEx(WS_EX_LTRREADING, L"Static", L"NÚMERO", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 190, 0, 190, 43, hwnd, NULL, NULL, NULL);
            CreateWindowEx(WS_EX_LTRREADING, L"Static", L"PICAS", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 380, 0, 190, 43, hwnd, NULL, NULL, NULL);
            CreateWindowEx(WS_EX_LTRREADING, L"Static", L"FIJAS", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 570, 0, 190, 43, hwnd, NULL, NULL, NULL);

            // Se crea el cuadro para ingresar el número
            attempText = CreateWindowEx(WS_EX_LTRREADING, L"Edit", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 312, 420, 68, 29, hwnd , NULL, NULL, NULL);

            // Establecer el límite máximo de caracteres
            SendMessage(attempText, EM_SETLIMITTEXT, MAX_CHAR_LIMIT, 0);

            // Se crea el botón
            CreateWindowEx(WS_EX_LTRREADING, L"Button", L"Adivinar", WS_CHILD | WS_VISIBLE | WS_BORDER, 382, 415, 68, 29, hwnd, (HMENU) CREATE_ATTEMPT, NULL, NULL);
            
            return 0;
         }
      case WM_CTLCOLORSTATIC:
         {
            HDC hdcStatic = (HDC) wParam;
            SetBkColor(hdcStatic, colorRows);
            return (INT_PTR) hBrushStatic;
         }
      case WM_COMMAND:
      {

         switch (LOWORD(wParam)) 
         {
            case CREATE_ATTEMPT:
            {
               // Se obtiene el número ingresado
               int len = GetWindowTextLength(attempText) + 1;
               textsaved1 = new char[len];
               GetWindowTextA(attempText, textsaved1, len);

               // Convertir datos a wstring
               wstring attemptStr = to_wstring(numAttemps);
               wstring textNum = wstring(textsaved1, textsaved1 + len - 1);

               // Crear la ventana con el número de intento
               CreateWindowEx(WS_EX_LTRREADING, L"Static", attemptStr.c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 0, 43 * numAttemps, 190, 43, hwnd, NULL, NULL, NULL);

               // Crear la ventana con el número ingresado
               CreateWindowEx(WS_EX_LTRREADING, L"Static", textNum.c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 190, 43 * numAttemps, 190, 43, hwnd, NULL, NULL, NULL);

               // Crear la ventana con el número de picas
               CreateWindowEx(WS_EX_LTRREADING, L"Static", L"No se sabe", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 380, 43 * numAttemps, 190, 43, hwnd, NULL, NULL, NULL);

               // Crear la ventana con el número de fijas
               CreateWindowEx(WS_EX_LTRREADING, L"Static", L"Tampoco se sabe", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 570, 43 * numAttemps, 190, 43, hwnd, NULL, NULL, NULL);

               // Crear el pincel para el color de fondo
               hBrushStatic = CreateSolidBrush(colorRows);

               numAttemps++;
            }
         }
         return 0;
      }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
