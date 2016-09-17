SELECT title,count(*) AS num_inst
FROM (SELECT DISTINCT title,instID FROM (Papers NATURAL JOIN Authors NATURAL JOIN Inst NATURAL JOIN Authorship))
GROUP BY title
HAVING count(*) > 1;
