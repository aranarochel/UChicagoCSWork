SELECT title FROM (Papers NATURAL JOIN Authorship) GROUP BY title Having count(title) < 2;
