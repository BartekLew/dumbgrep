#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

typedef void (*Action) (const char *file, const void *arg);
void walkin(Action act, void *param) {
    DIR *handle = opendir(".");
    if(handle == NULL) {
        char cwd[512];
        getcwd(cwd, 512);
        fprintf(stderr, "can't open directory: %s'\n", cwd);
        return;
    }

    struct dirent *node;
    while((node = readdir(handle)) != NULL) {
        if(strcmp(node->d_name, ".") == 0
            || strcmp(node->d_name, "..") == 0)
            continue;

        if(node->d_type == DT_DIR) {
            chdir(node->d_name);
            walkin(act, param);
            chdir("..");
        } else {
            act(node->d_name, param);
        }
    }
}

bool match_str(const char *str, const char *pattern) {
    const char *strc = str;
    for(const char *pattc = pattern; *pattc != 0; pattc++) {
        if(*pattc == '*') {
            pattc++;
            if(*pattc == 0) return true;

            while(*strc != 0 && *strc != *pattc) strc++;
            if(*strc == 0) return false;
            strc++;
            continue;
        } 

        if(*pattc != *strc) return false;
        strc++;
    }

    return true;
}

void match_file(const char *filename, const void *vpatt) {
    const char *pattern = vpatt;
    FILE *handle = fopen(filename, "r");
    if(handle == NULL) {
        char cwd[512];
        getcwd(cwd, 512);
        fprintf(stderr, "can't open file %s/%s.", cwd, filename);
    }

    char buff[512];
    while(fgets(buff, 512, handle)) {
        if(match_str(buff, pattern)) {
            printf("%s", buff);
        }
    }
}

int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "USAGE: %s <pattern>\n", argv[0]);
        return 1;
    }

    char *pattern = argv[1];
    walkin(&match_file, pattern);
}
