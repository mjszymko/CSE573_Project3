# CSE573_Project3
This project proposes to develop a virtual wall that a user can interact with.  The premise of this project is to use a stereo camera to percieve depth information in conjunction with a hand/finger recognition area so that the precise coordinates of where the user points can be deduced and recorded so that it can be queried for the API for the end user application.

The proof of concept experiment will be a virtual drumset, where images of drums will be placed at random places on the wall.  At startup, the program will locate and indentify all percussion instruments and load the associated sounds into its sound library.  The user will then be able to interact with the drumset and will be provided auditory feedback.

### Setting Up the Environment

This program will work on the Linux operating system and will be developing using C++, taking advantage of the Mynt Eye SDK.  Mynt Eye provides an API that was developed for users to interact with an take advantage of the available algorithms that can be used in conjunction with the camera feed.

First, we pulled the Mynt Eye API

```git clone https://github.com/slightech/MYNT-EYE-S-SDK.git```











### How To Get The Mynt Eye Branch With Python Wrapper
```
$ git init
$ git remote add origin https://github.com/slightech/MYNT-EYE-S-SDK.git
$ git pull
$ git reset --hard 721ad22
```
In order to use the Python wrapper, you must first install the boost Python libraries: https://mastodonboost.readthedocs.io/en/latest/install.html.  

Boost Side:

Download Boost from here: https://www.boost.org/users/history/version_1_67_0.html

```
$ tar zxvf boost-0.1.tar.gz
$ cd boost
$ python3 setup.py install --install-scripts=/usr/bin
$ ./b2 install --prefix=/...whatever_path_you_want

```

Or if you don't want to download from source, you can do `sudo apt-get install libboost-all-dev`

Python Side: If you have python 3 pip package manager, you can do `pip3 install boost`.
