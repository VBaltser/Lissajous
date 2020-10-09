#include "func.h"


Func::Func()
{
    //Инициализация начальных значений для X и Y
    X.t = 0.314f;
    X.A = 1;
    X.W = 1;
    X.D = 1;
    X.coord = 0;

    Y.t = 0.314f;
    Y.A = 1;
    Y.W = 1;
    Y.D = 1;
    Y.coord = 0;


    hStopEvent   = CreateEvent( nullptr, true, false, nullptr);
    hMutex       = CreateMutex( nullptr, false, L"mutex");
    hUpdateEvent = CreateEvent( nullptr, true, false, nullptr);
}

//Запуск потоков
bool Func::startWork()
{
    //Получить коды выхода потоков
    DWORD Code1, Code2;
    GetExitCodeThread(hThreadX,&Code1);
    GetExitCodeThread(hThreadY,&Code2);

    //Проверка запущены ли потоки
    if(Code1 == STILL_ACTIVE || Code2 == STILL_ACTIVE) return false;

    //Если не запущены, создать новые потоки
    hThreadX = CreateThread(nullptr,0,ThreadFuncX,this,0,nullptr);
    hThreadY = CreateThread(nullptr,0,ThreadFuncY,this,0,nullptr);

    return true;
}

//Завершение работы потоков
void Func::endWork()
{
    //Если они уже не работают - отмена
    if( hThreadX == INVALID_HANDLE_VALUE || hThreadY == INVALID_HANDLE_VALUE) return;

    //Сделать активным событие 'hStopEvent'
    SetEvent( hStopEvent );

    //Ожидание завершения работы
    if( WaitForSingleObject( hThreadX, 2000 ) != WAIT_OBJECT_0 )
    {
        //Если не завершился - убить
        TerminateThread( hThreadX, 1 );
    }

    if( WaitForSingleObject( hThreadY, 2000 ) != WAIT_OBJECT_0 )
    {
        TerminateThread( hThreadY, 1 );
    }

    //Закрыть HANDLE для каждого потока
    CloseHandle( hThreadX );
    CloseHandle( hThreadY );

    hThreadX = INVALID_HANDLE_VALUE;
    hThreadY = INVALID_HANDLE_VALUE;

    //Сделать неактивным событие "hStopEvent"
    ResetEvent(hStopEvent);
}

//Обновление X-координаты
void Func::updateX()
{
    //Занять Mutex
    WaitForSingleObject(hMutex, 2000);
    //Вычисление следующего шага
    X.t += M_PI / 10000;
    if(X.t > 2 * M_PI) X.t = 0;

    X.coord = (int)(X.A * sin(X.W * X.t + X.D));

    //Освободить Mutex
    ReleaseMutex(hMutex);

    //Сделать активным событие "hUpdateEvent"
    SetEvent(hUpdateEvent);
}

//Обновление Y-координаты
void Func::updateY()
{
    WaitForSingleObject(hMutex, 2000);
    Y.t += M_PI / 10000;
    if(Y.t > 2 * M_PI) Y.t = 0;

    Y.coord = (int)(Y.A * sin(Y.W * Y.t + Y.D));
    ReleaseMutex(hMutex);
    SetEvent(hUpdateEvent);
}

//Функция, возвращающая текущие координаты
COORD Func::getCoords()
{
    COORD c;
    c.X = X.coord;
    c.Y = Y.coord;

    //Сделать неактивным событие "hUpdateEvent"
    ResetEvent(hUpdateEvent);
    return c;
}

//Функция, возвращающая, обновились ли координаты
bool Func::isCoordUpdated()
{
    return WaitForSingleObject( hUpdateEvent, 0 ) == WAIT_OBJECT_0;
}

//Функция, обновляющая параметры A W D для X
void Func::setFuncParamsX(int a, int w, int d)
{
    //Занять Mutex, чтобы приостановить работу потоков
    WaitForSingleObject(hMutex, 2000);

    //Сохранение новых параметров
    X.A = a;
    X.W = w;
    X.D = d;

    //Освободить Mutex
    ReleaseMutex(hMutex);
}

//Функция, обновляющая параметры A W D для Y
void Func::setFuncParamsY(int a, int w, int d)
{
    WaitForSingleObject(hMutex, 2000);
    Y.A = a;
    Y.W = w;
    Y.D = d;
    ReleaseMutex(hMutex);
}

//Поток, вычисляющий координаты для X
DWORD Func::ThreadFuncX(void *c)
{
    Func* func = (Func*)c;

    while(true)
    {
        //Занять Mutex для вычисления новых координат
        WaitForSingleObject(func->hMutex, 2000);

        //Вызвать функцию обновления координаты
        func->updateX();

        //Поток "засыпает", "слушая" событие hStopEvent.
        if(WaitForSingleObject(func->hStopEvent, 10 ) != WAIT_TIMEOUT)
        {
            //Если hStopEvent активен, освобождается Mutex и
            //поток выходит из цикла, завершая свою работу
            ReleaseMutex(func->hMutex);
            break;
        }

        //Если hStopEvent неактивен, просто освобождается Mutex и поток продолжает работу
        ReleaseMutex(func->hMutex);
    }

    //Завершение работы
    return 0;
}

//Поток, вычисляющий координаты для Y
DWORD Func::ThreadFuncY(void *c)
{
    Func* func = (Func*)c;

    while(true)
    {
        WaitForSingleObject(func->hMutex, 2000);
        func->updateY();

        if(WaitForSingleObject(func->hStopEvent, 10 ) != WAIT_TIMEOUT)
        {
            ReleaseMutex(func->hMutex);
            break;
        }

        ReleaseMutex(func->hMutex);
    }

    return 0;
}
