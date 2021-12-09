from PIL import Image, ImageEnhance
from threading import Thread
import math

def createNormalMap(path, eps, eps2):
    print('Start creating normal map')
    imageO = Image.open(path)
    image = imageO.convert('L')
    #image.save("BW.jpg", "jpeg")

    enhancer = ImageEnhance.Contrast(image)
    image = enhancer.enhance(0.3)
    #image.save("contrast.jpg", "jpeg")

    width = image.size[0]
    height = image.size[1]
    #print(width, height)

    for i in range(0, width):
        for j in range(0, height):
          #  dx = ((image.getpixel((i + 2 * eps, j)) - image.getpixel((i, j))) / 2 / eps -
          #       (image.getpixel((i, j)) - image.getpixel((i - 2 * eps, j))) / 2 / eps)
          #  dy = ((image.getpixel((i, j + 2 * eps2)) - image.getpixel((i, j))) / 2 / eps2 -
          #       (image.getpixel((i, j)) - image.getpixel((i, j - 2 * eps))) / 2 / eps2)

            if i < width - eps:
                dx = (image.getpixel((i + eps, j)) - image.getpixel((i - eps, j))) / eps / 2
            else:
                dx = (image.getpixel((i, j)) - image.getpixel((i - eps, j))) / eps

            if j < height - eps2:
                dy = (image.getpixel((i, j + eps2)) - image.getpixel((i, j - eps2))) / eps2 / 2
            else:
                dy = (image.getpixel((i, j)) - image.getpixel((i, j - eps2))) / eps2

          #  dx = (image.getpixel((i, j)) - image.getpixel((i - eps, j))) / eps
          #  dy = (image.getpixel((i, j)) - image.getpixel((i, j - eps2))) / eps2

            denominator = (dx ** 2 + dy ** 2 + 1) ** 0.5

            Nx = -dx / denominator
            Ny = -dy / denominator

            R = (Nx + 1) / 2
            G = (Ny + 1) / 2

            imageO.putpixel((i, j), (int(255 * R), int(255 * G), 255))

    imageO.save('normal.jpg', 'jpeg')
    print('Normal map was successfully created')

def createMetallicMap(path, eps, metall):
    print('Start creating metallic map')
    image = Image.open(path)
    out = Image.new("RGB", (image.size[0], image.size[1]), (0,0,0))

    if metall == True:
        for i in range (image.size[0]):
            for j in range (image.size[1]):
                pix = image.getpixel((i,j))
                if ( math.sqrt((pix[0] - pix[1])**2 + (pix[0] - pix[2])**2 + (pix[1] - pix[2])**2) < eps ):
                    out.putpixel((i,j), (255,255,255))

    out.save('metallic.jpg', 'jpeg')
    print('Metallic map was successfully created')

def createRoughnessMap(path, eps):
    print('Start creating roughness map')
    image = Image.open(path)
    out = image
    enhancer = ImageEnhance.Contrast(image)
    image = enhancer.enhance(eps)
    image = image.convert('L')

    for i in range(image.size[0]):
        for j in range(image.size[1]):
            pix = image.getpixel((i, j))
            out.putpixel((i, j), (255 - pix, 255 - pix, 255 - pix))

    out.save('roughness.jpg', 'jpeg')
    print('Roughness map was successfully created')


path = 'albedo.jpg'
t1 = Thread(target=createNormalMap, args=(path, 5, 5,))
t2 = Thread(target=createMetallicMap, args=(path, 10, True,))
t3 = Thread(target=createRoughnessMap, args=(path, 5,))

t1.start()
t2.start()
t3.start()

t1.join()
t2.join()
t3.join()