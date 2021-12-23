# Tuchverdreher:in

## How to run

All your devices must be in the same network.

### Hardware

We used a Wemos D1 Mini and a TCM2208 on a breadboard. The powersupply for the motor has 24V.
Remember to set a current limit on the TCM2208.

| D1mini  	| TCM2208  	| 
|---	    |---	    |
| 3V  	    | VDD  	    |  
| GND   	| GND   	|   	
| GND   	| EN       	|   	
| D7    	| DIR   	|
| D8        | STEP      |

| NEMA17  	| TCM2208  	| 
|---	    |---	    |
| 1B  	    | 1B  	    |  
| 1A   	    | 1A     	|   	
| 2B    	| 2B       	|   	
| 2A    	| 2A    	|

| M.Power 	| TCM2208  	| 
|---	    |---	    |
| +  	    | + (M)     |  
| -   	    | GND (M)  	| 

### Software
Change the WIFI Settings and stepsPerRevolution in arduino\wemosd1mini_tcm2208_nema17.ino
Deploy arduino\wemosd1mini_tcm2208_nema17.ino to a Wemos D1 Mini or similar device.

You can control the device via HTTP_GET like 'http://192.168.84.34/Stop'


| Command 	                      | Description       	                  | 
|---	                          |---	                                  |
| /Stop                           | Stops the device                      |  
| /Init?Speed=500&Rotations=5     | Rotates 5x with speed 500             |      
| /Init?Speed=500&Steps=50000     | Rotates 50000 steps                   |      
| /Init?Speed=500&Accel=250       | Accelerates with 250/sec to speed 500 |      

### App 

Install dependencies with

```
npm -i
```

You could start the app in your browser with
```
npm start
```

or build the app use the following command:

```
ionic capacitor run android
```