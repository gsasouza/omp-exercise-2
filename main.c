#include <stdio.h>
#include <math.h>
#include <omp.h>

int calculate_perimeter(int height, int base) {
  return (2*height) + (2*base);
}

int calculate_area(int height, int base) {
  return height * base;
}

double calculate_diagonal(int height, int base) {
  return sqrt((height * height) + (base * base));
}

void task_perimeter(int *height, int *base, int *perimeter, int count) {
  #pragma omp parallel for private(i)
  {
    for (int i = 0; i < count; i++) {
      perimeter[i] = calculate_perimeter(height[i], base[i]);
    }
  }
}

void task_area(int *height, int *base,int *area, int count) {
  #pragma omp parallel for private(i)
  {
    for (int i = 0; i < count; i++) {
      area[i] = calculate_area(height[i], base[i]);
    }
  }
}

void task_diagonal(int *height, int *base, double *diagonal, int count){
  #pragma omp parallel for private(i)
  {
    for (int i = 0; i < count; i++) {
      diagonal[i] = calculate_diagonal(height[i], base[i]);
    }
  }
}

void main(void) {
  double start;
  start = omp_get_wtime();
  int count = 0;
  scanf("%d", &count);
  int height[count], base[count], area[count], perimeter[count];
  double diagonal[count];

  for (int i = 0; i < count; i++) scanf("%d", &height[i]);
  for (int i = 0; i < count; i++) scanf("%d", &base[i]);

  #pragma omp parallel shared(area, base, perimeter, diagonal, height, count)
  {
    #pragma opm single
    {
      #pragma opm task
      {
        task_area(height, base, area, count);
      }
      #pragma opm task
      {
        task_diagonal(height, base, diagonal, count);
      }
      #pragma opm task
      {
        task_perimeter(height, base, perimeter, count);
      }
    }
  };
  for (int i = 0; i < count; i++) {
    printf("base[%d]=%d ", i, base[i]);
    printf("alt[%d]=%d ", i, height[i]);
    printf("per[%d]=%d ", i, perimeter[i]);
    printf("area[%d]=%d ", i, area[i]);
    printf("diag[%d]=%.2f\n", i, diagonal[i]);
  }
  printf("time taken: %.5f", omp_get_wtime() - start);
}
