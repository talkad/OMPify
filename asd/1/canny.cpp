#include "canny.h"
#include "omp.h"
#include <cstring>
#include <iostream>
#include <math.h>

using namespace std;

void canny_edge_detect(const uint8_t *input_image, int height, int width,
                       int high_threshold, int low_threshold,
                       uint8_t *output_image) {

  uint64_t image_size = height * width;

  uint8_t *gaussian_blur_output = new uint8_t[image_size];
  double *gradient_magnitude = new double[image_size];
  uint8_t *gradient_direction = new uint8_t[image_size];
  double *nms_output = new double[image_size];
  uint8_t *double_thresh_output = new uint8_t[image_size];

  gaussian_blur(input_image, height, width, gaussian_blur_output);
  gradient_magnitude_direction(gaussian_blur_output, height, width,
                               gradient_magnitude, gradient_direction);
  non_max_suppression(gradient_magnitude, gradient_direction, height, width,
                      nms_output);
  thresholding(nms_output, height, width, high_threshold, low_threshold,
               double_thresh_output);
  hysteresis(double_thresh_output, height, width, output_image);

  delete[] gaussian_blur_output;
  delete[] gradient_magnitude;
  delete[] gradient_direction;
  delete[] nms_output;
  delete[] double_thresh_output;
}

void gaussian_blur(const uint8_t *input_image, int height, int width,
                   uint8_t *output_image) {

  const double kernel[9] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
  const int kernel_sum = 16;

#pragma omp parallel for
  for (int col = OFFSET; col < width - OFFSET; col++) {
    for (int row = OFFSET; row < height - OFFSET; row++) {
      double output_intensity = 0;
      int kernel_index = 0;
      int pixel_index = col + (row * width);
      for (int krow = -OFFSET; krow <= OFFSET; krow++) {
        for (int kcol = -OFFSET; kcol <= OFFSET; kcol++) {
          output_intensity +=
              input_image[pixel_index + (kcol + (krow * width))] *
              kernel[kernel_index];
          kernel_index++;
        }
      }
      output_image[pixel_index] = (uint8_t)(output_intensity / kernel_sum);
    }
  }
}

void gradient_magnitude_direction(const uint8_t *input_image, int height,
                                  int width, double *magnitude,
                                  uint8_t *direction) {
  const int8_t Gx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
  const int8_t Gy[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

#pragma omp parallel for
  for (int col = OFFSET; col < width - OFFSET; col++) {
    for (int row = OFFSET; row < height - OFFSET; row++) {
      double grad_x_sum = 0.0;
      double grad_y_sum = 0.0;
      int kernel_index = 0;
      int pixel_index = col + (row * width);

      for (int krow = -OFFSET; krow <= OFFSET; krow++) {
        for (int kcol = -OFFSET; kcol <= OFFSET; kcol++) {
          grad_x_sum += input_image[pixel_index + (kcol + (krow * width))] *
                        Gx[kernel_index];
          grad_y_sum += input_image[pixel_index + (kcol + (krow * width))] *
                        Gy[kernel_index];
          kernel_index++;
        }
      }

      int pixel_direction = 0;

      if (grad_x_sum == 0.0 || grad_y_sum == 0.0) {
        magnitude[pixel_index] = 0;
      } else {
        magnitude[pixel_index] = ((
            std::sqrt((grad_x_sum * grad_x_sum) + (grad_y_sum * grad_y_sum))));
        double theta = std::atan2(grad_y_sum, grad_x_sum);
        theta = theta * (360.0 / (2.0 * M_PI));

        if ((theta <= 22.5 && theta >= -22.5) || (theta <= -157.5) ||
            (theta >= 157.5))
          pixel_direction = 1; // horizontal -
        else if ((theta > 22.5 && theta <= 67.5) ||
                 (theta > -157.5 && theta <= -112.5))
          pixel_direction = 2; // north-east -> south-west/
        else if ((theta > 67.5 && theta <= 112.5) ||
                 (theta >= -112.5 && theta < -67.5))
          pixel_direction = 3; // vertical |
        else if ((theta >= -67.5 && theta < -22.5) ||
                 (theta > 112.5 && theta < 157.5))
          pixel_direction = 4; // north-west -> south-east \'
        else
          std::cout << "Wrong direction: " << theta << std::endl;
      }
      direction[pixel_index] = (uint8_t)pixel_direction;
    }
  }
}
void non_max_suppression(double *gradient_magnitude,
                         uint8_t *gradient_direction, int height, int width,
                         double *output_image) {
  memcpy(output_image, gradient_magnitude, width * height * sizeof(double));
#pragma omp parallel for
  for (int col = OFFSET; col < width - OFFSET; col++) {
    for (int row = OFFSET; row < height - OFFSET; row++) {
      int pixel_index = col + (row * width);

      // unconditionally suppress border pixels
      if(row == OFFSET || col == OFFSET || col == width - OFFSET - 1 || row == height - OFFSET - 1) {
        output_image[pixel_index] = 0;
        continue;
      }

      switch (gradient_direction[pixel_index]) {
      case 1:
        if (gradient_magnitude[pixel_index - 1] >=
                gradient_magnitude[pixel_index] ||
            gradient_magnitude[pixel_index + 1] >
                gradient_magnitude[pixel_index])
          output_image[pixel_index] = 0;
        break;
      case 2:
        if (gradient_magnitude[pixel_index - (width - 1)] >=
                gradient_magnitude[pixel_index] ||
            gradient_magnitude[pixel_index + (width - 1)] >
                gradient_magnitude[pixel_index])
          output_image[pixel_index] = 0;
        break;
      case 3:
        if (gradient_magnitude[pixel_index - (width)] >=
                gradient_magnitude[pixel_index] ||
            gradient_magnitude[pixel_index + (width)] >
                gradient_magnitude[pixel_index])
          output_image[pixel_index] = 0;
        break;
      case 4:
        if (gradient_magnitude[pixel_index - (width + 1)] >=
                gradient_magnitude[pixel_index] ||
            gradient_magnitude[pixel_index + (width + 1)] >
                gradient_magnitude[pixel_index])
          output_image[pixel_index] = 0;
        break;
      default:
        output_image[pixel_index] = 0;
        break;
      }
    }
  }
}
void thresholding(double *suppressed_image, int height, int width,
                  int high_threshold, int low_threshold,
                  uint8_t *output_image) {
#pragma omp parallel for
  for (int col = 0; col < width; col++) {
    for (int row = 0; row < height; row++) {
      int pixel_index = col + (row * width);
      if (suppressed_image[pixel_index] > high_threshold)
        output_image[pixel_index] = 255; // Strong edge
      else if (suppressed_image[pixel_index] > low_threshold)
        output_image[pixel_index] = 100; // Weak edge
      else
        output_image[pixel_index] = 0; // Not an edge
    }
  }
}

void hysteresis(uint8_t *input_image, int height, int width,
                uint8_t *output_image) {
  memcpy(output_image, input_image, width * height * sizeof(uint8_t));
  for (int col = OFFSET; col < width - OFFSET; col++) {
    for (int row = OFFSET; row < height - OFFSET; row++) {
      int pixel_index = col + (row * width);
      if (output_image[pixel_index] == 100) {
        if (output_image[pixel_index - 1] == 255 ||
            output_image[pixel_index + 1] == 255 ||
            output_image[pixel_index - width] == 255 ||
            output_image[pixel_index + width] == 255 ||
            output_image[pixel_index - width - 1] == 255 ||
            output_image[pixel_index - width + 1] == 255 ||
            output_image[pixel_index + width - 1] == 255 ||
            output_image[pixel_index + width + 1] == 255)
          output_image[pixel_index] = 255;
        else
          output_image[pixel_index] = 0;
      }
    }
  }
}
