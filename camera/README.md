## Installation
* use `raspi-config`:
    * Make sure you enable SSH, VNC, Camera, I2C interfaces

### Setup for new RaspberryPi
```bash
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install git cmake 
sudo apt-get install libopencv-video-dev libopencv-imgproc-dev libopencv-highgui-dev libopencv-dev
sudo modprobe bcm2835-v4l2 
```

### Compilation
```bash
git clone https://github.com/scagle/alt-f4.git
cd alt-f4
git checkout testing
cd camera/build
cmake ../
make
sudo ./camera
```
