#import statements
#region
import cv2
import pytesseract
#endregion


#Variables
#region

#pytesseract config setups
number_only_config = r'-c tessedit_char_whitelist=0123456789 --oem 1 --psm 7'
number_decimals_config = r'-c tessedit_char_whitelist=0123456789. --oem 1 --psm 7'
master_flex_config = r'--oem 1 --psm 1'

#Vertical coordinate
#region
vstart_top = 304 #Initial top vertical bound
vstart_bot = 360
vheight = 57 #Vertical box height
#endregion

#Horizontal coordinate
#Always the same, unlike vertical coords
#region
hitem_name1 = 605
hitem_name2 = 904
hquality1 = 988
hquality2 = 1147
hprice1 = 1465
hprice2 = 1632
#endregion

#Rescale percentages
#region
master_all_rescale = 120
item_name_rescale = 100
quality_rescale = master_all_rescale
price_rescale = master_all_rescale
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

#Image splitting function
#Return the image split into a vertically organized list of
#lists of split horizontal images
def split_image(image):
  num_vertical_lines = 10
  vimagelist = []

  for i in range(num_vertical_lines):
    himagelist = []
    vbound1 = vstart_top + i * vheight
    vbound2 = vstart_bot + i * vheight

    himagelist.append(image[vbound1:vbound2, hitem_name1:hitem_name2])
    himagelist.append(image[vbound1:vbound2, hquality1:hquality2])
    himagelist.append(image[vbound1:vbound2, hprice1:hprice2])

    vimagelist.append(himagelist)

  return vimagelist

#Image to text line function
#Take in array of images of a split image of a single line, return csv text line
def read_image_line(himagelist):
  item_name = pytesseract.image_to_string(rescale(himagelist[0], item_name_rescale), config=master_flex_config).replace("\n", "").split("[")[0]
  quality = pytesseract.image_to_string(rescale(himagelist[1], quality_rescale), config=number_only_config).replace("\n", "")
  price = pytesseract.image_to_string(rescale(himagelist[2], price_rescale), config=number_only_config).replace("\n", "")

  #Handle empty cases
  image_line_text = [item_name, quality, price]
  #if any([True for text in image_line_text if text == ""]):
  #  return None

  #item_name, quality, price
  print(item_name + ", " + quality + ", " + price)

#endregion






#load 1 image
#region
def get_grayscale(image):
  return cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
def get_threshold(image):
  return cv2.threshold(image, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)[1]

#img = cv2.imread(r"C:\Users\Glyph\Documents\program\ocr1\test_images\accs\0.jpg", 0)
img = cv2.imread(r"C:\Users\Glyph\Documents\program\ocr_lostark\test_images\batch_acc_test\Screenshot_220610_181835.jpg", 0)
img = get_threshold(img)


#image display for testing
cv2.imshow("threshold", img)
cv2.waitKey(0)
cv2.destroyAllWindows()


#endregion

#go through one image
splitted_image = split_image(img)

#go through splitted of the one image
for line in splitted_image:
  read_image_line(line)