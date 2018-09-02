var fs = require('fs'),
http = require('http'),
https = require('https'),
request = require('request'),
qs = require('querystring'),
osc = require('node-osc'),
url = require('url');
var XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;
var pos = process.argv.indexOf("-port")
var port = (pos != -1 && (process.argv.length > pos + 1) ? parseInt(process.argv[pos + 1]) : 4343);
var OSC_PORT = port + 1;
console.log("OSC Server in port: " + OSC_PORT);
var OSCserver = new osc.Server(OSC_PORT, '127.0.0.1');
var messages_detected = {
    "/LED": true,
    // "world": true,
    // "1": true,
    // "2": true,
    // "3": true,
};

OSCserver.on('message', function (args) {
    //client.send({ message: '/lp/scene ' + args });
    // console.log("msg! arg[0] " + args[0]);
    console.log("msg! " + args);
    if (messages_detected[args[0]]) {
        var req = 'http://'+args[1]+'/set?'+args[2]+'='+args[3];
        console.log('req:',req);
        request(req, {}, (err, res, body) => {
            if (err) { return console.log(err); }
            console.log(body.url);
            console.log(body.explanation);
        });
    }
});
