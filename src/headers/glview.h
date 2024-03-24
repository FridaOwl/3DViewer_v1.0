#ifndef GLVIEW_H
#define GLVIEW_H

#define GL_SILENCE_DEPRECATION
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <QMessageBox>
#include <QOpenGLWidget>
#include <QWidget>
#include <QtOpenGL>

extern "C" {
#include "headers/parser.h"
}

class glView : public QOpenGLWidget {
  Q_OBJECT

 private:
  void initializeGL() override;
  void paintGL() override;
  void DrawObject(obj_t *obj);

 public:
  glView(QWidget *parent = nullptr);
  obj_t *obj;
  void reset_transformation();
  void set_normalize();
  int display_method = 0;
  double rotate_x;
  double rotate_y;
  double rotate_z;
  double move_x;
  double move_y;
  double move_z;
  double LineWidth = 1;
  double PointSize = 1;
  double Sca = 500;
  int vertexShapeType, edgeLineType;
  int projectionType = 1;
  bool isProjected;
  QColor ColorEdge = Qt::black;
  QColor ColorBackground = Qt::white;
  QColor ColorVertices = Qt::red;
  GLfloat normalize;
};

#endif  // GLVIEW_H
