import numpy as np
import cv2
import copy

img = cv2.imread('left_rec.jpg')
depth = cv2.imread('depth.jpg')

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)  
# gray = np.float32(gray)
ret, thresh = cv2.threshold(gray, 120, 255, 0)  #reomve complezities and reduce to b/w
thresh = 255 - thresh  #invert so that ROI is white on black background and is picked as a contour

contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
lg_conts = sorted(contours, key=cv2.contourArea, reverse=True)[:5]  # grab largest 5 contours

for cont in lg_conts:
    if 35000 < cv2.contourArea(cont) < 50000:  #the area of the wall for this setup is known to be within this range
        perim = cv2.arcLength(cont,True)
        sides = cv2.approxPolyDP(cont, 0.05*perim, True)  # simplify contour to polygon
        if len(sides)==4:
            # cont_img = cv2.drawContours(img, [sides], -1, (0,255,0), 3)
            corners = sides

# print(corners)
pt_sums = np.sum(corners, 2)
tl_index = np.argmin(pt_sums)
temp = corners[:tl_index,:]
ordered_corners = copy.deepcopy(corners)
ordered_corners[:(4-tl_index),:] = corners[tl_index:,:]
ordered_corners[4-tl_index:,:] = temp

p1 = np.zeros(3)


p1[:1] = ordered_corners[0][0]
p1[2] = depth
p2 = ordered_corners[1][0]
p3 = ordered_corners[3][0]

print(depth[p2[0]][p2[1]])




v1 = p2-p1
v2 = p3-p1
n = np.cross(v1,v2)


dist = abs(np.dot((p3-p1),n))/np.linalg.norm(n)
unit_n = n/np.linalg.norm(n)
proj_loc = pt - dist*unit_n
name = proj_loc - p1


# print(pt_sums)
# print(temp)
# print(corners)
# print(ordered_corners)
# print(p1)

# pts = [list(corners[0][0]), list(corners[1][0]), list(corners[2][0]), list(corners[3][0])]
# print(pts[:,0])

# cont_img = cv2.drawContours(img, lg_conts, -1, (0,255,0), 3)



        # areas = []
        # sides = []
        # for cont in lg_conts:
            # cont_area = cv2.contourArea(cont)
            # cont_perim = cv2.arcLength(cont,True)
            # cont_sides = cv2.approxPolyDP(cont, 0.05*cont_perim, True)
            # areas.append(cont_area)
            # sides.append(len(cont_sides))

    # dst = cv2.cornerHarris(gray,2,3,0.04)

    # img[dst>0.04*dst.max()]=[0,0,255]

    # cv2.imshow('dst',img)
    # if cv2.waitKey(0) &0xff ==27:
        # cv2.destroyAllWindows()
        
        
    # cv2.imwrite('rect_contour_img.jpg', cont_img)
# print(areas)
# print(sides)

