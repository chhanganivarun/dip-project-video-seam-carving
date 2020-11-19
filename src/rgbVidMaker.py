import cv2
import numpy as np
from time import sleep
from config import *

cap = cv2.VideoCapture(VIDEO_PATH)
channel_frames = [[],[],[]]

while(cap.isOpened()):
	ret, frame = cap.read()
	if frame is None:
		break
	if RESOLUTION is not None:
		frame = cv2.resize(frame, RESOLUTION)
	frame = frame.astype('int')
	for i in range(3):
		channel_frames[i].append(frame[:,:,i])

cap.release()
channel_I = [np.array(channel_frames[i]).transpose(2,1,0) for i in range(3)]

X,Y,T = channel_I[0].shape

def decode(num):
	t = num%T
	y = ((num-t)//T)%Y
	x = (num-y*T-t)//(Y*T)
	return (x,y,t)

def remove_1seam_1channel(I, li):

	li = [decode(int(i)) for i in li]
	new_I = I.copy()

	for point in li:
		x,y,t = point
		new_I[x,y,t] = -1

	new_I = new_I.transpose(2,1,0)
	new_I = new_I.flatten()
	new_I = (new_I[new_I!=-1]).reshape(T,Y,X-1)
	new_I = new_I.transpose(2,1,0)
	return new_I

li_all = []
with open("rgbseams.txt", "r") as f:
	li_all = f.read().split(" ")[:-1]

skip_size = Y*T

for seam in range(NUM_SEAMS):
	li = li_all[seam*skip_size:(seam+1)*skip_size]
	for i in range(3):
		channel_I[i] = remove_1seam_1channel(channel_I[i], li)
	X-=1

channel_I = np.array(channel_I, dtype='uint8').transpose(3,2,1,0)

cap = cv2.VideoCapture(VIDEO_PATH)
fourcc = cv2.VideoWriter_fourcc(*'avc1')
video = cv2.VideoWriter(OUT_RGB_VIDEO_PATH,fourcc,10,(X,Y))

for frame in channel_I:
	video.write(frame)

video.release()
