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

#define BUFFER_SIZE 30000

int goToLoopStart(unsigned char *code, int iCode);
int goToLoopEnd(unsigned char *code, int iCode);

int main(int argc, char *argv[])
{
    FILE *f_in;
    unsigned char *code;
    long codeSize = 0;
    int iCode = 0;

    unsigned char buffer[BUFFER_SIZE] = {0};
    unsigned int cursor = 0;

    //1. Cargar el codigo en memoria

    if (argc == 1)
    {
        printf("ERROR: No arguments have been passed\n");
        return 1;
    }

    f_in = fopen(argv[1], "rb");
    if (f_in == NULL)
    {
        printf("ERROR: Couldn't open file '%s'\n", argv[1]);
        return 1;
    }
    fseek(f_in, 0, SEEK_END);
    codeSize = ftell(f_in) * sizeof(char);
    rewind(f_in);
    code = (unsigned char*) malloc(codeSize);
    if (code == NULL)
    {
        printf("ERROR(Internal): Couldn't allocate the '%ld' bytes \
            required to load into memory the code in '%s'\n",
            codeSize, argv[1]);
        return 1;
    }
    if (fread(code, codeSize, 1, f_in) != 1)
    {
        printf("ERROR(Internal): Couldn't load into memory the code in \
            '%s'\n", argv[1]);
        return 1;
    }
    fclose(f_in);

    //2. Ejecucion del programa

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
                if (cursor < BUFFER_SIZE - 1)
                    cursor++;
                break;
            case '<':
                if (cursor > 0)
                    cursor--;
                break;
            case '[':
                if (buffer[cursor] == 0)
                    iCode = goToLoopEnd(code,iCode);
                break;
            case ']':
                if (buffer[cursor] != 0)
                    iCode = goToLoopStart(code,iCode);
                break;
            case '.':
                printf("%c",buffer[cursor]);
                break;
            case ',':
                printf("in -> ");
                scanf(" %c", &buffer[cursor]);
                break;
        }
        if (iCode == -1)
        {
            printf("ERROR: Invalid code\n");
            return -1;
        }

        iCode++;
    }

    printf("\n");
    free(code);

    return 0;
}

/*
    Mueve el cursor del codigo (iCode) al '[' correspondiente al ']' sobre
    el que se encuentra iCode
    [!] iCursor debe encontrarse sobre un ']'*/
int goToLoopStart(unsigned char *code, int iCode){
    int loopDepth = 1;
    int newICode = iCode;
    while (loopDepth > 0)
    {
        newICode--;
        if (newICode < 0)
            return -1;

        if (code[newICode] == ']')
            loopDepth++;
        else if (code[newICode] == '[')
            loopDepth--;
    }
    return newICode;
}

/*
    Mueve el cursor del codigo (iCode) al ']' correspondiente al '[' sobre
    el que se encuentra iCode
    [!] iCursor debe encontrarse sobre un '['*/
int goToLoopEnd(unsigned char *code, int iCode){
    int loopDepth = 1;
    int newICode = iCode;
    while (loopDepth > 0)
    {
        newICode++;
        if (newICode > BUFFER_SIZE - 1)
            return -1;

        if (code[newICode] == ']')
            loopDepth--;
        else if (code[newICode] == '[')
            loopDepth++;
    }
    return newICode;
}