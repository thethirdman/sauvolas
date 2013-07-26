#include "sauvola.hh"

// Naive implementation of the mean in a window
static int slowMean(cv::Mat& inpImg, int x, int y, int window)
{
  // Bounds of the window
  int min_y = std::max(0, y - window/2);
  int max_y = std::min(inpImg.rows - 1, y + window/2);
  int min_x = std::max(0, x - window/2);
  int max_x = std::min(inpImg.cols - 1, x + window/2);

  int acc = 0;
  for (int j = min_y; j <= max_y; j++)
    for (int i = min_x; i <= max_x; i++)
      acc += (int) inpImg.at<uchar>(j,i);

  return acc/(window*window);
}

// With the integral image, we can compute the mean of a window of an image
// in constant time
static int fastMean(cv::Mat& integral,
                int x, int y, int window)
{
  // Bounds of the window
  int min_y = std::max(0, y - window/2);
  int max_y = std::min(integral.rows - 1, y + window/2);
  int min_x = std::max(0, x - window/2);
  int max_x = std::min(integral.cols - 1, x + window/2);

  int topright = integral.at<int>(max_y, max_x);
  int botleft  = integral.at<int>(min_y, min_x);
  int topleft  = integral.at<int>(max_y, min_x);
  int botright = integral.at<int>(min_y, max_x);

  int res =  ((topright + botleft - topleft - botright)/(window * window));

  return res;
}

// Compute the local derivative on a point
static int deviation(cv::Mat& inpImg, int x, int y)
{
  uchar cur = inpImg.at<uchar>(y,x);
  uchar d_x = inpImg.at<uchar>(y, std::max(0, x - 1));
  uchar d_y = inpImg.at<uchar>(std::max(0, y - 1), x);

  return (int) cur - d_x - d_y;
}

/// Threshold comoutation functions

static int slowThreshold(cv::Mat& inpImg, int x, int y, int window,  float k)
{
  float normalized_dev = 1 + k * ((((float) deviation(inpImg, x, y)) / 128) - 1);

  return slowMean(inpImg, x, y, window) * normalized_dev;
}

// Define the threshold in a window to decide whether a pixel is foreground
// or background
static int fastThreshold(cv::Mat& inpImg, cv::Mat& integral, int x,
              int y, int window,  float k)
{
  float normalized_dev = 1 + k * ((((float) deviation(inpImg, x, y)) / 128) - 1);

  return fastMean(integral, x, y, window) * normalized_dev;
}



/// Naive implementation
cv::Mat& naiveSauvola(cv::Mat& inpImg, cv::Mat& resImg, int window, float k)
{
  for (int y = 0; y < inpImg.rows; y++)
    for (int x = 0; x < inpImg.cols; x++)
    {
      uchar val = inpImg.at<uchar>(y,x) < slowThreshold(inpImg, x, y, window, k);
      resImg.at<uchar>(y,x) = (val == 0 ? 255: 0);
    }
  return resImg;
}

/// Optimized implementation
cv::Mat& optiSauvola(cv::Mat& inpImg, cv::Mat& resImg, int window, float k,
                      bool parallel)
{
  if (parallel)
    tbb::task_scheduler_init init();
  else
    tbb::task_scheduler_init init(1);

  cv::Mat integral;

  cv::integral(inpImg, integral);

  tbb::parallel_for(0, inpImg.rows, 1,
  [&] (int y)
  {
    for(int x = 0; x < inpImg.cols; x++)
    {
      uchar val = inpImg.at<uchar>(y,x) < fastThreshold(inpImg, integral, x, y, window, k);
      resImg.at<uchar>(y,x) = (val == 0 ? 255: 0);
    }
  });

  return resImg;
}
