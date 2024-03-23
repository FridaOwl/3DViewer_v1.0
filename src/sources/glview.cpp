#include "headers/glview.h"

glView::glView(QWidget* parent) : QOpenGLWidget(parent) { isProjected = false; }

void glView::initializeGL() { glEnable(GL_DEPTH_TEST); }

void glView::DrawObject(obj_t* obj) {
  scale(obj, 1.0);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_DOUBLE, 0, obj->vertices);

  if (vertexShapeType != 0) {
    glPointSize(PointSize);
    glColor3f(ColorVertices.redF(), ColorVertices.greenF(),
              ColorVertices.blueF());

    if (vertexShapeType == 1) {
      glEnable(GL_POINT_SMOOTH);
    } else {
      glDisable(GL_POINT_SMOOTH);
    }

    glDrawArrays(GL_POINTS, 0, obj->vertex_count);
  }

  glDisable(GL_LINE_STIPPLE);
  glLineWidth(LineWidth);
  glColor3f(ColorEdge.redF(), ColorEdge.greenF(), ColorEdge.blueF());

  if (edgeLineType != 0) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x1111);
  }

  glDrawElements(GL_LINES, obj->edge_count * 6, GL_UNSIGNED_INT, obj->edges);

  glDisableClientState(GL_VERTEX_ARRAY);
}

void glView::paintGL() {
  glClearColor(ColorBackground.redF(), ColorBackground.greenF(),
               ColorBackground.blueF(), 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  double aspect = (double)width() / (double)height();

  if (projectionType == 0) {
    glFrustum(-1 * aspect * normalize, 1 * normalize * aspect, -1 * normalize,
              1 * normalize, normalize, 1000 * normalize);
    glTranslatef(0, 0, -2 * normalize);
  }

  if (projectionType == 1) {
    glOrtho(-1 * normalize * aspect, 1 * normalize * aspect, -1 * normalize,
            1 * normalize, -1000 * normalize, 1000 * normalize);
  }

  glEnableClientState(GL_VERTEX_ARRAY);

  if (isProjected) {
    DrawObject(obj);
  }
}

void glView::set_normalize() {
  normalize = 0;
  const size_t vertexCount = obj->vertex_count * 3;

  for (size_t i = 0; i < vertexCount; i++) {
    const double vertexValue = fabs(obj->vertices[i]);
    if (vertexValue > normalize) {
      normalize = vertexValue;
    }
  }
}

void glView::reset_transformation() {
  move_x = 0;
  move_y = 0;
  move_z = 0;
  rotate_x = 0;
  rotate_y = 0;
  rotate_z = 0;
  Sca = 500;
  PointSize = 1;
  LineWidth = 1;
}
