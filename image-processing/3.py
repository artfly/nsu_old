import glob
import numpy as np
import cv2
import sys

def search_object(files):
	standart = cv2.imread(files['key*'])
	ret, thresh = cv2.threshold(standart, 80, 255, cv2.THRESH_BINARY)
	contours,hierarchy = cv2.findContours(thresh, 2, 1)
	cnt1 = contours[0]
	contours,hierarchy = cv2.findContours(thresh2,2,1)
	cnt2 = contours[0]

	ret = cv2.matchShapes(cnt1,cnt2,1,0.0)

def show_image(img, desc):
	cv2.imshow(desc,img)
	cv2.waitKey(0)
	cv2.destroyAllWindows()

def binaryTreshold(image):
	image = cv2.blur(image, (3, 3))
	thresh = cv2.adaptiveThreshold(image, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 43, 20)	
	show_image(thresh, "adaptiveThreshold")
	ret, thresh = cv2.threshold(image, 80, 255, cv2.THRESH_BINARY)
	print len(thresh)
	show_image(thresh, "Manual threshold")
	ret, thresh = cv2.threshold(image, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
	show_image(thresh, "Otsu threshold")

def connected_spaces(image):
	result = np.zeros(image.shape)
	print image.shape
	image = cv2.blur(image, (3, 3))
	ret, thresh = cv2.threshold(image, 80, 255, cv2.THRESH_BINARY)
	# ret,thresh = cv2.threshold(image, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)
	contours, hierarchy = cv2.findContours(thresh, cv2.RETR_LIST, cv2.CHAIN_APPROX_NONE)
	count = 0
	for contour in contours:
		if len(contour) > 200:
			count += 1
	print count

	cv2.drawContours(result, contours, -1, (255,255,255), 3)
	show_image(result, "contours")
	# show_image(contours, "connected_spaces")

def main():
	files = glob.glob("./train/*.jpg")
	search_object(files)
	for filename in files:
		img = cv2.imread(filename,0)
		# connected_spaces(img)
		# binaryTreshold(img)



if __name__ == "__main__":
    main()