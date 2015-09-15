# Picture converting app for use with internet pixel stick using PIL
# ver 1.0
# Powerd by radiona.org && Goran Mahovlic

# importing os used for file manipulation
import os, glob, time, operator
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
pixelstick.html
def get_first_image(files, _invert=False):
    """ Find and return the oldest file of input file names.
    Only one wins tie. Values based on time distance from present.
    Use of `_invert` inverts logic to make this a youngest routine,
    to be used more clearly via `get_youngest_file`.
    """
    gt = operator.lt if _invert else operator.gt
    # Check for empty list.
    if not files:
        return None
    # Raw epoch distance.
    now = time.time()
    # Select first as arbitrary sentinel file, storing name and age.
    oldest = files[0], now - os.path.getctime(files[0])
    # Iterate over all remaining files.
    for f in files[1:]:
        age = now - os.path.getctime(f)
        if gt(age, oldest[1]):
            # Set new oldest.
            oldest = f, age
    # Return just the name of oldest file.
    return oldest[0]

def get_last_image(files):
    return get_first_image(files, _invert=True)

# creating function we will send picture file name to this function and this function will convert image
def convert_image( image ):
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
				
while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    print "received message:", data
    if data.find('S') == 0: # If S is recieved start check for picture in folder
    files = glob.glob('*.png')
	f = get_first_image(files)
    print (f)
	convert_image(f)
	# we do not need this file any more
    os.remove(f)
    # write "O" (OK) to file, now ESP will know it is ready to ask for picture data
	sendToText("O")