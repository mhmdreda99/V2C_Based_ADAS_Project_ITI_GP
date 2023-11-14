const Temperature = document.getElementById("Temperature");
const FuelLevel = document.getElementById("FuelLevel");
const TimeElapsed = document.getElementById("TimeElapsed");
const BSAStats = document.getElementById("BSAStats");
const LoadingMenu = document.getElementById("LoadingMenu");

//const DistanceTraveled = document.getElementById("DistanceTraveled");
var LastIndUsed=0,CoordinatesArr=[],currentData="";



////// Initializing Variables
L_NO_TOUCH = false;
L_DISABLE_3D = false;
var MapmarkerCount=0,PolyLine= [];
var LocationMap = L.map("LocationMap",{
    center: [30.566, 32.6666],
    crs: L.CRS.EPSG3857,
    zoom: 15,
    zoomControl: true,
    preferCanvas: false,
});
var MapTitle = L.tileLayer(
    "https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png",
    {"attribution": "Data by \u0026copy; \u003ca target=\"_blank\" href=\"http://openstreetmap.org\"\u003eOpenStreetMap\u003c/a\u003e, under \u003ca target=\"_blank\" href=\"http://www.openstreetmap.org/copyright\"\u003eODbL\u003c/a\u003e.", "detectRetina": false, "maxNativeZoom": 18, "maxZoom": 18, "minZoom": 0, "noWrap": false, "opacity": 1, "subdomains": "abc", "tms": false}
).addTo(LocationMap);


FetchNShowData();
setInterval(function(){FetchNShowData()},5000)


function FetchNShowData(){
    


    var DataToBSent = new FormData();
    DataToBSent.append('val', '1242ITI231');
    DataToBSent.append('LastInd', LastIndUsed);
    fetch('DataFetch.php', {method: 'POST', body: DataToBSent})
    .then(r => r.text())
    .then((Response)=>{

        //console.log(Response)


        //Response =[{"Temperature":149,"FuelLevel":0,"Elapsedtime":1430,"BSA_state":3,"Latitude":30.6209415,"longitude":32.268716,"No":1,"timestamps":1699621954388},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1476,"BSA_state":3,"Latitude":30.620923166666667,"longitude":32.268717333333335,"No":10,"timestamps":1699621999421},{"Temperature":148,"FuelLevel":4,"Elapsedtime":31,"BSA_state":9,"Latitude":30.6209115,"longitude":32.268716833333336,"No":11,"timestamps":1699622009426},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1490,"BSA_state":3,"Latitude":30.620902166666667,"longitude":32.2687175,"No":12,"timestamps":1699622014435},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1496,"BSA_state":3,"Latitude":30.620904333333332,"longitude":32.2687185,"No":13,"timestamps":1699622019444},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1500,"BSA_state":3,"Latitude":30.6209,"longitude":32.268718666666665,"No":14,"timestamps":1699622024450},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1506,"BSA_state":3,"Latitude":30.62089883333333,"longitude":32.268718,"No":15,"timestamps":1699622029452},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1510,"BSA_state":3,"Latitude":30.6208955,"longitude":32.268717333333335,"No":16,"timestamps":1699622034458},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1516,"BSA_state":3,"Latitude":30.620887833333335,"longitude":32.268717,"No":17,"timestamps":1699622039492},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1520,"BSA_state":3,"Latitude":30.620880833333334,"longitude":32.268716833333336,"No":18,"timestamps":1699622044467},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1526,"BSA_state":3,"Latitude":30.620875333333334,"longitude":32.268716833333336,"No":19,"timestamps":1699622049530},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1436,"BSA_state":3,"Latitude":30.620935666666668,"longitude":32.26871533333333,"No":2,"timestamps":1699621959411},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1530,"BSA_state":3,"Latitude":30.620863666666665,"longitude":32.268716,"No":20,"timestamps":1699622054480},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1536,"BSA_state":3,"Latitude":30.62086,"longitude":32.2687165,"No":21,"timestamps":1699622059486},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1540,"BSA_state":3,"Latitude":30.620858166666668,"longitude":32.26871716666667,"No":22,"timestamps":1699622064487},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1546,"BSA_state":3,"Latitude":30.620856333333332,"longitude":32.268716166666664,"No":23,"timestamps":1699622069495},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1550,"BSA_state":3,"Latitude":30.620851000000002,"longitude":32.26871583333333,"No":24,"timestamps":1699622074501},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1556,"BSA_state":3,"Latitude":30.620859,"longitude":32.26871383333334,"No":25,"timestamps":1699622079505},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1560,"BSA_state":3,"Latitude":30.620865666666667,"longitude":32.2687145,"No":26,"timestamps":1699622084512},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1566,"BSA_state":3,"Latitude":30.620867,"longitude":32.2687145,"No":27,"timestamps":1699622089514},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1572,"BSA_state":3,"Latitude":30.620863833333335,"longitude":32.26871383333334,"No":28,"timestamps":1699622094518},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1576,"BSA_state":3,"Latitude":30.620861833333333,"longitude":32.268713,"No":29,"timestamps":1699622099522},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1440,"BSA_state":3,"Latitude":30.620936166666667,"longitude":32.268716,"No":3,"timestamps":1699621964393},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1582,"BSA_state":3,"Latitude":30.620863666666665,"longitude":32.268711833333334,"No":30,"timestamps":1699622104525},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1586,"BSA_state":3,"Latitude":30.620863833333335,"longitude":32.26871216666667,"No":31,"timestamps":1699622109557},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1592,"BSA_state":3,"Latitude":30.6208545,"longitude":32.2687115,"No":32,"timestamps":1699622114531},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1596,"BSA_state":3,"Latitude":30.62084966666667,"longitude":32.2687115,"No":33,"timestamps":1699622119533},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1602,"BSA_state":3,"Latitude":30.620845,"longitude":32.26870983333333,"No":34,"timestamps":1699622124535},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1606,"BSA_state":3,"Latitude":30.620832833333335,"longitude":32.268708833333335,"No":35,"timestamps":1699622129551},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1612,"BSA_state":3,"Latitude":30.620831166666665,"longitude":32.26870983333333,"No":36,"timestamps":1699622134553},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1616,"BSA_state":3,"Latitude":30.62082566666667,"longitude":32.268709,"No":37,"timestamps":1699622139664},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1622,"BSA_state":3,"Latitude":30.620819,"longitude":32.26870866666667,"No":38,"timestamps":1699622144554},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1626,"BSA_state":3,"Latitude":30.620815333333333,"longitude":32.268709,"No":39,"timestamps":1699622149559},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1446,"BSA_state":3,"Latitude":30.620935333333332,"longitude":32.268716166666664,"No":4,"timestamps":1699621969397},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1632,"BSA_state":3,"Latitude":30.620812,"longitude":32.26870816666667,"No":40,"timestamps":1699622154569},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1636,"BSA_state":3,"Latitude":30.620810333333335,"longitude":32.2687075,"No":41,"timestamps":1699622159577},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1642,"BSA_state":3,"Latitude":30.620813166666668,"longitude":32.2687075,"No":42,"timestamps":1699622164591},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1646,"BSA_state":3,"Latitude":30.620813833333333,"longitude":32.2687075,"No":43,"timestamps":1699622169750},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1652,"BSA_state":3,"Latitude":30.620814333333332,"longitude":32.268707666666664,"No":44,"timestamps":1699622174600},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1656,"BSA_state":3,"Latitude":30.620810833333334,"longitude":32.26870683333333,"No":45,"timestamps":1699622179597},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1662,"BSA_state":3,"Latitude":30.620809333333334,"longitude":32.268707166666665,"No":46,"timestamps":1699622184598},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1666,"BSA_state":3,"Latitude":30.6208055,"longitude":32.26870733333333,"No":47,"timestamps":1699622189602},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1672,"BSA_state":3,"Latitude":30.620802833333332,"longitude":32.268706333333334,"No":48,"timestamps":1699622194603},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1676,"BSA_state":3,"Latitude":30.6208,"longitude":32.268705833333335,"No":49,"timestamps":1699622199603},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1450,"BSA_state":3,"Latitude":30.620936,"longitude":32.268717,"No":5,"timestamps":1699621974400},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1682,"BSA_state":3,"Latitude":30.620798,"longitude":32.268705833333335,"No":50,"timestamps":1699622204607},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1456,"BSA_state":3,"Latitude":30.620935166666666,"longitude":32.268717,"No":6,"timestamps":1699621979402},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1460,"BSA_state":3,"Latitude":30.6209335,"longitude":32.26871666666667,"No":7,"timestamps":1699621984409},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1466,"BSA_state":3,"Latitude":30.620931833333334,"longitude":32.26871666666667,"No":8,"timestamps":1699621989411},{"Temperature":149,"FuelLevel":0,"Elapsedtime":1470,"BSA_state":3,"Latitude":30.620927666666667,"longitude":32.268717,"No":9,"timestamps":1699621994417}]
        //Response = JSON.stringify(Response);
        //console.log(Response)


        //Remove This and remove DIE from PHP to run code//////////////////////////////////////
        /*Response = JSON.stringify([
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1430,\"BSA_state\":3,\"Latitude\":30.6209415,\"longitude\":32.268716,\"No\":1,\"timestamps\":1699621954388}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1476,\"BSA_state\":3,\"Latitude\":30.620923166666667,\"longitude\":32.268717333333335,\"No\":10,\"timestamps\":1699621999421}",
            "{\"Temperature\":148,\"FuelLevel\":4,\"Elapsedtime\":31,\"BSA_state\":9,\"Latitude\":30.6209115,\"longitude\":32.268716833333336,\"No\":11,\"timestamps\":1699622009426}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1490,\"BSA_state\":3,\"Latitude\":30.620902166666667,\"longitude\":32.2687175,\"No\":12,\"timestamps\":1699622014435}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1496,\"BSA_state\":3,\"Latitude\":30.620904333333332,\"longitude\":32.2687185,\"No\":13,\"timestamps\":1699622019444}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1500,\"BSA_state\":3,\"Latitude\":30.6209,\"longitude\":32.268718666666665,\"No\":14,\"timestamps\":1699622024450}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1506,\"BSA_state\":3,\"Latitude\":30.62089883333333,\"longitude\":32.268718,\"No\":15,\"timestamps\":1699622029452}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1510,\"BSA_state\":3,\"Latitude\":30.6208955,\"longitude\":32.268717333333335,\"No\":16,\"timestamps\":1699622034458}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1516,\"BSA_state\":3,\"Latitude\":30.620887833333335,\"longitude\":32.268717,\"No\":17,\"timestamps\":1699622039492}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1520,\"BSA_state\":3,\"Latitude\":30.620880833333334,\"longitude\":32.268716833333336,\"No\":18,\"timestamps\":1699622044467}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1526,\"BSA_state\":3,\"Latitude\":30.620875333333334,\"longitude\":32.268716833333336,\"No\":19,\"timestamps\":1699622049530}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1436,\"BSA_state\":3,\"Latitude\":30.620935666666668,\"longitude\":32.26871533333333,\"No\":2,\"timestamps\":1699621959411}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1530,\"BSA_state\":3,\"Latitude\":30.620863666666665,\"longitude\":32.268716,\"No\":20,\"timestamps\":1699622054480}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1536,\"BSA_state\":3,\"Latitude\":30.62086,\"longitude\":32.2687165,\"No\":21,\"timestamps\":1699622059486}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1540,\"BSA_state\":3,\"Latitude\":30.620858166666668,\"longitude\":32.26871716666667,\"No\":22,\"timestamps\":1699622064487}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1546,\"BSA_state\":3,\"Latitude\":30.620856333333332,\"longitude\":32.268716166666664,\"No\":23,\"timestamps\":1699622069495}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1550,\"BSA_state\":3,\"Latitude\":30.620851000000002,\"longitude\":32.26871583333333,\"No\":24,\"timestamps\":1699622074501}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1556,\"BSA_state\":3,\"Latitude\":30.620859,\"longitude\":32.26871383333334,\"No\":25,\"timestamps\":1699622079505}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1560,\"BSA_state\":3,\"Latitude\":30.620865666666667,\"longitude\":32.2687145,\"No\":26,\"timestamps\":1699622084512}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1566,\"BSA_state\":3,\"Latitude\":30.620867,\"longitude\":32.2687145,\"No\":27,\"timestamps\":1699622089514}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1572,\"BSA_state\":3,\"Latitude\":30.620863833333335,\"longitude\":32.26871383333334,\"No\":28,\"timestamps\":1699622094518}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1576,\"BSA_state\":3,\"Latitude\":30.620861833333333,\"longitude\":32.268713,\"No\":29,\"timestamps\":1699622099522}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1440,\"BSA_state\":3,\"Latitude\":30.620936166666667,\"longitude\":32.268716,\"No\":3,\"timestamps\":1699621964393}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1582,\"BSA_state\":3,\"Latitude\":30.620863666666665,\"longitude\":32.268711833333334,\"No\":30,\"timestamps\":1699622104525}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1586,\"BSA_state\":3,\"Latitude\":30.620863833333335,\"longitude\":32.26871216666667,\"No\":31,\"timestamps\":1699622109557}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1592,\"BSA_state\":3,\"Latitude\":30.6208545,\"longitude\":32.2687115,\"No\":32,\"timestamps\":1699622114531}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1596,\"BSA_state\":3,\"Latitude\":30.62084966666667,\"longitude\":32.2687115,\"No\":33,\"timestamps\":1699622119533}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1602,\"BSA_state\":3,\"Latitude\":30.620845,\"longitude\":32.26870983333333,\"No\":34,\"timestamps\":1699622124535}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1606,\"BSA_state\":3,\"Latitude\":30.620832833333335,\"longitude\":32.268708833333335,\"No\":35,\"timestamps\":1699622129551}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1612,\"BSA_state\":3,\"Latitude\":30.620831166666665,\"longitude\":32.26870983333333,\"No\":36,\"timestamps\":1699622134553}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1616,\"BSA_state\":3,\"Latitude\":30.62082566666667,\"longitude\":32.268709,\"No\":37,\"timestamps\":1699622139664}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1622,\"BSA_state\":3,\"Latitude\":30.620819,\"longitude\":32.26870866666667,\"No\":38,\"timestamps\":1699622144554}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1626,\"BSA_state\":3,\"Latitude\":30.620815333333333,\"longitude\":32.268709,\"No\":39,\"timestamps\":1699622149559}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1446,\"BSA_state\":3,\"Latitude\":30.620935333333332,\"longitude\":32.268716166666664,\"No\":4,\"timestamps\":1699621969397}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1632,\"BSA_state\":3,\"Latitude\":30.620812,\"longitude\":32.26870816666667,\"No\":40,\"timestamps\":1699622154569}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1636,\"BSA_state\":3,\"Latitude\":30.620810333333335,\"longitude\":32.2687075,\"No\":41,\"timestamps\":1699622159577}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1642,\"BSA_state\":3,\"Latitude\":30.620813166666668,\"longitude\":32.2687075,\"No\":42,\"timestamps\":1699622164591}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1646,\"BSA_state\":3,\"Latitude\":30.620813833333333,\"longitude\":32.2687075,\"No\":43,\"timestamps\":1699622169750}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1652,\"BSA_state\":3,\"Latitude\":30.620814333333332,\"longitude\":32.268707666666664,\"No\":44,\"timestamps\":1699622174600}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1656,\"BSA_state\":3,\"Latitude\":30.620810833333334,\"longitude\":32.26870683333333,\"No\":45,\"timestamps\":1699622179597}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1662,\"BSA_state\":3,\"Latitude\":30.620809333333334,\"longitude\":32.268707166666665,\"No\":46,\"timestamps\":1699622184598}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1666,\"BSA_state\":3,\"Latitude\":30.6208055,\"longitude\":32.26870733333333,\"No\":47,\"timestamps\":1699622189602}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1672,\"BSA_state\":3,\"Latitude\":30.620802833333332,\"longitude\":32.268706333333334,\"No\":48,\"timestamps\":1699622194603}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1676,\"BSA_state\":3,\"Latitude\":30.6208,\"longitude\":32.268705833333335,\"No\":49,\"timestamps\":1699622199603}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1450,\"BSA_state\":3,\"Latitude\":30.620936,\"longitude\":32.268717,\"No\":5,\"timestamps\":1699621974400}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1682,\"BSA_state\":3,\"Latitude\":30.620798,\"longitude\":32.268705833333335,\"No\":50,\"timestamps\":1699622204607}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1456,\"BSA_state\":3,\"Latitude\":30.620935166666666,\"longitude\":32.268717,\"No\":6,\"timestamps\":1699621979402}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1460,\"BSA_state\":3,\"Latitude\":30.6209335,\"longitude\":32.26871666666667,\"No\":7,\"timestamps\":1699621984409}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1466,\"BSA_state\":3,\"Latitude\":30.620931833333334,\"longitude\":32.26871666666667,\"No\":8,\"timestamps\":1699621989411}",
            "{\"Temperature\":149,\"FuelLevel\":0,\"Elapsedtime\":1470,\"BSA_state\":3,\"Latitude\":30.620927666666667,\"longitude\":32.268717,\"No\":9,\"timestamps\":1699621994417}"
        ]);*/


        ////////////////////Validate Response from PHP
        if(typeof(Response) == "string"){
            if(Response.search("ListingFailed")>=0){
                console.log("Cannot list S3 JSON Files");return 0;
            }else if(Response.search("FileFetchFailed")>=0){
                console.log("Cannot fetch S3 JSON File");return 0;
            }else if(Response ==""){
                console.log("Nothing New");return 0;
            }else{
                var ReturnedJSON = JSON.parse(Response);
                if(ReturnedJSON.length<=0){
                    console.log("Nothing New");return 0;
                }
            }
        }else{
            if(typeof(Response) == "array"){
                if(Response.length>0){
                    var ReturnedJSON = Response;
                }
            }
            
        }

        console.log("Triggered");


        ////////////////////Sort By No
        ReturnedJSON.sort(function(a, b) {
            if (JSON.parse(a).No < JSON.parse(b).No) {
                return -1;
            }
            if (JSON.parse(a).No > JSON.parse(b).No) {
                return 1;
            }
            return 0;
        });






        //////////// Assign Variables and update HTML
        
        for (var i = 0; i < ReturnedJSON.length; i++) {
            currentData = JSON.parse(ReturnedJSON[i]);
            //console.log(currentData)            
            CoordinatesArr.push([currentData.Latitude,currentData.longitude]);
        }

        LastIndUsed = currentData.No;
        Temperature.innerHTML = currentData.Temperature + "°C";
        FuelLevel.innerHTML = currentData.FuelLevel + "%";
        TimeElapsed.innerHTML = secondsToHms(currentData.Elapsedtime);
        BSAStats.innerHTML = (currentData.BSA_state == 3)?"Safe" : "Danger";
        ModifyLocationOnMap(CoordinatesArr);
        LoadingMenu.classList.add("hidden");
    });
}



//ModifyLocationOnMap([[30.566, 32.6666],[30.544, 32.5],[30.533, 32.49]]);
//console.log(secondsToHms(7000))
function secondsToHms(d) {
    d = Number(d);
    var h = Math.floor(d / 3600);
    var m = Math.floor(d % 3600 / 60);
    var s = Math.floor(d % 3600 % 60);

    var hDisplay = h > 0 ? h.toString().padStart(2,"0") +":":"";
    var mDisplay = m > 0 ? m.toString().padStart(2,"0") +":" : "";
    var sDisplay = s > 0 ? s.toString().padStart(2,"0") : "00";
    return hDisplay+mDisplay+sDisplay;
}
function ModifyLocationOnMap(JSONResp){
    for (var i = 0; i < JSONResp.length; i++) {
        //console.log(JSON.stringify(JSONResp[i]))
        eval('var Marker_' + MapmarkerCount + '= L.marker(' + JSON.stringify(JSONResp[i]) + ',{}).addTo(LocationMap);');
        MapmarkerCount++;
        PolyLine.push(JSONResp[i]);
    }
    var PolylineDraw = L.polyline(
        PolyLine,
        {"bubblingMouseEvents": true, "color": "#6e99f0", "dashArray": null, "dashOffset": null, "fill": false, "fillColor": "#6e99f0", "fillOpacity": 0.8, "fillRule": "evenodd", "lineCap": "round", "lineJoin": "round", "noClip": false, "opacity": 1.0, "smoothFactor": 1.0, "stroke": true, "weight": 6}
    ).addTo(LocationMap);
    LocationMap.panTo(new L.LatLng(JSONResp[JSONResp.length-1][0], JSONResp[JSONResp.length-1][1]));
}