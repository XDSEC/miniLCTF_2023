<?php
function inquire($id, $conn) {
    $sql = "SELECT name FROM dbo.users WHERE id = convert(int,'$id')";
    $stmt = sqlsrv_query($conn, $sql);
    check_error_msg($stmt, $conn, $sql);
    $results = array();

    while ($row = sqlsrv_fetch_array($stmt, SQLSRV_FETCH_ASSOC)) {
        array_push($results, $row['name']);
    }

    return $results;
}


function get_id($number, $conn) {
    $sql = "SELECT id FROM dbo.users WHERE id = $number";
    $sql_commands = explode(";", $sql);
    $results = array();
    for ($i = 0; $i < count($sql_commands); $i++) {
        if (strlen(trim($sql_commands[$i])) > 0) {
            if ($i == 0) {
                $stmt = sqlsrv_query($conn, $sql_commands[$i]);
                check_error_msg($stmt, $conn, $sql_commands[$i]);
                while ($row = sqlsrv_fetch_array($stmt, SQLSRV_FETCH_ASSOC)) {
                    $results = array_merge($results, inquire($row['id'], $conn));
                }
            }
            else {
                $stmt = sqlsrv_query($conn, $sql_commands[$i]);
                check_error_msg($stmt, $conn, $sql_commands[$i]);
            }
        }
    }
    return $results;
}

function check_error_msg($stmt, $conn, $sql_string) {
    if ($stmt === false) {
        $errors = sqlsrv_errors();
        foreach ($errors as $error) {
            $error_message = "ERROR-MESSAGE: " . $error['message'] . "<br>" . "SQL-STATEMENT: " . $sql_string;
            $styled_message = "<div class='error-message'>" . $error_message . "</div>";
            die($styled_message);
        }
    }
}


function sql_filter($str) {
    $str = preg_replace('/\s+/', '', $str);
    $black_list = ['$', '%', '"', '\'', '<', '>', ' ', '*', '&', '\n', '\t', '\r\n', '\r', chr(0xC2).chr(0xA0), chr(0xE3).chr(0x80).chr(0x80), 'insert', 'alter', 'create', 'delete'];
    $str = str_replace($black_list, '', $str);
    return $str;
}
?>
<html>
<head>
    <title>CTF Challenge</title>
<style>
.error-message {
    background-color: #FFC0CB;
    color: #B22222;
    padding: 10px;
    border: 1px solid #B22222;
    margin-bottom: 10px;
}
</style>
</head>
</html>