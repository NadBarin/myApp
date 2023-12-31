# myApp
## Задание: 
Написать консольное приложение или php скрипт, который будет запускаться из консоли.
По каждому пункту оно должно принимать параметр командной строки и выполнять соответствующий пункт.
По ходу задания будут примеры. Для ФИО использовать английский язык. Решать проблему с отображением русского языка в консоли, если возникает, не нужно.
Приложение/скрипт должно подключаться к базе данных.

В качестве СУБД можно использовать любую SQL СУБД или MongoDB.
В качестве среды разработки можете использовать любой известный вам язык программирования.

В приложении должно быть:
1. Создание таблицы с полями представляющими ФИО, дату рождения, пол.

Пример запуска приложения:
myApp 1
Для php:
php myApp.php 1
Для java:
java myApp.class 1
или
java myApp.jar 1

2. Создание записи. Использовать следующий формат:
myApp 2 ФИО ДатаРождения Пол

3. Вывод всех строк с уникальным значением ФИО+дата, отсортированным по ФИО , вывести ФИО, Дату рождения, пол, кол-во полных лет.
Пример запуска приложения:
myApp 3

4. Заполнение автоматически 1000000 строк. Распределение пола в них должно быть относительно равномерным, начальной буквы ФИО также. Заполнение автоматически 100 строк в которых пол мужской и ФИО начинается с "F".
Пример запуска приложения:
myApp 4

5. Результат выборки из таблицы по критерию: пол мужской, ФИО начинается с "F". Сделать замер времени выполнения.
Пример запуска приложения:
myApp 5
Вывод приложения должен содержать время.

6. Произвести определенные манипуляции над базой данных для ускорения запроса из пункта 5. Убедиться, что время исполнения уменьшилось. Объяснить смысл произведенных действий. Предоставить результаты замера до и после.

## О решении:
Язык C++, СУБД MySQL, для соединения использовался MySQL C++ connector.
Пусть к исполняемому файлу: x64/Release/myApp.exe
1. Изначально, до оптимизации таблицы, таблица создавалась запросом "CREATE TABLE users (name VARCHAR(50), birthdate date, sex VARCHAR(1));"

2. Создание записи. Используется следующий формат: 2 ФИО ДатаРождения Пол
   
4. Вывод всех строк с уникальным значением ФИО+дата, отсортированным по ФИО , вывести ФИО, Дату рождения, пол, кол-во полных лет.

5. Имена и фамилии берутся из .txt файлов: LastNames.txt, MaleNames.txt, FemNames.txt.
   
6. Время выполенения подписано как Time taken:
   <image src="Screenshot_26.png" alt="Время выполенения">

7. Для ускорения запроса можно добавить индексы(ключи), поскольку при использовании индекса MySQL не читает все данные для нахождения значения столбца.
   Заменяем запрос из первого пункта на: "CREATE TABLE users (id MEDIUMINT NOT NULL AUTO_INCREMENT, name VARCHAR(50), birthdate date, sex VARCHAR(1),PRIMARY KEY (id));"
   Время выполенения c использованием индексов в таблице:
   <image src="Screenshot_29.png" alt="Время выполенения c использованием индексов в таблице">
   Производительность запросов SELECT может быть также значительно улучшена при использовани секционирования, т.к. в секциях могут храниться значения начинающиеся с одного и тогоже символа и числа и это могло бы упростить    поиск. Секционирование по первым буквам ФИО произвести нельзя, т.к. ФИО не может быть первичным ключом, т.к. не все ФИО содержат уникальные значения. Секционирование по id только увеличит время выполнения и не имеет       смысла.
   
