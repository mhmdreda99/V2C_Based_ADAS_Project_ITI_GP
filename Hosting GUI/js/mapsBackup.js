L_NO_TOUCH = false;
L_DISABLE_3D = false;
var LocationMap = L.map("LocationMap",{
    center: [30.566, 32.6666],//////////////////////Edit using JSON file
    crs: L.CRS.EPSG3857,
    zoom: 11,
    zoomControl: true,
    preferCanvas: false,
});
var Str = "Vld=95837ABS45";
var MapTitle = L.tileLayer(
    "https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png",
    {"attribution": "Data by \u0026copy; \u003ca target=\"_blank\" href=\"http://openstreetmap.org\"\u003eOpenStreetMap\u003c/a\u003e, under \u003ca target=\"_blank\" href=\"http://www.openstreetmap.org/copyright\"\u003eODbL\u003c/a\u003e.", "detectRetina": false, "maxNativeZoom": 18, "maxZoom": 18, "minZoom": 0, "noWrap": false, "opacity": 1, "subdomains": "abc", "tms": false}
).addTo(LocationMap);

var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
        this.responseText; // This is my response
    }else{
        //Loading
    }
};
xhttp.open("GET", "DataFetch.php", true);
xhttp.setRequestHeader('Content-type', 'application/json');
xhttp.send(Str);






var MapmarkerCount=0;
var PolyLine= [];
var JSONResp = [[30.566, 32.6666],[30.544, 32.5],[30.533, 32.49]];
for (var i = 0; i < JSONResp.length; i++) {
    eval('var Marker_' + MapmarkerCount + '= L.marker(' + JSON.stringify(JSONResp[i]) + ',{}).addTo(LocationMap);');
    MapmarkerCount++;
    PolyLine.push(JSONResp[i]);
}


/*
var marker_99919917048115bfa6a8bbdf17d4a44b = L.marker([30.566, 32.6666],{}).addTo(LocationMap);
var marker_627ae24df1bedd06b5a5c192daa3564a = L.marker([30.544, 32.5],{}).addTo(LocationMap);
var marker_076ba1f1ae9f3f7148b87551c3694fec = L.marker([30.533, 32.49],{}).addTo(LocationMap);


var poly_line_78483f6042aa1bcf0f79820f0079d1cd = L.polyline(
    [[30.566, 32.6666], [30.544, 32.5], [30.533, 32.49]],
    {"bubblingMouseEvents": true, "color": "blue", "dashArray": null, "dashOffset": null, "fill": false, "fillColor": "blue", "fillOpacity": 0.2, "fillRule": "evenodd", "lineCap": "round", "lineJoin": "round", "noClip": false, "opacity": 1.0, "smoothFactor": 1.0, "stroke": true, "weight": 3}
).addTo(LocationMap);
*/



var poly_line_78483f6042aa1bcf0f79820f0079d1cd = L.polyline(
    PolyLine,
    {"bubblingMouseEvents": true, "color": "blue", "dashArray": null, "dashOffset": null, "fill": false, "fillColor": "blue", "fillOpacity": 0.2, "fillRule": "evenodd", "lineCap": "round", "lineJoin": "round", "noClip": false, "opacity": 1.0, "smoothFactor": 1.0, "stroke": true, "weight": 3}
).addTo(LocationMap);