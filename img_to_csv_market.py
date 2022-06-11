#import statements
#region
import cv2
import pytesseract
import datetime
#endregion


#Variables
#region

#pytesseract config setups
number_only_config = r'-c tessedit_char_whitelist=0123456789 --oem 1 --psm 6'
number_decimals_config = r'-c tessedit_char_whitelist=0123456789. --oem 1 --psm 6'
flex_config = r'--oem 1 --psm 1'

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
havg_day_price1 = 907
havg_day_price2 = 1043
hrecent_price1 = 1077
hrecent_price2 = 1204
hlowest_price1 = 1239
hlowest_price2 = 1365
hcheapest_rem1 = 1399
hcheapest_rem2 = 1632
#endregion

#Rescale percentages
#region
all_rescale = 120
item_name_rescale = 100
avg_day_price_rescale = all_rescale
recent_price_rescale = all_rescale
lowest_price_rescale = all_rescale
cheapest_rem_rescale = all_rescale
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
    himagelist.append(image[vbound1:vbound2, havg_day_price1:havg_day_price2])
    himagelist.append(image[vbound1:vbound2, hrecent_price1:hrecent_price2])
    himagelist.append(image[vbound1:vbound2, hlowest_price1:hlowest_price2])
    himagelist.append(image[vbound1:vbound2, hcheapest_rem1:hcheapest_rem2])

    vimagelist.append(himagelist)

  return vimagelist

#Image to text line function
#Take in array of images of a split image, return csv text line
def read_image_line(himagelist):
  item_name = pytesseract.image_to_string(rescale(himagelist[0], item_name_rescale), config=flex_config).replace("\n", "")
  avg_day_price = pytesseract.image_to_string(rescale(himagelist[1], avg_day_price_rescale), config=number_decimals_config).replace("\n", "")
  recent_price = pytesseract.image_to_string(rescale(himagelist[2], recent_price_rescale), config=number_only_config).replace("\n", "")
  lowest_price = pytesseract.image_to_string(rescale(himagelist[3], lowest_price_rescale), config=number_only_config).replace("\n", "")
  cheapest_rem = pytesseract.image_to_string(rescale(himagelist[4], cheapest_rem_rescale), config=number_only_config).replace("\n", "")

  #Handle empty cases
  image_line_text = [item_name, avg_day_price, recent_price, lowest_price, cheapest_rem]
  if any([True for text in image_line_text if text == ""]):
    return None

  current_datetime = datetime.datetime.now()

  #item_name, avg_day_price, recent_price, lowest_price, cheapest_rem, year, month, day, hour, min, sec
  outfile = open("output.csv", "a")
  outfile.write(item_name + ", " + avg_day_price + ", " + recent_price + ", " + lowest_price + ", " + cheapest_rem + ", " 
        + str(current_datetime.year) + ", " + str(current_datetime.month) + ", " + str(current_datetime.day) + ", " 
        + str(current_datetime.hour) + ", " + str(current_datetime.minute) + ", " + str(current_datetime.second) + "\n")
  outfile.close()

#endregion






#load 1 image
#region
def get_grayscale(image):
  return cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
def get_threshold(image):
  return cv2.threshold(image, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)[1]

img = cv2.imread("test_images\Screenshot_220425_184214.jpg", 0)
#img = cv2.imread("test_images\Screenshot_220425_193605.jpg", 0)
#img = cv2.imread("test_images\Screenshot_220425_193610.jpg", 0)
#img = cv2.imread("test_images\Screenshot_220425_193622 (1).jpg", 0) #case with empty

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