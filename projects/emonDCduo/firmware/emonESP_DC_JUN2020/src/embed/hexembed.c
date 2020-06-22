/* hexembed.c - copyright Lewis Van Winkle, modified by Daniel Bates */
/* zlib license */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {printf("Usage:\n\thexembed <filename>\n"); return 1;}

    const char *fname = argv[1];
    FILE *fp = fopen(fname, "rb");
    if (!fp) {
        fprintf(stderr, "Error opening file: %s.\n", fname);
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    const long fsize = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    unsigned char *b = malloc(fsize);

    fread(b, fsize, 1, fp);
    fclose(fp);
    
    printf("/* Embedded file: %s */\n", fname);
    //printf("const long fsize_%s = %ld;\n", fname, fsize);
    
    char tmp[300];
    char *tmpptr = tmp;
    char *tmppt = tmp;
    int endSlashPosition = 0;
    int nullLocation = 0;
    memcpy(tmp, fname, 300);
    
    for (int i = 0 ;i < 300;i++) {
        if (tmp[i] == NULL) {
            nullLocation = i;
            break;
        }
    }    
    //printf("nullLocation = %d\r", nullLocation);

    for (int i = 0 ;i < nullLocation;i++) {
        if (tmp[i] == '/') {
            endSlashPosition = i;
            //break;
        }
    }    
    //printf("endSlashPosition = %d\r", endSlashPosition);

    int filenameLength = nullLocation - endSlashPosition;

    char tmp2[30];
    char *tmp2ptr = tmp + endSlashPosition+1;

    memcpy(tmp2, tmp2ptr, filenameLength);
    //printf("filenameLength = %d\r", filenameLength);
    
    for (int i = 0;i < filenameLength;i++) {
        if (tmp2[i] == '.') {
            tmp2[i] = '_';
            break;
        }
    }


    printf("static const char %s[%ld] PROGMEM = {\n", tmp2,fsize);

    for (int i = 0; i < fsize; ++i) {
        printf("0x%02x%s",
                b[i],
                i == fsize-1 ? "" : ((i+1) % 16 == 0 ? ",\n" : ","));
    }
    printf("\n};\n");

    free(b);
    return 0;
}