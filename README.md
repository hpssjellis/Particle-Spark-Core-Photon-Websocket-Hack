# Node.js Websocket Test

update Mar 27, 2017

So presently this does not seem to be working. A student did get it working last year so I am not sure what is different.






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






