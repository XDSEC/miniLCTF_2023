<?php
include "config.php";
include "sql.php";
error_reporting(0);
system("echo 'TestRoot!@#'|sudo -S chmod 644 ./*");
if (isset($_POST['id'])) {
    $id = $_POST['id'];
}
else {
    $id = 1;
}
$id = sql_filter($id);
$result = get_id($id, $conn);
$sql = "SELECT id FROM dbo.users WHERE id = $id";
?>
<!DOCTYPE html>
<html>
  <head>
    <title>CTF Challenge</title>
    <style>
      
      body {
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
        background-color: #f7f7f7;
      }

      form {
        display: flex;
        flex-direction: column;
        text-align: center;
        align-items: center;
        margin-bottom: 20px;
        padding: 20px;
        border-radius: 5px;
        box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.2);
        background-color: #fff;
      }
      label {
        margin-bottom: 10px;
        font-size: 18px;
        font-weight: bold;
      }
      input[type="text"] {
        width: 300px;
        height: 30px;
        font-size: 16px;
        margin-bottom: 10px;
        padding: 5px;
        border: 1px solid #ccc;
        border-radius: 5px;
      }
      input[type="submit"] {
        width: 100px;
        height: 30px;
        font-size: 16px;
        background-color: #4CAF50;
        color: white;
        border: none;
        border-radius: 5px;
        cursor: pointer;
      }

      #sql-statement {
        margin-bottom: 20px;
        padding: 20px;
        border-radius: 5px;
        box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.2);
        background-color: #fff;
        text-align: center;
        font-size: 16px;
        font-weight: bold;
        display: inline-block;
        max-width: 1000px;
        font-size: 25px;
      }

      #query-results {
        width: 100%;
        border-collapse: collapse;
      }
      #query-results th, #query-results td {
        padding: 10px;
        border: 1px solid #ccc;
      }
      #query-results th {
        font-weight: bold;
        text-align: center;
      }

      #container {
        display: flex;
        flex-direction: column;
        align-items: center;
        font-size: 20px;
      }
    </style>
  </head>
  <body>
    <div id="container">
      <form action="index.php" method="POST">
        <label for="id">ID:</label>
        <input type="text" id="id" name="id">
        <input type="submit" value="Submit">
      </form>
      <div id="sql-statement"><?php echo $sql?></div>
      <table id="query-results"><?php var_dump($result)?></table>
    </div>
  </body>
</html>
