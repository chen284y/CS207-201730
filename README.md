Introduction of Project
<p>This project is a modification of a traffic live map. It input traffic live data from google map, and represent it on a real map by LEDs in red or green colour. LEDs is putted in each intersection to provide traffic status. [1] If the traffic is crowded, the green LED will be turned off, and the red LED's intensity will be increased. Otherwise, red LED will be turned off and the green's intensity will be increased due to the traffic. The original project only provide 3 LEDs on the Golden Gate Bridge, and after modify my project has six LEDs and a SERVO arrow. </p><p>
<img src="https://raw.githubusercontent.com/chen284y/CS207-201730/img/IMG_0548.JPG" />
The live map will represent the two choices if the user wants to drive from U of R to Airport. The two possible way according to Google Map are :</p><p>
1. 23rd Ave - Albert St - Regina Ave. And</p><p>
2. Ring Road - Lewvan Drive.</p><p>
The second choice is 4 km longer than the first choice but it has the same amount of time costs. Additionally, the second choice only has one traffic light.</p><p>
<img src="https://raw.githubusercontent.com/chen284y/CS207-201730/img/IMG_0549.JPG" /></p><p>
That means if there are not many vehicles in the first choice, the first way will be good. But if the traffic is jammed in that area, the second choice will be better. This project will gather live traffic data from Google Map and represent the data on a real map by LED. It will accords data in following three intersections :</p><p>
1. Wascana Parkway & 23Ave</p><p>
2. 23Ave &Albert St</p><p>
3. Albert st & Regina Ave</p><p>
The status is represented from no vehicle to highest vehicle number by highest intensity with green LED to highest intensity with red LED. And also it will processes the data to generates the best way. The arrow controlled by our SERVO will point to the best way.</p><p>
  
Repository Contents
============
Here's where you'll provide a map of all the top-level contents (where applicable):

* **/img** - This is where the image files for this readme are! Yay!
* **/Representation.ino** - This is the code file of first draft showed in representation
* **/1st_Version.ino** - This is the code for first try of Google API Key
* **/2nd_Version.ino** - This is the last try of Google API Key input and process
* **/README.md** - The file you're reading now! :-D

Requirements and Materials
============

This is where you talk about what sort of libraries are required and what sort of hardware is needed:

Dependencies:
important libraries that need to be installed
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>
#include <GoogleMapsApi.h>
#include <ArduinoJson.h>
#include <DoubleResetDetector.h>
#include <Adafruit_NeoPixel.h>
#include <NTPClient.h>
#include <Servo.h>

Bill of Materials:
* 1 x Servo
* 3 x green LEDs
* 3 x red LEDs
* 6 x 220 ohm resistor
* 1 x breadboard
* 1 x Arduino Uno

<img src="https://raw.githubusercontent.com/chen284y/CS207-201730/img/IMG_0546.JPG" />
<img src="https://raw.githubusercontent.com/chen284y/CS207-201730/img/IMG_0547.JPG" />
<img src="https://raw.githubusercontent.com/chen284y/CS207-201730/img/IMG_0550.JPG" />
<img src="https://raw.githubusercontent.com/chen284y/CS207-201730/img/IMG_0551.JPG" />
<img src="https://raw.githubusercontent.com/chen284y/CS207-201730/img/IMG_0552.JPG" />
<img src="https://raw.githubusercontent.com/chen284y/CS207-201730/img/IMG_0553.JPG" />
<img src="https://raw.githubusercontent.com/chen284y/CS207-201730/img/IMG_0554.JPG" />
<img src="https://raw.githubusercontent.com/chen284y/CS207-201730/img/IMG_0555.JPG" />
<img src="https://raw.githubusercontent.com/chen284y/CS207-201730/img/IMG_0556.JPG" />
<img src="https://raw.githubusercontent.com/chen284y/CS207-201730/img/IMG_0557.JPG" />
