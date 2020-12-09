
### 库是否被真实链接

```c
yzg@yzg-VirtualBox:~/mygit/board_test/code-c/compile$ cat v1.c
int func(int v)
{
   return v+1;
}

yzg@yzg-VirtualBox:~/mygit/board_test/code-c/compile$ cat v2.c
int func(int v)
{
   return v+2;
}

yzg@yzg-VirtualBox:~/mygit/board_test/code-c/compile$ cat main.c
#include<stdio.h>
extern int func(int);

int main()
{
  printf("version=%d\n", func(0));
}
```

1. v1.c 与 v2.c 定义的函数一致，导致第二个库实际上没有链接进去。
编译不报错

```
Makefile:
all:
	gcc -c v1.c
	gcc -c v2.c
	ar crv libv1.a v1.o
	ar crv libv2.a v2.o
	gcc -o main_test_v1 main.c libv1.a libv2.a
	gcc -o main_test_v2 main.c libv2.a libv1.a

yzg@yzg-VirtualBox:~/mygit/board_test/code-c/compile$ ./main_test_v1
version=1
yzg@yzg-VirtualBox:~/mygit/board_test/code-c/compile$ ./main_test_v2
version=2
```

2. 如果直接一起编译，会提示重复定义。函数定义默认为强符号
```
yzg@yzg-VirtualBox:~/mygit/board_test/code-c/compile$ gcc -o t v1.c v2.c main.c
/tmp/ccThhIDK.o: In function `func':
v2.c:(.text+0x0): multiple definition of `func'
/tmp/ccoZ86WN.o:v1.c:(.text+0x0): first defined here
collect2: error: ld returned 1 exit status
```

3.调用v2库里面的函数，保证v2一定会被链接。
gcc -o main_test_v1 main.c libv1.a libv2.a 提示func重定义。
gcc -o main_test_v2 main.c libv2.a libv1.a 成功，实际v1被忽略。
```
v1.c不变
v2.c修改为：
int func(int v)
{
   return v+2;
}

int test(int t)
{
 return t + 100;
}

#include<stdio.h>

extern int func(int);
extern int test(int t);

int main()
{
   test(100);
   printf("version=%d\n", func(0));
}

```


### 弱符号

1. 未初始化的全局变量为弱符号, 同名弱符号，正常链接。

```
yzg@yzg-VirtualBox:~/mygit/board_test/code-c/compile/v2$ cat v1.c
//not initialized, it is weak symbol.
int var_g;

int func(int v)
{
   return v + var_g ;
}

yzg@yzg-VirtualBox:~/mygit/board_test/code-c/compile/v2$ cat v2.c
//not initialized, it is weak symbol.
int var_g;

int func(int v)
{
   return v + var_g + 1;
}

yzg@yzg-VirtualBox:~/mygit/board_test/code-c/compile/v2$ cat main.c
#include<stdio.h>

extern int var_g;
extern int func(int v);

int main()
{
   var_g = 1;
   printf("version=%d\n", func(0));
}

yzg@yzg-VirtualBox:~/mygit/board_test/code-c/compile/v2$ ./main_test_v1
version=1
yzg@yzg-VirtualBox:~/mygit/board_test/code-c/compile/v2$ ./main_test_v2
version=2

```


2. 同名弱符号，最终占用的空间为同名弱符号中的最大值。
```
////////////// v1.c
int a;
//not initialized, it is weak symbol.
int var_g;
int func1(int v)
{
   printf("v1.c : sizeof(var_g)=%lu  addr of var_g:%p   &a:%p\n", sizeof(var_g), &var_g, &a);
   return v + var_g ;
}

void set_varg(int g)
{
   var_g = g ;
}


/////////////////////// v2.c
int b;
//not initialized, it is weak symbol.
long long var_g;

int func2(int v)
{
   printf("v2.c : sizeof(var_g)=%lu  addr of var_g:%p  &b:%p\n", sizeof(var_g), &var_g, &b);
   return v + var_g + 1;
}

////////////////  main.c
#include<stdio.h>
extern int var_g;
void set_varg(int g);
extern int func1(int v);
extern int func2(int v);

int main()
{
    set_varg(1);
    func1(1);
    func2(1);
    printf("main.c: addr of var_g:%p\n", &var_g);
    printf("sizeof(var_g)=%lu ", sizeof(var_g));
}
```
位于COMMON区：
yzg@yzg-VirtualBox:~/mygit/board_test/code-c/compile/v2$ readelf -s v1.o
Symbol table '.symtab' contains 15 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
    10: 0000000000000004     4 OBJECT  GLOBAL DEFAULT  COM var_g

yzg@yzg-VirtualBox:~/mygit/board_test/code-c/compile/v2$ readelf -s v2.o
Symbol table '.symtab' contains 14 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
    10: 0000000000000008     8 OBJECT  GLOBAL DEFAULT  COM var_g

yzg@yzg-VirtualBox:~/mygit/board_test/code-c/compile/v2$ readelf -s main_test_v1
   61: 0000000000201020     8 OBJECT  GLOBAL DEFAULT   24 var_g


3. 已初始化的全局变量为强符号，如果弱符号的size大于强符号，链接器警告。
usr/bin/ld: Warning: alignment 4 of symbol `var_g' in libv2.a(v2.o) is smaller than 8 in libv1.a(v1.o)
/usr/bin/ld: Warning: size of symbol `var_g' changed from 8 in libv1.a(v1.o) to 4 in libv2.a(v2.o)

4. 同时存在弱符号和强符号，最终使用强符号（如：初值）。
