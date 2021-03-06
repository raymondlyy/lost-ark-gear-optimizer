#import statements
#region
import cv2
import pytesseract
import numpy as np
import glob
import math
#endregion

#Variables
#region

#pytesseract config setups
number_only_config = r'-c tessedit_char_whitelist=0123456789 --oem 1 --psm 6'
quality_config = r'-c tessedit_char_whitelist=0123456789 --oem 1 --psm 7'
price_config = r'-c tessedit_char_whitelist=0123456789 --oem 1 --psm 7'
number_decimals_config = r'-c tessedit_char_whitelist=0123456789. --oem 1 --psm 6'
master_flex_config = r'--oem 1 --psm 1'
sub_flex_config = r'--oem 1 --psm 4'

#Master image coords
#Vertical coords
#region
vstart_top = 304 #Initial top vertical bound
vstart_bot = 360
vheight = 57 #Vertical box height
#endregion
#Horizontal coords
#region
hitem_name1 = 605
hitem_name2 = 904
hquality1 = 988
hquality2 = 1147
hprice1 = 1465
hprice2 = 1601
#endregion

#Sub image coords
#region
vtop = 0
vbot = 1080
hleft = 718
hright = 1027
#endregion

#Rescale percentages
#region
master_all_rescale = 120
sub_all_rescale = 140
item_name_rescale = 100
quality_rescale = 110
price_rescale = master_all_rescale
#endregion

#endregion

#Image pre-processing functions
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

#Grayscale image function
#Returns grayscaled image
def get_grayscale(image):
  return cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

#Threshold image function
#Returns Thresholded image
def get_threshold(image):
  return cv2.threshold(image, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)[1]

#Contrast increase function
#Returns image with contrast increased
def get_contrasted(image):
    alpha = 3#contrast value from 1 to 3
    #beta = 0#brightness value from 0 to 100
    return cv2.convertScaleAbs(image, alpha=alpha, beta=0)

#Master image splitting function
#Return the image split into a vertically organized list of
#lists of split horizontal images
def split_image(image):
  num_vertical_lines = 10
  vimagelist = []

  for i in range(num_vertical_lines):
    himagelist = []
    vbound1 = vstart_top + i * vheight
    vbound2 = vstart_bot + i * vheight
    vqualityadd = 28 #Cut out quality bar to only have quality number
    vitemnameminus = 24 #Cut number of times tradable to only have item name

    himagelist.append(image[vbound1:vbound2-vitemnameminus, hitem_name1:hitem_name2])
    himagelist.append(image[(vbound1+vqualityadd):vbound2, hquality1:hquality2])
    himagelist.append(image[vbound1:vbound2, hprice1:hprice2])

    vimagelist.append(himagelist)

  return vimagelist

#Sub image cutting function
#Return the image with its sides cut out
def cut_image(img):
  return img[vtop:vbot, hleft:hright]

#endregion

#Image reading functions
#region

#Master image line to text line function
#Take in array of images of a split image of a single line
#Output data to be used in sub-image csv lines + return number of sub-images
def read_image_line(himagelist):
  item_name = pytesseract.image_to_string(rescale(himagelist[0], item_name_rescale), config=master_flex_config).replace("\n", "").split("[")[0]
  quality = pytesseract.image_to_string(rescale(himagelist[1], quality_rescale), config=quality_config).replace("\n", "")
  price = pytesseract.image_to_string(rescale(himagelist[2], price_rescale), config=price_config).replace("\n", "")

  """
  #image display for testing
  cv2.imshow("grayscalecropped ver", himagelist[0])
  cv2.waitKey(0)
  cv2.destroyAllWindows()
  """
  #If item_name or quality are empty
  #There should be no row, so return None for no row
  if(item_name == ""): #or quality == ""):
    return None

  #Check for price empty (this means no buy now price)
  #(item_name/quality aren't empty)
  if(price == ""):
    return [item_name, None]

  #item_name, quality, price
  return [item_name, price]

#Sub-image to text line function
#Take in array of images of a split image, output csv text line
def read_image(img):
  output = pytesseract.image_to_string(rescale(img, sub_all_rescale), config=sub_flex_config)
  output = output.replace("\n\n", "\n")

  bonus_effect_split = output.split("Bonus Effect")[1].split("\n")
  stat1 = bonus_effect_split[1]
  stat2 = bonus_effect_split[2]

  random_engraving_effect_split = output.split("Random Engraving Effect")[1].split("\n")
  engraving1 = random_engraving_effect_split[1]
  engraving2 = random_engraving_effect_split[2]
  engraving3 = random_engraving_effect_split[3]

  return [stat1, stat2, engraving1, engraving2, engraving3]

#endregion

#Other functions
#region

#Takes stat values
#Returns quality
def calc_quality(stat1, stat2):
  stat = int(stat1) + int(stat2)

  if 160 <= stat <= 200:
      minstat = 160
      statrange = 40
  elif 240 <= stat <= 300:
      minstat = 240
      statrange = 60
  else: #800 <= stat <= 1000
      minstat = 800
      statrange = 200

  return str(math.floor(100*((stat - minstat) / statrange)))

#endregion

#Read in all images in a folder
#glob = glob.glob(r"test_images/batch_acc_test/*.jpg")
glob = glob.glob(r"test_images/raymond_test/*.jpg")
#glob = glob.glob(r"test_images/punctuation_test/*.jpg")
#print(len(glob))

#Guaranteed to always start with a master image
i = 0
outfile = open("batch_output.csv", "a")
while i < len(glob):
  print(i)
  master_acc_data = []
  sub_acc_indeces = []

  #Read in master image
  #master_img = get_threshold(cv2.imread(glob[i], 0))
  master_img = get_threshold(get_grayscale(get_contrasted(cv2.imread(glob[i]))))

  """
  #image display for testing
  cv2.imshow("grayscalecropped ver", master_img)
  cv2.waitKey(0)
  cv2.destroyAllWindows()
  """

  split_master_img = split_image(master_img)
  #Get line count, store acc data
  for line in split_master_img:
    img_line_data = read_image_line(line)
    #print(img_line_data)

    if(img_line_data is not None): #If the line is None, skip it by not incrementing i
      i+=1 #Update index to index of sub-image for tracking in sub_acc_indeces
      if(img_line_data[1] is not None): #Only add to sub_acc_indeces if 'buy now price' isn't None
        master_acc_data.append(img_line_data)
        sub_acc_indeces.append(i)

  i+=1

  #print(len(sub_acc_indeces))
  #Read in sub images
  j = 0
  for index in sub_acc_indeces:
    acc_img = cut_image(get_grayscale(get_contrasted(cv2.imread(glob[index]))))
    acc_data = read_image(acc_img)

    print(acc_data, master_acc_data[j])
    stat1 = acc_data[0].split("+")
    if(acc_data[1] != "Random Engraving Effect"):
      stat2 = acc_data[1].split("+")
    else:
      stat2 = ["None", "0"]
    stat1_val = stat1[1].replace(".", "").replace(",", "")
    stat2_val = stat2[1].replace(".", "").replace(",", "")
    engrave1 = acc_data[2].replace(".", "").replace(",", "").split("+")
    engrave2 = acc_data[3].replace(".", "").replace(",", "").split("+")
    engrave3 = acc_data[4].replace(".", "").replace(",", "").split("+")
    
    outfile.write(master_acc_data[j][0] + "," + "Relic" + "," + calc_quality(stat1_val, stat2_val) + ","
          + stat1[0].strip() + "," + stat1_val + ","
          + stat2[0].strip() + "," + stat2_val + ","
          + engrave1[0].split("]")[0].replace("[", "") + "," + engrave1[1].replace(".", "") + ","
          + engrave2[0].split("]")[0].replace("[", "") + "," + engrave2[1].replace(".", "") + ","
          + engrave3[0].split("]")[0].replace("[", "") + "," + engrave3[1].replace(".", "") + ","
          + master_acc_data[j][1] + "," + "25"
          + "\n")
    j+=1

outfile.close()
