#include <stdio.h>
#include <stdlib.h>

void print_env(void) {
    printf("---- ENV Vars ----\n");
    extern char** environ;
    char** env;
    for (env = environ; env && *env; ++env) {
        printf("%s\n", *env);
    }
    printf("------------------\n");
}

int main(void){
    char env[256];
    const char* name = "MYNAME";
    // add
    sprintf(env, "%s=cdd", name);
    putenv(env);
    printf("%s=%s\n", name, getenv(name));
    // modify
    sprintf(env, "%s=bdd", name);
    putenv(env);
    printf("%s=%s\n", name, getenv(name));
    
    setenv(name, "xdd", 0);
    printf("%s=%s\n", name, getenv(name));
    setenv(name, "xdd", 1);
    printf("%s=%s\n", name, getenv(name));
    
    //delete
    unsetenv(name);
    printf("%s=%s\n", name, getenv(name));

    //print_env();
    
    return 0;
}
