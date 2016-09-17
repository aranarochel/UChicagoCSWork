SELECT authID,fname,lname,count(authID) AS num_papers_authored
FROM (Papers NATURAL JOIN Authors NATURAL JOIN Inst NATURAL JOIN Authorship)
GROUP BY authID,fname,lname
ORDER BY lname ASC,fname ASC;

