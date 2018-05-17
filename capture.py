import numpy as np
import cv2
import math

cap = cv2.VideoCapture(0)

# Los colores en opencv por defecto estan en BGR!
colors = {
    'r': [68, 60, 196],
    'g': [123, 145, 10],
    'b': [191, 103, 10],
    'w': [211, 215, 202],
    'y': [70, 171, 186],
    'o': [76, 69, 60],
}


# https://en.wikipedia.org/wiki/Color_difference
def distance(color1, color2):
    r = (color1[2] + color2[2]) / 2
    delta_r = color1[2] - color2[2]
    delta_g = color1[1] - color2[1]
    delta_b = color1[0] - color2[0]
    return math.sqrt(2 * delta_r**2 + 4 * delta_g**2 + 3 * delta_b**2 + r*(delta_r**2 - delta_b**2) / 256)


def closest(color):
    col = ''
    min_dist = float('inf')
    for k in colors:
        dist = distance(colors[k], color)
        # print(dist)
        if dist < min_dist:
            col = k
            min_dist = dist

    return col


captured_colors = ''

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    # Our operations on the frame come here
    # hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    cv2.rectangle(hsv_frame,(200,120), (260,180), (255,0,255), 2)
    cv2.rectangle(hsv_frame,(380,120), (440,180), (255,0,255), 2)
    cv2.rectangle(hsv_frame,(200,300), (260,360), (255,0,255), 2)
    cv2.rectangle(hsv_frame,(380,300), (440,360), (255,0,255), 2)

    # Display the resulting frame
    cv2.imshow('frame',hsv_frame)

    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
    elif key == ord(' '):
        q0 = frame[120 + 3:180 - 3, 200 + 3:260 - 3]
        q1 = frame[120 + 3:180 - 3, 380 + 3:440 - 3]
        q2 = frame[300 + 3:360 - 3, 200 + 3:260 - 3]
        q3 = frame[300 + 3:360 - 3, 380 + 3:440 - 3]
        q0_average = np.average(np.average(q0, axis=0), axis=0)
        q1_average = np.average(np.average(q1, axis=0), axis=0)
        q2_average = np.average(np.average(q2, axis=0), axis=0)
        q3_average = np.average(np.average(q3, axis=0), axis=0)
        print(q0_average[0], q1_average[0], q2_average[0], q3_average[0])
        print(q0_average)
        print(q1_average)
        print(q2_average)
        print(q3_average)
        print(closest(q0_average))
        print(closest(q1_average))
        print(closest(q2_average))
        print(closest(q3_average))
        captured_colors += closest(q0_average)
        captured_colors += closest(q1_average)
        captured_colors += closest(q2_average)
        captured_colors += closest(q3_average)

print(captured_colors)
# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
