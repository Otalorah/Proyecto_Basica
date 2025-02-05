#ifndef UNICODE
#define UNICODE
#endif 

#include <cwchar>
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
void newAttempt(HWND hwnd); 
int spacesToPoints(int points); 
void displayDialogW(HWND hwnd);
void displayDialogR(HWND hwnd);
struct data createRow(HWND hwnd);
void initializeInterface(HWND hwnd);
int calculatePoints(int fixeds, int spades);
void registerDialogClassW(HINSTANCE hInstance);
void registerDialogClassR(HINSTANCE hInstance);
float spacesToUsername(char username[MAX_USERNAME]);
LRESULT CALLBACK EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void paintModal(HWND hwnd, wstring text, int x, int y, int width, COLORREF color);
LRESULT CALLBACK DialogWProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DialogRProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


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
COLORREF colorBG = RGB(225, 160, 0);
COLORREF colorRows = RGB(225, 225, 225);
vector<int> secretNum = {1, 2, 3, 4, 5};
string secretNumStr = vectorToString(secretNum);
wstring secretNumW(secretNumStr.begin(), secretNumStr.end());

// Se declaran variables de la ventana de bienvenida
HWND hTitleW;
HWND hAuthorO;
HWND hAuthorB;
HWND hAuthorT;
HWND hSubTitle;
HWND hShowScores;
HWND hUsernameLabel;
HWND hUsernameInput;

// Se declaran variables de la ventana de resultados
HWND hExit;
HWND hTitleR;
HWND hPoints;
HWND hNumSecret;
HWND hSaveScores;
wstring textTitleR;
COLORREF colorModalR = RGB(0, 125, 30);

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
   registerDialogClassW(hInstance);
   registerDialogClassR(hInstance);

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
         displayDialogW(hwnd);
         // Se inicializa la interfaz
         initializeInterface(hwnd);
         return 0;
      }
      case WM_PAINT:
      {
         PAINTSTRUCT ps;
         // Se crea el pincel
         HBRUSH brush = CreateSolidBrush(colorBG);
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
   CreateWindowEx(WS_EX_LTRREADING, L"Button", L"ADIVINAR", WS_CHILD | WS_VISIBLE | WS_BORDER, 358, 525, 88, 30, hwnd, (HMENU)NEW_ATTEMPT, NULL, NULL);
}

// Función para mostrar el resultado de cada intento
struct data createRow(HWND hwnd)
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

   return results;
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
      if (numAttemps < MAX_ATTEMPTS) 
      {
         // Se crea la fila
         struct data results = createRow(hwnd);
         // Se valida si el usuario adivinó el número
         if (results.val1 == NUM_DIGITS) 
         {
            // Se le añade una bonificación al jugador
            player.points += (MAX_ATTEMPTS - numAttemps) * 1000;

            // Se convierte el username a wstring
            wstring usernameText(player.username, player.username + strlen(player.username));

            textTitleR = L"FELICIDADES " + usernameText;
            displayDialogR(hwnd);
            numAttemps--;
         }
         numAttemps++;
      } 
      // Si ha superado la máxima cantidad de intentos
      else 
      {
         // Se crea la fila
         struct data results = createRow(hwnd);

         // Se convierte el username a wstring
         wstring usernameText(player.username, player.username + strlen(player.username));

         // Se valida si el usuario adivinó el número
         if (results.val1 < NUM_DIGITS) 
         {
            colorModalR = RGB(200, 0, 0);
            textTitleR = L"HAS PERDIDO " + usernameText;
            displayDialogR(hwnd);
         }
         else 
         {
            textTitleR = L"FELICIDADES " + usernameText;
            displayDialogR(hwnd);
         }
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
void registerDialogClassW(HINSTANCE hInstance)
{
    WNDCLASS dialogW = { };

    // Se le asignan las propiedades al objeto dialog
    dialogW.hInstance = hInstance;
    dialogW.lpfnWndProc = DialogWProcedure;
    dialogW.lpszClassName = L"DialogWClass";

    RegisterClass(&dialogW);
}

// Función para manejar los mensajes de la ventana de diálogo
LRESULT CALLBACK DialogWProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg)
   {
      case WM_CLOSE:
      {
         // Se obtiene el username ingresado
         GetWindowTextA(hUsernameInput, player.username, MAX_USERNAME);    

         // Se valida si el usuario ingresó un nombre
         if (strlen(player.username) > 0) DestroyWindow(hwnd); // Se elimina la ventana de diálogo
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
         paintModal(hTitleW, L"¡PICAS Y FIJAS!", 200, 0, 15, colorBG);
         paintModal(hSubTitle, L"El juego consiste en adivinar un número de 5 dígitos", 80, 0, 52, colorBG);
         paintModal(hUsernameLabel, L"Por favor ingrese su usuario:", 25, 0, 30, colorBG);
         paintModal(hAuthorO, L"AUTORES: Juan Manuel Otálora Hernández", 100, 0, 40, colorBG);
         paintModal(hAuthorB, L"Johan Stevan Bermeo Buitrago", 140, 0, 28, colorBG);
         paintModal(hAuthorT, L"Juan Camilo Triana Paipa", 157, 0, 25, colorBG);

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
               if (strlen(player.username) > 0) DestroyWindow(hwnd); // Se elimina la ventana de diálogo
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
void displayDialogW(HWND hwnd)
{
   // Se crea la ventana de diálogo 
   HWND dialogModal = CreateWindowW(L"DialogWClass", L"Bienvenido", WS_VISIBLE | WS_OVERLAPPEDWINDOW | SS_CENTERIMAGE | SS_CENTER, 525, 100, 500, 500, hwnd, NULL, NULL, NULL);

   // Se crea el texto de bienvenida
   hTitleW = CreateWindowEx(WS_EX_LTRREADING, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE | WS_EX_TRANSPARENT, 0, 25, 500, 50, dialogModal, NULL, NULL, NULL);

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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - Funciones para el diálogo modal de resultados - - - - - - - - - - - - - - - - - - - - - - - - //

// Función para registrar la clase de la ventana de diálogo
void registerDialogClassR(HINSTANCE hInstance)
{
    WNDCLASS dialogR = { };

    // Se le asignan las propiedades al objeto dialog
    dialogR.hInstance = hInstance;
    dialogR.lpfnWndProc = DialogRProcedure;
    dialogR.lpszClassName = L"DialogRClass";

    RegisterClass(&dialogR);
}

// Función para manejar los mensajes de la ventana de diálogo
LRESULT CALLBACK DialogRProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   switch (uMsg)
   {
      case WM_CLOSE:
      {
         // Se cierra la ventana principal
         DestroyWindow(hmainWindow);
         return 0;
      }
      case WM_PAINT:
      {
         PAINTSTRUCT ps;
         // Comienza el trazo
         HDC hdc = BeginPaint(hwnd, &ps);

            // Se crea el pincel
            HBRUSH hBrush = CreateSolidBrush(colorModalR);
            // Se pinta la pantalla del modal
            FillRect(hdc, &ps.rcPaint, hBrush);

         EndPaint(hwnd, &ps);
         DeleteObject(hBrush);
         // Se pintan las ventanas dentro del modal
         int spacesTitle = spacesToUsername(player.username);
         paintModal(hTitleR, textTitleR, 180 - round(spacesTitle*5.5), 0, 16 + round(spacesTitle*2.7), colorModalR);

         // Se convierte el número secreto a wstring
         wstring numSecretText = L"El número es: " + secretNumW;
         paintModal(hNumSecret, numSecretText, 180, 0, 20, colorModalR);

         int spacesPoints = spacesToPoints(player.points);
         // Se convierte el puntaje a wstring
         string pointsStr = to_string(player.points);
         wstring pointsW(pointsStr.begin(), pointsStr.end());
         wstring pointsText = L"Puntos: " + pointsW;
         paintModal(hPoints, pointsText, 205 - spacesPoints, 0, 11 + spacesPoints, colorModalR);

         return 0;
      } 
      case WM_COMMAND:
      {
         // TODO: guardar los puntajes
         // TODO: volver a jugar
         switch (LOWORD(wParam))
         {
            case CLOSE_MODAL:
            {
               return 0;
            }
         } 
         return 0;
      }
   }
   return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Función para calcular los espacios para los puntos
int spacesToPoints(int points) 
{
   if (points < 1000) return 1;
   else if (points < 10000) return 2;
   else return 3;
}

// Función para calcular los espacios para el username
float spacesToUsername(char username[MAX_USERNAME])
{
   if (strlen(username) < 5) return 0.8;
   else if (strlen(username) < 11) return 2.5;
   else if (strlen(username) < 14) return 3.2;
   else if (strlen(username) < 17) return 4.2;
   else return 6.1;
}

// Función para mostrar la ventana de diálogo
void displayDialogR(HWND hwnd)
{
   // Se crea la ventana de diálogo 
   HWND dialogModal = CreateWindowW(L"DialogRClass", L"FIN DEL JUEGO", WS_VISIBLE | WS_OVERLAPPEDWINDOW | SS_CENTERIMAGE | SS_CENTER, 520, 200, 510, 300, hwnd, NULL, NULL, NULL);

   // Se crea el título del modal
   hTitleR = CreateWindowEx(WS_EX_LTRREADING, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE | WS_EX_TRANSPARENT, 0, 25, 500, 50, dialogModal, NULL, NULL, NULL);

   // Se crea el texto para mostrar el número secreto
   hNumSecret = CreateWindowEx(WS_EX_LTRREADING, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 0, 55, 500, 50, dialogModal, NULL, NULL, NULL);

   // Se crea el texto para mostrar el puntaje
   hPoints = CreateWindowEx(WS_EX_LTRREADING, L"Static", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 0, 100, 500, 50, dialogModal, NULL, NULL, NULL);

   // Se crea el botón para guardar los puntajes
   hSaveScores = CreateWindowEx(WS_EX_LTRREADING, L"Button", L"GUARDAR", WS_CHILD | WS_VISIBLE | WS_BORDER, 40, 175, 150, 30, dialogModal, (HMENU) SHOW_SCORES, NULL, NULL);

   // Se crea el boton para salir
   hExit = CreateWindowEx(WS_EX_LTRREADING, L"Button", L"VOLVER A JUGAR", WS_CHILD | WS_VISIBLE | WS_BORDER, 310, 175, 150, 30, dialogModal, (HMENU) CLOSE_MODAL, NULL, NULL);

}

// Función para pintar las ventanas del modal
void paintModal(HWND hwnd, wstring text, int x, int y, int width, COLORREF color)
{
   PAINTSTRUCT ps;
   HBRUSH hBrush = CreateSolidBrush(color);

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
