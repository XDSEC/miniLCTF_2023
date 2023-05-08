CREATE DATABASE ctf;
GO

USE ctf;
GO

CREATE TABLE dbo.users (
    id INT NOT NULL,
    name VARCHAR (50) NOT NULL
);
GO

INSERT INTO dbo.users VALUES (1, 'admin');
INSERT INTO dbo.users VALUES (2, 'MiNl');
INSERT INTO dbo.users VALUES (3, 'L');
INSERT INTO dbo.users VALUES (4, '2023');
GO

backup database ctf to disk = '/tmp/bak.bak';
GO
