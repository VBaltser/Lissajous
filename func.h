#ifndef FUNC_H
#define FUNC_H

#include <windows.h>
#include <math.h>

class Func
{
    //Структура для хранения параметров функции
    struct FuncParams
    {
        float t;
        int A;
        int W;
        int D;
        int coord;
    };

public:
    Func();
    bool startWork();
    void endWork();
    COORD  getCoords();

    void updateX();
    void updateY();
    void setFuncParamsX(int a, int w, int d);
    void setFuncParamsY(int a, int w, int d);
    bool isCoordUpdated();

    //Создание структур для X и Y
    FuncParams X;
    FuncParams Y;

    HANDLE hMutex;

private:

    HANDLE hStopEvent;
    HANDLE hUpdateEvent;

    HANDLE hThreadX;
    HANDLE hThreadY;

    static DWORD WINAPI ThreadFuncX(void * P);
    static DWORD WINAPI ThreadFuncY(void * P);
};

#endif // FUNC_H
