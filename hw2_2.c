#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int numOfLines;

int main(int argc, char * argv[]) {
    
    int opt, fd;
    char* filename = argv[argc-1];
    char *map;
    off_t fileSize;
    
    while((opt = getopt(argc, argv, "n:")) != -1) {
        switch(opt) {
            case 'n':
                numOfLines = atoi(optarg);
                break;
            case ':':
                printf("option needs a value\n"); break;
            case '?':
                printf("unknown option: %c\n", optopt); break;
        }
        
    }

    fd = open(filename, O_RDONLY);
    
    if(fd == -1)
    {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }
    
    fileSize = lseek(fd, 0, SEEK_END);
    
    map = mmap(0, fileSize, PROT_READ, MAP_SHARED, fd, 0);
    
    if(map == MAP_FAILED)
    {
        perror("Error mapping the file.");
        exit(EXIT_FAILURE);
    }
    
    int i=0;
    
    while (i < fileSize) {
        
        lseek(fd, 0, SEEK_SET);
        
        if(map[i] == '\n' && numOfLines > 0)
            --numOfLines;
        
        printf("%c", map[i]);
        
        if(numOfLines == 0) break;
        
        ++i;
    }
    
    if(munmap(map, fileSize) == -1)
    {
        perror("Error un-mapping the file");
    }
    
    close(fd);
    
    return 0;
}

