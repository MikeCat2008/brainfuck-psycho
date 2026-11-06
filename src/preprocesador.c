/*  preprocesador.c
    Analiza el codigo de entrada y guarda en un archivo de salida
    unicamente las instrucciones de BrainFuck encontradas. El archivo de
    salida tiene como ultimo byte al caracter '\0' para que el interprete
    pueda procesar el codigo como una string.

    Copyright (C) 2026  MikeCat2008

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

#define INPUT_FILE "script.bf"
#define OUTPUT_FILE "script.bf.i"

int isbfkey(char key);

int main(void)
{
    FILE *f_in;
    FILE *f_out;
    char current_char;

    f_in = fopen(INPUT_FILE, "rb");
    if (f_in == NULL)
    {
        printf("ERROR: Couldn't open file '%s'\n", INPUT_FILE);
        return 1;
    }
    f_out = fopen(OUTPUT_FILE, "w");
    if (f_out == NULL)
    {
        fclose(f_in);
        printf("ERROR: Couldn't create file '%s'\n", OUTPUT_FILE);
        return 1;
    }

    while (fread(&current_char, sizeof(current_char), 1, f_in) == 1)
    {
        if (isbfkey(current_char))
            fprintf(f_out, "%c", current_char);
    }
    fprintf(f_out, "%c", '\0');

    fclose(f_in);
    fclose(f_out);
    return 0;
}

/*  Comprueba si el caracter es una instruccion de BrainFuck. */
int isbfkey(char key)
{
    switch (key) {
        case '+':
        case '-':
        case '>':
        case '<':
        case '.':
        case ',':
        case '[':
        case ']':
            return 1;
        default:
            return 0;
    }
}
