var fs = require('fs'),
http = require('http'),
https = require('https'),
request = require('request'),
qs = require('querystring'),
osc = require('node-osc'),
url = require('url');
const dns = require('dns');
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
const addresses = new Map();
const options = {
    hints: dns.ADDRCONFIG | dns.V4MAPPED,
};
lookupaddresses = () => {
    lookupaddress('lady.local');
    lookupaddress('man.local');
}

lookupaddress = (host) => {
    dns.lookup(host, options, (err, address, family) => {
        if (!err) {
            addresses[host] = address;
            console.log('host %j, address: %j family: IPv%s',host, address, family);
        }
    });
}
lookupaddresses();
var interval = setInterval(() => {
    console.log('look up', addresses);
    if(addresses['lady.local'] !== undefined && addresses['man.local'] !== undefined && addresses['lady.local'] !== null && addresses['man.local'] !== null ){
        console.log('clearInterval')
        clearInterval(interval);
    } else {
        lookupaddresses();
    } 
},10000);

OSCserver.on('message', function (args) {
    //client.send({ message: '/lp/scene ' + args });
    // console.log("msg! arg[0] " + args[0]);
    console.log("msg! " + args);
    if (messages_detected[args[0]]) {
        const address = addresses[args[1]];
        if(address !== undefined) {
            var req = 'http://'+address+'/set?'+args[2]+'='+args[3];
            console.log('req:',req);
            request(req, {}, (err, res, body) => {
                if (err) { return console.log(err); }
                console.log("OK");
            });
        }
    }
});
