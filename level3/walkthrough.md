- attack: format string exploitation
- binary behavior: reprints argument, if global argument = 64 run he shell
- targeted functions: v and main
- vulnerability in v: printf()
- defense: fget

Method:
- find the buffer adress
- find the return address used by strdup
- overwrite eip by the return address
- call shell using shellcode
- cat passwd when get console with root privedges
 
Info:
- 0x804988c - global variable
- 8c980408 - same in little-endian
- same in hex - \x8c\x98\x04\x08

- eip register address after segfault - 0x37634136
- its offset - 80 (from wiremask.eu)
- offset code structure: 80 = 4 (return address) + 52 (shell code) + 28


Code:
objdump -d ./level3
ltrace ./level3

cd ..; cat level4/.pass


Functions

0804851a <main>:
 804851a:       55                      push   %ebp
 804851b:       89 e5                   mov    %esp,%ebp
 804851d:       83 e4 f0                and    $0xfffffff0,%esp
 8048520:       e8 7f ff ff ff          call   80484a4 <v>
 8048525:       c9                      leave
 8048526:       c3                      ret



080484a4 <v>:
 80484a4:       55                      push   %ebp
 80484a5:       89 e5                   mov    %esp,%ebp
 80484a7:       81 ec 18 02 00 00       sub    $0x218,%esp
 80484ad:       a1 60 98 04 08          mov    0x8049860,%eax
 80484b2:       89 44 24 08             mov    %eax,0x8(%esp)
 80484b6:       c7 44 24 04 00 02 00    movl   $0x200,0x4(%esp)
 80484bd:       00
 80484be:       8d 85 f8 fd ff ff       lea    -0x208(%ebp),%eax
 80484c4:       89 04 24                mov    %eax,(%esp)
 80484c7:       e8 d4 fe ff ff          call   80483a0 <fgets@plt> #defence
 80484cc:       8d 85 f8 fd ff ff       lea    -0x208(%ebp),%eax
 80484d2:       89 04 24                mov    %eax,(%esp)
 80484d5:       e8 b6 fe ff ff          call   8048390 <printf@plt> #exploit for attack
 80484da:       a1 8c 98 04 08          mov    0x804988c,%eax 
 80484df:       83 f8 40                cmp    $0x40,%eax # compaison of eax with 64
 80484e2:       75 34                   jne    8048518 <v+0x74> # if equal to 0 
 80484e4:       a1 80 98 04 08          mov    0x8049880,%eax
 80484e9:       89 c2                   mov    %eax,%edx
 80484eb:       b8 00 86 04 08          mov    $0x8048600,%eax
 80484f0:       89 54 24 0c             mov    %edx,0xc(%esp)
 80484f4:       c7 44 24 08 0c 00 00    movl   $0xc,0x8(%esp)
 80484fb:       00
 80484fc:       c7 44 24 04 01 00 00    movl   $0x1,0x4(%esp)
 8048503:       00
 8048504:       89 04 24                mov    %eax,(%esp)
 8048507:       e8 a4 fe ff ff          call   80483b0 <fwrite@plt>
 804850c:       c7 04 24 0d 86 04 08    movl   $0x804860d,(%esp)
 8048513:       e8 a8 fe ff ff          call   80483c0 <system@plt> #exploit for shell
 8048518:       c9                      leave # jump here if 0
 8048519:       c3                      ret

             