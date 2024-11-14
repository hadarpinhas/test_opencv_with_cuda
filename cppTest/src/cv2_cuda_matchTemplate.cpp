#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafilters.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/cudacodec.hpp>
#include <chrono>
#include <iostream>

int main() {
    // Load images
    cv::Mat frame = cv::imread("..\\..\\input_imgs\\helmets_template.jpg");
    cv::Mat template_img = cv::imread("..\\..\\input_imgs\\template.jpg");

    if (frame.empty() || template_img.empty()) {
        std::cerr << "Could not load images. Ensure 'frame_image.jpg' and 'template.jpg' are in the directory." << std::endl;
        return -1;
    }

    // Template dimensions
    int h = template_img.rows;
    int w = template_img.cols;

    // --- CPU Template Matching ---
    auto start_cpu = std::chrono::high_resolution_clock::now();
    cv::Mat result_cpu;
    cv::matchTemplate(frame, template_img, result_cpu, cv::TM_CCOEFF_NORMED);
    auto end_cpu = std::chrono::high_resolution_clock::now();
    double cpu_time = std::chrono::duration<double>(end_cpu - start_cpu).count();
    std::cout << "CPU time: " << cpu_time << " seconds" << std::endl;

    // Find best match on CPU
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result_cpu, &minVal, &maxVal, &minLoc, &maxLoc);
    cv::Point top_left_cpu = maxLoc;
    cv::Point bottom_right_cpu = cv::Point(top_left_cpu.x + w, top_left_cpu.y + h);

    // Draw rectangle on CPU result
    cv::Mat result_frame_cpu = frame.clone();
    cv::rectangle(result_frame_cpu, top_left_cpu, bottom_right_cpu, cv::Scalar(0, 255, 0), 2);
    cv::imwrite("..\\results\\matched_result_cpu.jpg", result_frame_cpu);

    // --- GPU Template Matching ---
    cv::cuda::GpuMat gpu_frame, gpu_template, result_gpu;
    gpu_frame.upload(frame);
    gpu_template.upload(template_img);

    // Create CUDA template matcher
    auto template_matcher = cv::cuda::createTemplateMatching(gpu_frame.type(), cv::TM_CCOEFF_NORMED);

    auto start_gpu = std::chrono::high_resolution_clock::now();
    template_matcher->match(gpu_frame, gpu_template, result_gpu);
    auto end_gpu = std::chrono::high_resolution_clock::now();
    double gpu_time = std::chrono::duration<double>(end_gpu - start_gpu).count();
    std::cout << "GPU time: " << gpu_time << " seconds" << std::endl;

    // Download result from GPU
    cv::Mat result_gpu_host;
    result_gpu.download(result_gpu_host);

    // Find best match on GPU
    cv::minMaxLoc(result_gpu_host, &minVal, &maxVal, &minLoc, &maxLoc);
    cv::Point top_left_gpu = maxLoc;
    cv::Point bottom_right_gpu = cv::Point(top_left_gpu.x + w, top_left_gpu.y + h);

    // Draw rectangle on GPU result
    cv::Mat result_frame_gpu = frame.clone();
    cv::rectangle(result_frame_gpu, top_left_gpu, bottom_right_gpu, cv::Scalar(255, 0, 0), 2);
    cv::imwrite("..\\results\\matched_result_gpu.jpg", result_frame_gpu);

    return 0;
}
