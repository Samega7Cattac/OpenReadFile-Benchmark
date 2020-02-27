#include <iostream>
#include <vector>
#include <chrono>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
#include <sys/mman.h>
#include <string.h>

#define MAX_READ 255

void usage(char * call);

int main(int argc, char * argv[])
{
    if (argc != 2) usage(argv[0]);
    std::chrono::_V2::system_clock::time_point start;
    int max_read = MAX_READ;
    char str[max_read];
    unsigned int time = 0;
    
    start = std::chrono::high_resolution_clock::now();
    FILE * fd = fopen(argv[1], "rb");
    time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    std::cout << "[OPEN] fopen: " << std::fixed << time << " microsec" << std::endl;
    
    start = std::chrono::high_resolution_clock::now();
    while(fread(str, 1, max_read, fd))
        ;
    time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    std::cout << "[READ] fread: " << std::fixed << time << " microsec" << std::endl;
    
    rewind(fd);
    start = std::chrono::high_resolution_clock::now();
    while(fgets(str, max_read, fd))
        ;
    time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    std::cout << "[READ] fgets: " << std::fixed << time << " microsec" << std::endl;
    
    start = std::chrono::high_resolution_clock::now();
    fclose(fd);
    time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    std::cout << "[CLOSE] fclose: " << std::fixed << time << " microsec" << std::endl;
    
    start = std::chrono::high_resolution_clock::now();
    int fint = open(argv[1], O_RDONLY);
    time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    std::cout << "[OPEN] open: " << std::fixed << time << " microsec" << std::endl;
    
    start = std::chrono::high_resolution_clock::now();
    while(read(fint, str, max_read))
        ;
    time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    std::cout << "[READ] read: " << std::fixed << time << " microsec" << std::endl;
    
    unsigned long long int file_size = lseek64(fint, 0, SEEK_END);
    lseek64(fint, 0, SEEK_SET);
    int x = 0;
    start = std::chrono::high_resolution_clock::now();
    char * file_mmap = static_cast<char *>(mmap64(nullptr, file_size, PROT_READ, MAP_SHARED/*MAP_PRIVATE*/, fint, 0));
    for (unsigned long long int i = 0; i * max_read < file_size; ++i)
    {
        x = i * max_read;
        if (x > file_size) max_read = x - file_size;
        memcpy(str, file_mmap + i * max_read, max_read);
    }
    munmap(file_mmap, file_size);
    time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    std::cout << "[READ] memory mapping: " << std::fixed << time << " microsec" << std::endl;
    
    start = std::chrono::high_resolution_clock::now();
    close(fint);
    time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    std::cout << "[CLOSE] close: " << std::fixed << time << " microsec" << std::endl;
    
    start = std::chrono::high_resolution_clock::now();
    std::ifstream ff(argv[1], std::ios::in);
    time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    std::cout << "[OPEN] ifstream: " << std::fixed << time << " microsec" << std::endl;
    
    start = std::chrono::high_resolution_clock::now();
    while(ff >> str)
        ;
    time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    std::cout << "[READ] fstream >> operator: " << std::fixed << time << " microsec" << std::endl;
    
    start = std::chrono::high_resolution_clock::now();
    ff.close();
    time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    std::cout << "[CLOSE] fstream .close() method: " << std::fixed << time << " microsec" << std::endl;
    
    return 0;
}

void usage(char * call)
{
    std::cout << "USAGE:\n\t" << call << " <filename>" << std::endl;
    exit(0);
}
