select * from roi.s_stats
where trim(task1_score) = '.' or trim(task2_score) = '.' or trim(task3_score) = '.' or trim(task4_score) = '.' or trim(task5_score) = '.'
	or trim(task6_score) = '.' or trim(task7_score) = '.' or trim(task8_score) = '.';
	
