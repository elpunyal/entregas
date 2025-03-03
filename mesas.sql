CREATE TABLE mesas (
    id SERIAL PRIMARY KEY,
    cartas VARCHAR(4) NOT NULL
);

INSERT INTO mesas (cartas) VALUES
('Ace'),
('King'),
('Queen'),
('Joker');