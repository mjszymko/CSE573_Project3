import cv2
import numpy

def main():

	videoSrc = '../video/ExampleVideo.mp4'

	video = cv2.VideoCapture(videoSrc)
	cv2.namedWindow('Virtual Wall Example')

	while(video.isOpened()):

		isFrame,frame = video.read()

		if(isFrame):
			cv2.imshow('Virtual Wall Example', frame)
			cv2.waitKey(40)  # wait 40 ms to give the true 25 fps

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

	video.release()

	cv2.destroyAllWindows()


if __name__ == '__main__':
	main()