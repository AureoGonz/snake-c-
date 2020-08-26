#include <windows.h>
#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>

using namespace std;

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
#define ESC 27

const int dirs[] = {ARRIBA, IZQUIERDA, DERECHA, ABAJO};
int cuerpo[200][2];
int dir;
int tamano = 3;
int n = 0;
bool actualizar = false;
int xc = 2, yc = 3;
bool gameover = false;

void gotoxy(int x, int y)
{
    HANDLE hCon;
    COORD dwPos;

    dwPos.X = x;
    dwPos.Y = y;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hCon, dwPos);
}

void pintarLimites()
{
    //esquinas
    gotoxy(2, 3);
    printf("%c", 201);
    gotoxy(77, 3);
    printf("%c", 187);
    gotoxy(2, 23);
    printf("%c", 200);
    gotoxy(77, 23);
    printf("%c", 188);
    //lineas horizontales
    for (int i = 3; i < 77; i++)
    {
        gotoxy(i, 3);
        printf("%c", 205);
        gotoxy(i, 23);
        printf("%c", 205);
    }
    //lineas verticales
    for (int i = 4; i < 23; i++)
    {
        gotoxy(2, i);
        printf("%c", 186);
        gotoxy(77, i);
        printf("%c", 186);
    }
}

int xInside()
{
    return (rand() % 73) + 3;
}

int yInside()
{
    return (rand() % (17)) + 5;
}

void pintarParteCuerpo(int i)
{
    gotoxy(cuerpo[i][0], cuerpo[i][1]);
    if (i > 0)
    {
        printf("%c", 219);
    }
    else
    {
        switch (dir)
        {
        case ARRIBA:
            printf("v");
            break;
        case IZQUIERDA:
            printf(">");
            break;
        case DERECHA:
            printf("<");
            break;
        case ABAJO:
            printf("^");
            break;
        }
    }
}

void borrarCola()
{
    gotoxy(cuerpo[tamano - 1][0], cuerpo[tamano - 1][1]);
    printf(" ");
}

void iniciarCuerpo()
{
    dir = dirs[rand() % 3];
    int xi = xInside();
    int yi = yInside();
    switch (dir)
    {
    case ARRIBA:
        yi = yi <= 6 ? 7 : yi;
        yi = yi >= 21 ? 20 : yi;
        for (int i = 0; i < tamano; i++)
        {
            cuerpo[i][0] = xi;
            cuerpo[i][1] = yi++;
            pintarParteCuerpo(i);
        }
        break;
    case IZQUIERDA:
        xi = xi <= 4 ? 5 : xi;
        xi = xi >= 75 ? 74 : xi;
        for (int i = 0; i < tamano; i++)
        {
            cuerpo[i][0] = xi++;
            cuerpo[i][1] = yi;
            pintarParteCuerpo(i);
        }
        break;
    case DERECHA:
        xi = xi <= 4 ? 5 : xi;
        xi = xi >= 75 ? 74 : xi;
        for (int i = 0; i < tamano; i++)
        {
            cuerpo[i][0] = xi--;
            cuerpo[i][1] = yi;
            pintarParteCuerpo(i);
        }
        break;
    case ABAJO:
        yi = yi <= 6 ? 7 : yi;
        yi = yi >= 21 ? 20 : yi;
        for (int i = 0; i < tamano; i++)
        {
            cuerpo[i][0] = xi;
            cuerpo[i][1] = yi--;
            pintarParteCuerpo(i);
        }
        break;
    }
}

bool topaCuerpo(int xh, int yh)
{
    for (int i = 1; i < tamano; i++)
        if ((cuerpo[i][0] == xh) && (cuerpo[i][1] == yh))
            return true;
    return false;
}

bool actualizarCuerpo()
{
    int xh = cuerpo[0][0];
    int yh = cuerpo[0][1];
    actualizar = false;
    switch (dir)
    {
    case ARRIBA:
        if (yh - 1 != 3)
            actualizar = true;
        cuerpo[0][1]--;
        break;
    case IZQUIERDA:
        if (xh - 1 != 2)
            actualizar = true;
        cuerpo[0][0]--;
        break;
    case DERECHA:
        if (xh + 1 != 77)
            actualizar = true;
        cuerpo[0][0]++;
        break;
    case ABAJO:
        if (yh + 1 != 23)
            actualizar = true;
        cuerpo[0][1]++;
        break;
    }
    if (actualizar)
    {
        borrarCola();
        for (int i = tamano - 1; i > 1; i--)
        {
            cuerpo[i][0] = cuerpo[i - 1][0];
            cuerpo[i][1] = cuerpo[i - 1][1];
        }
        cuerpo[1][0] = xh;
        cuerpo[1][1] = yh;
    }
    return actualizar && !topaCuerpo(cuerpo[0][0], cuerpo[0][1]);
}

void dibujarCuerpo()
{
    gameover = !actualizarCuerpo();
    pintarParteCuerpo(0);
    pintarParteCuerpo(1);
}

void manejarTeclas()
{
    if (kbhit())
    {
        switch (getch())
        {
        case ARRIBA:
            dir = dir == ABAJO ? dir : ARRIBA;
            break;
        case IZQUIERDA:
            dir = dir == DERECHA ? dir : IZQUIERDA;
            break;
        case DERECHA:
            dir = dir == IZQUIERDA ? dir : DERECHA;
            break;
        case ABAJO:
            dir = dir == ARRIBA ? dir : ABAJO;
            break;
        case ESC:
            dir = ESC;
            break;
        default:
            break;
        }
    }
}

void teclear()
{
    manejarTeclas();
    manejarTeclas();
}

void dibujarComida()
{
    bool servida = false;
    while (!servida)
    {
        xc = (rand() % 73) + 3;
        yc = (rand() % 18) + 4;
        servida = true;
        for (int i = 0; i < tamano; i++)
            servida = servida && ((cuerpo[i][0] != xc) || (cuerpo[i][1] != yc));
        if (servida)
        {
            gotoxy(xc, yc);
            printf("%c", 4);
            break;
        }
    }
}

void comer()
{
    if (((cuerpo[0][0] == xc) && (cuerpo[0][1] == yc)))
    {
        tamano++;
        dibujarComida();
    }
}

int main()
{
    srand((unsigned int)time(NULL));
    pintarLimites();
    iniciarCuerpo();
    dibujarComida();
    while (dir != ESC && !gameover)
    {
        comer();
        teclear();
        dibujarCuerpo();
        Sleep(200);
    }
    system("pause>null");
    return 0;
}