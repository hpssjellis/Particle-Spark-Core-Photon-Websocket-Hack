# Node.js Websocket Test


Active webpage at http://hpssjellis.github.io/Particle-Spark-Core-Photon-Websocket-Hack/


update Mar 28, 2017

Briefly it was not working, seems to be working now.


Load socket.ino onto the photon (wait 25 s for D7 to go out)

can use putty to monitor the serial connection.

run server.js using node js. (auto installation by running npm install)

using your node js server open index.html, enter ID and access code then click connect (uses the cloud to connect the websocket)

Then try the websocket commands can use any single character (case sensitive so should have 255 possible commands)
click the A button to light D7, the B button to turn off D7

Regualr cloud commands average at about 1200 ms these commands average about 10 ms.



If the above  works try the carsocket.ino and car-control.html pages for controlling a toy car.


.



.




.




.





.


























Originally a tiny demo using the [einaros/ws](http://einaros.github.io/ws/) WebSockets implementation.

I have changed this demo to work with the Spark Core. Flash the .ino file onto your spark core and then load this github site onto a cloud9 server. The web page buttons should activate the D7 LED on the spark core.

Note: the "connect" function will have to be activated to get the Particle.io Photon (formerly Spark.io Core) ready to recieive websocket style information.

Updated Oct4, 2015.
My bad, I should have given a method to activatet the connect function. I have added a new webpage called activation.html, which needs your core ID and access Token and then click the button "connect" before the Photon can talk to your nodeJs server.

#running on Cloud9 http://c9.io
make a login
Create A new NODEJS workspace
Clone from URL
https://github.com/hpssjellis/Particle-Spark-Core-Photon-Websocket-Hack.git

Open a terminal
npm install

right click on the server.js file
run

main menu --> preview --> preview running app

This will load the connection page but you will have to first load activation.html


Note. Users will have to run activation.html to enter their photon ID and access code before any of this will work. Then you can click the connect button to load the main index.html page that communicates with the photon

.

.

.

.

.

.

.

.

From the original Github heroku-examples/node-ws-test

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




To see a full discussion of this process check out the forum at https://community.particle.io/t/tcp-server-and-client-example-socket-programs-d7-on-please/11307

Note: Use at your own risk.






