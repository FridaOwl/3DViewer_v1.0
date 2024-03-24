#ifndef MAINVIEWER_H
#define MAINVIEWER_H

#include <qgifimage.h>

#include <QColorDialog>
#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QMainWindow>
#include <QMessageBox>
#include <QOpenGLFramebufferObject>
#include <QRadioButton>
#include <QSettings>

extern "C" {
#include "headers/parser.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainViewer;
}
QT_END_NAMESPACE

class MainViewer : public QMainWindow {
  Q_OBJECT

 public:
  MainViewer(QWidget *parent = nullptr);
  ~MainViewer();

 private slots:

  void on_load_file_clicked();

  void on_spin_move_x_valueChanged(double value);

  void on_spin_move_y_valueChanged(double value);

  void on_spin_move_z_valueChanged(double value);

  void on_spin_rotate_x_valueChanged(double value);

  void on_spin_rotate_y_valueChanged(double value);

  void on_spin_rotate_z_valueChanged(double value);

  void on_SCREEN_clicked();

  void startTimergif();

  void on_gif_clicked();

  void on_lineWidth_valueChanged(double value);

  void on_pointSize_valueChanged(double value);

  void on_hor_scale_valueChanged(double value);

  void on_Color_Backg_clicked();

  void on_Color_Edge_clicked();

  void on_Color_Vertic_clicked();

  void on_radio_Paralel_clicked();

  void on_radio_Central_clicked();

  void loadSettings();
  void saveSettings();

  void on_radio_solid_clicked();

  void on_radio_dashed_clicked();

  void on_radio_no_clicked();

  void on_radio_Circle_clicked();

  void on_radio_Square_clicked();

 private:
  double Sca = 500;
  Ui::MainViewer *ui;
  obj_t objMain;
  QSettings *settings;
  QGifImage *gif_frame;
  QTimer *timer;
  QString filePat;
  int timecount = 0;
};
#endif  // MAINWINDOW_H
