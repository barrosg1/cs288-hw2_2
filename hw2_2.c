#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int numOfLines;

int main(int argc, char * argv[]) {
    
    struct stat statBuff;
    int opt, fd;
    int lineCount = 0;
    char* filename = argv[argc-1];
    char *map;
    
    char c = *(filename);
    
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
    
    map = mmap(0, statBuff.st_size, PROT_READ, MAP_SHARED, fd, 0);
    
    if(map == MAP_FAILED)
    {
        perror("Error mapping the file.");
        exit(EXIT_FAILURE);
    }
    
    int i=0;
    while (i < statBuff.st_size) {
        c = map[i];
        
        if(c == '\n') ++lineCount;
        
        if(lineCount == numOfLines) break;
        
        printf("%c", c);
        i++;
    }
    
    if(munmap(map, statBuff.st_size) == -1)
    {
        perror("Error un-mapping the file");
        
    }
    
    close(fd);
    
    return 0;
}

