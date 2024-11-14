
## This code compares the execution time of template matching on the CPU and GPU

import os 
# To add CUDA and cuDNN DLLs to the path an opencv binary directory
os.add_dll_directory(r"C:\Program Files\NVIDIA\CUDNN\v9.1\bin\11.8")  # For cuDNN DLLs
os.add_dll_directory(r"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.8\bin")  # For cuda DLLs
os.add_dll_directory(r"C:\opencv\install\x64\vc16\bin")  # For OpenCV DLLs

import cv2
import time

# Load the frame and template images
frame = cv2.imread("../input_imgs/frame.jpg")
template = cv2.imread("../input_imgs/template.jpg")

# Ensure images are in 8-bit unsigned 3-channel format (CV_8UC3)
if frame is None or template is None:
    raise FileNotFoundError("Could not load images. Make sure 'frame_image.jpg' and 'template_image.jpg' are in the working directory.")

# Get template dimensions
h, w = template.shape[:2]

### CPU Execution ###
start_cpu = time.time()
result_cpu = cv2.matchTemplate(frame, template, cv2.TM_CCOEFF_NORMED)
end_cpu = time.time()
cpu_time = end_cpu - start_cpu
print(f"CPU time: {cpu_time:.4f} seconds")

# Find best match location on CPU
_, _, _, max_loc_cpu = cv2.minMaxLoc(result_cpu)
top_left_cpu = max_loc_cpu
bottom_right_cpu = (top_left_cpu[0] + w, top_left_cpu[1] + h)

# Draw rectangle on the CPU result
result_frame_cpu = frame.copy()
cv2.rectangle(result_frame_cpu, top_left_cpu, bottom_right_cpu, (0, 255, 0), 2)
cv2.imwrite("results/matched_result_cpu.jpg", result_frame_cpu)

### GPU Execution ###
# Upload images to GPU
gpu_frame = cv2.cuda_GpuMat()
gpu_template = cv2.cuda_GpuMat()
gpu_frame.upload(frame)
gpu_template.upload(template)

# Create CUDA template matcher
template_matcher = cv2.cuda.createTemplateMatching(cv2.CV_8UC3, cv2.TM_CCOEFF_NORMED)

# Measure GPU time
start_gpu = time.time()
result_gpu = template_matcher.match(gpu_frame, gpu_template)
end_gpu = time.time()
gpu_time = end_gpu - start_gpu
print(f"GPU time: {gpu_time:.4f} seconds")

# Download result from GPU
result_gpu_host = result_gpu.download()

# Find best match location on GPU
_, _, _, max_loc_gpu = cv2.minMaxLoc(result_gpu_host)
top_left_gpu = max_loc_gpu
bottom_right_gpu = (top_left_gpu[0] + w, top_left_gpu[1] + h)

# Draw rectangle on the GPU result
result_frame_gpu = frame.copy()
cv2.rectangle(result_frame_gpu, top_left_gpu, bottom_right_gpu, (255, 0, 0), 2)
cv2.imwrite("results/matched_result_gpu.jpg", result_frame_gpu)
