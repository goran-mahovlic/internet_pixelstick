# Picture converting app for use with internet pixel stick
# ver 1.0
# by Goran Mahovlić

# importing Python Magic
import PythonMagick as Magick
# putting image names in variable so we can manipulate with them
inputImage = "radiona.png"
outputImage = "new_radiona.png"
# creating object img and puting img in that object
img = Magick.Image(inputImage)
# printing filename od img object that now contains input image
print img.fileName()
# printing image type
print img.magick()
# print text
print "Size before converting"
# print image site
print img.size().width()
print img.size().height()
# print text
print "Converting width to 640"
# get image size and put it into geometry
geometry = img.size()
# put width in w and height in h
w, h = geometry.width(), geometry.height()
# set new width
new_width = 640
# getting factor to maintain aspect ratio
factor = new_width/float(w)
# set new height
new_height = int(h * factor)
# resize image with new data
img.resize("{}x{}".format(new_width, new_height))
# write image to output file
img.write(outputImage)
# putting new image in img object
img = Magick.Image(outputImage)
# printing new size
print img.size().width()
print img.size().height()