/*  interprete.c
    Interprete de BrainFuck.

    Copyright (C) 2026  CommandoMagia MikeCat2008

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. */
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define MAX_CODE_LENGHT 500

typedef struct
{
    int start;
    int end;
} loop;

int countLoops(const unsigned char code[]);
void setupLoops(const unsigned char code[], loop *loops,
    int loopsSize);
int getLoopEnd(loop loops[], int loopSize, int start);
int getLoopStart(loop loops[], int loopSize, int end);

int main(void)
{
    unsigned char buffer[BUFFER_SIZE] = {0};

    unsigned char cursor = 0;
    unsigned char code[MAX_CODE_LENGHT];

    loop *loops;
    int loopsSize;

    int iCode = 0;

    scanf("%499s", code);

    loopsSize = countLoops(code);
    if (loopsSize == -1){
        printf("ERROR: Invalid code\n");
        return 1;
    }
    loops = (loop*) malloc(loopsSize * sizeof(loop));
    setupLoops(code, loops, loopsSize);

    while (code[iCode] != '\0')
    {
        switch (code[iCode])
        {
            case '+':
                buffer[cursor]++;
                break;
            case '-':
                buffer[cursor]--;
                break;
            case '>':
                cursor++;
                break;
            case '<':
                cursor--;
                break;
            case '[':
                if (buffer[cursor] == 0)
                    iCode = getLoopEnd(loops, loopsSize, iCode);
                break;
            case ']':
                if (buffer[cursor] != 0)
                    iCode = getLoopStart(loops, loopsSize, iCode);
                break;
            case '.':
                printf("%c",buffer[cursor]);
                break;
            case ',':
                printf("in -> ");
                scanf(" %c", &buffer[cursor]);
                break;
        }
        iCode++;
    }
    printf("\n");

    return 0;
}

/*  Toma el codigo cargado en memoria (code) y devuelve el numero de
    bucles que contiene. En caso de que lo bucles esten mal definidos,
    devuelve -1 */
int countLoops(const unsigned char code[500])
{
    int iCode = 0;
    int iLoops = 0;
    int loopsCount = 0;

    while (code[iCode] != '\0')
    {
        if (code[iCode] == '[')
        {
            loopsCount++;
            iLoops++;
        }
        else if (code[iCode] == ']')
            iLoops--;

        if (iLoops < 0)
            return -1;

        iCode++;
    }
    if (iLoops != 0)
        return -1;

    return loopsCount;
}

/*  Toma el codigo cargado en memoria (code) y guarda las posiciones de
    los pares de corchetes en la lista loops.
    [!] El codigo ha de tener los bucles bien definidos, y se ha de
    conocer de antemano el numero de bulces (usar countLoops() para
    ambas condiciones) */
void setupLoops(const unsigned char code[500], loop *loops,
    int loopsSize)
{
    int iCode = 0;
    int iClosedLoops = 0;
    int iOpenLoops = loopsSize-1;

    while (iClosedLoops < loopsSize)
    {
        if (code[iCode] == '[')
        {
            loops[iOpenLoops].start = iCode;
            iOpenLoops--;
        }
        else if (code[iCode] == ']')
        {
            iOpenLoops++;
            loops[iOpenLoops].end = iCode;
            loops[iClosedLoops] = loops[iOpenLoops];
            iClosedLoops++;
            
        }
        iCode++;
    }
    return;
}

/*  A partir de la posicion de una apertura de corchete '[', devuelve la
    posicion de su respectivo cierre de corchete ']'.
    [!] Debe de ejecutarse antes setupLoops para conocer la estructura de
    bucles del codigo. */
int getLoopEnd(loop loops[], int loopSize, int start)
{
    int i = 0;
    int end = -1;

    while(end == -1 && i < loopSize)
    {
        if (loops[i].start == start)
            end = loops[i].end;
        i++;
    }
    return end;
}

/*  A partir de la posicion de un cierre de corchete ']', devuelve la
    posicion de su respectiva apertura de corchete '['.
    [!] Debe de ejecutarse antes setupLoops para conocer la estructura de
    bucles del codigo. */
int getLoopStart(loop loops[], int loopSize, int end)
{
    int i = 0;
    int start = -1;

    while(start == -1 && i < loopSize)
    {
        if (loops[i].end == end)
            start = loops[i].start;
        i++;
    }
    return start;
}
