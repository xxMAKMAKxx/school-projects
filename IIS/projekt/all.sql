DROP TABLE Internal_flight;
DROP TABLE Flying_from;
DROP TABLE International_flight;
DROP TABLE Ticket;
DROP TABLE Flight;
DROP TABLE Gate;
DROP TABLE Terminal;
DROP TABLE Seat;
DROP TABLE Plane;
DROP TABLE Plane_type;
DROP TABLE Airport_employee;


CREATE TABLE Airport_employee (
  login NVARCHAR(255),
  PRIMARY KEY(login),
  password NVARCHAR(255) NOT NULL,
  role NVARCHAR(255) NOT NULL,
  superior_login  NVARCHAR(255),
  position  NVARCHAR(255),
  `name`  NVARCHAR(255),
  surname  NVARCHAR(255),
  street  NVARCHAR(255),
  city  NVARCHAR(255),
  postal_code  INTEGER,
  birth_number NVARCHAR(255),
  created TIMESTAMP
)
ENGINE=InnoDB;

CREATE TABLE Internal_flight (
  flight_id INTEGER,
  representing_company NVARCHAR(255),
  drink NVARCHAR(512),
  PRIMARY KEY(flight_id)
)
ENGINE=InnoDB;

CREATE TABLE International_flight (
  flight_id INTEGER,
  services NVARCHAR(512),
  movies NVARCHAR(512),
  food_list NVARCHAR(512),
  PRIMARY KEY(flight_id)
)
ENGINE=InnoDB;

CREATE TABLE Terminal (
  terminal_id CHAR(1),
  services NVARCHAR(512),
  terminal_type NVARCHAR(512),
  PRIMARY KEY(terminal_id)
)
ENGINE=InnoDB;

CREATE TABLE Gate (
  gate_id INTEGER AUTO_INCREMENT,
  capacity INTEGER,
  gate_type NVARCHAR(255),
  terminal_id CHAR(1) NOT NULL,
  PRIMARY KEY(gate_id)
)
ENGINE=InnoDB;

CREATE TABLE Flight (
  flight_id INTEGER AUTO_INCREMENT,
  PRIMARY KEY(flight_id),
  departure TIMESTAMP,
  arrival TIMESTAMP,
  `from` NVARCHAR(255),
  `to` NVARCHAR(255),
  company NVARCHAR(255),
  employee_login NVARCHAR(255) NOT NULL,
  plane_id NVARCHAR(128) NOT NULL,
  gate_id INTEGER NOT NULL
)
ENGINE=InnoDB;

CREATE TABLE Ticket (
  ticket_id INTEGER AUTO_INCREMENT,
  PRIMARY KEY(ticket_id),
  passenger_name NVARCHAR(255),
  passenger_surname NVARCHAR(255),
  passenger_email NVARCHAR(255),
  passport_number INTEGER,
  boarding_pass CHAR(1),
  flight_id INTEGER NOT NULL,
  seat_number INTEGER NOT NULL,
  plane_id NVARCHAR(128) NOT NULL,
  ticket_price INTEGER
)
ENGINE=InnoDB;

CREATE TABLE Seat (
  seat_number INTEGER,
  `class` INTEGER,
  sector CHAR(1),
  plane_id NVARCHAR(128) NOT NULL
)
ENGINE=InnoDB;

CREATE TABLE Plane (
  plane_id NVARCHAR(128),
  PRIMARY KEY(plane_id),
  manufacture_date DATE,
  last_revision DATE,
  crew_number INTEGER,
  plane_type NVARCHAR(255) NOT NULL
)
ENGINE=InnoDB;

CREATE TABLE Plane_type (
  plane_type NVARCHAR(128),
  PRIMARY KEY(plane_type),
  manufacturer NVARCHAR(128),
  number_of_engines INTEGER,
  width INTEGER,
  lenght INTEGER,
  height INTEGER
)
ENGINE=InnoDB;

CREATE TABLE Flying_from (
  gate_id INTEGER NOT NULL,
  plane_type NVARCHAR(128) NOT NULL
)
ENGINE=InnoDB;

ALTER TABLE Airport_employee
	ADD CONSTRAINT fk_superior_login
	FOREIGN KEY (superior_login)
	REFERENCES Airport_employee(login)
  ON DELETE CASCADE
  ON UPDATE CASCADE;

ALTER TABLE Internal_flight
  ADD CONSTRAINT fk_flight_id_internal_flight
    FOREIGN KEY(flight_id)
    REFERENCES Flight(flight_id)
    ON DELETE CASCADE
    ON UPDATE CASCADE;

ALTER TABLE International_flight
  ADD CONSTRAINT fk_flight_id_international
    FOREIGN KEY(flight_id)
    REFERENCES Flight(flight_id)
    ON DELETE CASCADE
    ON UPDATE CASCADE;

ALTER TABLE Gate
 	ADD CONSTRAINT fk_terminal_id
    FOREIGN KEY(terminal_id)
    REFERENCES Terminal(terminal_id)
    ON DELETE CASCADE
    ON UPDATE CASCADE;

ALTER TABLE Flight
 	ADD CONSTRAINT fk_employee_login
    FOREIGN KEY(employee_login)
    REFERENCES Airport_employee(login)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  ADD CONSTRAINT fk_plane_id
    FOREIGN KEY(plane_id)
    REFERENCES Plane(plane_id)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  ADD CONSTRAINT fk_gate_id
    FOREIGN KEY(gate_id)
    REFERENCES Gate(gate_id)
    ON DELETE CASCADE
    ON UPDATE CASCADE;

ALTER TABLE Seat
	ADD CONSTRAINT pk_seat
	  PRIMARY KEY(plane_id, seat_number),
  ADD CONSTRAINT fk_plane_id_seat
    FOREIGN KEY(plane_id)
    REFERENCES Plane(plane_id)
    ON DELETE CASCADE
    ON UPDATE CASCADE;

ALTER TABLE Ticket
	ADD CONSTRAINT fk_flight_id_ticket
    FOREIGN KEY(flight_id)
    REFERENCES Flight(flight_id)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
	ADD CONSTRAINT fk_seat_number
    FOREIGN KEY(plane_id, seat_number)
    REFERENCES Seat(plane_id, seat_number)
    ON DELETE CASCADE
    ON UPDATE CASCADE;

ALTER TABLE Plane
  ADD CONSTRAINT fk_plane_type
    FOREIGN KEY(plane_type)
    REFERENCES Plane_type(plane_type)
    ON DELETE CASCADE
    ON UPDATE CASCADE;

ALTER TABLE Flying_from
	ADD CONSTRAINT pk_flying_from
    PRIMARY KEY(gate_id, plane_type),
  ADD CONSTRAINT fk_gate_id_flyingfrom
    FOREIGN KEY(gate_id)
    REFERENCES Gate(gate_id)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  ADD CONSTRAINT fk_plane_type_flyingfrom
    FOREIGN KEY(plane_type)
    REFERENCES Plane_type(plane_type)
    ON DELETE CASCADE
    ON UPDATE CASCADE;



INSERT INTO Airport_employee VALUES (
'xkocou08',
'heslo',
'manager',
NULL, 
'Director',
'Martin', 
'Kocour', 
'Muraniho 7',
'Nitra', 
94911, 
'9402117098',
NOW()
);

INSERT INTO Airport_employee VALUES (
'xkacma03',
'heslo',
'supervisor',
'xkocou08', 
'Back office',
'Martin', 
'Kacmarcik',
'Sedliacka 99', 
'Bruntal',
61200 , 
'9312115296',
NOW()
);
            
                    
INSERT INTO Airport_employee VALUES (
'xnovak00',
'heslo',
'employee',
'xkocou08', 
'Ticket admin',
'Klara', 
'Novakova', 
'Sedliacka 98', 
'Brunt�l', 
61200 , 
'9655051241',
NOW()
);

INSERT INTO Terminal VALUES('B', NULL, 'Cargo-Arrivals');
INSERT INTO Terminal VALUES('A', 'Buffet, Bar, Coffee Shop, Restaurant', 'Passenger-Departures');

INSERT INTO Gate VALUES(1, 700, 'EXIT with BUS', 'A');
INSERT INTO Gate VALUES(2, 1000, 'EXIT standart', 'A');
INSERT INTO Gate VALUES(3, 2000, 'EXIT standart', 'A');
INSERT INTO Gate VALUES(4, 2500, 'EXIT standart', 'B');
INSERT INTO Gate VALUES(5, 500, 'EXIT with BUS', 'B');
INSERT INTO Gate VALUES(6, 800, 'EXIT with BUS', 'B');

INSERT INTO Plane_type VALUES('A380', 'Airbus', 2, 50, 70, 10);	
INSERT INTO Plane_type VALUES('747-800', 'Boeing', 2, 55, 65, 15);
INSERT INTO Plane_type VALUES('666', 'AirJano', 1, 25, 40, 8);	
INSERT INTO Plane_type VALUES('B321', 'Kocour Corporation', 8, 80, 90, 25);
INSERT INTO Plane_type VALUES('AirDream', 'Kocour Corporation', 4, 40, 45, 13);
INSERT INTO Plane_type VALUES('M3000', 'MAK', 5, 55, 60, 15);
INSERT INTO Plane_type VALUES('The God', 'MAK', 5, 100, 120, 25);

INSERT INTO Plane VALUES('A380199', STR_TO_DATE('01.01.1999', '%d.%m.%Y'), STR_TO_DATE('01.01.2015', '%d.%m.%Y'), 5, '747-800');
INSERT INTO Plane VALUES('A380200', STR_TO_DATE('01.01.2005', '%d.%m.%Y'), NULL, 3, 'A380');
INSERT INTO Plane VALUES('A380201', STR_TO_DATE('11.12.2014', '%d.%m.%Y'), NULL, 10, 'The God');
INSERT INTO Plane VALUES('A380202', STR_TO_DATE('11.12.2014', '%d.%m.%Y'), NULL, 10, 'The God');
INSERT INTO Plane VALUES('A380203', STR_TO_DATE('11.12.2014', '%d.%m.%Y'), NULL, 10, 'The God');
INSERT INTO Plane VALUES('A380204', STR_TO_DATE('11.05.2011', '%d.%m.%Y'), NULL, 8, 'AirDream');
INSERT INTO Plane VALUES('A380205', STR_TO_DATE('11.05.2011', '%d.%m.%Y'), NULL, 8, 'AirDream');
INSERT INTO Plane VALUES('A380206', STR_TO_DATE('18.06.2013', '%d.%m.%Y'), NULL, 15, 'B321');
INSERT INTO Plane VALUES('A380207', STR_TO_DATE('18.06.2013', '%d.%m.%Y'), NULL, 15, 'B321');
INSERT INTO Plane VALUES('A380208', STR_TO_DATE('18.06.2013', '%d.%m.%Y'), NULL, 15, 'B321');
INSERT INTO Plane VALUES('A380666', STR_TO_DATE('06.06.2015', '%d.%m.%Y'), NULL, 66, '666');
INSERT INTO Plane VALUES('A380266', STR_TO_DATE('06.06.2015', '%d.%m.%Y'), NULL, 66, '666');
INSERT INTO Plane VALUES('A386666', STR_TO_DATE('06.06.2015', '%d.%m.%Y'), NULL, 66, '666');

INSERT INTO Seat VALUES(1, 1, 'A', 'A386666');
INSERT INTO Seat VALUES(2, 1, 'B', 'A386666');
INSERT INTO Seat VALUES(3, 1, 'C', 'A386666');
INSERT INTO Seat VALUES(4, 1, 'D', 'A386666');
INSERT INTO Seat VALUES(5, 1, 'E', 'A386666');
INSERT INTO Seat VALUES(6, 1, 'F', 'A386666');
INSERT INTO Seat VALUES(7, 1, 'F', 'A386666');
INSERT INTO Seat VALUES(8, 1, 'F', 'A386666');
INSERT INTO Seat VALUES(9, 1, 'F', 'A386666');
INSERT INTO Seat VALUES(10, 1, 'F', 'A386666');

INSERT INTO Seat VALUES(1, 1, 'A', 'A380266');
INSERT INTO Seat VALUES(2, 1, 'B', 'A380266');
INSERT INTO Seat VALUES(3, 1, 'C', 'A380266');
INSERT INTO Seat VALUES(4, 1, 'D', 'A380266');
INSERT INTO Seat VALUES(5, 1, 'E', 'A380266');
INSERT INTO Seat VALUES(6, 1, 'F', 'A380266');
INSERT INTO Seat VALUES(7, 1, 'F', 'A380266');
INSERT INTO Seat VALUES(8, 1, 'F', 'A380266');
INSERT INTO Seat VALUES(9, 1, 'F', 'A380266');
INSERT INTO Seat VALUES(10, 1, 'F', 'A380266');


INSERT INTO Seat VALUES(1, 1, 'A', 'A380666');
INSERT INTO Seat VALUES(2, 1, 'B', 'A380666');
INSERT INTO Seat VALUES(3, 1, 'C', 'A380666');
INSERT INTO Seat VALUES(4, 1, 'D', 'A380666');
INSERT INTO Seat VALUES(5, 1, 'E', 'A380666');
INSERT INTO Seat VALUES(6, 1, 'F', 'A380666');
INSERT INTO Seat VALUES(7, 1, 'F', 'A380666');
INSERT INTO Seat VALUES(8, 1, 'F', 'A380666');
INSERT INTO Seat VALUES(9, 1, 'F', 'A380666');
INSERT INTO Seat VALUES(10, 1, 'F', 'A380666');

INSERT INTO Seat VALUES(1, 1, 'A', 'A380208');
INSERT INTO Seat VALUES(2, 1, 'B', 'A380208');
INSERT INTO Seat VALUES(3, 1, 'C', 'A380208');
INSERT INTO Seat VALUES(4, 1, 'D', 'A380208');
INSERT INTO Seat VALUES(5, 1, 'E', 'A380208');
INSERT INTO Seat VALUES(6, 1, 'F', 'A380208');
INSERT INTO Seat VALUES(7, 2, 'A', 'A380208');
INSERT INTO Seat VALUES(8, 2, 'B', 'A380208');
INSERT INTO Seat VALUES(9, 2, 'C', 'A380208');
INSERT INTO Seat VALUES(10, 2, 'D', 'A380208');

INSERT INTO Seat VALUES(1, 1, 'A', 'A380207');
INSERT INTO Seat VALUES(2, 1, 'B', 'A380207');
INSERT INTO Seat VALUES(3, 1, 'C', 'A380207');
INSERT INTO Seat VALUES(4, 1, 'D', 'A380207');
INSERT INTO Seat VALUES(5, 1, 'E', 'A380207');
INSERT INTO Seat VALUES(6, 1, 'F', 'A380207');
INSERT INTO Seat VALUES(7, 2, 'A', 'A380207');
INSERT INTO Seat VALUES(8, 2, 'B', 'A380207');
INSERT INTO Seat VALUES(9, 2, 'C', 'A380207');
INSERT INTO Seat VALUES(10, 2, 'D', 'A380207');

INSERT INTO Seat VALUES(1, 1, 'A', 'A380206');
INSERT INTO Seat VALUES(2, 1, 'B', 'A380206');
INSERT INTO Seat VALUES(3, 1, 'C', 'A380206');
INSERT INTO Seat VALUES(4, 1, 'D', 'A380206');
INSERT INTO Seat VALUES(5, 1, 'E', 'A380206');
INSERT INTO Seat VALUES(6, 1, 'F', 'A380206');
INSERT INTO Seat VALUES(7, 2, 'A', 'A380206');
INSERT INTO Seat VALUES(8, 2, 'B', 'A380206');
INSERT INTO Seat VALUES(9, 2, 'C', 'A380206');
INSERT INTO Seat VALUES(10, 2, 'D', 'A380206');

INSERT INTO Seat VALUES(1, 1, 'A', 'A380205');
INSERT INTO Seat VALUES(2, 1, 'B', 'A380205');
INSERT INTO Seat VALUES(3, 1, 'C', 'A380205');
INSERT INTO Seat VALUES(4, 1, 'D', 'A380205');
INSERT INTO Seat VALUES(5, 2, 'A', 'A380205');
INSERT INTO Seat VALUES(6, 2, 'B', 'A380205');
INSERT INTO Seat VALUES(7, 2, 'C', 'A380205');
INSERT INTO Seat VALUES(8, 2, 'D', 'A380205');
INSERT INTO Seat VALUES(9, 2, 'E', 'A380205');
INSERT INTO Seat VALUES(10, 2, 'F', 'A380205');

INSERT INTO Seat VALUES(1, 1, 'A', 'A380204');
INSERT INTO Seat VALUES(2, 1, 'B', 'A380204');
INSERT INTO Seat VALUES(3, 1, 'C', 'A380204');
INSERT INTO Seat VALUES(4, 1, 'D', 'A380204');
INSERT INTO Seat VALUES(5, 2, 'A', 'A380204');
INSERT INTO Seat VALUES(6, 2, 'B', 'A380204');
INSERT INTO Seat VALUES(7, 2, 'C', 'A380204');
INSERT INTO Seat VALUES(8, 2, 'D', 'A380204');
INSERT INTO Seat VALUES(9, 2, 'E', 'A380204');
INSERT INTO Seat VALUES(10, 2, 'F', 'A380204');

INSERT INTO Seat VALUES(1, 1, 'A', 'A380201');
INSERT INTO Seat VALUES(2, 1, 'B', 'A380201');
INSERT INTO Seat VALUES(3, 1, 'C', 'A380201');
INSERT INTO Seat VALUES(4, 1, 'D', 'A380201');
INSERT INTO Seat VALUES(5, 2, 'A', 'A380201');
INSERT INTO Seat VALUES(6, 2, 'B', 'A380201');
INSERT INTO Seat VALUES(7, 2, 'C', 'A380201');
INSERT INTO Seat VALUES(8, 2, 'D', 'A380201');
INSERT INTO Seat VALUES(9, 2, 'D', 'A380201');
INSERT INTO Seat VALUES(10, 2, 'D', 'A380201');

INSERT INTO Seat VALUES(1, 1, 'A', 'A380202');
INSERT INTO Seat VALUES(2, 1, 'B', 'A380202');
INSERT INTO Seat VALUES(3, 1, 'C', 'A380202');
INSERT INTO Seat VALUES(4, 1, 'D', 'A380202');
INSERT INTO Seat VALUES(5, 2, 'A', 'A380202');
INSERT INTO Seat VALUES(6, 2, 'B', 'A380202');
INSERT INTO Seat VALUES(7, 2, 'C', 'A380202');
INSERT INTO Seat VALUES(8, 2, 'D', 'A380202');
INSERT INTO Seat VALUES(9, 2, 'D', 'A380202');
INSERT INTO Seat VALUES(10, 2, 'D', 'A380202');

INSERT INTO Seat VALUES(1, 1, 'A', 'A380203');
INSERT INTO Seat VALUES(2, 1, 'B', 'A380203');
INSERT INTO Seat VALUES(3, 1, 'C', 'A380203');
INSERT INTO Seat VALUES(4, 1, 'D', 'A380203');
INSERT INTO Seat VALUES(5, 2, 'A', 'A380203');
INSERT INTO Seat VALUES(6, 2, 'B', 'A380203');
INSERT INTO Seat VALUES(7, 2, 'C', 'A380203');
INSERT INTO Seat VALUES(8, 2, 'D', 'A380203');
INSERT INTO Seat VALUES(9, 2, 'D', 'A380203');
INSERT INTO Seat VALUES(10, 2, 'D', 'A380203');

INSERT INTO Seat VALUES(1, 1, 'A', 'A380199');
INSERT INTO Seat VALUES(2, 1, 'B', 'A380199');
INSERT INTO Seat VALUES(3, 1, 'C', 'A380199');
INSERT INTO Seat VALUES(4, 1, 'D', 'A380199');
INSERT INTO Seat VALUES(5, 2, 'A', 'A380199');
INSERT INTO Seat VALUES(6, 2, 'B', 'A380199');
INSERT INTO Seat VALUES(7, 2, 'B', 'A380199');
INSERT INTO Seat VALUES(8, 2, 'B', 'A380199');
INSERT INTO Seat VALUES(9, 2, 'B', 'A380199');
INSERT INTO Seat VALUES(10, 2, 'B', 'A380199');

INSERT INTO Seat VALUES(1, 2, 'A', 'A380200');
INSERT INTO Seat VALUES(2, 2, 'B', 'A380200');
INSERT INTO Seat VALUES(3, 2, 'C', 'A380200');
INSERT INTO Seat VALUES(4, 2, 'D', 'A380200');
INSERT INTO Seat VALUES(5, 2, 'D', 'A380200');
INSERT INTO Seat VALUES(6, 2, 'D', 'A380200');
INSERT INTO Seat VALUES(7, 2, 'D', 'A380200');
INSERT INTO Seat VALUES(8, 2, 'D', 'A380200');
INSERT INTO Seat VALUES(9, 2, 'D', 'A380200');
INSERT INTO Seat VALUES(10, 2, 'D', 'A380200');


INSERT INTO Flying_from VALUES(2, 'The God');
INSERT INTO Flying_from VALUES(2, 'M3000');
INSERT INTO Flying_from VALUES(2, 'AirDream');
INSERT INTO Flying_from VALUES(2, 'B321');
INSERT INTO Flying_from VALUES(3, 'The God');
INSERT INTO Flying_from VALUES(3, 'M3000');
INSERT INTO Flying_from VALUES(4, 'AirDream');
INSERT INTO Flying_from VALUES(4, 'B321');
INSERT INTO Flying_from VALUES(6, '666');
INSERT INTO Flying_from VALUES(1, '747-800');
INSERT INTO Flying_from VALUES(1, 'A380');
INSERT INTO Flying_from VALUES(1, 'The God');
INSERT INTO Flying_from VALUES(1, 'M3000');
INSERT INTO Flying_from VALUES(5, '747-800');

INSERT INTO Flight VALUES(	1,
STR_TO_DATE('03-02-2015 12:25:00', '%d-%m-%Y %h:%i:%s'),
STR_TO_DATE('04-02-2015 01:25:00', '%d-%m-%Y %h:%i:%s'),
'Prague',
'New York',
'Air Berlin',
'xkacma03',
'A380200',
1);

INSERT INTO Flight VALUES(	12,
STR_TO_DATE('08-02-2011 18:25:00', '%d-%m-%Y %h:%i:%s'),
STR_TO_DATE('08-02-2011 23:25:00', '%d-%m-%Y %h:%i:%s'),
'Prague',
'Janov',
'Air 666',
'xkacma03',
'A386666',
6);

INSERT INTO Flight VALUES(	123,
STR_TO_DATE('11-08-2015 11:00:00', '%d-%m-%Y %h:%i:%s'),
STR_TO_DATE('11-08-2015 19:00:00', '%d-%m-%Y %h:%i:%s'),
'Prague',
'Moscow',
'Lufthansa',
'xkocou08',
'A380199',
2);

INSERT INTO Flight VALUES(	1234,
STR_TO_DATE('11-12-2014 04:00:00', '%d-%m-%Y %h:%i:%s'),
STR_TO_DATE('11-12-2014 15:00:00', '%d-%m-%Y %h:%i:%s'),
'Prague',
'Brno',
'Lufthansa',
'xkacma03',
'A380199',
1);

INSERT INTO Flight VALUES(	12345,
STR_TO_DATE('01-06-2014 01:00:00', '%d-%m-%Y %h:%i:%s'),
STR_TO_DATE('02-06-2014 12:00:00', '%d-%m-%Y %h:%i:%s'),
'Prague',
'New York',
'MAK air',
'xkacma03',
'A380199',
1);

INSERT INTO Flight VALUES(	123456,
STR_TO_DATE('08-06-2014 12:00:00', '%d-%m-%Y %h:%i:%s'),
STR_TO_DATE('08-06-2014 17:00:00', '%d-%m-%Y %h:%i:%s'),
'Prague',
'Olomouc',
'MAK air',
'xkacma03',
'A386666',
6);


INSERT INTO Internal_flight VALUES(	123456,
'Air Berlin',
'Vodka, Juice, Vine');

INSERT INTO International_flight VALUES(1234,
'Swimming pool, Bar, Restaurant, Jakuzzi',
'Hello America, Good Bye Russia, Welcome my soviet friend',
'Gordon blue, Apple pie, ...');


INSERT INTO Ticket VALUES(	999,
'Roman',
'Zkamene',
NULL,
7894561,
'T',
1234,
1,
'A380199',
50000);

INSERT INTO Ticket VALUES(	1000,
'Smradoch',
'Smrduty',
NULL,
7894569,
'T',
12345,
2,
'A380199',
50000);

INSERT INTO Flight VALUES(	1234567,
'8-6-2014 12:00:00',
'8-6-2014 17:00:00',
'Prague',
'Olomouc',
'MAK air',
'xkacma03',
'A380666',
6);


INSERT INTO Ticket VALUES(	10001,
'Rasto',
'Pisko',
'pisko@gmail.com',
7894562,
'F',
1234,
3,
'A380199',
50000);

INSERT INTO Ticket VALUES(	10002,
'Rasto',
'Pisko',
'pisko@gmail.com',
7894562,
'F',
123456,
4,
'A386666',
450005);

COMMIT;

