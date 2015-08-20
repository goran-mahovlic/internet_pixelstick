# Picture converting app for use with internet pixel stick using PIL
# ver 1.0
# Powerd by radiona.org && Goran Mahovlic

# importing Python Magic
from PIL import Image

# putting image names in variable so we can manipulate with them
file_in = "radiona.png"
file_out = "radiona.bmp"
# creating object img and puting img in that object
img = Image.open(file_in)
# print text
print "Size before converting"
# print image size
print img.size
# setting variable newwidth that holds new width value
newwidth = 100
# setting variable that holds new angle value
angle = -90
# getting factor to maintain aspect ratio
wpercent = (newwidth/float(img.size[0]))
# setting new hight with maintaing aspect ratio
hsize = int((float(img.size[1])*float(wpercent)))
# resizing imaga
img = img.resize((newwidth,hsize), Image.ANTIALIAS)
# rotate image
img = img.rotate(angle)
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
for x in range(hsize):
    for y in range(newwidth):
        # get RGB values of (x,y) pixel
        r, g, b = img.getpixel((x, y))
        # print pixel values
        print r, g, b