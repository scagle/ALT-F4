End of semester demo for 490 Part A
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
cd alt-f4/<desired project>/build
cmake ../
make

# May need to run in sudo depending on whether you are apart of the right groups
./camera 
```

### Things to do
- [x] Figure out exactly how opencv stores Mat.data
- [x] Figure out how to lower camera's resolution
- [ ] Figure out how to operator overload Frame class []
- [ ] Figure out how to incorporate exponential moving average to find flashies
- [ ] Figure out if we need to cluster groups together (to optimize)
- [ ] Figure out how to multithread the frame gathering to seperate it from processing (maybe cores?)
