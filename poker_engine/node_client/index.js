const { createServer } = require('node:http');
const express = require('express');
const path = require('path');
const poker_engine = require('./RustPokerEngine.linux-x64-gnu.node');

const hostname = 'localhost';
const port = 3000;

const app = express();

app.use(express.static('public'));


app.get('/', function(req, res) {
	res.sendFile(path.join(__dirname, '/index.html'));
	console.log(poker_engine.plus_100(56));
});

app.listen(port);

console.log('Server started at http://localhost:' + port);