// Это компилилось в MS Visual C++ 6.0
// Компилятор - Intel C++ Compiler 9.1
// работает под Win2K, WinXP, Vista (from time to time)

// Copyright (c) 2006 УлГУ :)
// За бездумное копирование кода - будет больно экзамене :)

#include <windows.h>	// подрубаем основные функции и типы из Windows API

// Тип TCHAR и макрос TEXT в зависимости от типа компиляции выбирают использовать юникод или обычный ASCII
const TCHAR* szClassName = TEXT("ClassName");			// имя класса окошка
const TCHAR* szWindowTitle = TEXT("Hello World!");		// заголовок и имя класса окошка
RECT rect = {0};										// размер клиентской области

// главная функция обработки сообщений ( лекцию помните ? :) )
LRESULT APIENTRY WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		case WM_PAINT:	// происходит при перерисовке экрана
			{	// т.к. я здесь завожу переменную, то нужны эти скобочки
				// получать HDC в обработчике WM_PAINT можно _только_ при помощи BeginPaint
				// объяснения ждите на лекциях или читайте в MSDN Library
				PAINTSTRUCT ps;									// сюда будет занесена доп. информация, необходимая
																// для обработки перерисовки окна
				HDC hdc = BeginPaint(hWnd, &ps);				// получаем HDC - хендл на "графический контекст"
				// Выводим по центру окна текст: Hello world!
				DrawText(hdc, szWindowTitle, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				EndPaint(hWnd, &ps);							// завершаем перерисовку
			}
			break;
		case WM_SIZE:	// происходит при изменении размеров окна
			// закомментить эти 2 строчки если это компилится на VC60
			rect.right = LOWORD(lParam);	// записываем в rect новую ширину окошка
			rect.bottom = HIWORD(lParam);	// записываем в rect новую высоту окошка
			break;
		case WM_DESTROY:	// если этого не сделать, то все ваши жалкие попытки закрыть окно будут проигнорированы
			PostQuitMessage(0);	// читайте первую лекцию :) если вы на ней были
			break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// точка входа в программу
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nShowCmd)
{
	// 1й этап
	// регистрируется класс
	WNDCLASS wc = {0};						// я же говорил что WNDCLASSEX можно не юзать, но MSDN ругается
	wc.style = CS_HREDRAW | CS_VREDRAW;		// говорим окну перерисовываться при изменении размеров окна
    wc.lpfnWndProc = &WndProc;				// это самое главное :)
    wc.hInstance = hInst;					// это почти самое главное
    wc.hCursor = LoadCursor(0, IDC_ARROW);	// мулечки и рюшечки в виде курсора
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // GetStockObject возвращает хендл на белую кисточку, для фона окна
    wc.lpszClassName = szClassName;		// это почти почти самое главное
	if ( !RegisterClass(&wc) ) return(0);	// регистрируем ( не получилось - уходим по английски )

	// 2й этап
	// создается окно
	HWND hWnd = CreateWindow(
		szClassName,				// имя класса
		szWindowTitle,				// заголовок
		WS_OVERLAPPEDWINDOW,		// тип окошка
		CW_USEDEFAULT, 0,			// положение
		CW_USEDEFAULT, 0,			// размер
		0, 0, hInst, 0);			// [censored]
	if ( !hWnd ) return(0);

	// теперь показываем окошко ( nShowCmd - как его показать? минимизированным, обычным или ... )
	ShowWindow(hWnd, nShowCmd);
	// и зачем это нужно? сам не знаю, но все так делают. на семенаре поговорим подробнее
    UpdateWindow(hWnd);

	// 3й этап
	// главный цикл обработки сообщений
	MSG msg = {0};					// мессага, которую будем обрабатывать
	while( GetMessage(&msg, 0, 0, 0) ) {	// ждем сообщение
		TranslateMessage(&msg);		// преобразуем виртуальную клавишу в ASCII-код и посылаем сообщение WM_CHAR (тут не нужно)
		DispatchMessage(&msg);		// передаем сообщения для обработки в "главную функцию обработки сообщений"
	}

	return( (int)msg.wParam );		// т.к. это функция, то вернем параметр WM_QUIT сообщения (см. PostQuitMessage)
}