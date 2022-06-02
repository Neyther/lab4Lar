#ifndef GLSCENE_H
#define GLSCENE_H

#include <QtOpenGLWidgets/QtOpenGLWidgets>
#include <iostream>
#include <algorithm>
#include <QDebug>

class glScene : public QOpenGLWidget
{
public:
    glScene(QWidget *parent = nullptr);
    void initializeGL() override;
    void resizeGL(int _w, int _h) override;
    void paintGL() override;
    std::vector<QPoint> getHexagonCoords();
    void drawHexagon(std::vector<QPoint> points);
    void drawTriangle(QPoint point1, QPoint point2, QPoint point3);
    void drawCircles(QPoint point1, QPoint point2);
    void drawElement(std::vector<QPoint> points, QPoint circlePoint);
    void findCirclesIntersection();
    std::vector<QPoint> getPointsForElement(QPoint point1, QPoint point2, int num);
    std::vector<std::pair<double, double>> getRotateMatrix(double angle);
    QPoint getNewCoords(std::vector<std::pair<double, double>> rotateMatrix, QPoint oldCoords, QPoint point);
public slots:
    void drawGL();
    void next();
    void prev();

private:
    float _height, _width;
    QPoint hexagonCenter;
    double radius;
    std::vector<QPoint> circlePoints;
    std::vector<QPoint> circlesIntersection;
    int level = -1;
};

#endif // GLSCENE_H
