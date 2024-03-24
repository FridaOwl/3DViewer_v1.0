#ifndef SRC_HEADERS_VIEWER_H_
#define SRC_HEADERS_VIEWER_H_

#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EPS 1e-6
#define LENGTH 255

typedef struct obj_struct {
  double *vertices;
  int vertex_count;
  int *edges;
  int edge_count;
} obj_t;

enum returns {
  OK,
  FILENAME_ERROR,
  OBJ_VALUES_ERROR,
};

int open_file(const char *path, obj_t *obj);
int parse_obj(FILE *fp, obj_t *obj);
int parse_vertex(char *ptr, obj_t *obj, int v_count);
int parse_edge(char *ptr, obj_t *obj, int e_count);
void init_obj(obj_t *obj, int vertex_count, int edge_count);
void clean_obj(obj_t *obj);

void move_x(obj_t *obj, float value);
void move_y(obj_t *obj, float value);
void move_z(obj_t *obj, float value);
void rotate_x(obj_t *obj, float angle);
void rotate_y(obj_t *obj, float angle);
void rotate_z(obj_t *obj, float angle);
void scale(obj_t *obj, float ratio);

#endif  // SRC_HEADERS_VIEWER_H_
