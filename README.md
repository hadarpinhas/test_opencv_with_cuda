
# This repo contains test in python and c++ of opencv with cuda


# Windows installation opencv cuda cpp instructions

Install CMake (during installation choose add to path or after add to system environmental variable, cmake:  C:\Program Files\CMake\bin)

Trying to install cuda 11.8 with cudnn 9.1.1 installer (in the installer only 11.8) and adding to path the bin dir:
C:\Program Files\NVIDIA\CUDNN\v9.1\bin\11.8
C:\Program Files\NVIDIA\CUDNN\v9.1\include\11.8
C:\Program Files\NVIDIA\CUDNN\v9.1\lib\11.8\x64

download opencv and opencv-contrib (zip and then extracted them), whichever version you want (ultralytics required at least 4.6 on Nov-2024), I rebuild from 4.5.5 to 4.10.0: 
https://github.com/opencv/opencv
https://github.com/opencv/opencv_contrib
I downloaded to C:\Users\hadar\Downloads\opencvDownloads. make sure the opencv-4.10.0 does not contain opencv-4.10.0 which happens when extracting optionally into opencv-4.10.0.

open cmake gui
Browse source: "C:\Users\hadar\Downloads\opencvDownloads\opencv-4.10.0"
browse build: C:\opencv

press configure: 
specify the generator: vs2019 
Optional platform for generator: x64
then press finish

once finished, choose on the Search line (you have an option to see the params faster with unchecking the Grouped box next to Search line):
1. with_cuda
2. opencv_dnn_cuda
3. enable_fast_math
4. opencv_extra_modules_path -> browse to the extracted opencv-contrib (C:\Users\hadar\Downloads\opencvDownloads\opencv_contrib-4.10.0\modules): 
	opencv_conrib/ modules
5. build_opencv_world

press configure

choose from search

6. cuda_fast_math
   
8. cuda_arch_bin
	6.1 (for GTX 1050)
   
10. cmake_configuration_types
	Release

press configure

press Generate

cmd (or powershell): 
cmake --build  <build folder (e.g., "C:\opencv")>  --target INSTALL --config Release
should see that CMakeCache.txt was created in <build folder (C:\opencv)> so the binaries should be created there 

Will take a few hours...

add to path:
C:\opencv\install\x64\vc16\bin
C:\opencv\install\x64\vc16\lib


## Cuda includes and libs are added here as an example but not necessarily used.


### vs2019 make sure x64 and Release


## Adding includes and libraries of opencv with cuda

right click on the project -> properties:
1. On top set Configuration: Active(Release), Platform: Active(x64)
2. Configuration properties -> C/C++:
include Directories -> scroll down -> edit -> create new line (small little folder icon on the top-right) -> 
C:\opencv\install\include ; C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.8\include\

press Ok, Apply

3. Configuration properties -> Linker (The dirs that contains what we will use next: opencv_world455.lib, cudart.lib, nppist.lib):
General -> Additional Include Directories -> 
C:\opencv\install\x64\vc16\lib ; C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.8\lib\x64

Input -> Additional dependencies -> scroll down -> edit, add opencv_worldxxx.lib (or whatever opencv version opencv_world4100.lib you have in C:\opencv\install\x64\vc16\lib, or any other place you built opencv with cuda)

Optional-not use here: For cuda I also added:
cudart.lib
nppist.lib
(add any other lib you need)

press Apply



## python cv2 (vscode on windows)

### 1. These folders, that contain dlls, were added to solve the following errors:

"ImportError: DLL load failed while importing cv2: The specified module could not be found."

### 1. Solution:

os.add_dll_directory(r"C:\Program Files\NVIDIA\CUDNN\v9.1\bin\11.8")  # For cuDNN DLLs

os.add_dll_directory(r"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.8\bin")  # For cuda DLLs

os.add_dll_directory(r"C:\opencv\install\x64\vc16\bin")  # For OpenCV DLLs

### 2. Another error for ultralytics-8.3.29 doesn't match opencv 4.5.5:

File "C:\Users\hadar\AppData\Local\Programs\Python\Python310\lib\site-packages\cv2\gapi\__init__.py", line 323, in <module> cv.gapi.wip.GStreamerPipeline = cv.gapi_wip_gst_GStreamerPipeline
AttributeError: partially initialized module 'cv2' has no attribute 'gapi_wip_gst_GStreamerPipeline' (most likely due to a circular import)

### 2. Soultion: compile newer version of opencv > 4.6, so I compiled opencv-4.10.0 and used in python code all the add_dll_directory mentioned above.

Only after add_dll_directory we import cv and ultralytics


## c++ on vscode

open vs code, install extensions: c/c++ and cmake tools, might need to restart
first time, press ctrl+shift+p for command pallete and search cmake:configure, choose vs 2019 86 x64.
then, ctrl+shift+p search cmake: quick start -> project name: testgpucpp -> c++ -> executable.
