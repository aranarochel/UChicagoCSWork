SELECT title,max(num_authors) AS num_authors
FROM (SELECT title,count(title) AS num_authors
      FROM (Papers NATURAL JOIN Authors NATURAL JOIN Inst NATURAL JOIN Authorship)
      GROUP BY title);
