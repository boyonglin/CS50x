-- list the names of all people who starred in a movie in which Kevin Bacon (born in 1958) also starred
SELECT DISTINCT p2.name FROM stars AS s1
JOIN stars AS s2 ON s1.movie_id = s2.movie_id
JOIN people AS p1 ON s1.person_id = p1.id
JOIN people AS p2 ON s2.person_id = p2.id
WHERE p1.id IN (SELECT id FROM people WHERE name = 'Kevin Bacon' AND birth = 1958)
AND p2.id <> (SELECT id FROM people WHERE name = 'Kevin Bacon' AND birth = 1958);
