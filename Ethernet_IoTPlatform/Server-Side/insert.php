<?php
    $key ="";
    $sensorvalue = "0";
    //Check if 'key' is provided in the POST request
    //if so assign value to $key
    if(isset($_POST['key'])) $key = $_POST['key'];

    //Check if 'sensorvalue' is provided in the POST request
    //if so assign value to $sensorvalue
    if(isset($_POST['sensorvalue'])) $sensorvalue = $_POST['sensorvalue'];

    //Check if key provided is equal to '4326'
    if($key == '4326'){

        //Get current unix timestamp
        $date =  time();

        //convert sensorvalue to integer
        $intsensorvalue = intval($sensorvalue);

        //Create a new PDO instance that connects to an SQLite 
        //database called SensorDB.sqlite
        $db =  new PDO('sqlite:SensorDB.sqlite');


        //Create a table called 'readings' if it doesn't exist already
        //The table should have two columns; date and sensorvalue
        $db->exec("CREATE TABLE IF NOT EXISTS readings(date INTEGER, sensorvalue INTEGER);");

        //Insert a new row into the 'readings' table 
        //The current timestamp and integer sensor value are inserted
        $db->exec("INSERT INTO readings(date, sensorvalue) VALUES('$date','$intsensorvalue');");
        
        //Ok to indicate script executed successfully.
        echo "ok";
    }




?>