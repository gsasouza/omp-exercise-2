#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define T 8

int calculate_perimeter(int height, int base) {
  return (2*height) + (2*base);
}

int calculate_area(int height, int base) {
  return height * base;
}

double calculate_diagonal(int height, int base) {
  return sqrt((height * height) + (base * base));
}

int main(void) {
  omp_set_nested(1);
  omp_set_dynamic(0);
  double wtime = omp_get_wtime();
  int count = 0, i;
  scanf("%d", &count);
  int *height = (int*) malloc(count * sizeof(int));
  int *base = (int*) malloc(count * sizeof(int));
  int *area = (int*) malloc(count * sizeof(int));
  int *perimeter = (int*) malloc(count * sizeof(int));
  double *diagonal = (double*) malloc(count * sizeof(double));

  for (int i = 0; i < count; i++) scanf("%d", &height[i]);
  for (int i = 0; i < count; i++) scanf("%d", &base[i]);

  #pragma omp parallel
  {
    #pragma omp sections
    {
      #pragma omp section
      {
        #pragma omp parallel for private(i) firstprivate(height, base)
        for (i = 0; i < count; i++) {
          area[i] = calculate_area(height[i], base[i]);
        }
      }
      #pragma omp section
      {
        #pragma omp parallel for private(i) firstprivate(height, base)
        for (i = 0; i < count; i++) {
          diagonal[i] = calculate_diagonal(height[i], base[i]);
        }
      }
      #pragma omp section
      {
        #pragma omp parallel for private(i) firstprivate(height, base)
        for (i = 0; i < count; i++) {
          perimeter[i] = calculate_perimeter(height[i], base[i]);
        }
      }
    }
  };
//  for (int i = 0; i < count; i++) {
//    printf("base[%d]=%d ", i, base[i]);
//    printf("alt[%d]=%d ", i, height[i]);
//    printf("per[%d]=%d ", i, perimeter[i]);
//    printf("area[%d]=%d ", i, area[i]);
//    printf("diag[%d]=%.2f\n", i, diagonal[i]);
//  }
  printf("time taken: %.5f", omp_get_wtime() - wtime);
}
