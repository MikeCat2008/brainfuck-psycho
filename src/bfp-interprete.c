#include <stdio.h>

#define MAX_LOOPS 20

typedef struct {
    int start;
    int end;
} loop;

void setupCode(unsigned char code[500], loop loops[MAX_LOOPS], int *loopsSize){
    int iCode = 0;
    int iLoops = 0;
    *loopsSize = 0;

    while (
        iLoops > -1 && iLoops < MAX_LOOPS &&
        code[iCode] != '\0'
    ){
        if (code[iCode] == '['){
            loops[iLoops].start = iCode;
            iLoops++;
            *loopsSize += 1;
        }
        else if (code[iCode] == ']' && iLoops > 0){
            iLoops--;
            loops[iLoops].end = iCode;
        }
        iCode++;
    }
}

int getLoopEnd(loop loops[], int loopSize, int start){
    int i = 0;
    int end = -1;
    while(end == -1 && i < loopSize){
        if (loops[i].start == start)
            end = loops[i].end;
        i++;
    }
    return end;
}
int getLoopStart(loop loops[], int loopSize, int end){
    int i = 0;
    int start = -1;
    while(start == -1 && i < loopSize){
        if (loops[i].end == end)
            start = loops[i].start;
        i++;
    }
    return start;
}

int main(){
    unsigned char buffer[256] = {0};

    unsigned char cursor = 0;
    unsigned char code[500];

    int codeSize;
    loop loops[MAX_LOOPS];
    int loopsSize = 0;
    
    scanf("%500s",code);
    setupCode(code,loops,&loopsSize);
    
    int iCode = 0;
    while (code[iCode] != '\0'){
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
                iCode = getLoopEnd(loops,loopsSize,iCode);
            break;
        case ']':
            if (buffer[cursor] != 0)
                iCode = getLoopStart(loops,loopsSize,iCode);
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
    puts("");

    return 0;
}
