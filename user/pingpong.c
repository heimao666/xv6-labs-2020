#include <kernel/types.h>
#include <user/user.h>

int main(){
    // pipe1(p1)；写端父进程，读端子进程
    //pipe2(p2)； 写端子进程，读端父进程
// 创建管道会得到一个长度为 2 的 int 数组
	// 其中 0 为用于从管道读取数据的文件描述符，1 为用于向管道写入数据的文件描述符
	int pp2c[2], pc2p[2];
	pipe(pp2c); // 创建用于 父进程 -> 子进程 的管道
	pipe(pc2p); // 创建用于 子进程 -> 父进程 的管道
	
	if(fork() != 0) { // parent process
		write(pp2c[1], "!", 1); // 1. 父进程首先向发出该字节
		char buf;
		read(pc2p[0], &buf, 1); // 2. 父进程发送完成后，开始等待子进程的回复
		printf("%d: received pong\n", getpid()); // 5. 子进程收到数据，read 返回，输出 pong
		wait(0);
	} else { // child process
		char buf;
		read(pp2c[0], &buf, 1); // 3. 子进程读取管道，收到父进程发送的字节数据
		printf("%d: received ping\n", getpid());
		write(pc2p[1], &buf, 1); // 4. 子进程通过 子->父 管道，将字节送回父进程
	}
	exit(0);
    
   /*  //pipe1(p1)：写端父进程，读端子进程
    //pipe2(p2)；写端子进程，读端父进程
    int p1[2],p2[2];
    //来回传输的字符数组：一个字节
    char buffer[] = {'X'};
    //传输字符数组的长度
    long length = sizeof(buffer);
    //父进程写，子进程读的pipe
    pipe(p1);
    //子进程写，父进程读的pipe
    pipe(p2);
    //子进程
    if(fork() == 0){
        //关掉不用的p1[1]、p2[0]
        close(p1[1]);
        close(p2[0]);
		//子进程从pipe1的读端，读取字符数组
		if(read(p1[0], buffer, length) != length){
			printf("a--->b read error!");
			exit(1);
		}
		//打印读取到的字符数组
		printf("%d: received ping\n", getpid());
		//子进程向pipe2的写端，写入字符数组
		if(write(p2[1], buffer, length) != length){
			printf("a<---b write error!");
			exit(1);
		}
        exit(0);
    }
    //关掉不用的p1[0]、p2[1]
    close(p1[0]);
    close(p2[1]);
	//父进程向pipe1的写端，写入字符数组
	if(write(p1[1], buffer, length) != length){
		printf("a--->b write error!");
		exit(1);
	}
	//父进程从pipe2的读端，读取字符数组
	if(read(p2[0], buffer, length) != length){
		printf("a<---b read error!");
		exit(1);
	}
	//打印读取的字符数组
	printf("%d: received pong\n", getpid());
    //等待进程子退出
    wait(0);
	exit(0); */

}