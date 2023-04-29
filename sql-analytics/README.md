# Анализ данных с использованием SQL

## Материалы для изучения

Пройдите тьюториал по языку SQL:

https://www.w3schools.com/sql/default.asp

## Анализ статистики результатов заключительных этапов Всероссийской олимпиады школьников по информатике за 2014 - 2020 г.г.

### Подготовка к работе

1. Установите СУБД PostgreSQL:

https://www.postgresql.org/

Для Ubuntu:

	sudo apt-get install postgresql
	
Чтобы устанавливать соединение от имени суперпользователя postgres нужно от имени пользователя postgres операционной системы запустить psql:

	sudo -u postgres psql postgres

И задать пароль для пользователя:

	ALTER USER postgres password '1';

2. Установите JDBC-клиент DBeaver:

https://dbeaver.io/

3. Создайте схему roi.

4. В файле roi-results-2014-2020.csv представлены результаты заключительных этапов Всероссийской олимпиады школьников по информатике за 2014 - 2020 годы 
(за 2020 год в качестве результаты Вседомашней олимпиады). Файл записан в csv-формате с заголовком, с разделителем столбцов - символом табуляции, с разделителем строк 
в стиле Windows - CR LF (\r\n), в кодировке UTF-8 (без BOM).

Загрузите файл в БД в таблицу roi.stats следующей структуры:

	create table roi.stats
	(
	
		year				smallint,
		place				smallint,
		fullname			varchar(256),
		region				varchar(256),
		class_education		smallint,
		class_participation	smallint,
		task1_score			smallint,
		task2_score			smallint,
		task3_score			smallint,
		task4_score			smallint,
		task5_score			smallint,
		task6_score			smallint,
		task7_score			smallint,
		task8_score			smallint,
		total_score			smallint,
		diploma				varchar(32)
		
	);

	
	COMMENT ON TABLE roi.stats IS 'Статистика результатов заключительных этапов Всероссийской олимпиады школьников по информатике';

	COMMENT ON COLUMN roi.stats.year IS 'Год олимпиады';

	COMMENT ON COLUMN roi.stats.place IS 'Итоговое место участника';

	COMMENT ON COLUMN roi.stats.fullname IS 'ФИО участника';

	COMMENT ON COLUMN roi.stats.class_education IS 'Класс обучения участника';

	COMMENT ON COLUMN roi.stats.class_participation IS 'Класс участия участника';

	COMMENT ON COLUMN roi.stats.task1_score IS 'Балл по задаче № 1';
	
	COMMENT ON COLUMN roi.stats.task2_score IS 'Балл по задаче № 2';
	
	COMMENT ON COLUMN roi.stats.task3_score IS 'Балл по задаче № 3';
	
	COMMENT ON COLUMN roi.stats.task4_score IS 'Балл по задаче № 4';
	
	COMMENT ON COLUMN roi.stats.task5_score IS 'Балл по задаче № 5';
	
	COMMENT ON COLUMN roi.stats.task6_score IS 'Балл по задаче № 6';
	
	COMMENT ON COLUMN roi.stats.task7_score IS 'Балл по задаче № 7';
	
	COMMENT ON COLUMN roi.stats.task8_score IS 'Балл по задаче № 8';
	
	COMMENT ON COLUMN roi.stats.total_score IS 'Суммарный балл по всем задачам';
	
	COMMENT ON COLUMN roi.stats.diploma IS 'Полученный диплом';
	
Обратите внимание, что поле class_participation (класс участия участника) не содержится в исходном файле - его необходимо вычислить. Также обратите внимание, что 
в исходном файле по тем задачам, по которым у участника не было попыток, в соответствующем столбце проставлен символ '.' (точка). 
В целевой таблице в таких полях требуется проставить значение NULL. Поэтому загрузку исходных данных в целевую таблицу необходимо осуществить в 2 этапа: 
сначала загрузить данные в промежуточную таблицу другой структуры и с текстовыми типами полей баллов по задачам, а затем при помощи операции

	INSERT INTO ... SELECT ...
	
загрузить данные в целевую таблицу.

	insert into roi.stats 
	(
		"year",
		place,
		fullname,
		region,
		class_education,
		class_participation,
		task1_score,
		task2_score,
		task3_score,
		task4_score,
		task5_score,
		task6_score,
		task7_score,
		task8_score,
		total_score,
		diploma
	)
	select
		cast("year" as smallint),
		cast(place as smallint),
		fullname,
		region,
		cast(class_education as smallint),
		greatest(cast(class_education as smallint), 9),
		cast((case when task1_score in ('.', ' ', '. ', '') then 0 else cast(task1_score as smallint) end) as smallint),
		cast((case when task2_score in ('.', ' ', '. ', '') then 0 else cast(task2_score as smallint) end) as smallint),
		cast((case when task3_score in ('.', ' ', '. ', '') then 0 else cast(task3_score as smallint) end) as smallint),
		cast((case when task4_score in ('.', ' ', '. ', '') then 0 else cast(task4_score as smallint) end) as smallint),
		cast((case when task5_score in ('.', ' ', '. ', '') then 0 else cast(task5_score as smallint) end) as smallint),
		cast((case when task6_score in ('.', ' ', '. ', '') then 0 else cast(task6_score as smallint) end) as smallint),
		cast((case when task7_score in ('.', ' ', '. ', '') then 0 else cast(task7_score as smallint) end) as smallint),
		cast((case when task8_score in ('.', ' ', '. ', '') then 0 else cast(task8_score as smallint) end) as smallint),
		cast((case when total_score in ('.', ' ', '. ', '') then 0 else cast(total_score as smallint) end) as smallint),
		diploma
	from
		roi.stats_import

### Задания

Если для выполнения заданий вам потребуется создание дополнительных таблиц, создавайте их в отдельной схеме с названием, соответствующим вашей фамилии.

В качестве результатов выполнения заданий приведите один или несколько SQL-запросов, вычисляющих требуемую величину. Запросы запишите в текстовых файлах с расширением .sql,
в названии файла укажите номер задания. В одном файле можно привести ответ на несколько заданий (в этом случае в названии файла должен быть указан диапазон номеров
заданий, например: task_1_5.sql).

1. Определите самого молодого участника олимпиады (по классу обучения) за весь период.

2. Определите самого молодого участника за каждый год.

3. Определите процент участников олимпиады по каждому классу участия (9, 10, 11) суммарно и за каждый ког в отдельности.

4. Определите процент призёров олимпиады по каждому классу участия (9, 10, 11) суммарно и за каждый ког в отдельности.

5. Составьте рейтинг регионов по количеству участников олимпиады.

6. Составьте рейтинг регионов по количеству участников олимпиады за каждый год в отдельности.

7. Составьте рейтинг регионов по количеству победителей олимпиады.

8. Составьте рейтинг регионов по суммарному количеству победителей и призёров олимпиады.

9. Определите задачи с самым малым и самым большим суммарным количеством набранных балов всеми участниками.

10. Определите самую редкорешаемую задачу (то есть с самым малым количеством посылок).

11. Составьте рейтинг регионов по количеству участников олимпиады на 1 млн. жителей региона.

12. Составьте рейтинг регионов по количеству участников олимпиады на 1 млн. жителей региона по каждому году отдельно.

13. Определите топ-10 участников по количеству участий в олимпиаде.

14. Определите, есть ли полные тезки среди участников олимпиады (за все годы).

15. Определеите топ-10 участников по количеству занятых призовых мест.

16. Определите процент участников мужского пола среди всех участников олимпиады.

17. Определите процент участников мужского пола среди участников, занявших призовые места.

18. Для каждого k-го персентиля (от 0 до 99) определите вероятность получения участником призового места по итогу 2-х дней, если после первого дня олимпиады его место соответствовало k-му персентилю.



