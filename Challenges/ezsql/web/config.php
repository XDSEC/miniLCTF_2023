<?php
$serverName = "db";
$connectionOptions = array(
    "Database" => "ctf",
    "Uid" => "sa",
    "PWD" => "P@ss1234word"
);

$conn = sqlsrv_connect($serverName, $connectionOptions);
if ($conn === false) {
    die(print_r(sqlsrv_errors(), true));
}