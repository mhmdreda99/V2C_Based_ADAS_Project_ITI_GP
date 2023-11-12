<?php

session_start();

if (isset($_GET['logout'])) {
    session_destroy();
    unset($_SESSION['username']);
    header("location: login.php");die();
}
if (!isset($_SESSION['username'])) {
    header('location: login.php');die();
}

?>




<!DOCTYPE html>
<html>
<head>
    <meta charset='utf-8'>
    <meta http-equiv='X-UA-Compatible' content='IE=edge'>
    <title>ITI Nexus Dashboard</title>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <link rel='stylesheet' type='text/css' media='screen' href='css/style.css'>    
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Poppins:wght@400;500;600;700;800&display=swap" rel="stylesheet">





    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/leaflet@1.9.3/dist/leaflet.css"/>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/css/bootstrap.min.css"/>
    <link rel="stylesheet" href="https://netdna.bootstrapcdn.com/bootstrap/3.0.0/css/bootstrap.min.css"/>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/@fortawesome/fontawesome-free@6.2.0/css/all.min.css"/>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/Leaflet.awesome-markers/2.0.2/leaflet.awesome-markers.css"/>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/gh/python-visualization/folium/folium/templates/leaflet.awesome.rotate.min.css"/>
</head>
<body>
    <div class="FullCont">
        <img class="Logo" src="img/ITINexus.svg">
        <div class="DataContainer">
            
            <div class="SDataCont">
                <img src="img/cloudy-weather-3311758-2754892.png">
                <div>
                    <p id="Temperature">25°C</p>
                    <p>Temperature</p>
                </div>                
            </div>


            <div class="SDataCont">
                <img src="img/fuel-can-5019532-4185548.png">
                <div>
                    <p id="FuelLevel">25%</p>
                    <p>Fuel Level</p>
                </div>                
            </div>



            <!--<div class="SDataCont">
                <img src="img/scroll-map-8616025-6815679.png">
                <div>
                    <p id="DistanceTraveled">2 KM</p>
                    <p>Distance Traveled</p>
                </div>                
            </div>-->


            <div class="SDataCont">
                <img src="img/clock-8701712-7097715.webp">
                <div>
                    <p id="TimeElapsed">09:10</p>
                    <p>Elapsed Time</p>
                </div>                
            </div>

            <div class="SDataCont">
                <img src="img/motion-sensor-4035927-3342606.webp">
                <div>
                    <p id="BSAStats">Safe</p>
                    <p>Blind Spot Alert</p>
                </div>
            </div>



        </div>
        <div class="MapContainer">            
            <div class="folium-map" id="LocationMap" ></div>
        </div>
    </div>



    <div class="" id="LoadingMenu">
        <div class="WhiteBgContainer">
            <svg version="1.1" id="L9" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px" viewBox="0 0 100 100" enable-background="new 0 0 0 0" xml:space="preserve">
                <path d="M73,50c0-12.7-10.3-23-23-23S27,37.3,27,50 M30.9,50c0-10.5,8.5-19.1,19.1-19.1S69.1,39.5,69.1,50">
                    <animateTransform attributeName="transform" attributeType="XML" type="rotate" dur="1s" from="0 50 50" to="360 50 50" repeatCount="indefinite">
                    </animateTransform>
                </path>
            </svg>
        </div>
    </div>

    
    <script src="https://cdn.jsdelivr.net/npm/leaflet@1.9.3/dist/leaflet.js"></script>
    <script src="https://code.jquery.com/jquery-1.12.4.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.2/dist/js/bootstrap.bundle.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/Leaflet.awesome-markers/2.0.2/leaflet.awesome-markers.js"></script>
    <script src='js/script.js'></script>
    <script src='js/maps.js'></script>
</body>
</html>