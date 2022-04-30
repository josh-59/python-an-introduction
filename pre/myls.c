#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main() {
    struct dirent* dir_entry;
    DIR* dirp;
    
    if( (dirp = opendir(".")) == NULL ) {
        fprintf(stderr, "Could not open directory!\n");
        return 1;
    }

    while( (dir_entry = readdir(dirp)) ) {
        printf("%s\n", dir_entry->d_name);
    }

    return 0;
}
