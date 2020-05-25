delete from roi.stats;

insert into roi.stats
	(
		year,
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
		year,
		place,
		fullname,
		region,
		cast(class_education as smallint),
		(case when cast(class_education as smallint) < 9 then 9 else cast(class_education as smallint) end) as class_participation,
		cast( (case when trim(task1_score) = '.' then null else task1_score end) as smallint),
		cast( (case when trim(task2_score) = '.' then null else task2_score end) as smallint),
		cast( (case when trim(task3_score) = '.' then null else task3_score end) as smallint),
		cast( (case when trim(task4_score) = '.' then null else task4_score end) as smallint),
		cast( (case when trim(task5_score) = '.' then null else task5_score end) as smallint),
		cast( (case when trim(task6_score) = '.' then null else task6_score end) as smallint),
		cast( (case when trim(task7_score) = '.' then null else task7_score end) as smallint),
		cast( (case when trim(task8_score) = '.' then null else task8_score end) as smallint),
		total_score,
		diploma
	from
		roi.s_stats;
		