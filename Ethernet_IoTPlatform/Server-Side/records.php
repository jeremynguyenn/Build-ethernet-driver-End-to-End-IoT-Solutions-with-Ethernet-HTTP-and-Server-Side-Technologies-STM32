<?php
     //Set the Content-Type of the HTTP response
    //Data should be interpretated as excel spreadsheet data
    header('Content-Type: application/xls');

    //Set content disposition to attachment, this tells the browser that
    //it is downloadable, give the file a name:  SensorValues.xls
    header('Content-Disposition: attachment; filename=SensorValues.xls');
?>
<table>
    <tr>
        <th>Date in GMT</th>
        <th>Sensor Value</th>
    </tr>

    <?php

        $selectDay = 0;
        //Check if 'selectDay1' is set and not empty in the GET parameters
        if(isset($_GET['selectDay1']) && $_GET['selectDay1'] != ""){

            //Convert 'selectDay1' from GET parameters to a timestamp
            $selectDay =  strtotime($_GET['selectDay1']);

        }
        else{
            //If 'selectDay1 is not provided, set default time to 12 hours ago
            $selectDay =  time()  - 43200; //43200seconds = 12hours
        }

        //Calculate the timestamp for the next day (24hours later) 
        $nextDay =  $selectDay + 86400; //86400 seconds = 24hours

        //Create a new PDO object to connect to 'SensorDB.sqlite'
        $db = new PDO('sqlite:SensorDB.sqlite');

        //Create SQL query to select readings between the chosen date and the next day
        $query = "SELECT * FROM readings WHERE date BETWEEN " . $selectDay .  " AND " . $nextDay . " ORDER BY date DESC";

        //Execute the query
        $result = $db->query($query );

        //Loop through each row of the result
        foreach ($result as $row){
            echo "<tr><td>". date('d/m/Y H:i:s', $row['date'])."</td>";
            echo "<td>". $row['sensorvalue'] . "</td></tr>";
        }
    
    ?>
</table>