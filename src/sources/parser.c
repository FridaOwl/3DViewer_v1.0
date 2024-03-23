#include "../headers/parser.h"

int open_file(const char *path, obj_t *obj) {
  int rtn = OK;
  FILE *file = NULL;
  file = fopen(path, "r");
  if (!file) {
    rtn = FILENAME_ERROR;
  }
  if (!rtn) {
    int error_code = parse_obj(file, obj);
    if (error_code != OK) {
      rtn = error_code;
    }
  }

  if (rtn != FILENAME_ERROR) {
    fclose(file);
  }
  if (!obj) {
    if (obj->vertices) {
      free(obj->vertices);
    }
    if (obj->edges) {
      free(obj->edges);
    }
    obj = NULL;
  }

  return rtn;
}

int parse_obj(FILE *fp, obj_t *obj) {
  int error_code = OK;
  char line[LENGTH] = {0};
  while (fgets(line, LENGTH, fp)) {
    if (*line == 'v' && *(line + 1) == ' ') {
      obj->vertex_count++;
    } else if (*line == 'f' && *(line + 1) == ' ') {
      obj->edge_count++;
    }
  }
  obj->vertices =
      (double *)realloc(obj->vertices, sizeof(double) * obj->vertex_count * 3);
  obj->edges = (int *)realloc(obj->edges, sizeof(int) * obj->edge_count * 6);
  if (obj->vertices && obj->edges) {
    fseek(fp, 0, SEEK_SET);

    for (int v_count = obj->vertex_count, e_count = obj->edge_count;
         fgets(line, LENGTH, fp);) {
      if (*line == 'v' && *(line + 1) == ' ') {
        error_code = parse_vertex(line, obj, v_count--);
      } else if (*line == 'f' && *(line + 1) == ' ') {
        error_code = parse_edge(line, obj, e_count--);
      }
      if (error_code != OK) {
        break;
      }
    }
  }
  *line = '\0';
  return error_code;
}

int parse_vertex(char *ptr, obj_t *obj, int v_count) {
  int error_code = OK;
  ptr += 2;

  int vertex_index = (obj->vertex_count - v_count) * 3;

  double values[3] = {0.0};

  for (int i = 0; i < 3; i++) {
    char *endptr;
    double value = strtod(ptr, &endptr);

    if (ptr == endptr) {
      values[i] = 0.0;
    } else {
      values[i] = value;
    }

    while (*endptr == ' ' || *endptr == '\t') {
      endptr++;
    }

    ptr = endptr;

    if (*ptr == '\0') {
      break;
    }
  }

  for (int i = 0; i < 3; i++) {
    obj->vertices[vertex_index + i] = values[i];
  }

  return error_code;
}

int parse_edge(char *ptr, obj_t *obj, int e_count) {
  int error_code = OK;

  for (int i = 0; i < 3; i++) {
    if (strchr(ptr, ' ') == NULL) {
      error_code = OBJ_VALUES_ERROR;
      break;
    }

    ptr = strchr(ptr, ' ');
    int index = (int)strtol(ptr, &ptr, 10);

    if (index < 0) {
      index = obj->vertex_count + index;
    } else {
      index--;
    }

    int base_index = (obj->edge_count - e_count) * 6;
    obj->edges[base_index + i] = index;
    obj->edges[base_index + i + 3] = index;
  }

  return error_code;
}

void init_obj(obj_t *obj, int vertex_count, int edge_count) {
  obj->vertices = (double *)calloc(vertex_count, sizeof(double *));
  obj->edges = (int *)calloc(edge_count, sizeof(int *));
  obj->vertex_count = 0;
  obj->edge_count = 0;
}

void clean_obj(obj_t *obj) {
  if (obj) {
    if (obj->vertices) {
      free(obj->vertices);
    }
    if (obj->edges) {
      free(obj->edges);
    }
    obj = NULL;
  }
}
