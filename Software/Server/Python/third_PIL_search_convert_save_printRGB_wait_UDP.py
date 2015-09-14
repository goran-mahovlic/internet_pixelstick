# Picture converting app for use with internet pixel stick using PIL
# ver 1.0
# Powerd by radiona.org && Goran Mahovlic

# importing os used for file manipulation
import os
import socket
# importing PIL
from PIL import Image

UDP_IP = "178.62.187.251" # server IP 
UDP_PORT = 2390 # server port
sock = socket.socket(socket.AF_INET, # Internet
socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

# creating name for output bmp file
file_out = "picture.bmp"

# creating function that will save pixel data to text file(webpage)
def sendToText(text):
    text_file = open("/var/www/pixelstick.html", "w")
    text_file.write(str(text))

# creating function we will send picture file name to this function and this function will convert image
def convert( image ):
	img = Image.open(image)
	# print text
	print "Size before converting"
	# print image size
	print img.size
	# setting variable newwidth that holds new width value 
	newwidth = 60
	# setting variable that holds new angle value
	angle = -90
	# getting factor to maintain aspect ratio
	wpercent = (newwidth/float(img.size[0]))
	# setting new hight with maintaing aspect ratio
	hsize = int((float(img.size[1])*float(wpercent)))
        # rotate image
        img = img.rotate(angle)
	# resizing imaga
	img = img.resize((newwidth,hsize), Image.ANTIALIAS)
	# split image to rgb value needed for saving into bmp
	r, g, b = img.split()
	# merging all rgb data into img 
	img = Image.merge("RGB", (r, g, b))
	# save bmp file
	img.save(file_out)
	# get new image size
	print img.size
	# print text
	print "Get x and y color of each pixel"
	# get each pixel value and print it
	for x in range(newwidth):
		for y in range(hsize):
        	# get RGB values of (x,y) pixel
        		r, g, b = img.getpixel((x, y))
        		# print pixel values we will sore it later and send it to pixelstick
        		print r, g, b
				
# search for all files in folder
files = [f for f in os.listdir('.') if os.path.isfile(f)]
# for each file find in folder
for f in files:
	# if file is ending with .bmp
        if f.endswith(".bmp"):
                # print text
                print "If bmp is found print it and remove it"
                # print filename
                print(f)
                # we do not want bmp files so we will only remove it
                os.remove(f)
        # if file is ending with .png
        elif f.endswith(".png"):
                print"If png is found we need to convert it to bmp and remove it"
                print(f)
		# now we pass finded .png file to convert function that will convert it to rgb values 
                convert(f)
		# we do not need this file any more
                os.remove(f)
        # if file is ending with .jpg
        elif f.endswith(".jpg"):
                print"If jpg is found we need to convert it to bmp and remove it"
                print(f)
                # same as with .png we pass finded .jpg file to convert function that will convert it to rgb values
                convert(f)
		# and remove it
                os.remove(f)
				
while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    print "received message:", data
    sendToText(data)

