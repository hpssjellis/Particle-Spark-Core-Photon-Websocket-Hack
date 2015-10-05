# Node.js Websocket Test

Originally a tiny demo using the [einaros/ws](http://einaros.github.io/ws/) WebSockets implementation.

I have changed this demo to work with the Spark Core. Flash the .ino file onto your spark core and then load this github site onto a cloud9 server. The web page buttons should activate the D7 LED on the spark core.

Note: the "connect" function will have to be activated to get the Particle.io Photon (formerly Spark.io Core) ready to recieive websocket style information.

#running on Cloud9 http://c9.io
make a login
Create A new workspace
Clone from URL
https://github.com/hpssjellis/node-ws-test

Open a terminal
npm install

right click on the server.js file
run

main menu-preview
preview running app


From the original Github

# Running Locally

``` bash
npm install
npm start
```




# Running on Heroku

``` bash
heroku create
git push heroku master
heroku open
```
