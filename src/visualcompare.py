import cv2
import numpy as np
from time import sleep
from config import *
# Converts array.txt into a comparison video of before and after videos
with open("array.txt", "r") as f:
	li = f.readline().split(' ')[1:]

X,Y,T = [int(i) for i in li]
I = np.loadtxt('array.txt', dtype='int').reshape(X,Y,T).transpose(2,1,0)

frame_count = 0
cap = cv2.VideoCapture(VIDEO_PATH)
while(cap.isOpened()):
	ret, frame = cap.read()
	if frame is None:
		break
	frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	if RESOLUTION is not None:
		frame = cv2.resize(frame, RESOLUTION)
	padded_frame = np.ones_like(frame)*255
	padded_frame[:Y,:X] = I[frame_count].astype('uint8')
	compframe = np.vstack([frame, padded_frame])
	cv2.imshow('out', compframe)
	frame_count+=1
	sleep(0.1)
	if cv2.waitKey(1) & 0xFF == ord('q'):
		break

cap.release()
cv2.destroyAllWindows()