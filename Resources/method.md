#0

- attack: password revealing
- binary behaviour: segfault with no argument, "No" with any number except the password

- 0x1a7 = 423

#1
- attack: buffer overflow with direct shell call
- binary behaviour: get argument and do nothing
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

- offset code structure: 76 = 4 (return address) + 72 (padding)

QA:
- how do we know the correct offset?
- overflow by any number -> get address and put it in 'find offset' (pattern and offset are related)

- how do we find the return function address?
- find it in assembler code before function name: 08048444 <run>

- why simple multiplication of same symbol doesn't work for overflowing in gdb and we need random pattern generation?

- why we use cat here?
- /bin/sh is called through the pipe and became non-interactive (it reads stdin with EOF and closes). cat enables interactive listening here

#2
- attack: buffer overflow with shell call payload
- binary behaviour: reprints argument
- targeted functions: p and main
- vulnerability in main: gets()
- vulnerability in p: strdup()
- defence: no stack overwriting is possible because of comparison of eip address with predefined eip address 

Method:
- find the offset padding size
- overflow the binary
- find the buffer address used by strdup
- overwrite buffer by code calling shell
- overwrite eip by the buffer address
- cat passwd when get console with root privileges

Info:
- 0x0804a008 - buffer address
- 0x08a00480 - same in little-endian
- same in hex - \x08\xa0\x04\x08

- offset of eip register - 80 (from wiremask.eu)
- offset code structure: 80 = 4 (return address) + 52 (shell code) + 28

QA
- How do we know buffer address?
- ltrace() and see address of strdup

#3
- attack: format string exploitation (address leaks)
- binary behaviour: fgets argument and send it to printf, if global argument = 64 run the shell
- targeted functions: v and main
- vulnerability in v: printf() without argument (Bugs section in printf man)
- defence: fget

Method:
- do stack leakage
- find in leakage the position of the argument (find repetition and compare with hex ASCII table)
- find the address of global variable and its correct condition
- check that you can put global variable address instead of argument
- write 64 to the global variable using %n and %u modifiers
 
Info:
- position of buffer in stack - 4

- 0804988c - address of global variable
- 8c980408 - same in little-endian
- same in hex - \x8c\x98\x04\x08
- the password - 0x40 or 64

- offset code structure: 64 = 4 (global variable address) + 60 (padding)

QA
- why the leakage of global variable happens in printf?
- Printf has no specifications. If we pass %smth in the stdin, printf will consider it as specification. Printf will search for arguments in stack printing in the following order: address of format string, values of arguments (buffer), addresses of arguments

- how to find a global variable?
- objdump -t level3 -> 0804988c g     O .bss   00000004              m

- Where exactly do we write the address of global variable
- The format string is usually located on the stack itself. Arguments of prinf should be located after it

- how do we pass argument for the global varibale?
- "A" * 60 + "%4$n or "%60u%4$n" - %n modifier writes the number of preceding bytes (64) and puts it the pointed location

#4
- attack: format string exploitation
- binary behaviour: fgets argument and send it to printf, if global argument = 16930116 run the shell
- targeted functions: n, p and main
- vulnerability in p: printf() without argument (Bugs section in printf man)
- defence: fget

Method:
- do stack leakage
- find in leakage the position of the argument 
- find the global variable (password)
- overwrite buffer with address of global variable 
- and pass 16930116 as its value
 
Info:
- position of argument in stack - 12

- 0x8049810 - address global variable
- 10980408 - same in little-endian
- same in hex - \x10\x98\x04\x08
- the password - 0x1025544 or 16930116

- offset code structure: 16930112 = 16930116 (password) - 4 (buffer address)

#5
- attack: format string exploitation using Global Offset Table
- binary behaviour: fgets argument and send it to printf
- targeted functions: n, o and main
- vulnerability in n: printf() without argument (Bugs section in printf man)
- vulnerability in o: no ASLR (address space layout realisation)
- defense1: fget
- defense2: function o with system call is non-called

Method:
- find the address of exit call in n
- find the address of o function
- find the exit call position 
- write o function address instead of exit address through argument buffer
 
Info:
- positon of exit function (buffer) in stack - 4
- 08049838 - address of exit function
- same in hex - \x38\x98\x04\x08

- 080484a4 - address of o function
- 134513828 - same in decimal

- offset code structure: 134513828 = 134513824 (o address) - 4 (buffer address)

QA:
- why we can rewrite exit function
- <exit@plt> from n calls real 'exit' address from PLT. PLT si procedure linkage table, keeps addresses of libc library. We can change the value inside GOT by the new address

- what is the function of PLT?
- instead of searaching the location of function each call, it is faster to call the function that will remember it

#6
- attack: buffer overflow
- binary behaviour: segfault without parameter, ‘Nope’ with parameter
- targeted functions: n, m and main
- vulnerability in n: strcpy() in comparison to strncpy() has no limitimg argument
- defence: function n with system call is non-called

Method:
- find the overflow size of strcpy
- find eip offset size after overwlofing strcpy
- find the address of n function
- overflow and change eip address by n address
 
Info:
- offset size - 72
- 08048454 - address of n function
- same in hex - \x54\x84\x04\x08

#7
- attack: buffer overflow using GOT + format string exploitation
- binary behaviour: segfault without or with 1 parameter, ~~ with 2 parameters
- targeted functions: m and main
- vulnerability in m: strcpy() in comparison to strncpy() has no limitimg argument
- defense: 

Method: send address of fopens (after secdons strcpy) to the function m with printf
- find the overflow offset for first strcpy
- check it - correct offset will put payload into second strcpy after overflowing

- find the address of 'puts' function in GOT
- find the address of m function

- write in GOT 'm' address instead of 'puts' address 
- enter to the m function
- 'm' will execute printf with variable pass

Info:
- offset size - 20
- 8049928 - address of puts in GOT
- same in hex - \x28\x99\x04\x08

- 080484f4 - address of m function
- same in hex - \xf4\x84\x04\x08
