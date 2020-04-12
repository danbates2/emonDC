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
    /*
    int slash_pos;
    int dot_pos;
    for (int i = 0; i < sizeof(fname); ++i) {
        if (fname[i] == '/') slash_pos = i; slash_pos++; printf("slash:%d\n", slash_pos);
    }
    char *name = fname; name += slash_pos; 
    printf("name:%d\n", sizeof(name));// return 1;
    for (int i = 0; i < sizeof(name); ++i) {
        if (name[i] == '.') dot_pos = i; dot_pos++; printf("dot:%d\n\n", dot_pos);
    }
    name[strlen(name)-dot_pos] = 0;
    */
    
    //return 1;
    printf("/* Embedded file: %s */\n", fname);
    printf("const long fsize_%s = %ld;\n", fname, fsize);
    printf("static unsigned char *%s[%ld] PROGMEM = {\n", fname,fsize);

    
    for (int i = 0; i < fsize; ++i) {
        printf("0x%02x%s",
                b[i],
                i == fsize-1 ? "" : ((i+1) % 16 == 0 ? ",\n" : ","));
    }
    printf("\n};\n");

    free(b);
    return 0;
}