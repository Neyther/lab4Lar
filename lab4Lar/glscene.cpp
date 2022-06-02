#include "glscene.h"

glScene::glScene(QWidget *parent) : QOpenGLWidget(parent)
{
    this->level = -1;
}

void glScene::initializeGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void glScene::resizeGL(int _x, int _y) {
    glViewport(0, 0, _x, _y);
    this->_width = _x;
    this->_height = _y;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (float)_x, 0, (float)_y, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void glScene::paintGL() {
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(level == 8)
        this->circlePoints.clear();

    std::vector<QPoint> points = getHexagonCoords();
    if(this->level == 0 || this->level > 0)
        drawHexagon(points);

    if(level == 1 || level > 1)
        drawTriangle(points[0], points[2], points[4]);
    if(level == 2 || level > 2)
        drawTriangle(points[1], points[3], points[5]);

    if(level == 3 || level > 3)
        drawCircles(this->hexagonCenter, points[0]);
    if(level == 4 || level > 4)
        drawCircles(this->hexagonCenter, points[1]);
    if(level == 5 || level > 5)
        drawCircles(this->hexagonCenter, points[2]);
    if(level == 6 || level > 6)
        drawCircles(this->hexagonCenter, points[3]);
    if(level == 7 || level > 7)
        drawCircles(this->hexagonCenter, points[4]);
    if(level == 8 || level > 8)
        drawCircles(this->hexagonCenter, points[5]);

    if(level == 9){
        findCirclesIntersection();
    }

    if(level == 9 || level > 9){
        std::vector<QPoint> points1 = getPointsForElement(this->hexagonCenter, points[0], 0);
        drawElement(points1, this->circlesIntersection[0]);
    }

    if(level == 10 || level > 10){
        std::vector<QPoint> points2 = getPointsForElement(this->hexagonCenter, points[1], 1);
        drawElement(points2, this->circlesIntersection[1]);
    }

    if(level == 11 || level > 11){
        std::vector<QPoint> points3 = getPointsForElement(this->hexagonCenter, points[2], 2);
        drawElement(points3, this->circlesIntersection[2]);
    }

    if(level == 12 || level > 12){
        std::vector<QPoint> points4 = getPointsForElement(this->hexagonCenter, points[3], 3);
        drawElement(points4, this->circlesIntersection[3]);
    }

    if(level == 13 || level > 13){
        std::vector<QPoint> points5 = getPointsForElement(this->hexagonCenter, points[4], 4);
        drawElement(points5, this->circlesIntersection[4]);
    }

    if(level == 14 || level > 14){
        std::vector<QPoint> points6 = getPointsForElement(this->hexagonCenter, points[5], 5);
        drawElement(points6, this->circlesIntersection[5]);
    }
}

std::vector<QPoint> glScene::getHexagonCoords() {
    QPoint tempPoint;
    std::vector<QPoint> points;
    double hexagonSide = (this->_height - 40)/2;
    double oldX, oldY;

    // First point
    tempPoint.setX(this->_width/2);
    tempPoint.setY(20);
    points.push_back(tempPoint);
    this->hexagonCenter.setX(tempPoint.x());
    this->hexagonCenter.setY(tempPoint.y() + hexagonSide);

    // Second point
    oldX = tempPoint.x();
    oldY = tempPoint.y();
    double verticalSide = hexagonSide/2;
    double horizontalSide = hexagonSide * cos(30 * M_PI / 180);
    oldX += horizontalSide;
    oldY += verticalSide;
    tempPoint.setX(oldX);
    tempPoint.setY(oldY);
    points.push_back(tempPoint);

    // Third point
    oldY = tempPoint.y() + hexagonSide;
    tempPoint.setY(oldY);
    points.push_back(tempPoint);

    // Fourth point
    tempPoint = points[0];
    oldY = tempPoint.y() + hexagonSide * 2;
    tempPoint.setY(oldY);
    points.push_back(tempPoint);

    // Fifth point
    tempPoint = points[2];
    oldX = tempPoint.x() - horizontalSide * 2;
    tempPoint.setX(oldX);
    points.push_back(tempPoint);

    // Sixth point
    oldY = tempPoint.y() - hexagonSide;
    tempPoint.setY(oldY);
    points.push_back(tempPoint);

    return points;
}

void glScene::drawHexagon(std::vector<QPoint> points) {
    for(int i = 0;i < 5; i++){
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);    glVertex2d(points[i].x(), points[i].y());
        glColor3f(0.0f, 0.0f, 0.0f);    glVertex2d(points[i+1].x(), points[i+1].y());
        glEnd();
    }

    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);    glVertex2d(points[5].x(), points[5].y());
    glColor3f(0.0f, 0.0f, 0.0f);    glVertex2d(points[0].x(), points[0].y());
    glEnd();
}

void glScene::drawTriangle(QPoint point1, QPoint point2, QPoint point3) {
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);    glVertex2d(point1.x(), point1.y());
    glColor3f(0.0f, 0.0f, 0.0f);    glVertex2d(point2.x(), point2.y());
    glColor3f(0.0f, 0.0f, 0.0f);    glVertex2d(point3.x(), point3.y());
    glColor3f(0.0f, 0.0f, 0.0f);    glVertex2d(point1.x(), point1.y());
    glEnd();
}

void glScene::drawCircles(QPoint point1, QPoint point2) {
    QPoint centerPoint;
    double radius;
    centerPoint.setX((point1.x() + point2.x()) / 2);
    centerPoint.setY((point1.y() + point2.y()) / 2);

    QPoint circleCenter;
    circleCenter.setX((point1.x() + centerPoint.x()) / 2);
    circleCenter.setY((point1.y() + centerPoint.y()) / 2);
    this->circlePoints.push_back(circleCenter);

    // Find radius
    QPoint tempPoint;
    tempPoint.setX((centerPoint.x() + point2.x()) / 2);
    tempPoint.setY((centerPoint.y() + point2.y()) / 2);

    tempPoint.setX((centerPoint.x() + tempPoint.x()) / 2);
    tempPoint.setY((centerPoint.y() + tempPoint.y()) / 2);

    tempPoint.setX((centerPoint.x() + tempPoint.x()) / 2);
    tempPoint.setY((centerPoint.y() + tempPoint.y()) / 2);

    radius = sqrt(pow(tempPoint.x() - circleCenter.x(), 2) + pow(tempPoint.y() - circleCenter.y(), 2));
    this->radius = radius;

    glBegin(GL_LINE_LOOP);
    for(double i = 0; i < 360; i+=0.01){
        double theta = 2.0f * M_PI * i / 360;

        double x = radius * cosf(theta);
        double y = radius * sinf(theta);

        tempPoint.setX(x + circleCenter.x());
        tempPoint.setY(y + circleCenter.y());

        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2d(tempPoint.x(), tempPoint.y());
    }
    glEnd();
}

void glScene::drawElement(std::vector<QPoint> points, QPoint circlePoint) {
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0f, 0.0f, 0.0f);    glVertex2d(circlePoint.x(), circlePoint.y());
    glColor3f(0.0f, 0.0f, 0.0f);    glVertex2d(points[1].x(), points[1].y());
    glColor3f(0.0f, 0.0f, 0.0f);    glVertex2d(points[0].x(), points[0].y());
    glColor3f(0.0f, 0.0f, 0.0f);    glVertex2d(points[2].x(), points[2].y());
    glColor3f(0.0f, 0.0f, 0.0f);    glVertex2d(circlePoint.x(), circlePoint.y());
    glEnd();
}

void glScene::findCirclesIntersection() {
    QPoint A = this->circlePoints[5];
    QPoint C = this->circlePoints[1];
    QPoint B;
    QPoint H; H.setX((C.x() + A.x())/2); H.setY(C.y());

    double BC = this->radius;
    double CH = sqrt(pow(C.x()-H.x(), 2));

    double BH = sqrt(pow(BC, 2) - pow(CH, 2));
    B.setX(H.x());
    B.setY(H.y() + BH);

    double smallRadius = sqrt(pow(B.x() - this->hexagonCenter.x(), 2) + pow(B.y() - this->hexagonCenter.y(), 2));

    QPoint tempPoint = QPoint(B.x(), B.y() - smallRadius*2);
    this->circlesIntersection.push_back(tempPoint);

    tempPoint = QPoint(tempPoint.x() + smallRadius * cos(30*M_PI/180), tempPoint.y() + smallRadius/2);
    this->circlesIntersection.push_back(tempPoint);

    tempPoint = QPoint(tempPoint.x(), tempPoint.y() + smallRadius);
    this->circlesIntersection.push_back(tempPoint);

    tempPoint = QPoint(tempPoint.x() - smallRadius*cos(30*M_PI/180), tempPoint.y() + smallRadius/2);
    this->circlesIntersection.push_back(tempPoint);

    tempPoint = QPoint(tempPoint.x() - smallRadius*cos(30*M_PI/180), tempPoint.y() - smallRadius/2);
    this->circlesIntersection.push_back(tempPoint);

    tempPoint = QPoint(tempPoint.x(), tempPoint.y() - smallRadius);
    this->circlesIntersection.push_back(tempPoint);
}

std::vector<QPoint> glScene::getPointsForElement(QPoint point1, QPoint point2, int num) {
    std::vector<QPoint> outputPoints;
    QPoint tempPoint, centerPoint;
    std::vector<std::pair<double, double>> rotateMatrix;
    
    // First point
    tempPoint.setX((point1.x() + point2.x()) / 2);
    tempPoint.setY((point1.y() + point2.y()) / 2);
    centerPoint = tempPoint;

    tempPoint.setX((tempPoint.x() + point2.x()) / 2);
    tempPoint.setY((tempPoint.y() + point2.y()) / 2);

    tempPoint.setX((tempPoint.x() + point2.x()) / 2);
    tempPoint.setY((tempPoint.y() + point2.y()) / 2);

    outputPoints.push_back(tempPoint);
    
    //Second point
    QPoint A = outputPoints[0];
    QPoint B = this->circlePoints[num+1];
    QPoint C;
    if(num == 0){
        C = this->circlePoints[5];
    }
    else{
        C = this->circlePoints[num-1];
    }
    QPoint D = this->circlePoints[num];
    QPoint E;
    E.setX(D.x());
    E.setY(D.y() + this->radius);

    double AB = sqrt(pow(A.x() - B.x(), 2) + pow(A.y() - B.y(), 2));
    double BC = sqrt(pow(B.x() - C.x(), 2) + pow(B.y() - C.y(), 2));
    double AC = sqrt(pow(A.x() - C.x(), 2) + pow(A.y() - C.y(), 2));

    double alpha = acos((pow(AB, 2) + pow(AC, 2) - pow(BC, 2)) / (2 * AB * AC)) * 180.0 / M_PI;
    double beta;
    switch(num){
    case(0):
        beta = alpha/2-22.5;
        break;
    case(1):
        beta = alpha/2-60;
        break;
    case(2):
        beta = 37.5-alpha/2;
        break;
    case(3):
        beta = 7.5-alpha/2;
        break;
    case(4):
        beta = alpha/2-52.5;
        break;
    case(5):
        beta = 45-alpha/2;
        break;
    }

    rotateMatrix = getRotateMatrix(-beta);
    tempPoint = getNewCoords(rotateMatrix, this->circlePoints[num+1], A);
    double newRadius = this->radius * sqrt(pow(A.x() - tempPoint.x(), 2) + pow(A.y() - tempPoint.y(), 2)) / AB;
    if(num == 1 || num == 2)
        tempPoint.setX(tempPoint.x()+newRadius);
    else if(num == 4 || num == 5)
        tempPoint.setX(tempPoint.x() - newRadius);
    else if(num == 0)
        tempPoint.setY(tempPoint.y() - newRadius);
    else
        tempPoint.setY(tempPoint.y() + newRadius);
    rotateMatrix = getRotateMatrix(beta);
    tempPoint = getNewCoords(rotateMatrix, tempPoint, A);
    outputPoints.push_back(tempPoint);

    rotateMatrix = getRotateMatrix(-beta-(alpha/2));
    if(num == 0)
        tempPoint = getNewCoords(rotateMatrix, this->circlePoints[5], A);
    else
        tempPoint = getNewCoords(rotateMatrix, this->circlePoints[num-1], A);
    if(num == 1 || num == 2)
        tempPoint.setX(tempPoint.x()+newRadius);
    else if(num == 4 || num == 5)
        tempPoint.setX(tempPoint.x() - newRadius);
    else if(num == 0)
        tempPoint.setY(tempPoint.y() - newRadius);
    else
        tempPoint.setY(tempPoint.y() + newRadius);
    rotateMatrix = getRotateMatrix(beta+(alpha/2));
    tempPoint = getNewCoords(rotateMatrix, tempPoint, A);
    outputPoints.push_back(tempPoint);

    return outputPoints;
}

std::vector<std::pair<double, double>> glScene::getRotateMatrix(double angle){
    std::vector<std::pair<double, double>> returnMatrix;
    std::pair<double, double> tempLine;

    tempLine.first = cos(angle * M_PI/180);
    tempLine.second = -sin(angle * M_PI/180);
    returnMatrix.push_back(tempLine);

    tempLine.first = sin(angle * M_PI/180);
    tempLine.second = cos(angle * M_PI/180);
    returnMatrix.push_back(tempLine);

    return returnMatrix;
}

QPoint glScene::getNewCoords(std::vector<std::pair<double, double>> rotateMatrix, QPoint oldCoords, QPoint point){
    QPoint returnCoords;

    returnCoords.setX(rotateMatrix[0].first * (oldCoords.x() - point.x()) + rotateMatrix[0].second * (oldCoords.y() - point.y()));
    returnCoords.setY(rotateMatrix[1].first * (oldCoords.x() - point.x()) + rotateMatrix[1].second * (oldCoords.y() - point.y()));

    returnCoords.setX(returnCoords.x() + point.x());
    returnCoords.setY(returnCoords.y() + point.y());

    return returnCoords;
}

void glScene::next(){
    if(this->level == 14)
        return;
    this->level++;
    this->update();
}

void glScene::prev(){
    if(this->level == -1)
        return;
    this->level--;
    this->update();
}
