# ИДЗ-1 ОС, Порфирьев Антон БПИ219, 24 вар
![image](https://user-images.githubusercontent.com/90344389/226197838-65bec71e-5f85-4415-a951-ccefbef25f1f.png)

## Введение
Эта не претендует, на оценку 10 (реализованы только от 4 до 9).
### Архитектура
В корне репазитория лежат папки 04-09, tests. Такжде лежат файлы README.md (он же отчёт) и файл check.sh
#### tests
В этой папке лежат 5 общих тестов для всех решений.

Файлы без расширения, это сами тесты.

Файлы с расширением ```.a```, это првильные ответы.

#### check.sh
Это bash-script который запускает проверку всех тестах у заданного решения. Скрипт принимает один аргумент - название папки в которой происходит решение.

Например ```./check.sh 04```.

Этот скрипт запускает, программу решения на какую-то оценку и выводит дифф с правильными ответами.
ВАЖНО: Этот скрипт написан, мягко говоря, не очень качественно, поэтому я прошу 2 вещи:
1. Не понижать оценку из-за качества скрипта
2. Запускать только из папки где он лежит, т.е не ```../check.sh``` или ```work/check.sh```, тогда он упадёт. Запускайте только ```./check.sh```

#### 04-09

Здесь лежат ответы на 5 тестов (обнавляются при запуске ```./check.sk```).

ans{i}.txt - ответ на i-ый тест соответсвенно

Также в этом файле лежит сам код, на соответсвующую оценку.

## Описание решений

### 4 балла

Всего 3 процесса
+ Главный, который производит подсчёт и пораждает дочерние процессы
+ 1-ый дочерний - считывает данные
+ 2-ой дочерний - выводит данные в файл

### 5 баллов

То-же, что и выше, только используя 2 именнованых канла, для связи: ```read``` и ```write```

### 6 баллов

Всего 2 процесса
+ Главный, который производит подсчёт и пораждает дочерний
+ Дочерний, который считывает данные, а также выводит ответ полученный главным процессом в файл.

### 7 баллов

То-же, что и выше, только используя 2 именнованых канла, для связи: ```read``` и ```write```

### 8 баллов

Всего 3 процесса
+ Главный, который только пораждает 2 независимых. Использует 2 именнованых канала ```read``` и ```write```
+ 1-ый дочерний, который считывает данные, а также выводит ответ полученный главным процессом в файл.
+ 2-ый дочерний, который производит обработку.

### 9 Баллов

То-же, что и 8. Однако теперь считывание идёт порционно по 16 байт
