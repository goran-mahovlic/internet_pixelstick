# Picture converting app for use with internet pixel stick using PIL
# ver 1.0
# Powerd by radiona.org && Goran Mahovlic

# importing os used for file manipulation
import os, glob, time, operator
import socket
# importing PIL
from PIL import Image
import array

UDP_IP = "178.62.187.251" # server IP 
UDP_PORT = 2390 # server port
sock = socket.socket(socket.AF_INET, # Internet
socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

# creating name for output bmp file
file_out = "picture.bmp"

# save pixel data to text file(webpage)
def appendToText(text):
    text_file = open("/var/www/picture.text", "a")
    text_file.write(str(text))

def writeToText(text):
    text_file = open("/var/www/picture.text", "w")
    text_file.write(str(text))

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
def convert_save_image( image ):
    img = Image.open(image)
    # setting variable newwidth that holds new width value 
    newwidth = 50
    # setting variable that holds new angle value
    angle = -90
    # rotate image
    img = img.rotate(angle)
    # getting factor to maintain aspect ratio
    wpercent = (newwidth/float(img.size[0]))
    # setting new hight with maintaing aspect ratio
    hsize = int((float(img.size[1])*float(wpercent)))
    # resizing imaga
    img = img.resize((newwidth,hsize), Image.ANTIALIAS)
    # split image to rgb value needed for saving into bmp
    r, g, b = img.split()
    # merging all rgb data into img 
    img = Image.merge("RGB", (r, g, b))
    # save bmp file
    img.save(file_out)

# get size of saved bmp image
def get_bmp_picture_size():
    img = Image.open(file_out)
    width, height = img.size
    return width,height

# get rgb values of saved bmp image
def get_bmp_X_Y(x,y):
    img = Image.open(file_out)
    r, g, b = img.getpixel((x, y))
    return (r,g,b)
		
notDone = False
pictureReady = False	
#sendToText("No picture")
pixelGroup = ""
#newFileBytes = array.array('B')
RGB = []
while True:
    data, addr = sock.recvfrom(10240) # buffer size is 1024 bytes
    print data
    if data.find("Start") == 0: # If S is recieved start check for picture in folder
    	files = glob.glob('*.png')
	f = get_first_image(files)
        print (f)        
        if f is None:
            print("No picture")
        else:
            # we convert image and save it to bmp so we can get XY
	    convert_save_image(f)
	    # we do not need this file any more
#    	    os.remove(f)
            # write "O" (OK) to file, now ESP will know it is ready to ask for picture data
	    print("Picture is ready")
            width, hight = get_bmp_picture_size()
            print width, hight
            writeToText("")
            newFileBytes = array.array('B')
            for y in range (0,hight):
               appendToText(",")
               for x in range(0, 50):
                  #newFileBytes.append(get_bmp_X_Y(x,y))
                  pixelGroup = pixelGroup + str(get_bmp_X_Y(x,y))
                  pixelGroup = pixelGroup.replace("(", "")
                  pixelGroup = pixelGroup.replace(")", "")
                  pixelGroup = pixelGroup.replace(" ", "")
                  #pixelGroup = pixelGroup + ","
                  RGB = pixelGroup.split(',')
                  if int(RGB[0]) == 255:
                      newFileBytes.append(254)
                  else:
                      newFileBytes.append(int(RGB[0]))
                  if int(RGB[1]) == 255:
                      newFileBytes.append(254)
                  else:
                      newFileBytes.append(int(RGB[1]))
                  if int(RGB[2]) == 255:
                      newFileBytes.append(254)
                  else:
                      newFileBytes.append(int(RGB[2]))
#                  newFileBytes.append(RGB[0])
#                  newFileBytes.append(RGB[1])
#                  newFileBytes.append(RGB[2])
                  pixelGroup = "" 
#               print newFileBytes
               #newFileByteArray = bytearray(newFileBytes)
               #newfile=open("/var/www/picture.bin","wb")
               #newfile.write(newFileBytes)
               f = file('/var/www/picture.bin', 'wb')
               newFileBytes.tofile(f)
               f.close()
               appendToText(pixelGroup + ","  + "\n")
 #              print newFileBytes
               pixelGroup = ""
            print "All done"
            print hight
    elif data.find("Delete") == 0: # If D is recieved delete BMP file and empty HTML file
        print ("BMP deleted")
        #sendToText("All Done")
        #os.remove(file_out)
