# RC Car Monitor

Monitoring and tasking the RC Car while in operation.

## About

RC Car Monitor is part of a personal project to bring an (old) toy RC car to life as an autonomous vehicle, by adding a microcontroller and sensors. The project exists out of two repositories. This repository contains the software the software for monitoring and tasking the RC car while it is in operation. The software of the microcontroller that controls the RC car is located in the [AutonomousRCCar](https://github.com/nvbln/AutonomousRCCar/tree/main) repository and is responsible for handling the sensors and motors while also communicating with the RCCarMonitor.

The focus of the project is to create an autonomous car that is able to navigate well in its environment, and ultimately also fulfill some tasks like gathering data. Although the project is made with a specific RC Car and the Arduino Nano 33 BLE Sense, it is meant to be easily extendable to other vehicles and microcontrollers.

In the case of RCCarMonitor, it is written to communicate over Bluetooth and can in principle be used with any device that exposes the same communication channels over Bluetooth.

## Getting Started

Make sure that you have CMake installed with a version between 3.15 and 4.2 (`cmake --version`).

Clone the repository and its submodules:
```bash
git clone --recurse-submodules https://github.com/nvbln/RCCarMonitor.git
```

Generate the build system:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```

Build the application:
```bash
 cmake --build build --target monitor
```

Now the executable can be found in the `build` directory:
```bash
./build/monitor
```

The tests can be run by specifying the test target after generating the build system:
```bash
cmake --build build --target monitor_test
```

Now the test executable can also be found in the `build` directory:
```bash
./build/monitor_test
```

## Contributing

Although this is intended as a personal project, I am more than happy to hear about issues, suggestions, or contributions. In the case of the latter, please open an issue to discuss the matter before creating a PR.
