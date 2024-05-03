#include <stdio.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <unistd.h>


bool get_var(const char* path,char*** buff) {
    FILE* file=NULL;
    if((file=fopen(path,"r"))!=NULL) {
        size_t ind=0;
        size_t cnt=7;
        while(cnt--) {
            (*buff)[ind]=(char*)realloc((*buff)[ind],(280)*sizeof(char));
            fscanf(file,"%s",(*buff)[ind++]);
        }
        fclose(file);
        return true;
    }
    perror("File error");
    return false;
    }

bool find_child_env(char** envp,const char* name_var) {
    size_t ind=0;
    size_t len=strlen(name_var);
    bool flag_find=false;
    while(envp[ind]) {
        flag_find=true;
        for(size_t i=0;i<len;++i) {
            if(envp[ind][i]!=name_var[i]) {
                flag_find=false;
                break;
            }
        }
        if(flag_find ==true)
            break;
        ind++;
    }
    if(flag_find==false) return false;
    size_t path_len = strlen(name_var);
    size_t find_str_len=strlen(envp[ind]);
    for(size_t i = path_len+1;i<find_str_len;++i) {
        printf("%c",envp[ind][i]);
    }
    printf("\n");
    return true;
}

int main(int argc,char* argv[],char* envp[]) {
    printf("NAME PROGRAM:%s\n", argv[0]);
    printf("PID PROGRAM %d\n",getpid());
    printf("PPID PROGRAM:%d\n",getppid());
    char** array_var=(char**)calloc(7,sizeof(char*));
    if(get_var(argv[1],&array_var)) {
        if(strcmp(argv[2],"+")==0) {
            for(size_t i=0;i<7;++i) {
                printf("%s\n",getenv(array_var[i]));
            }
        }
        else if(strcmp(argv[2],"*")==0) {
            for(size_t i=0;i<7;++i)
                find_child_env(envp,array_var[i]);
        }
        else if (strcmp(argv[2],"&")==0) {
            for(size_t i=0;i<7;++i)
                find_child_env(__environ,array_var[i]);
        }
    }
    for(size_t i=0;i<7;++i)
        free(array_var[i]);
    free(array_var);
    return 0;
}

