#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <search.h>

const int max_size = 10000; // Размер

void prefix(int* pr, int n, char* string) {
	pr[0] = 0;
	for (int i = 1; i < n; ++i) {
		int j = pr[i - 1];
		while (j > 0 && string[i] != string[j]) {
			j = pr[j - 1];
		}
		if (string[i] == string[j]) {
			j++;
		}
		pr[i] = j;
	}
}

int read_file(int* fd, char* name, char* string) {
	if ((fd[0] = open(name, O_RDONLY)) < 0) {
		printf("Can\'t open input file\n");
		return -1;
	}
    if((fd[1] = open("read", O_WRONLY)) < 0){
        printf("Can\'t open FIFO for writting\n");
        exit(-1);
    }
    
    int read_bytes;
    char buf[20];
    do { // Считываем и записываем циклически
        read_bytes = read(fd[0], buf, 16);
        if (read_bytes == -1) {
            printf("Can\'t read \"%s\"\n", name);
            exit(-1);
        }
        buf[read_bytes] = '\0';
		write(fd[1], buf, read_bytes);
    } while (read_bytes == 16);
	close(fd[1]);
	return 0;
}

int processing(int* fd_read, char* string, char* res) {
    if((fd_read[1] = open("read", O_RDONLY)) < 0){
        printf("Can\'t open FIFO for reading\n");
        return -1;
    }
    char buf[20];
    int str_size = 0, read_bytes;
    do { // Считываем и записываем циклически
        read_bytes = read(fd_read[1], buf, 16);
        if (read_bytes == -1) {
            printf("Can\'t read from \"read\"-chanell\n");
            exit(-1);
        }
        buf[read_bytes] = '\0';
		for (int i = 0; i < read_bytes; ++i, ++str_size) {
			string[str_size] = buf[i];
		}
		
    } while (read_bytes == 16);
    close(fd_read[1]);
    
    int pr[str_size + 1];
    prefix(pr, str_size, string);
    int len = 0;
    while (string[len] != '#') {
    	len++;
    }
    
    int j = 0;
    for (int i = 0; i < str_size; ++i) {
    	if (pr[i] == len) {
    		char cur[20];
    		sprintf(cur, "%d\n", i - 2 * len);
    		int now = 0;
    		while (cur[now] != '\0') {
    			res[j] = cur[now];
    			j++;
    			now++;
    		}
    	}
    }
    return j;
}

int write_file(int* fd, char* name, char* buffer) {
    if ((fd[0] = open(name, O_WRONLY)) < 0) {
        printf("Can\'t open output file\n");
        return -1;
    }
    if((fd[1] = open("write", O_RDONLY)) < 0){
        printf("Can\'t open FIFO for reading\n");
        return -1;
    }
    
    int read_bytes;
    do { // Считываем и записываем циклически
        int read_bytes = read(fd[1], buffer, 16);
        if (read_bytes == -1) {
            printf("Can\'t read \"%s\"\n", name);
            exit(-1);
        }
        buffer[read_bytes] = '\0';
		if (write(fd[0], buffer, read_bytes) == -1) {
		    printf("Can\'t write\n");
		    return -1;
		}
    } while (read_bytes == 16);
    close(fd[1]);
    return 0;	
}

int main(int argc, char* argv[]) {
	char string[max_size + 1]; // Строка
	char res[max_size + 1]; // Результат
	char buffer[max_size +1]; // Строка для вывода

    int fd_read[2];
    int fd_write[2];
    
    int j;
    mknod("read", S_IFIFO | 0666, 0);
    mknod("write", S_IFIFO | 0666, 0);
    pid_t pid;
    
    switch(pid=fork()) {
        case -1:
            perror("fork"); // ошибка
            return 1;
        case 0:
            int read_er = read_file(fd_read, argv[1], string);
            if (read_er != 0) {
            	return read_er;
            }
            int write_er = write_file(fd_write, argv[2], buffer);
            if (write_er != 0) {
            	return write_er;
            }
            return 0;
        default:
        	switch(pid=fork()) {
        	    case -1:
				    perror("fork"); // ошибка
				    return 1;
				case 0:
				    j = processing(fd_read, string, res);
				    if (j == -1) 
				    	return j;
				    if((fd_write[1] = open("write", O_WRONLY)) < 0){
				        printf("Can\'t open FIFO for writting\n");
				        exit(-1);
				    }
				    
				    int i = 0;
				    while (i < j) {
				    	char buff[20];
				    	int count = 16;
				    	if (count > j - i) {
				    		count = j - i;
				    	}
				    	for (int q = 0; q < count; ++q) {
				    		buff[q] = res[i + q];
				    	}
				    	i += count;
				    	if (write(fd_write[1], buff, count) == -1) {
							printf("Can\'t write\n");
							return -1;
						}
				    }
				    close(fd_write[1]);
            }
    }
    return 0;
}
