<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <!-- Include Bootstrap CSS -->
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/5.0.0-alpha1/css/bootstrap.min.css" rel="stylesheet">
    <!-- Add favicon -->
    <link rel="icon" href="favicon.ico" type="image/x-icon" />

    <style>
        body
        {
            background: #56CCF2;  /* fallback for old browsers */
            background: -webkit-linear-gradient(to right, #2F80ED, #56CCF2);  /* Chrome 10-25, Safari 5.1-6 */
            background: linear-gradient(to right, #2F80ED, #56CCF2); /* W3C, IE 10+/ Edge, Firefox 16+, Chrome 26+, Opera 12+, Safari 7+ */
            color: #404040;        
        }

       .main-header{
        text-align :  center;
        padding : 20px 0;
       } 
       .logo{
         height: 80px;
       }
       .container{
        background-color: rgba(255,255,255,0.8);
        border-radius: 15px;
        padding: 20px;
        margin-top:50px;
        box-shadow: 0 2px 10px rgba(0,0,0,0.1)
       }
       .btn:hover{
        background-color:#FF3D67;
        border-color: #FF3D67;
       }
     
       .form-control{
        border-radius: 0;
        background-color: #fff;
        color: #000;
       }
       .chart-container{
        height:  400px;
        width: 100%;
        margin: 20px auto;
        background-color: #fff;
        border-radius: 10px;
        box-shadow: 0 2px 10px rgba(0,0,0,0.1)
       }
       .table{
        background-color: #fff;
        box-shadow: 0 2px 10px rgba(0,0,0,0.1)
        color :#000;
        width: 100%;
        margin: 0 auto;
       }
       .table th,
       .table td{
         vertical-align: middle;
         text-align: center;
       }
       .table thead{
        background-color: #0BF4AE;
        color:#fff
       }
       .table tbody tr:nth-child(even){
        background-color:#F0F0F0;
       }

       .footer{
            background: #333;
            color: #fff;
            text-align: center;
            padding: 15px;
            position:  fixed;
            width: 100%;
            bottom: 0;
            font-size: 14px;

       }

    </style>
    <title>Sensor Readings</title>
</head>
<body>
    <div class="main-header">
    <img src="logo.png" alt="Company Logo" class="logo">
    </div>
    <div class= "container mt-4 p-4">
        <div class="row">
            <div class="col-md-6">
                <form action="/records.php" class="mb-3">
                    <div class="form-group">
                        <label for="selectDay1">Download Historical Data</label>
                        <div class="d-flex">
                            <input type="date" id="selectDay1" name= "selectDay1" class="form-control mr-2">
                            <input type="submit" value="Download" class="btn btn-primary">
                        </div>
                    </div>
                 </form>
            </div>

            <div class="col-md-6">
             <form action="/index.php">
                <div class="form-group">
                <label for="selectDay2">Display Historical Data</label>
                <div class="d-flex">
                    <input type="date" id="selectDay2" name= "selectDay2" class="form-control mr-2">
                    <input type="submit" value="Display" class="btn btn-primary">
               </div>

                </div>
             </form>
            </div>
         </div>
        <div class = "chart-container">
             <canvas id="myChart"></canvas>
        </div>
        <table class="table table-stripped">
            <thead>
                <tr>
                    <th>Date</th>
                    <th>Sensor Value</th>
                </tr>
            </thead>
            <tbody>
                <?php
                    $selectDay = 0;

                    //Check if 'selectDay2' is set and not empty in the GET parameters
                    if(isset($_GET['selectDay2']) && $_GET['selectDay2'] != ""){

                        //Convert 'selectDay2' from GET parameters to a timestamp
                        $selectDay =  strtotime($_GET['selectDay2']);

                    }
                    else{
                        //If 'selectDay2' is not provided, set default time to 12 hours ago
                        $selectDay =  time()  - 43200; //43200seconds = 12hours
                    }

                     //Calculate the timestamp for the next day (24hours later) 
                     $nextDay =  $selectDay + 86400; //86400 seconds = 24hours
                    
                     //Create a new PDO object to connect to SQLite database 'SensorDB.sqlite'
                     $db  = new PDO('sqlite:SensorDB.sqlite');
                    //Create SQL query to select readings between the chosen date and the next day
                     $query = "SELECT * FROM readings WHERE date BETWEEN " . $selectDay .  " AND " . $nextDay . " ORDER BY date DESC";
                    
                     //Execute the query
                     $result = $db->query($query);

                     //Loop through each row of the result
                     foreach ($result as $row){
                        echo "<tr><td>". date('d/m/Y H:i:s', $row['date'])."</td>";
                        echo "<td>". $row['sensorvalue'] . "</td></tr>";
                     }
               ?>

            </tbody>
        </table>
    </div>
 <?php 

   //Create query to select readings between two specific dates
   $query =  "SELECT * FROM readings WHERE date BETWEEN ". $selectDay . " AND " . $nextDay . " ORDER BY date ASC";
  
   //Execute query
   $result = $db->query($query);

   //Create a two-dimensional array called rows
   $rows[][] =  array();

   $rows[0][0] = "Date";
   $rows[0][1] = "Sensor Value";

   $count =  1;

   //Iterate over each record in the $result set
   foreach($result as $row){

    
        //For each record store the date(formatted as hour:minute) in the first column
        $rows[$count][0] =  date('H:i', $row['date']);

         //Store the sensor value  in the second column
        $rows[$count][1] =  intval( $row["sensorvalue"]);

        $count++;

   }

   $jsonTable =  json_encode($rows)
 ?>
 <!--Import Chart from CDN-->
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
<script>

    var ctx = document.getElementById('myChart').getContext('2d');
    
    //Create a new lin chart
    var myChart =  new Chart(ctx,{
        //Set type as line
        type: 'line',
        
        //Data for the chart
        data: {

            //Get labels(dates) for the chart from the first column of the $rows array
            labels: <?php echo json_encode(array_column($rows,0)); ?>,

            //Define the dataset for the chart
            datasets: [{
                label: 'Sensor Value',

                //Get sensor values for the chart from the second column of $rows array
                data: <?php echo json_encode(array_column($rows,1)); ?>,
                backgroundColor: 'rgba(11,244,57,1)',
                borderColor: 'rgba(11,244,57,1)',
                borderWidth: 1
            }]

        },

        //Options for customising appearance
        options:{

            responsive: true,
            maintainAspectRatio: false,
            scales:{
                y: {
                    beginAtZero: true
                }
            }
        }
    });
</script>
<div class='footer'>
    <p>©<?php echo date('Y'); ?> EmbeddedExpertIO | All rights reserved. </p>
</div>
</body>
</html>
