#import statements
#region
import cv2
import pytesseract
import numpy as np
#endregion

#Variables
#region

#pytesseract config setups
number_only_config = r'-c tessedit_char_whitelist=0123456789 --oem 1 --psm 6'
number_decimals_config = r'-c tessedit_char_whitelist=0123456789. --oem 1 --psm 6'
sub_flex_config = r'--oem 1 --psm 4'


#Coordinates for cutting out left/right sides
#region
vtop = 0
vbot = 1080
hleft = 718
hright = 1027
#endregion



#Rescale percentages
#region
sub_all_rescale = 140
#endregion

#endregion

#Functions
#region

#Rescale image function
#Returns image rescaled by scale_percent
def rescale(img, scale_percent):
  w = int((img.shape[1] * scale_percent)/100)
  h = int((img.shape[0] * scale_percent)/100)
  dimensions = (w, h)
  
  #resize image
  resized = cv2.resize(img, dimensions, interpolation = cv2.INTER_AREA) #change interpolation tag to vary results
  return resized

#Image cutting function
#Return the image with its sides cut out
def cut_image(img):
  return img[vtop:vbot, hleft:hright]

#Image to text line function
#Take in array of images of a split image, return csv text line
def read_image(img):
  output = pytesseract.image_to_string(rescale(img, sub_all_rescale), config=sub_flex_config)
  print(output)
  print("------------------")
  output = output.replace("\n\n", "\n")

  bonus_effect_split = output.split("Bonus Effect")[1].split("\n")
  stat1 = bonus_effect_split[1]
  stat2 = bonus_effect_split[2]
  print(stat1)
  print(stat2)

  random_engraving_effect_split = output.split("Random Engraving Effect")[1].split("\n")
  engraving1 = random_engraving_effect_split[1]
  engraving2 = random_engraving_effect_split[2]
  engraving3 = random_engraving_effect_split[3]
  print(engraving1)
  print(engraving2)
  print(engraving3)


#endregion





#load 1 image
#region
def get_grayscale(image):
  return cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
def get_threshold(image):
  return cv2.threshold(image, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)[1]

img = cv2.imread(r"C:\Users\Glyph\Documents\program\ocr1\test_images\accs\5.jpg")

alpha = 3#contrast value from 1 to 3
#beta = 0#brightness value from 0 to 100
img = cv2.convertScaleAbs(img, alpha=alpha, beta=0)

#image display for testing
cv2.imshow("threshold", img)
cv2.waitKey(0)
cv2.destroyAllWindows()

cv2.imwrite('result1.png',img)
img = get_grayscale(img)

#endregion

#go through one image
cut_image = cut_image(img)

#ocr the image and print results
read_image(cut_image)