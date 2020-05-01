# CSE573_Project3
This project proposes to develop a virtual wall that a user can interact with.  The premise of this project is to use a stereo camera to percieve depth information in conjunction with a hand/finger recognition area so that the precise coordinates of where the user points can be deduced and recorded so that it can be queried for the API for the end user application.

The proof of concept experiment will be a virtual drumset, where images of drums will be placed at random places on the wall.  At startup, the program will locate and indentify all percussion instruments and load the associated sounds into its sound library.  The user will then be able to interact with the drumset and will be provided auditory feedback.


### How To Get The Mynt Eye Branch With Python Wrapper
```
git init
git remote add origin https://github.com/slightech/MYNT-EYE-S-SDK.git
git pull
git reset --hard 721ad22
```
