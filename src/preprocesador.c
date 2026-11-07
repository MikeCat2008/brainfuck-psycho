/*  preprocesador.c
    Analiza el codigo de entrada e imprime unicamente las instrucciones de
    BrainFuck encontradas por STDOUT, o si se especifica, a un archivo. La
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
#include <string.h>

/* cliparser() */
/* Flags */
/* Especificar archivo de salida */
#define FLAG_O1 "-o"
#define FLAG_O2 "--output"
/* Codigos Errores */
#define E_TMI 1 /*Error: Too many inputs*/
#define E_TMO 2 /*Error: Too many outputs*/
#define E_NOA 3 /*Error: No output argument*/

/* Argument Position */
typedef struct
{
    int input_file;
    int output_file;
} ArgPos;

int cliparser(int argc, char *argv[], ArgPos *argp);
int isbfkey(char key);

int main(int argc, char *argv[])
{
    ArgPos argp = {-1, -1};
    int cliparser_code = 0;

    FILE *f_in;
    FILE *f_out;

    char current_char;

    if (argc == 1)
    {
        printf("ERROR: No arguments have been passed\n");
        return 1;
    }
    cliparser_code = cliparser(argc, argv, &argp);
    switch (cliparser_code) {
        case 0:
            break;
        case E_TMI:
            printf("ERROR: Too many inputs\n");
            return 1;
        case E_TMO:
            printf("ERROR: Too many outputs\n");
            return 1;
        case E_NOA:
            printf("ERROR: No output argument after flag\n");
            return 1;
        default:
            printf("ERROR(Internal): Not expected parser code\n");
            return 1;
    }

    if (argp.input_file == -1)
    {
        printf("ERROR: No input file specified\n");
        return 1;
    }
    f_in = fopen(argv[argp.input_file], "rb");
    if (f_in == NULL)
    {
        printf("ERROR: Couldn't open file '%s'\n", argv[argp.input_file]);
        return 1;
    }
    if (argp.output_file == -1)
    {
        f_out = stdout;
    }
    else
    {
        f_out = fopen(argv[argp.output_file], "w");
        if (f_out == NULL)
        {
            fclose(f_in);
            printf("ERROR: Couldn't create file '%s'\n", argv[argp.output_file]);
            return 1;
        }
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

/*  Analiza la entrada por linea de comando a la hora de invocar el
    programa (argv[]), para encontrar los argumentos se necesitan.
    La posicion de los argumentos en argv[] se indican en el registro
    argp de tipo ArgPos. Si no se ha encontrado uno de los argumentos,
    su respectivo campo en argp mantendra su valor por defecto de -1.
    La funcion devuelve 0 si la entrada es valida, o el respectivo
    codigo de error si hay algo incorrecto en la sintaxis de los
    argumentos. */
int cliparser(int argc, char *argv[], ArgPos *argp)
{
    int i;
    int inFlag = 0;
    int outFlag = 0;

    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], FLAG_O1) || !strcmp(argv[i], FLAG_O2))
        {
            if ((i + 1) >= argc)
                return E_NOA;
            if (outFlag || !strcmp(argv[i + 1], FLAG_O1) ||
                !strcmp(argv[i + 1], FLAG_O2))
                return E_TMO;
            (*argp).output_file = ++i;
            outFlag = 1;
        }
        else
        {
            if (inFlag)
                return E_TMI;
            (*argp).input_file = i;
            inFlag = 1;
        }
    }
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
