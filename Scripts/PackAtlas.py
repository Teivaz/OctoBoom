
try:
	from PIL import Image
	import PIL
except ImportError:
	raise ImportError('This script requires module `pillow`\nTry `sudo pip install pillow`')
import json, os

# Primitive script to pack sprites into atlas
# Needs optimization.

RAW_RESOURCES_PATH = '../RawResources/sprites'
RESORCES_PATH = '../Resources'

DEST_FORMAT = '.bmp'
CONFIG_NAME = 'config.json'

def packSprites(sprites):
	config = []
	atlasX = 755
	atlasY = 639
	atlas = Image.new('RGBA', (atlasX, atlasY), (0, 0, 0, 0))
	x = 0
	y = 0
	maxY = 0
	marginX = 1
	marginY = 1

	for sprite in sprites:
		name = sprite['file']
		img = sprite['img']
		size = img.size
		if(size[0] > atlasX or size[1] > atlasY):
			print('Error. Can not fit image `{0}` to atlas with size {1}'.format(name, size))
			continue

		if(size[0] > atlasX-x):
			#start new line
			y = maxY + marginY
			x = 0

		if(size[1] > atlasY-y):
			print('Error. No space left in the atlas for {0}'.format(name))
			continue
		atlas.paste(img, (x, y))
		config.append({
			'name' : os.path.basename(name),
			'w' : size[0],
			'h' : size[1],
			'x' : x,
			'y' : y
			})

		maxY = max(maxY, size[1]+y)
		x += size[0] + marginX

	return config, atlas


def pack(srcDir, destDir):
	files = []
	for f in os.listdir(srcDir):
		fullPath = os.path.join(srcDir, f) 
		if os.path.isfile(fullPath):
			files.append(fullPath)

	sprites = []
	for f in files:
		try:
			img = Image.open(f).convert('RGBA')
			sprites.append({
				'file' : f,
				'img' : img
				})
		except (IOError):
			print('PIL can not open file `{0}`. Skipping'.format(f))
	config, atlas = packSprites(sprites)
	atlasName = 'atlas' + DEST_FORMAT
	atlasPath = os.path.join(destDir, atlasName)
	atlas.save(atlasPath)

	config = {
	'atlas': atlasName,
	'sprites': config
	}

	configPath = os.path.join(destDir, CONFIG_NAME)
	configFile = open(configPath, 'w')
	configFile.write(json.dumps(config))
	configFile.close()

pack(RAW_RESOURCES_PATH, RESORCES_PATH)
