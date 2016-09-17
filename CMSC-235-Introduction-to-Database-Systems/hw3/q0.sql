SELECT title,lname,height_mm*0.03937 AS height_in,width_mm*0.03937 AS width_in
FROM (artists NATURAL JOIN paintings)
WHERE Artists.artistID==Paintings.painterID
ORDER BY height_in*width_in DESC;
 
