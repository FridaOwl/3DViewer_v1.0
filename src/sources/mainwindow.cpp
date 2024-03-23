#include "headers/mainwindow.h"

#include <QMainWindow>
#include <QTimer>

#include "headers/parser.h"
#include "ui_mainwindow.h"

MainViewer::MainViewer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainViewer) {
  ui->setupUi(this);
  init_obj(&objMain, 1, 1);
  timer = new QTimer(0);
  QString settingsFilePath;
#if defined(Q_OS_WIN)
  settingsFilePath = QCoreApplication::applicationDirPath() + "/Settings.ini";
#elif defined(Q_OS_MAC)
  settingsFilePath = QCoreApplication::applicationDirPath() + "/Settings.ini";
#elif defined(Q_OS_LINUX)
  settingsFilePath =
      QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) +
      "/Settings.ini";
#endif

  settings = new QSettings(settingsFilePath, QSettings::IniFormat, this);
  loadSettings();
  connect(timer, SIGNAL(timeout()), this, SLOT(startTimergif()));
}
MainViewer::~MainViewer() {
  clean_obj(&objMain);
  saveSettings();
  delete settings;
  timer->stop();
  delete ui;
  delete timer;
}

void MainViewer::on_load_file_clicked() {
  QString path =
      QFileDialog::getOpenFileName(this, "Выберите файл", "/", "Файлы (*.obj)");
  if (objMain.edge_count > 1 && path.length() != 0) {
    clean_obj(&objMain);
    init_obj(&objMain, 1, 1);
  }
  int error_code = open_file(qPrintable(path), &objMain);
  if (error_code != OK && path.length() > 0) {
    ui->label_file->setText("error File");
  } else if (path.length() > 0) {
    qsizetype idx = path.lastIndexOf('/') + 1;
    QString file_name = path.right(path.size() - idx);
    ui->label_file->setText(file_name);
    ui->count_vertices->setText(QString::number(objMain.vertex_count));
    ui->count_edges->setText(QString::number(objMain.edge_count));
    ui->openGLWidget->obj = &objMain;
    ui->openGLWidget->isProjected = true;
    ui->openGLWidget->set_normalize();
    ui->openGLWidget->reset_transformation();
    ui->spin_move_x->setValue(ui->openGLWidget->move_x);
    ui->spin_move_y->setValue(ui->openGLWidget->move_y);
    ui->spin_move_z->setValue(ui->openGLWidget->move_z);
    ui->spin_rotate_x->setValue(ui->openGLWidget->rotate_x);
    ui->spin_rotate_y->setValue(ui->openGLWidget->rotate_y);
    ui->spin_rotate_z->setValue(ui->openGLWidget->rotate_z);
    ui->hor_scale->setValue(ui->openGLWidget->Sca);
    ui->lineWidth->setValue(ui->openGLWidget->LineWidth);
    ui->pointSize->setValue(ui->openGLWidget->PointSize);
    update();
  }
}

/////////////         SCALE      ///////////////
void MainViewer::on_hor_scale_valueChanged(double value) {
  double temp = value;
  if (Sca != 0) value /= Sca;
  scale(&objMain, value);

  Sca = temp;
  ui->openGLWidget->update();
}

/////////////         MOVE             ///////////////
void MainViewer::on_spin_move_x_valueChanged(double value) {
  static double prevValue = 0.0;
  double delta = value - prevValue;
  move_x(&objMain, delta / 100.0);
  prevValue = value;
  ui->openGLWidget->update();
}

void MainViewer::on_spin_move_y_valueChanged(double value) {
  static double prevValue = 0.0;
  double delta = value - prevValue;
  move_y(&objMain, delta / 100.0);
  prevValue = value;
  ui->openGLWidget->update();
}

void MainViewer::on_spin_move_z_valueChanged(double value) {
  static double prevValue = 0.0;
  double delta = value - prevValue;
  move_z(&objMain, delta / 100.0);
  prevValue = value;
  ui->openGLWidget->update();
}

/////////////      ROTATE          ///////////////
void MainViewer::on_spin_rotate_x_valueChanged(double value) {
  static double prevValue = 0.0;
  double delta = value - prevValue;
  rotate_x(&objMain, delta / 100.0);
  prevValue = value;
  ui->openGLWidget->update();
}

void MainViewer::on_spin_rotate_y_valueChanged(double value) {
  static double prevValue = 0.0;
  double delta = value - prevValue;
  rotate_y(&objMain, delta / 100.0);
  prevValue = value;
  ui->openGLWidget->update();
}

void MainViewer::on_spin_rotate_z_valueChanged(double value) {
  static double prevValue = 0.0;
  double delta = value - prevValue;
  rotate_z(&objMain, delta / 100.0);
  prevValue = value;
  ui->openGLWidget->update();
}

/////////////    SCREEN        ///////////////
void MainViewer::on_SCREEN_clicked() {
  QString filePath = QFileDialog::getSaveFileName(
      this, tr("Save File"), "/", tr("Text Files (*.jpeg);;Файлы (*.bmp)"));
  QFile file(filePath);
  if (file.open(QIODevice::WriteOnly)) {
    QImage img = ui->openGLWidget->grabFramebuffer();
    if (img.save(filePath)) {
      QMessageBox::information(this, tr("Success"),
                               tr("Image saved successfully."));
    } else {
      QMessageBox::warning(this, tr("Error"), tr("Failed to save the image."));
    }
    file.close();
  } else {
    QMessageBox::warning(this, tr("Error"), tr("Could not save the file."));
  }
}

/////////////    GIF      ///////////////
void MainViewer::on_gif_clicked() {
  filePat = QFileDialog::getSaveFileName(this, tr("Save File"), "/",
                                         tr("Файлы (*.gif)"));

  if (filePat.isEmpty()) {
    QMessageBox::warning(this, tr("Error"), tr("Could not save the file."));
  } else {
    ui->gif->setEnabled(false);
    gif_frame = new QGifImage;
    gif_frame->setDefaultDelay(10);
    timer->setInterval(100);
    timecount = 0;
    timer->start();
  }
}

void MainViewer::startTimergif() {
  if (gif_frame && timer) {
    QImage img = ui->openGLWidget->grabFramebuffer();
    QSize img_size(640, 480);
    QImage scaled_img = img.scaled(img_size);
    gif_frame->addFrame(scaled_img);
    const int MaxFrames = 50;
    if (timecount == MaxFrames) {
      timer->stop();
      gif_frame->save(filePat);
      QMessageBox::information(0, "/", "Gif animation saved successfully");
      delete gif_frame;
      gif_frame = nullptr;
      ui->gif->setEnabled(true);
    }
    timecount++;
  }
}

/////////////    COLOR    ///////////////
void MainViewer::on_Color_Backg_clicked() {
  QColor ColorValue = QColorDialog::getColor(ui->openGLWidget->ColorBackground,
                                             this, tr("Select Color"));
  if (ColorValue.isValid()) {
    ui->openGLWidget->ColorBackground = ColorValue;
    ui->openGLWidget->update();
  }
}

void MainViewer::on_Color_Edge_clicked() {
  QColor ColorValue = QColorDialog::getColor(ui->openGLWidget->ColorEdge, this,
                                             tr("Select Color"));
  if (ColorValue.isValid()) {
    ui->openGLWidget->ColorEdge = ColorValue;
    ui->openGLWidget->update();
  }
}

void MainViewer::on_Color_Vertic_clicked() {
  QColor ColorValue = QColorDialog::getColor(ui->openGLWidget->ColorVertices,
                                             this, tr("Select Color"));
  if (ColorValue.isValid()) {
    ui->openGLWidget->ColorVertices = ColorValue;
    ui->openGLWidget->update();
  }
}

/////////////    BUTTON      ///////////////
void MainViewer::on_radio_Paralel_clicked() {
  ui->openGLWidget->projectionType = 0;
  ui->openGLWidget->update();
}

void MainViewer::on_radio_Central_clicked() {
  ui->openGLWidget->projectionType = 1;
  ui->openGLWidget->update();
}

void MainViewer::on_lineWidth_valueChanged(double value) {
  ui->openGLWidget->LineWidth = value;
  ui->openGLWidget->update();
}

void MainViewer::on_pointSize_valueChanged(double value) {
  ui->openGLWidget->PointSize = value;
  ui->openGLWidget->update();
}

/////////////   Settings     ///////////////
void MainViewer::saveSettings() {
  settings->setValue("TypeProjection", ui->openGLWidget->projectionType);
  settings->setValue("ThicknessOfEdge", ui->openGLWidget->LineWidth);
  settings->setValue("ColorEdge",
                     ui->openGLWidget->ColorEdge.name(QColor::HexRgb));
  settings->setValue("SizeVertices", ui->openGLWidget->PointSize);
  settings->setValue("ColorVertices",
                     ui->openGLWidget->ColorVertices.name(QColor::HexRgb));
  settings->setValue("ColorBackground",
                     ui->openGLWidget->ColorBackground.name(QColor::HexRgb));
  settings->setValue("EdgeLineType", ui->openGLWidget->edgeLineType);
  settings->setValue("VertexShapeType", ui->openGLWidget->vertexShapeType);
}

void MainViewer::loadSettings() {
  int projectionType = settings->value("TypeProjection", 0).toInt();
  double lineWidth = settings->value("ThicknessOfEdge", 1.0).toDouble();
  QString colorEdge = settings->value("ColorEdge", "#000000").toString();
  double pointSize = settings->value("SizeVertices", 1.0).toDouble();
  QString colorVertices =
      settings->value("ColorVertices", "#000000").toString();
  QString colorBackground =
      settings->value("ColorBackground", "#FFFFFF").toString();
  int edgeLineType = settings->value("EdgeLineType", 0).toInt();
  int vertexShapeType = settings->value("VertexShapeType", 0).toInt();

  ui->openGLWidget->projectionType = projectionType;
  ui->openGLWidget->LineWidth = lineWidth;
  ui->openGLWidget->ColorEdge = QColor(colorEdge);
  ui->openGLWidget->PointSize = pointSize;
  ui->openGLWidget->ColorVertices = QColor(colorVertices);
  ui->openGLWidget->ColorBackground = QColor(colorBackground);
  ui->openGLWidget->edgeLineType = edgeLineType;
  ui->openGLWidget->vertexShapeType = vertexShapeType;
  ui->openGLWidget->update();
}

/////////////    solid & dashed    ///////////////
void MainViewer::on_radio_solid_clicked() {
  ui->openGLWidget->edgeLineType = 0;
  ui->openGLWidget->update();
}

void MainViewer::on_radio_dashed_clicked() {
  ui->openGLWidget->edgeLineType = 1;
  ui->openGLWidget->update();
}

/////////////    no circle square  ///////////////
void MainViewer::on_radio_no_clicked() {
  ui->openGLWidget->vertexShapeType = 0;
  ui->openGLWidget->update();
}

void MainViewer::on_radio_Circle_clicked() {
  ui->openGLWidget->vertexShapeType = 1;
  ui->openGLWidget->update();
}

void MainViewer::on_radio_Square_clicked() {
  ui->openGLWidget->vertexShapeType = 2;
  ui->openGLWidget->update();
}
