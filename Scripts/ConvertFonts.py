
try:
	from PIL import Image
	import PIL
except ImportError:
	raise ImportError('This script requires module `pillow`\nTry `sudo pip install pillow`')
import os

# Script for converting fonts from BW format
# to white on transparent

RAW_RESOURCES_PATH = '../RawResources/font'
RESORCES_PATH = '../Resources'

DEST_FORMAT = '.bmp'

def conversion(color):
	value = color[0] + color[1] + color[2]
	if value < 127*3:
		return(255, 255, 255, 255)
	else:
		return(0, 0, 0, 0)

def convertFont(font, dest):
	font = font.convert('RGBA')
	size = font.size
	for x in range(size[0]):
		for y in range(size[1]):
			font.putpixel((x,y), conversion(font.getpixel((x,y))))
	font.save(dest)


def pack(srcDir, destDir):
	files = []
	for f in os.listdir(srcDir):
		fullPath = os.path.join(srcDir, f) 
		if os.path.isfile(fullPath):
			files.append(fullPath)

	for f in files:
		try:
			outFile = os.path.join(destDir, os.path.splitext(os.path.basename(f))[0] + DEST_FORMAT)
			img = Image.open(f)
			convertFont(img, outFile)
		except (IOError):
			print('PIL can not open file `{0}`. Skipping'.format(f))

pack(RAW_RESOURCES_PATH, RESORCES_PATH)