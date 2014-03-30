var express = require('express');
var app = express();
var stylus = require('stylus');
var http = require('http');
var path = require('path');
var nib = require('nib');
var net = require('net');
var server = require('http').createServer(app);
var io = require('socket.io').listen(server);

function compile(str, path, fn) {
    return stylus(str)
            .set('filename', path)
            .set('compress', true);
}

app.use(stylus.middleware({
    src: __dirname + '/public/stylesheets/',
    compile: compile
}));

app.configure(function() {
    app.set('port', process.env.PORT || 3000);
    app.set('views', __dirname + '/views');
    app.set('view engine', 'jade');
    app.use(express.logger('dev'));
    app.use(stylus.middleware({
        src: __dirname + '/public',
        dest: __dirname + '/public',
        compile: function(str, path, fn) {
            return stylus(str)
                    .set('filename', path)
                    .set('compress', true);
        }}
    ));

    app.use(express.static(__dirname + '/public'));
    app.use(express.cookieParser());
    app.use(express.session({secret: 'secret', key: 'express.sid'}));
    app.use(express.bodyParser());
});


//ROUTING
app.get('/', function(req, res) {
    res.render('index', {title: 'INDEX'});
});

//Server fängt an zu listen
server.listen(app.get('port'), function() {
    console.log('Express server listening on port ' + app.get('port'));
});

/*
 * IO - Sockets
 * Hier kommt der spannende Teil
 */

//Dass des socket.io ned so rummüllt in der Konsole
io.set('log level', 1);

//erstellt neuen Socket für den cserver
var cserver = new net.Socket();
//Adresse des cservers
var HOST = '127.0.0.1';
var PORT = 5000;

//Verbindet sich mit dem cserver
cserver.connect(PORT, HOST, function() {
});
//Genereller Listener vom cserver
cserver.on('data', function(data){
    console.log(data.toString());
});

/*
 * Brücke zwischen socket.io und dem socket in c
 */

//Listener für eingehende 'Connections' vom Browser
io.sockets.on('connection', function(socket) {
    console.log('eingehende Browserverbindung');
    console.log(socket.id);
    //Listener vom cserver
    cserver.on('data', function(data) {
        //Leitet die empfangenen Daten an den Browser weiter
        socket.emit('changetext', {message: data.toString()});
    });
    //Listener vom Browser
    socket.on('message', function(data) {
        //Leitet die empfangenen Daten an den cserver weiter
        cserver.write(data.message);
    });
});

io.sockets.on('disconnect', function() {

});