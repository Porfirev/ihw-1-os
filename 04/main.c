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
	int str_size = read(fd[0], string, max_size); 
	if (str_size == -1) {
		printf("Can\'t read input file\n");
		return -1;
	}
	str_size = write(fd[1], string, str_size);
	close(fd[0]);
	close(fd[1]);
	return 0;
}

int processing(int* fd_read, char* string, char* res) {
    int str_size = read(fd_read[0], string, max_size);
    close(fd_read[0]);
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
    if ((fd[1] = open(name, O_WRONLY)) < 0) {
        printf("Can\'t open output file\n");
        return -1;
    }
    int size = read(fd[0], buffer, max_size);
    if (write(fd[1], buffer, size) == -1) {
        printf("Can\'t read input file\n");
        return -1;
    }
    close(fd[0]);
    close(fd[1]);
    return 0;	
}

int main(int argc, char* argv[]) {
	char string[max_size + 1]; // Строка
	char res[max_size + 1]; // Результат
	char buffer[max_size +1]; // Строка для вывода

    int fd_read[2];
    int fd_write[2];
    
    pipe(fd_read);
    pipe(fd_write);
    pid_t pid;
    
    int j;
    switch(pid=fork()) {
        case -1:
            perror("fork"); // ошибка
            return 1;
        case 0:
            return read_file(fd_read, argv[1], string);
        default:
            j = processing(fd_read, string, res);
    }
    
    switch(pid=fork()) {
        case -1:
            perror("fork"); // ошибка
            return 1;
        case 0:
            return write_file(fd_write, argv[2], buffer);
        default:
        	write(fd_write[1], res, j);
			close(fd_write[0]);
			close(fd_write[1]);
    }
    return 0;
}
