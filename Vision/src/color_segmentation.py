import cv2
import cv2.cv as cv
import numpy as np

cap = cv2.VideoCapture(0)

while(1):

    # Take each frame
    ret, frame = cap.read()
    f = open("output.txt", "w")
    
    # Convert BGR to HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    for item in hsv:
     	f.write(str(item))

    f.close()

    # define range of blue color in HSV
    # lower_blue = np.array([0,160,0])
    # upper_blue = np.array([22,255,255])
    
    lower_blue = np.array([2,100,0])
    upper_blue = np.array([22,255,255])
    
    # Threshold the HSV image to get only blue colors
    mask = cv2.inRange(hsv, lower_blue, upper_blue)
	#erosion
    kernel = np.ones((5,5),np.uint8)
    erosion = cv2.erode(mask,kernel,iterations=3)

    #dilation
    dilation = cv2.dilate(erosion,kernel,iterations=5)

    img = cv2.medianBlur(dilation,11)
    img = cv2.medianBlur(img,11)

    # Bitwise-AND mask and original image
    res = cv2.bitwise_and(frame,frame, mask=dilation)

    circles = cv2.HoughCircles(img,cv.CV_HOUGH_GRADIENT,2,len(img)/4,
                            param1=100,param2=25,minRadius=50,maxRadius=400)

    print circles

    if circles is not None:
    	circles = np.uint16(np.around(circles))
    	for i in circles[0,:]:
			#draw the outer circle
			cv2.circle(frame,(i[0],i[1]),i[2],(0,255,0),2)
		# # draw the center of the circle
			cv2.circle(frame,(i[0],i[1]),2,(0,0,255),3)

    cv2.imshow('frame',frame)
    cv2.imshow('mask',img)
    #cv2.imshow('cor',res)
    cv2.imshow('detected circles',res)
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()