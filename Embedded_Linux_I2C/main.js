function httpGet(url, func)
{
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() { 
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
            func(xmlHttp.responseText);
    }
    xmlHttp.open("GET", url, true); // true for asynchronous 
    xmlHttp.send(null);
}

function updateTemperature(temp) {
    document.getElementById('temp').cells[1].appendChild(document.createTextNode(temp + 'C'));
    console.log("Temperature : " + temp);
}

function updateHumdity(humid) {
    document.getElementById('humid').cells[1].appendChild(document.createTextNode(humid + '%'));
    console.log("Humidity : " + humid);
}

setInterval(function () {
    httpGet("cgi-bin/temp", updateTemperature);
    httpGet("cgi-bin/humid", updateHumdity);
}, 1000);

updateTemperature(12.5);
updateHumdity(125);
