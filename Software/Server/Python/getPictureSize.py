import PythonMagick
image = PythonMagick.Image("radiona.png")
print image.fileName()
print image.magick()
print image.size().width()
print image.size().height()