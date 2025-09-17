#include <stdio.h>

#define N 15
#define M 13

void transform(int *buf, int **matr, int n, int m);
void reset_picture(int **picture, int n, int m);
void make_picture(int **picture, int n, int m);
void output(int **matrix, int n, int m);

static void draw_sun(int **picture);
static void draw_tree(int **picture);
static void draw_frame(int **picture);

int main(void) {
  int picture_data[N][M];
  int *picture[N];

  transform(&picture_data[0][0], picture, N, M);
  make_picture(picture, N, M);

  return 0;
}

void transform(int *buf, int **matr, int n, int m) {
  for (int i = 0; i < n; i++) {
    matr[i] = buf + i * m;
  }
}

void reset_picture(int **picture, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      picture[i][j] = 0;
    }
  }
}

void make_picture(int **picture, int n, int m) {
  reset_picture(picture, n, m);

  draw_sun(picture);
  draw_tree(picture);
  draw_frame(picture);

  output(picture, n, m);
}

static void draw_sun(int **picture) {
  int sun_data[6][5] = {{0, 6, 6, 6, 6}, {0, 0, 6, 6, 6}, {0, 0, 6, 6, 6},
                        {0, 6, 0, 0, 6}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
  int len_sun_h = sizeof(sun_data) / sizeof(sun_data[0]);
  int len_sun_w = sizeof(sun_data[0]) / sizeof(sun_data[0][0]);

  for (int i = 0; i < len_sun_h; i++) {
    for (int j = 0; j < len_sun_w; j++) {
      picture[i + 1][j + 7] = sun_data[i][j];
    }
  }
}

static void draw_tree(int **picture) {
  int tree_trunk[] = {7, 7, 7, 7};
  int tree_foliage[] = {3, 3, 3, 3};
  int len_tree_fol = sizeof(tree_foliage) / sizeof(tree_foliage[0]);
  int len_tree_trunk = sizeof(tree_trunk) / sizeof(tree_trunk[0]);

  for (int p = 0; p < 2; p++) {
    for (int i = 0; i < len_tree_fol; i++) {
      picture[p + 3][i + 2] = tree_foliage[i];
      picture[i + 2][p + 3] = tree_foliage[i];
    }
  }

  for (int i = 0; i < len_tree_trunk; i++) {
    for (int j = 0; j < len_tree_trunk; j++) {
      if (i < 2) {
        picture[j + 6][i + 3] = tree_trunk[j];
      }
    }
    picture[10][i + 2] = tree_trunk[i];
  }
}

static void draw_frame(int **picture) {
  int frame_w[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  int frame_h[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  int len_fw = sizeof(frame_w) / sizeof(frame_w[0]);
  int len_fh = sizeof(frame_h) / sizeof(frame_h[0]);

  for (int i = 0; i < len_fw; i++) {
    picture[0][i] = frame_w[i];
    picture[7][i] = frame_w[i];
    picture[14][i] = frame_w[i];
  }

  for (int j = 0; j < len_fh; j++) {
    picture[j][0] = frame_h[j];
    picture[j][6] = frame_h[j];
    picture[j][12] = frame_h[j];
  }
}

void output(int **matrix, int n, int m) {
  for (int i = 0; i < n; i++) {
    printf("%d", matrix[i][0]);
    for (int j = 1; j < m; j++) {
      printf(" %d", matrix[i][j]);
    }
    if (i != n - 1) {
      printf("\n");
    }
  }
}
