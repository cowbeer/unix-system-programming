#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main() {
    FILE* fp = fopen("stdio.dat", "wb");
    unsigned int i;
    for (i = 0; i< 1000000; i++) {
        fwrite(&i, sizeof(i), 1, fp);
    }
    
    fclose(fp);

    return 0;
}
