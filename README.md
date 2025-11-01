# sfdc-quick-deployer

## Description

sfdc-quick-deployer is a C++ project to upload and download APEX from a salesforce server.
It is faster that Metadata API because is constructed on the Tooling API.

## Installation

After download the repo just use CMake to build
```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

If you face any issue please make sure you have **libcurl** and **nlohmann** installed in your system

## Usage

1. Update Login Details:
```
./sfdc-quick-deployer -l -u username@force.com -p password -r https://login.salesforce.com
```

2. Get all metadata from org into the dictionary
```
./sfdc-quick-deployer -g
```

3. Download a resource
```
./sfdc-quick-deployer -d -f ../resources/LT1_Ctrl.cls
```

4. Upload a resource
```
./sfdc-quick-deployer -f ../resources/LT1_Ctrl.cls
```
