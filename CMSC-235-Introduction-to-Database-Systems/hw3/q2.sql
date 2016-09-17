SELECT title,lname,year-birthYear AS age 
FROM (artists NATURAL JOIN 
	(SELECT title,year,painterID AS artistID FROM paintings 
	UNION 
	SELECT title,year,sculptorID AS artistID FROM sculptures)) 
ORDER BY age ASC;
 

