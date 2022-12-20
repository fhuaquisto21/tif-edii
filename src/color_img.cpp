#include <iostream>
#include <typeinfo>
#include <bitset>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat img_h, img_o, img_hiden, img_result;

void set_image(char * path_h, char * path_o) {
  // Obtener matriz de la imagen a color;
  img_h = cv::imread(path_h);
  img_o = cv::imread(path_o);

  // Redimensionar imagen "origen" a 3 veces menor que la imagen "host";
  cv::resize(img_o, img_o, cv::Size(img_h.rows / 3, img_h.cols / 3));
}

cv::Mat hiden_image(cv::Mat host, cv::Mat origen) {
  int prev_k, prev_l = 0;
  for (int i = 0; i < origen.rows; i++) {
    if (i != 0)
      prev_k = ((i - 1) * 3) + 3;
    int curr_k = (i * 3) + 3;

    for (int j = 0; j < origen.cols; j++) {
      if (j != 0)
        prev_l = ((j - 1) * 3) + 3;
      int curr_l = (j * 3) + 3;

      uchar aux_b = 0;
      cv::Vec3b pixel_o = origen.at<cv::Vec3b>(i, j);
      std::bitset<8UL> pixel_ob = std::bitset<8>(pixel_o.val[0]);
      std::bitset<8UL> pixel_og = std::bitset<8>(pixel_o.val[1]);
      std::bitset<8UL> pixel_or = std::bitset<8>(pixel_o.val[2]);

      for (int k = prev_k; k < curr_k; k++) {
        for (int l = prev_l; l < curr_l; l++) {
          if (aux_b == 8)
            break;
          
          cv::Vec3b pixel_h = host.at<cv::Vec3b>(k, l);
          std::bitset<8UL> pixel_hb = std::bitset<8>(pixel_h.val[0]);
          std::bitset<8UL> pixel_hg = std::bitset<8>(pixel_h.val[1]);
          std::bitset<8UL> pixel_hr = std::bitset<8>(pixel_h.val[2]);   

          pixel_hb[0] = pixel_ob[aux_b];
          pixel_hg[0] = pixel_og[aux_b];
          pixel_hr[0] = pixel_or[aux_b];

          host.at<cv::Vec3b>(k, l) = cv::Vec3b((uchar)pixel_hb.to_ulong(), (uchar)pixel_hg.to_ulong(), (uchar)pixel_hr.to_ulong());
          aux_b++;
        }
      }
    }
  }
  return host;
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
  if (argc != 3) {
    std::cerr << "ERROR: No se envió imágenes." << std::endl;
    return -1;
  }

  set_image(argv[1], argv[2]); 
  cv::imwrite("./result/img_result.png", img_result);
  cv::imwrite("./result/img_hiden.png", img_hiden);

  img_hiden = hiden_image(img_h, img_o);

  img_result = result_image(img_hiden);

  cv::imwrite("./images/img_o.png", img_o);
  cv::imwrite("./images/img_h.png", img_h);

  return 0;
}
