/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var net = require('net');

var HOST = '127.0.0.1';
var PORT = 5000;

var client = new net.Socket();

exports.connect = function(){
    client.connect(PORT, HOST, function() {

    });
};

exports.on = client.on;