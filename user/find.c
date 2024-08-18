#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

/*
	系统文件名与要查找的文件名，若一致，打印系统文件完整路径
*/
void eq_print(char *fileName, char *findName){
	if(strcmp(fmtname(fileName), findName) == 0){
		printf("%s\n", fileName);
	}
}

void find(char *path, char *target)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    // 如果文件名结尾匹配 `/target`，则视为匹配
    // 可使用eq_print函数
    if(strcmp(path+strlen(path)-strlen(target),target)==0){

        printf("%s\n",path);
    }
    // printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      // 不要进入 `.` 和 `..`

      if(strcmp(buf+strlen(buf)-2,"/.")!=0 && strcmp(buf+strlen(buf)-3, "/..") != 0){
        find(buf, target);
      }

    //   printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{


  if(argc < 3){
    printf("please input name");
    exit(0);
  }
  char target[512];
  target[0]='/';
  strcpy(target+1,argv[2]);
  find(argv[1],target);
  exit(0);


//   for(i=1; i<argc; i++)
//     find(argv[i]);
//   exit(0);
}
