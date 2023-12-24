#ifndef DRAWPAD28X28_H
#define DRAWPAD28X28_H

#define RES_X 28
#define RES_Y 28

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class DrawPad28x28 : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
    float texture[RES_Y][RES_X];
    GLuint textureId;

public:
    DrawPad28x28(QWidget* parent = nullptr);

    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;

    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    void drawDot(float normX, float normY);

signals:
    void inputUpdated(float* data);
};

#endif // DRAWPAD28X28_H
