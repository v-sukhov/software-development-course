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

