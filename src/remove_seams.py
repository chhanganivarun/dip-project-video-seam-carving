import os
import cv2
import numpy as np
from tqdm import tqdm
from config import *


# Driver, first file to be run
# Coverts video, resize it according to config.py
# Calls flow.bat or bash_flow.sh
# Remove NUM_SEAMS number of seams from video.


cap = cv2.VideoCapture(VIDEO_PATH)
frames = []
while(cap.isOpened()):
	ret, frame = cap.read()
	if frame is None:
		break
	frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	if RESOLUTION is not None:
		frame = cv2.resize(frame, RESOLUTION)
	frames.append(frame.astype('int'))

cap.release()

I = np.array(frames).transpose(2,1,0)
X,Y,T = I.shape
np.savetxt('array.txt', I.flatten(), fmt='%.0f', header=str(X)+' '+str(Y)+' '+str(T))


script_name = "flow.bat"
from sys import platform
if platform == "linux" or platform == "linux2":
	script_name = "bash flow.sh"

# NUM_SEAMS is the number of seams to be removed from video
for i in tqdm(range(NUM_SEAMS)):
	os.system(script_name)