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

#define MAX_LOOPS 20

typedef struct
{
    int start;
    int end;
} loop;

void setupLoops(const unsigned char code[500], loop loops[MAX_LOOPS],
    int *loopsSize);
int getLoopEnd(loop loops[], int loopSize, int start);
int getLoopStart(loop loops[], int loopSize, int end);

int main(void)
{
    unsigned char buffer[256] = {0};

    unsigned char cursor = 0;
    unsigned char code[500];

    loop loops[MAX_LOOPS];
    int loopsSize = 0;

    int iCode = 0;

    scanf("%500s", code);
    setupLoops(code, loops, &loopsSize);

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

/*  Toma el codigo cargado en memoria (code) y guarda las posiciones de
    los pares de corchetes en la lista loops. */
void setupLoops(const unsigned char code[500], loop loops[MAX_LOOPS],
    int *loopsSize)
{
    int iCode = 0;
    int iLoops = 0;
    *loopsSize = 0;

    while (
        iLoops > -1 && iLoops < MAX_LOOPS &&
        code[iCode] != '\0'
    )
    {
        if (code[iCode] == '[')
        {
            loops[iLoops].start = iCode;
            iLoops++;
            *loopsSize += 1;
        }
        else if (code[iCode] == ']' && iLoops > 0)
        {
            iLoops--;
            loops[iLoops].end = iCode;
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
