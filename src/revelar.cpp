#include <iostream>
#include <typeinfo>
#include <bitset>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat img_hiden, img_result;

void set_image(char * path_hiden) {
  // Obtener matriz de la imagen a color;
  img_hiden = cv::imread(path_hiden);
}

cv::Mat result_image(cv::Mat hidden) {
  int size_h = hidden.rows / 3;
  int size_w = hidden.cols / 3;
  cv::Mat result = cv::Mat::zeros(size_h, size_w, CV_8UC3);

  int prev_k, prev_l = 0;
  for (int i = 0; i < size_h; i++) {
    if (i != 0)
      prev_k = ((i - 1) * 3) + 3;
    int curr_k = (i * 3) + 3;
    for (int j = 0; j < size_w; j++) {
      if (j != 0)
        prev_l = ((j - 1) * 3) + 3;
      int curr_l = (j * 3) + 3;

      uchar aux_b = 0;
      cv::Vec3b pixel_o = cv::Vec3b(0, 0, 0);
      std::bitset<8UL> pixel_ob = std::bitset<8>(pixel_o.val[0]);
      std::bitset<8UL> pixel_og = std::bitset<8>(pixel_o.val[1]);
      std::bitset<8UL> pixel_or = std::bitset<8>(pixel_o.val[2]);

      for (int k = prev_k; k < curr_k; k++) {
        for (int l = prev_l; l < curr_l; l++) {
          if (aux_b == 8)
            break;
          
          cv::Vec3b pixel_h = hidden.at<cv::Vec3b>(k, l);
          std::bitset<8UL> pixel_hb = std::bitset<8>(pixel_h.val[0]);
          std::bitset<8UL> pixel_hg = std::bitset<8>(pixel_h.val[1]);
          std::bitset<8UL> pixel_hr = std::bitset<8>(pixel_h.val[2]);

          pixel_ob[aux_b] = pixel_hb[0];
          pixel_og[aux_b] = pixel_hg[0];
          pixel_or[aux_b] = pixel_hr[0];
          aux_b++;
        }
      }
      result.at<cv::Vec3b>(i, j) = cv::Vec3b((uchar)pixel_ob.to_ulong(), (uchar)pixel_og.to_ulong(), (uchar)pixel_or.to_ulong());
    }
  }
  return result;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "ERROR: No se envió imágen." << std::endl;
    return -1;
  }

  set_image(argv[1]); 

  img_result = result_image(img_hiden);

  cv::imwrite("./result/img_result.png", img_result);

  return 0;
}
