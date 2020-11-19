import numpy as np
import cv2

# seam removed array is created using seamout.txt and stored in array.txt

with open("array.txt", "r") as f:
	li = f.readline().split(' ')[1:]

X,Y,T = [int(i) for i in li]
I = np.loadtxt('array.txt', dtype='int').reshape(X,Y,T)

def decode(num):
	t = num%T
	y = ((num-t)//T)%Y
	x = (num-y*T-t)//(Y*T)
	return (x,y,t)

li = []
with open("seamout.txt", "r") as f:
	li = f.read().split(" ")[:-1]

li = [decode(int(i)) for i in li]
new_I = I.copy()
# new_I = np.zeros_like(I)

for point in li:
	x,y,t = point
	new_I[x,y,t] = -1

new_I = new_I.transpose(2,1,0)
new_I = new_I.flatten()
new_I = (new_I[new_I!=-1]).reshape(T,Y,X-1)
new_I = new_I.transpose(2,1,0)

np.savetxt('array.txt', new_I.flatten(), fmt='%.0f', header=str(X-1)+' '+str(Y)+' '+str(T))