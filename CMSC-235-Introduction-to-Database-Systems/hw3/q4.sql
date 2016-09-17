SELECT artistID,lname,group_concat(" "||medium||" on "||mediumon) AS media 
FROM (artists NATURAL JOIN (SELECT DISTINCT medium,mediumon,painterID AS artistID FROM paintings))
GROUP BY artistID
ORDER BY lname ASC;
