var WebSocketServer = require("ws").Server
var http = require("http")
var express = require("express")
var app = express()
var port = process.env.PORT || 8080

app.use(express.static(__dirname + "/"))

var server = http.createServer(app)
server.listen(port)


console.log("http server listening on %d", port)

var wss = new WebSocketServer({server: server})
console.log("websocket server created")


wss.myBroadcast = function(data) {
  
  // I think for security make this a "for" loop with just 2 or 3 connections instead of anyone.
  // for (var i = 0; i <= 1; i++){  // this would allow one webpage and one photon
  for (var i in this.clients){
    
    
     this.clients[i].send(data);
     console.log('sent to client[' + i + '] ' + data);
  }
};


wss.on("connection", function(ws) {
  var id = setInterval(function() {
      console.log("send ping: C")
      //ws.send(JSON.stringify(new Date()), function() {  })
      ws.send("C",function() {  })
    
  }, 12000)   
  
  
  console.log("websocket connection open " )
  
  ws.on('message', function(message) {
      console.log('received: %s', message);
      wss.myBroadcast(message);  
  });  
        

  ws.on("close", function() {
     console.log("websocket connection close")
     clearInterval(id)
  })
  
  
})  // end wss.on
