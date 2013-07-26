#ifndef SAUVOLA_HH_
# define SAUVOLA_HH_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <tbb/task_scheduler_init.h>
#include <tbb/parallel_for.h>

// A naive implementation of the sauvola algorithm
cv::Mat& naiveSauvola(cv::Mat& inpImg, cv::Mat& resImg, int window, float k);

// A optimized version using the integral image, with a parallel option
cv::Mat& optiSauvola (cv::Mat& inpImg, cv::Mat& resImg, int window, float k,
                      bool parallel);

#endif
