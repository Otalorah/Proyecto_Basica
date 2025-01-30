#ifndef UNICODE
#define UNICODE
#endif 

#include <string>
#include <windows.h>
// #include "functions.h"

using namespace std;

// Se definen las constantes
# define NUM_DIGITS 5
# define NEW_ATTEMPT 1
# define CLOSE_MODAL 2
# define MAX_ATTEMPTS 10

// Se declaran las funciones
void createRow(HWND hwnd);
void displayDialog(HWND hwnd);
void initializeInterface(HWND hwnd);
void registerDialogClass(HINSTANCE hInstance);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void paintModalW(HWND hwnd, wstring text, int x, int y, int width);
LRESULT CALLBACK DialogProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Se declaran variables globales
HWND hInfo;
HWND hattempText;
HWND hmainWindow;
HWND hcontentRows; // Para ver lo del scroll
int numAttemps = 1;
HBRUSH hBrushStatic;
char numInput[NUM_DIGITS + 1];
COLORREF colorRows = RGB(225, 225, 225);

// Se declaran variables de la ventana de diálogo
HWND hTitle;
HWND hAuthorO;
HWND hAuthorB;
HWND hAuthorT;
HWND hSubTitle;
HWND hUsernameLabel;
HWND hUsernameInput;

// Función principal
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
   hmainWindow = CreateWindowEx(0, CLASS_NAME, L"PICAS Y FIJAS", WS_OVERLAPPEDWINDOW,
      400, 50, 760, 600, // La posición y tamaño de la ventana
      NULL, NULL, hInstance, NULL
   );

   // En caso de no poder crear la ventana
   if (hmainWindow == NULL) return 0;
   ShowWindow(hmainWindow, nCmdShow);

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
      // Se crea el pincel
      HBRUSH brush = CreateSolidBrush(RGB(225, 160, 0));
      // Comienza el trazo
      HDC hdc = BeginPaint(hwnd, &ps);

         // Se pinta la pantalla de la ventana principal
         FillRect(hdc, &ps.rcPaint, brush);

      // Finaliza el trazo
      EndPaint(hwnd, &ps);

      // Commienza el trazo para el texto de guía
      HDC hdcI = BeginPaint(hInfo, &ps);

         // Se pinta la ventana
         FillRect(hdcI, &ps.rcPaint, brush);
         // Se elimina el pincel
         DeleteObject(brush);

         // Dibujar texto en la ventana
         SetTextColor(hdcI, RGB(255, 255, 255)); // Color del texto
         SetBkMode(hdcI, TRANSPARENT); // Fondo transparente para el texto
         TextOut(hdcI, 0, 0, L"INGRESE EL NÚMERO PARA ADIVINAR", 32);

      // Finaliza el trazo
      EndPaint(hInfo, &ps);

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
      // Obtener el dispositivo de contexto desde wParam
      HDC hdcStatic = (HDC) wParam;
      // Establecer el color de fondo
      SetBkColor(hdcStatic, colorRows);
      // Devolver el manejador del pincel
      return (INT_PTR)hBrushStatic;
   }
   case WM_COMMAND:
   {
      switch (LOWORD(wParam))
      {
         case NEW_ATTEMPT:
         {
               // Si no ha superado la máxima cantidad de itentos
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

   // Texto para guiar al usuario
   hInfo = CreateWindowEx(WS_EX_LTRREADING, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 248, 485, 268, 15, hwnd, NULL, NULL, NULL);
   
   // Se crea el cuadro para ingresar el número
   hattempText = CreateWindowEx(WS_EX_LTRREADING, L"Edit", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 348, 507, 68, 15, hwnd, NULL, NULL, NULL);

   // Establecer el límite máximo de caracteres
   SendMessage(hattempText, EM_SETLIMITTEXT, NUM_DIGITS, 0);

   // Se crea el botón para adivinar
   CreateWindowEx(WS_EX_LTRREADING, L"Button", L"Adivinar", WS_CHILD | WS_VISIBLE | WS_BORDER, 348, 525, 68, 30, hwnd, (HMENU)NEW_ATTEMPT, NULL, NULL);
}

// Función para mostrar el resultado de cada intento
void createRow(HWND hwnd)
{
   // Se calcula la altura de la fila
   int height = 43 * numAttemps;

   // Se obtiene el número ingresado
   GetWindowTextA(hattempText, numInput, NUM_DIGITS + 1);

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
   SetWindowText(hattempText, L"");
}

// Función para registrar la clase de la ventana de diálogo
void registerDialogClass(HINSTANCE hInstance)
{
    WNDCLASS dialog = { };

    // Se le asignan las propiedades al objeto dialog
    dialog.hInstance = hInstance;
    dialog.lpfnWndProc = DialogProcedure;
    dialog.lpszClassName = L"DialogClass";

    RegisterClass(&dialog);
}

// Función para manejar los mensajes de la ventana de diálogo
LRESULT CALLBACK DialogProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg)
   {
      case WM_DESTROY:
      {
         DestroyWindow(hwnd);
         return 0;
      }
      case WM_PAINT:
      {
         PAINTSTRUCT ps;
         // Comienza el trazo
         HDC hdc = BeginPaint(hwnd, &ps);

            // Se crea el pincel
            HBRUSH hBrush = CreateSolidBrush(RGB(225, 160, 0));
            // Se pinta la pantalla del modal
            FillRect(hdc, &ps.rcPaint, hBrush);

         EndPaint(hwnd, &ps);
         DeleteObject(hBrush);
         // Se pintan las ventanas dentro de la modal
         paintModalW(hTitle, L"¡PICAS Y FIJAS!", 200, 0, 15);
         paintModalW(hSubTitle, L"El juego consiste en adivinar un número de 5 dígitos", 80, 0, 52);
         paintModalW(hUsernameLabel, L"Por favor ingrese su usuario:", 25, 0, 30);
         paintModalW(hAuthorO, L"AUTORES: Juan Manuel Otálora Hernández", 100, 0, 40);
         paintModalW(hAuthorB, L"Johan Stevan Bermeo Buitrago", 140, 0, 28);
         paintModalW(hAuthorT, L"Juan Camilo Triana Paipa", 157, 0, 25);

         return 0;
      } 
      case WM_COMMAND:
      {
         switch (LOWORD(wParam))
         {
            case CLOSE_MODAL:
            {
               char username[25];
               // Se obtiene el username ingresado
               GetWindowTextA(hUsernameInput, username, 25);

               // Se valida si el usuario ingresó un nombre
               if (strlen(username) > 0)
               {
                  // Se elimina la ventana de diálogo
                  DestroyWindow(hwnd);
               }
               else
               {
                  MessageBox(hwnd, L"Por favor ingrese un usuario", L"Error", MB_OK);
               };
            }
         } 
         return 0;
      }
   }
   return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Función para mostrar la ventana de diálogo
void displayDialog(HWND hwnd)
{
   // Se crea la ventana de diálogo 
   HWND dialogModal = CreateWindowW(L"DialogClass", L"Bienvenido", WS_VISIBLE | WS_OVERLAPPEDWINDOW | SS_CENTERIMAGE | SS_CENTER, 525, 100, 500, 500, hwnd, NULL, NULL, NULL);

   // Se crea el texto de bienvenida
   hTitle = CreateWindowEx(WS_EX_LTRREADING, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE | WS_EX_TRANSPARENT, 0, 25, 500, 50, dialogModal, NULL, NULL, NULL);

   // Se crea el texto para explicar el juego
   hSubTitle = CreateWindowEx(WS_EX_LTRREADING, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 0, 55, 500, 50, dialogModal, NULL, NULL, NULL);

   // Se crea el texto para indicar el ingreso de un username
   hUsernameLabel = CreateWindowEx(WS_EX_LTRREADING, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 125, 225, 250, 15, dialogModal, NULL, NULL, NULL);

   // Se crea el cuadro para ingresar el username
   hUsernameInput = CreateWindowEx(WS_EX_LTRREADING, L"Edit", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 120, 250, 250, 15, dialogModal, NULL, NULL, NULL);

   // Se crea el texto para mostrar a los autores
   hAuthorO = CreateWindowEx(WS_EX_LTRREADING, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 0, 325, 500, 25, dialogModal, NULL, NULL, NULL);
   hAuthorB = CreateWindowEx(WS_EX_LTRREADING, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 0, 350, 500, 25, dialogModal, NULL, NULL, NULL);
   hAuthorT = CreateWindowEx(WS_EX_LTRREADING, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 0, 375, 500, 25, dialogModal, NULL, NULL, NULL);

   // Se crea el boton para iniciar
   CreateWindowEx(WS_EX_LTRREADING, L"Button", L"INICIAR", WS_CHILD | WS_VISIBLE | WS_BORDER, 216, 425, 68, 30, dialogModal, (HMENU) CLOSE_MODAL, NULL, NULL);

}

// Función para pintar las ventanas de la modal
void paintModalW(HWND hwnd, wstring text, int x, int y, int width)
{
   PAINTSTRUCT ps;
   HBRUSH hBrush = CreateSolidBrush(RGB(225, 160, 0));

   // Commienza el trazo
   HDC hdc = BeginPaint(hwnd, &ps);

      // Se pinta la ventana
      FillRect(hdc, &ps.rcPaint, hBrush);
      // Se elimina el pincel
      DeleteObject(hBrush);

      // Dibujar texto en la ventana
      SetTextColor(hdc, RGB(255, 255, 255)); // Color del texto
      SetBkMode(hdc, TRANSPARENT); // Fondo transparente para el texto
      TextOut(hdc, x, y, text.c_str(), width);

   // Finaliza el trazo
   EndPaint(hwnd, &ps);

}