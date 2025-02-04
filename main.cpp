#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "functions.h"

using namespace std;

// Se definen las constantes
# define NUM_DIGITS 5
# define NEW_ATTEMPT 1
# define CLOSE_MODAL 2
# define SHOW_SCORES 3
# define MAX_ATTEMPTS 10
# define MAX_USERNAME 50

// Se declaran las funciones
void createRow(HWND hwnd);
void newAttempt(HWND hwnd); 
void displayDialog(HWND hwnd);
void initializeInterface(HWND hwnd);
int calculatePoints(int fixeds, int spades);
void registerDialogClass(HINSTANCE hInstance);
void paintModalW(HWND hwnd, wstring text, int x, int y, int width);
LRESULT CALLBACK EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DialogProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Se declara la estructura del jugador
struct Player 
{
   int points;
   char username[MAX_USERNAME];
} player;

// Se declaran variables globales
HWND hInfo;
HWND hattempText;
HWND hmainWindow;
int numAttemps = 1;
HBRUSH hBrushStatic;
WNDPROC oldEditProc;
vector<int> secretNum = hideNum();
COLORREF colorRows = RGB(225, 225, 225);

// Se declaran variables de la ventana de diálogo
HWND hTitle;
HWND hAuthorO;
HWND hAuthorB;
HWND hAuthorT;
HWND hSubTitle;
HWND hShowScores;
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
      370, 50, 800, 600, // La posición y tamaño de la ventana
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
      case WM_CREATE:
      {
         // Se muestra el modal
         displayDialog(hwnd);
         // Se inicializa la interfaz
         initializeInterface(hwnd);
         return 0;
      }
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
      case WM_CTLCOLORSTATIC:
      {
         // Obtener el dispositivo de contexto desde wParam
         HDC hdcStatic = (HDC) wParam;
         // Establecer el color de fondo
         SetBkColor(hdcStatic, colorRows);
         // Devolver el manejador del pincel
         return (INT_PTR) hBrushStatic;
      }
      case WM_COMMAND:
      {
         // Se escucha el mesaje para crear un nuevo intento
         switch (LOWORD(wParam)) case NEW_ATTEMPT: newAttempt(hwnd);
         return 0;
      }
   }
   return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Función para crear las ventanas iniciales de la interfaz
void initializeInterface(HWND hwnd)
{
   // Se crean los títulos de las columnas
   CreateWindowEx(WS_EX_LTRREADING, L"Static", L"INTENTO #", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 0, 0, 160, 43, hwnd, NULL, NULL, NULL);
   CreateWindowEx(WS_EX_LTRREADING, L"Static", L"NÚMERO", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 160, 0, 160, 43, hwnd, NULL, NULL, NULL);
   CreateWindowEx(WS_EX_LTRREADING, L"Static", L"PICAS", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 320, 0, 160, 43, hwnd, NULL, NULL, NULL);
   CreateWindowEx(WS_EX_LTRREADING, L"Static", L"FIJAS", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 480, 0, 160, 43, hwnd, NULL, NULL, NULL);
   CreateWindowEx(WS_EX_LTRREADING, L"Static", L"PUNTOS", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 640, 0, 160, 43, hwnd, NULL, NULL, NULL);

   // Texto para guiar al usuario
   hInfo = CreateWindowEx(WS_EX_LTRREADING, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 268, 485, 268, 15, hwnd, NULL, NULL, NULL);
   
   // Se crea el cuadro para ingresar el número
   hattempText = CreateWindowEx(WS_EX_LTRREADING, L"Edit", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 358, 507, 88, 15, hwnd, NULL, NULL, NULL);
   // Establecer el límite máximo de caracteres
   SendMessage(hattempText, EM_SETLIMITTEXT, NUM_DIGITS, 0);
   // Subclasear el cuadro de texto para haceder a los mensajes de teclado
   oldEditProc = (WNDPROC) SetWindowLongPtr(hattempText, GWLP_WNDPROC, (LONG_PTR)EditProc);

   // Se crea el botón para adivinar
   CreateWindowEx(WS_EX_LTRREADING, L"Button", L"Adivinar", WS_CHILD | WS_VISIBLE | WS_BORDER, 358, 525, 88, 30, hwnd, (HMENU)NEW_ATTEMPT, NULL, NULL);
}

// Función para mostrar el resultado de cada intento
void createRow(HWND hwnd)
{
   char numInput[NUM_DIGITS + 1];

   // Se calcula la altura de la fila
   int height = 43 * numAttemps;

   // Se obtiene el número ingresado
   GetWindowTextA(hattempText, numInput, NUM_DIGITS + 1);

   // Convertir datos a wstring
   wstring attemptStr = to_wstring(numAttemps);
   wstring textNum = to_wstring(stoi(numInput));

   // Covertir el número ingresado a un vector
   vector<int> tryVec = attempt(stoi(numInput));

   // Extraer las picas y fijas
   struct data results = checkUp(secretNum, tryVec);
   int fixeds = results.val1;
   int spades = results.val2;

   int rowPoints = calculatePoints(fixeds, spades);
   player.points += rowPoints;

   // Crear la ventana con el número de intento
   CreateWindowEx(WS_EX_LTRREADING, L"Static", attemptStr.c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 0, height, 160, 43, hwnd, NULL, NULL, NULL);

   // Crear la ventana con el número ingresado
   CreateWindowEx(WS_EX_LTRREADING, L"Static", textNum.c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 160, height, 160, 43, hwnd, NULL, NULL, NULL);

   // Crear la ventana con el número de picas
   CreateWindowEx(WS_EX_LTRREADING, L"Static", to_wstring(spades).c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 320, height, 160, 43, hwnd, NULL, NULL, NULL);

   // Crear la ventana con el número de fijas
   CreateWindowEx(WS_EX_LTRREADING, L"Static", to_wstring(fixeds).c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 480, height, 160, 43, hwnd, NULL, NULL, NULL);

   // Crear la ventana con el número de puntos
   CreateWindowEx(WS_EX_LTRREADING, L"Static", to_wstring(rowPoints).c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 640, height, 160, 43, hwnd, NULL, NULL, NULL);

   // Crear el pincel para el color de fondo
   hBrushStatic = CreateSolidBrush(colorRows);

   // Eliminar el texto ingresado por el usuario
   SetWindowText(hattempText, L"");
}

// Función para crear un nuevo intento
void newAttempt(HWND hwnd) 
{
   char attempText[NUM_DIGITS + 1];
   GetWindowTextA(hattempText, attempText, NUM_DIGITS + 1);

   // Se valida si el número ingresado tiene 5 cifras
   if (strlen(attempText) < NUM_DIGITS) MessageBox(hwnd, L"Ingrese un número de 5 cifras", L"Número inválido", MB_OK);
   // Se valida si el número empieza por 0
   else if (attempText[0] == '0') MessageBox(hwnd, L"El número no puede empezar por 0", L"Número inválido", MB_OK);
   // Se valida si el número ingresado tiene letras
   else if (!fullNum(attempText)) MessageBox(hwnd, L"No se aceptan carácteres que no sean dígitos", L"Número inválido", MB_OK);
   // Se valida si el número ingresado tiene un número repetido
   else if (!checkNum(attempText)) MessageBox(hwnd, L"El número no puede tener dígitos repetidos", L"Número inválido", MB_OK);
   else 
   {
      // Si no ha superado la máxima cantidad de itentos
      if (numAttemps <= MAX_ATTEMPTS) {
         // Se crea la fila
         createRow(hwnd);
         numAttemps++;
      } else {
         // Convertir vector a string
         string secretNumStr = vectorToString(secretNum);
         // Convertir string a wstring
         wstring secretNumW(secretNumStr.begin(), secretNumStr.end());
         MessageBox(hwnd, secretNumW.c_str(), L"Fin del juego", MB_OK);
      }
   } 
}

// Función para calcular puntos
int calculatePoints(int fixeds, int spades)
{
   return (fixeds * 100) + (spades * 50);
}

// Función para manejar los mensajes del cuadro de texto
LRESULT CALLBACK EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   if (uMsg == WM_KEYDOWN && wParam == VK_RETURN) 
   {
      // Reenviar el mensaje a la ventana principal
      SendMessage(GetParent(hwnd), WM_COMMAND, NEW_ATTEMPT, 0);
      return 0;
   }
   return CallWindowProc(oldEditProc, hwnd, uMsg, wParam, lParam);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - Funciones para el diálogo modal de bienvenida - - - - - - - - - - - - - - - - - - - - - - - - //

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
      case WM_CLOSE:
      {
         // Se obtiene el username ingresado
         GetWindowTextA(hUsernameInput, player.username, MAX_USERNAME);    

         // Se valida si el usuario ingresó un nombre
         if (strlen(player.username) > 0) 
         {
            DestroyWindow(hwnd); // Se elimina la ventana de diálogo
            GetWindowTextA(hUsernameInput, player.username, MAX_USERNAME); // Se obtiene el username ingresado
         }
         else MessageBox(hwnd, L"Por favor ingrese un usuario", L"Error", MB_OK); // Se muestra un mensaje de error
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
               // Se obtiene el username ingresado
               GetWindowTextA(hUsernameInput, player.username, MAX_USERNAME);

               // Se valida si el usuario ingresó un nombre
                        // Se valida si el usuario ingresó un nombre
               if (strlen(player.username) > 0) 
               {
                  DestroyWindow(hwnd); // Se elimina la ventana de diálogo
                  GetWindowTextA(hUsernameInput, player.username, MAX_USERNAME); // Se obtiene el username ingresado
               }
               else MessageBox(hwnd, L"Por favor ingrese un usuario", L"Error", MB_OK); // Se muestra un mensaje de error
               return 0;
            }
            case SHOW_SCORES:
            {
               // Se muestra un mensaje con los puntajes
               MessageBox(hwnd, L"Los puntajes se mostrarán aquí", L"Puntajes", MB_OK);
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

   // Se crea el botón para ver los puntajes
   hShowScores = CreateWindowEx(WS_EX_LTRREADING, L"Button", L"VER PUNTAJES", WS_CHILD | WS_VISIBLE | WS_BORDER, 196, 275, 108, 30, dialogModal, (HMENU) SHOW_SCORES, NULL, NULL);

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