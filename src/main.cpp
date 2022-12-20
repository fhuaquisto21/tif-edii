#include <iostream>
#include <typeinfo>
#include <string>
#include <bitset>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

// Declarando variables
cv::Mat img_h, img_o;

// Conseguir imagen
void get_image(char * path_h, char * path_o) {
  img_h = cv::imread(path_h, cv::IMREAD_GRAYSCALE);
  img_o = cv::imread(path_o, cv::IMREAD_GRAYSCALE);
  cv::resize(img_h, img_h, cv::Size(750, 750));
  cv::resize(img_o, img_o, cv::Size(250, 250));
}

// Modificando imagen host ocultando la imagen origen
void hidden_img_3(cv::Mat & imh, cv::Mat imo) {
  //auto a = std::bitset<8>(imo.at<uchar>(0, 0));
  int last_i, last_j = 0;
  for (int i = 0; i < imo.rows; i++) {
    if (i != 0)
      last_i = ((i - 1) * 3) + 3;
    int aux_i = (i * 3) + 3;
    for (int j = 0; j < imo.cols; j++) {
      if (j != 0)
        last_j = ((j - 1) * 3) + 3;
      int aux_j = (j * 3) + 3;
      int aux_b = 0;
      auto bits_o = std::bitset<8>(imo.at<uchar>(i, j));
      for (int k = last_i; k < aux_i; k++) {
        for (int l = last_j; l < aux_j; l++) {
          if (aux_b == 8)
            break;
          auto bits_h = std::bitset<8>(imh.at<uchar>(k, l));
          //std::cout << imh.at<uchar>(k, l) << " -> ";
          bits_h[0] = bits_o[aux_b];
          imh.at<uchar>(k, l) = (uchar)bits_h.to_ulong();
          //std::cout << imh.at<uchar>(k, l) << std::endl;
          aux_b++;
        }
      }
      //std::cout << "(" << last_i << ", " << last_j << ")" << std::endl;
    }
    //std::cout << aux_i << std::endl;
  }
  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
  cv::imshow("Display Image", imh);
  cv::waitKey(0);
  cv::destroyWindow("Display Image");
}

// Conseguir la imagen origen del host
cv::Mat get_img_3(cv::Mat & imh) {
  int size_h = imh.rows / 3;
  int size_w = imh.cols / 3;
  cv::Mat reimg_o = cv::Mat::zeros(size_h, size_w, CV_8UC1);

  int last_i, last_j = 0;
  for (int i = 0; i < size_h; i++) {
    if (i != 0)
      last_i = ((i - 1) * 3) + 3;
    int aux_i = (i * 3) + 3;
    for (int j = 0; j < size_w; j++) {
      if (j != 0)
        last_j = ((j - 1) * 3) + 3;
      int aux_j = (j * 3) + 3;
      int aux_b = 0;
      std::bitset<8UL> bits_o = std::bitset<8>(imh.at<uchar>(0, 0));
      for (int k = last_i; k < aux_i; k++) {
        for (int l = last_j; l < aux_j; l++) {
          if (aux_b == 8)
            break;
          auto bits_h = std::bitset<8>(imh.at<uchar>(k, l));
          bits_o[aux_b] = bits_h[0];
          aux_b++;
        }
      }

      reimg_o.at<uchar>(i, j) = (uchar)bits_o.to_ulong();
      //std::cout << "(" << last_i << ", " << last_j << ")" << std::endl;
    }
    //std::cout << aux_i << std::endl;
  }

  cv::namedWindow("Display Image O", cv::WINDOW_AUTOSIZE);
  cv::imshow("Display Image O", reimg_o);
  cv::waitKey(0);
  cv::destroyWindow("Display Image O");

  return reimg_o;
}

// Convertir entero a binario
void conversor (int decimal, short B[8]) {
  for (int i = 0; i < 8; i++)
  {
    B[i] = decimal % 2;
    decimal /= 2;
  }
}

int main(int argc, char** argv) {
  if ( argc != 3 ) {
    printf("usage: DisplayImage.out <Image_Path>\n");
    return -1;
  }
  get_image(argv[1], argv[2]);
  /*
  cv::namedWindow("Display Image Host", cv::WINDOW_AUTOSIZE);
  cv::namedWindow("Display Image Original", cv::WINDOW_AUTOSIZE);
  cv::imshow("Display Image Host", img_h);
  cv::imshow("Display Image Original", img_o);
  */
  //img_o.at<uchar>(0, 0) = 0;
  /*
  std::cout << img_o << std::endl;
  std::cout << std::bitset<8>(img_h.at<uchar>(0, 0)) << std::endl;
  std::cout << std::bitset<8>(img_o.at<uchar>(0, 0))[7] << std::endl;
  std::cout << (int)img_o.at<uchar>(0, 0) << std::endl;
  */
  hidden_img_3(img_h, img_o);
  get_img_3(img_h);
  return 0;
}
