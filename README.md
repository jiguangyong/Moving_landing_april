# Moving_landing-april
This is stable version branch for quardrotor landing on a moving platform project.

## Acknowledgements
- The planning framework of this repository is based on [Fast-Perching](https://github.com/ZJU-FAST-Lab/Fast-Perching.git).
- The px4ctrl FSM node of this repository is based on [Fast-Drone-250](https://github.com/ZJU-FAST-Lab/Fast-Drone-250.git).

## Getting start
Compiling tests passed on ubuntu 20.04 with ros installed. 

install:
```
sudo apt-get install ros-noetic-geodesy ros-noetic-mavros ros-noetic-apriltag-ros
git clone -b master https://github.com/RM-Huang/Moving_landing-april
cd Moving_landing_april/src/utils
unzip mavlink_msg.zip
cd ../..
catkin_make -DCATKIN_WHITELIST_PACKAGES="chcnav"
catkin_make -DCATKIN_WHITELIST_PACKAGES=""
source devel/setup.bash
```

install odometry data transport module on car computer:
```
unzip car_data_trans.zip  -d /home
cd car_data_trans
catkin_make
```

## Simulation
You need to install gazebo and rviz with correct ros version.
Run the following script to start simulation.
```
./sim_traj_follow.sh
```
You can use ***WSAD*** in the second terminator to adjust velocity and attitude of the car.
<p align = "center">
<img src="pic/car-control-terminator.png" width = "640" border="5" />
</p>

And nodelet status would be publised on the following terminator.
<p align = "center">
<img src="pic/nodelet%20status.png" width = "640" border="5" />
</p>

Then use the following script to takeoff UAV.
```
./takeoff.sh
```
After vehicle stablized, run the following script to start planning:
```
./pub_triger.sh
```

<p align = "center">
<img src="pic/simulation.gif" width = "640" border="5" />
</p>

## Realfight run
You have to read the _README.md_ file in the px4ctrl package before you run the script.
Execute the following commands to take off your vehical after you connecting _Autopilot_ to _flight computer_. 
```
cd Moving_landing_april/sh_utils
./realflight_landing.sh
./takeoff.sh
```
Than run the following sctipt to start planning.
```
./pub_triger.sh
```

<p align = "center">
<img src="pic/realflight.gif" width = "640" border="5" />
</p>

