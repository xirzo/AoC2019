#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>

#define BUF_SIZE 2048

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

typedef struct {
  int x;
  int y;
} Vector2;

typedef enum {
  COUNTER_CLOCKWISE = -1,
  COLLINEAR = 0,
  CLOCKWISE = 1,
} Orientation;

typedef struct {
  Vector2 (*items)[2];
  size_t size;
  size_t cap;
} Vector;

Vector *create_vector(size_t cap) {
  Vector *v = malloc(sizeof(Vector));

  if (!v) {
    return NULL;
  }

  v->size = 0;
  v->cap = cap;

  v->items = malloc(v->cap * sizeof(*v->items));

  if (!v->items) {
    free(v);
    return NULL;
  }

  return v;
}


void free_vector(Vector *v) {
  if (!v) {
    return;
  }

  free(v->items);
  free(v);
  return;
}

void insert_back_vector(Vector *v, Vector2 item[2]) {
  if (v->size >= v->cap) {
    size_t new_cap = v->cap * 2;

    Vector2 (*tmp)[2] = realloc(v->items, new_cap * sizeof(*v->items));

    if (!tmp) {
      fprintf(stderr, "Failed to reallocate a vector\n");
      // unlikely, so I decided not to handle this
      return;
    }

    v->cap = new_cap;
    v->items = tmp;
  }

  v->items[v->size][0] = item[0];  
  v->items[v->size][1] = item[1]; 
  v->size++;
}

Orientation orientation(Vector2 p1, Vector2 p2, Vector2 p3) {
  int or = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);

  if (or < 0) {
    return COUNTER_CLOCKWISE;
  }

  if (or > 0) {
    return CLOCKWISE;
  }

  return COLLINEAR;
}

bool segments_intersect(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
  int abc = orientation(a, b, c);
  int abd = orientation(a, b, d);

  int cda = orientation(c, d, a);
  int cdb = orientation(c, d, b);

  if (abc != abd && cda != cdb) {
    return true;
  }

  if (abc == COLLINEAR && abd == COLLINEAR && cda == COLLINEAR && cdb == COLLINEAR) {
    return (MAX(a.x, b.x) >= MIN(c.x, d.x) && MAX(c.x, d.x) >= MIN(a.x, b.x)) &&
           (MAX(a.y, b.y) >= MIN(c.y, d.y) && MAX(c.y, d.y) >= MIN(a.y, b.y));
  }

  return false;
}


Vector2 intersection_point(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
    long long denom = (long long)(a.x - b.x) * (c.y - d.y) - (long long)(a.y - b.y) * (c.x - d.x);
    
    if (denom == 0) {
        return (Vector2){0, 0};
    }
    
    long long num_x = (long long)(a.x * b.y - a.y * b.x) * (c.x - d.x) -
                      (long long)(a.x - b.x) * (c.x * d.y - c.y * d.x);
    
    long long num_y = (long long)(a.x * b.y - a.y * b.x) * (c.y - d.y) -
                      (long long)(a.y - b.y) * (c.x * d.y - c.y * d.x);
    
    int x = num_x / denom;
    int y = num_y / denom;
    
    return (Vector2){x, y};
}

// 1. save first line as list of points (or lines)
// 2. convert second line into points (or lines) live
// 3. check second line points  with each of first list

int main(void) {
  FILE *f = fopen("input3.txt", "r");

  if (!f) {
    fprintf(stderr, "ERROR: Failed to open file\n");
    return 1;
  }

  char buf[BUF_SIZE] = {0};
  Vector *v = create_vector(256);

  if (fgets(buf, BUF_SIZE, f) == NULL) {
    fprintf(stderr, "ERROR: Failed to read first line\n");
    return 1;
  }

  long long min_d = INT_MAX;
  Vector2 cur_pos = {0};
  for (size_t i = 0; i < BUF_SIZE; i++) {
    if (buf[i] == '\0')  {
      break;
    }

    char dir = buf[i];
    i++;

    size_t num_start = i;

    while(buf[i] != ',' && buf[i] != '\n') {
      i++;
    }

    size_t num_end = i;
    size_t len = num_end - num_start + 1;

    char* str_num = malloc(num_end - num_start + 2);
    if (!str_num) {
      fprintf(stderr, "ERROR: Failed to allocate number substring\n");
      return 1;
    }

    strncpy(str_num, buf + num_start, len);
    str_num[len] = '\0';

    int num = atoi(str_num);
    // printf("%d\n", num);
    free(str_num);

    Vector2 prev_pos = cur_pos; 
    // save current point and save after switch

    switch(dir) {
    case 'U': {
      cur_pos.y += num;
      break;
    }
    case 'D': {
      cur_pos.y -= num;
      break;
    }
    case 'L': {
      cur_pos.x -= num;
      break;
    }
    case 'R': {
      cur_pos.x += num;
      break;
    }
    }

    Vector2 items[2];

    items[0] = prev_pos;
    items[1] = cur_pos;

    insert_back_vector(v, items);
  }

  memset(buf, 0, BUF_SIZE);

  if (fgets(buf, BUF_SIZE, f) == NULL) {
    fprintf(stderr, "ERROR: Failed to read second line\n");
    return 1;
  }

  cur_pos = (Vector2){0,0};
  for (size_t i = 0; i < BUF_SIZE; i++) {
    if (buf[i] == '\0')  {
      break;
    }

    char dir = buf[i];
    i++;

    size_t num_start = i;

    while(buf[i] != ',' && buf[i] != '\n') {
      i++;
    }

    size_t num_end = i;
    size_t len = num_end - num_start + 1;

    char* str_num = malloc(num_end - num_start + 2);
    if (!str_num) {
      fprintf(stderr, "ERROR: Failed to allocate number substring\n");
      return 1;
    }

    strncpy(str_num, buf + num_start, len);
    str_num[len] = '\0';

    int num = atoi(str_num);
    // printf("%d\n", num);
    free(str_num);

    Vector2 prev_pos = cur_pos; 
    // save current point and save after switch

    switch(dir) {
    case 'U': {
      cur_pos.y += num;
      break;
    }
    case 'D': {
      cur_pos.y -= num;
      break;
    }
    case 'L': {
      cur_pos.x -= num;
      break;
    }
    case 'R': {
      cur_pos.x += num;
      break;
    }
    }

    for (size_t i = 0; i < v->size; i++) {
      Vector2 items[2];
      items[0] = (*(v->items + i))[0];
      items[1] = (*(v->items + i))[1];
      // printf("%d %d -> %d %d\n", items[0].x, items[0].y, items[1].x, items[1].y);

      bool do_intersect = segments_intersect(items[0], items[1], prev_pos, cur_pos);

      if (!do_intersect) {
	continue;
      }

      Vector2 inter = intersection_point(items[0], items[1], prev_pos, cur_pos);

      // NOTE: this is bad, as need to check each of the pairs
      // (or not... now i think i do not need other checks)
      if (inter.x == 0 && inter.y == 0) {
        continue;
      }

      // printf("%d %d\n", inter.x, inter.y);

      // assuming start at 0,0
      long long d = abs(inter.x - 0) + abs(inter.y - 0);
      min_d = MIN(min_d, d);
    }
  }

  printf("%d\n", min_d);

  free_vector(v);
  fclose(f);
  return 0;
}
