import cv2

vid = cv2.VideoCapture("rtsp:192.168.1.68:8080/h264_ulaw.sdp")


while(vid.isOpened()):
    ret,image = vid.read()
    if ret == True:
        resized = cv2.resize(image, (450, 360), interpolation=cv2.INTER_AREA)
        cv2.imshow('ConnectingEverything',resized)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    else: break

vid.release()
cv2.destroyAllWindows()
