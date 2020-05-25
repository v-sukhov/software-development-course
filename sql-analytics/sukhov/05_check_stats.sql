select
	s_stats.cnt,
	stats.cnt,
	(case when s_stats.cnt = stats.cnt then 'OK' else 'FAILED' end) as SUCCESS
from
	(select count(*) as cnt from roi.s_stats) as s_stats cross join
	(select count(*) as cnt from roi.stats) as stats;
	