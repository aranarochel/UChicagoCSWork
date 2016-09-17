SELECT authID,lname,fname,sum(num_authors)-count(*) AS shared_auth
FROM
	(SELECT title,count(title) AS num_authors
	 FROM (Papers NATURAL JOIN Authors NATURAL JOIN Authorship)
	 GROUP BY title)
	NATURAL JOIN
	(SELECT authID,fname,lname,title 
	 FROM (Papers NATURAL JOIN Authors NATURAL JOIN Authorship))
GROUP BY lname,fname
ORDER BY lname ASC,fname ASC;
