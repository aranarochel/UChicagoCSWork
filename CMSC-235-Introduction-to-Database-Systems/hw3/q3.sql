SELECT lname,title
FROM (artists NATURAL JOIN
    (SELECT title,painterID AS artistID FROM paintings WHERE location IS NULL
    UNION
	SELECT title,sculptorID AS artistID FROM sculptures WHERE location IS NULL));
