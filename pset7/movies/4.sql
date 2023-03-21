-- determine the number of movies with an IMDb rating of 10.0
SELECT COUNT(id) FROM movies
WHERE id IN (SELECT movie_id FROM ratings WHERE rating = '10.0');