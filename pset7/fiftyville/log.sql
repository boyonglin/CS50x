-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Looking for a crime scene report that matches the date and the location of the crime.
SELECT description FROM crime_scene_reports
WHERE month = 7 AND day = 28
AND street = 'Humphrey Street';

-- According to Ruth's interview transcript, the suspect left within ten minutes of the theft, listing the matching license plate number and the time of day he entered the bakery.
SELECT hour, minute, license_plate FROM bakery_security_logs
WHERE hour = 10 AND minute BETWEEN 15 AND 25
AND month = 7 AND day = 28
AND activity = 'exit';

-- Match license plate number to vehicle owner.
SELECT license_plate, name FROM people
WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE hour = 10 AND minute BETWEEN 15 AND 25 AND month = 7 AND day = 28 AND activity = 'exit');

-- View the interview record on July 28.
SELECT name, transcript FROM interviews WHERE month = 7 AND day = 28;

-- According to Eugene's interview transcript, the thief withdrew money from an ATM on Leggett Street before Eugene entered the bakery.
SELECT hour, minute, people.name FROM bakery_security_logs AS bsl
JOIN people ON people.license_plate = bsl.license_plate
WHERE people.name = 'Eugene' AND activity = 'entrance' AND hour < 10;

SELECT atm.account_number, p.name FROM atm_transactions AS atm
JOIN bank_accounts AS ba ON atm.account_number = ba.account_number
JOIN people AS p ON ba.person_id = p.id
WHERE month = 7 AND day = 28
AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';

-- Cross-compare suspects leaving the bakery with suspects withdrawing money
SELECT atm.account_number, p.name FROM atm_transactions AS atm
JOIN bank_accounts AS ba ON atm.account_number = ba.account_number
JOIN people AS p ON ba.person_id = p.id
WHERE month = 7 AND day = 28
AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'
AND p.name IN (SELECT name FROM people
WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE hour = 10 AND minute BETWEEN 15 AND 25 AND month = 7 AND day = 28 AND activity = 'exit'));

-- According to Raymond's interview transcript, the thief called someone after leaving bakery.
SELECT pc.caller, p1.name, pc.receiver, p2.name FROM phone_calls AS pc
JOIN people AS p1 ON pc.caller = p1.phone_number
JOIN people AS p2 ON pc.receiver = p2.phone_number
WHERE month = 7 AND day = 28 AND duration < 60
AND p1.name IN ('Bruce', 'Diana', 'Iman', 'Luca');

-- Confirm the Fiftyville airport information and find the thief who is going to board the earliest flight on July 29.
SELECT id, origin_airport_id AS departure, destination_airport_id AS destination, month, day, hour, minute FROM flights
WHERE origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville')
AND month = 7 AND day = 29
ORDER BY hour;

SELECT f.id, a1.city, a2.city, pe.name FROM passengers AS pa
JOIN people AS pe ON pa.passport_number = pe.passport_number
JOIN flights AS f ON pa.flight_id = f.id
JOIN airports AS a1 ON f.origin_airport_id = a1.id
JOIN airports AS a2 ON f.destination_airport_id = a2.id
WHERE pe.name IN ('Bruce', 'Diana', 'Philip') AND pa.flight_id = 36;
-- Found the thief is Bruce, and the accomplice is Robin, and the city he escaped to is New York City.