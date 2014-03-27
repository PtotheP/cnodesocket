/*
 * Autor: Philipp Seitz
 * Contact: seitz.philipp@outlook.com
 */

//wird aufgerufen sobald die Seite geladen ist
$(document).ready(function() {
    init();
});

var socket;

function init() {

    console.log('Here we go');

    $('#button0').click(function() {
        script0();
    });

    initsocketio();
}

function initsocketio() {
    socket = io.connect('http://localhost');

    //Socket listener
    socket.on('changetext', function(data) {
        //Aktuallisiert den Text der DIV text
        $("#text").html(data.message);
        console.log(data);
    });
}

function script0() {
    socket.emit('message', {message: 'data'});
}


