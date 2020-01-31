SELECT name FROM people WHERE id IN
(SELECT person_id FROM stars WHERE movie_id IN
(SELECT id from movies
JOIN stars ON movies.id = stars.movie_id
WHERE stars.person_id IN
(SELECT id from people WHERE name = "Kevin Bacon")))
AND name <> "Kevin Bacon";