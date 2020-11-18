import cv2
import numpy as np
from time import sleep
from config import *

with open("array.txt", "r") as f:
	li = f.readline().split(' ')[1:]

X,Y,T = [int(i) for i in li]
I = np.loadtxt('array.txt', dtype='int').reshape(X,Y,T).transpose(2,1,0)

frame_count = 0
cap = cv2.VideoCapture(VIDEO_PATH)
fourcc = cv2.VideoWriter_fourcc(*'avc1')
video = cv2.VideoWriter(OUT_VIDEO_PATH,fourcc,10,(X+N, 2*Y))

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
	print(compframe.shape)
	video.write(cv2.cvtColor(compframe, cv2.COLOR_GRAY2BGR))
	frame_count+=1
	print("Done", frame_count)

cap.release()
video.release()
cv2.destroyAllWindows()