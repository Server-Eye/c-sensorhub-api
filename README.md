# c-sensorhub-api
The ServerEye.Extern.API can be used to create a custom Sensorhub. It can be included or be a part of any product that wants to save monitoring data in the Server-Eye cloud.


## Dependencies
The project uses the following libraries:

* boost (version 1.56)
* json_spirit (version 4.08)
* libcurl (version 7.37)


## Build on Windows
	
### Compiling
The project can be compiled with Microsoft Visual Studio 2013:

a) by using the VS project file `ServerEyeAPI.vcxproj`

b) by using the included "buildall.bat". You need to adjust the path to	your Visual Studio installation in the `buildall.bat` file before first use. If the project was compiled via `buildall.bat` you can use the `deployAPI.bat` to copy all necessary files (includes & libs) to the new folder `dist`.	
	

## Build on Linux
The project has been compiled and tested on Debian 7.8.0 with GNU g++ 7.4.2

### Required packages:		
* build-essential
* libcurl4-openssl-dev
* libbz2-dev
* python-dev
* autotools-dev
* libicu-dev
		
### Further requirements:
* compiled and installed boost version 1.56
		
### Compiling
The project contains a config file ("Jamroot") for the boost.build system. You can use boost.build's "b2" command without further parameters inside the ServerEye.Extern.API^^ folder to build the project. This will:
* compile the release and debug versions.
* copy all necessary files (includes & libs) to the new folder `dist`.
		
If you compile the project manually please use -std=c++1 to enablethe c++11 standard.
