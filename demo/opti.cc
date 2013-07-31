#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "../src/sauvola.hh"

int main(int argc, char** argv)
{
  if (argc != 5)
  {
    std::cerr << "Usage: " << argv[0] << " k w input output" << std::endl;
    std::cerr << "k is the tuning parameter (floating point)" << std::endl;
    std::cerr << "w in the window size in pixels" << std::endl;
    return 1;
  }
  else
  {

    cv::Mat image = cv::imread(argv[3]);
    cv::Mat imGray;
    cv::Mat res;

    cv::cvtColor(image, imGray, CV_RGB2GRAY);
    res = imGray.clone();

    optiSauvola(imGray, res, atoi(argv[2]), atof(argv[1]), false);

    cv::imwrite(argv[4], res);

    return 0;
  }
}
