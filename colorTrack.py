import cv2
import numpy as np

cap = cv2.VideoCapture(0)

while(1):

    _, frame = cap.read()
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    lower_blue = np.array([110,50,50])
    upper_blue = np.array([130,255,255])

    # Threshold the HSV image to get only white colors
    mask = cv2.inRange(hsv, lower_blue, upper_blue)
    # Bitwise-AND mask and original image
    res = cv2.bitwise_and(frame,frame, mask= mask)

    cv2.imshow('frame',frame)
    cv2.imshow('mask',mask)
    cv2.imshow('res',res)
    maskList = mask.tolist()
    intensity = mask.item(0, 0)
    #print (maskList)

    rowavg = 0;
    colavg = 0;
    numseen = 0;
    zoomed = [[0 for x in range(100)] for y in range(100)]
    for i in range(len(maskList)):
        for j in range(len(maskList[i])):
            if mask.item(i, j) > 0:
                rowavg = rowavg + i;
                colavg = colavg + j;
                numseen = numseen + 1;
    rowavg = rowavg / numseen
    rowavgpoint = int(round(rowavg))
    colavg = colavg / numseen
    colavgpoint = int(round(colavg))
    print ("(%f,%f)" % (colavg, rowavg))
    for i in range(100):
        for j in range(100):
            
    #print 	(mask.
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()
