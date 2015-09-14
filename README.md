# internet_pixelstick
ESP8266 downloads data from server and show on pixelstick

Server will wait for new images in folder and if picture is available it will convert it to RGB, and wait on UDP sockets for message from ESP. ESP will ask is the picture ready every minute.

Server writes all responses on HTML page, and ESP reads that page.

If picture is ready ESP sends XY coordinates od pixels, server recives USP message and write XY on webpage. 

ESP then collect XY and displays it on the stick.
