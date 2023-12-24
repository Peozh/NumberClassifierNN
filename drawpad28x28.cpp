#include "drawpad28x28.h"
#include <QMouseEvent>
#include <QFile>

DrawPad28x28::DrawPad28x28(QWidget* parent) : QOpenGLWidget(parent)
{
    for (uint64_t y = 0; y < RES_Y; ++y)
    {
        for (uint64_t x = 0; x < RES_X; ++x)
        {
            texture[y][x] = 0.0f;
        }
    }
}

void DrawPad28x28::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    glGenTextures(1, &textureId); // 텍스쳐 id 에 텍스쳐를 하나 할당합니다.
    glActiveTexture(GL_TEXTURE0); // 활성화할 텍스쳐 슬롯을 지정합니다.
    glBindTexture(GL_TEXTURE_2D, textureId); // 현재 활성화된 텍스쳐 슬롯에 실질 텍스쳐를 지정합니다.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, RES_X, RES_Y, 0, GL_RED, GL_FLOAT, texture); // 텍스쳐 이미지가 RED 단일 채널이며, float 입니다. border 는 0 만 유효합니다.
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED ); // Red to Gray scale
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED ); // Red to Gray scale
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // s 축의 비어있는 텍스쳐 외부를 border 색상으로 채웁니다.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); // t 축의 비어있는 텍스쳐 외부를 border 색상으로 채웁니다.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // 텍스쳐 확대 시 fragment 를 최근접 값으로 설정합니다.
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // 텍스쳐 축소 시 fragment 를 최근접 값으로 설정합니다.
    float colour[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colour); // 텍스쳐 border 색상을 결정합니다.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // (default)

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}

void DrawPad28x28::paintGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double left = -1.0;
    double right = 1.0;
    double top = 1.0;
    double bottom = -1.0;
    glOrtho(left, right, bottom, top, -1, 1);

    GLfloat backgroundVertices[4][3] = {
        {-1, -1, 0},
        { 1, -1, 0},
        { 1,  1, 0},
        {-1,  1, 0} };
    GLubyte VertexOrder[4] = { 0, 1, 2, 3 };
    GLfloat texture2DCoords[4][2] = {
        {0, 1},
        {1, 1},
        {1, 0},
        {0, 0}
    };

    glClear(GL_COLOR_BUFFER_BIT);


    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glVertexPointer(3, GL_FLOAT, 0, backgroundVertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texture2DCoords);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, VertexOrder);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);

    // clean up
    glFlush();
    glDisableClientState(GL_VERTEX_ARRAY);
}

void DrawPad28x28::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double left = -1.0;
    double right = 1.0;
    double top = 1.0;
    double bottom = -1.0;
    glOrtho(left, right, bottom, top, -1, 1);
}

void DrawPad28x28::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        float x = event->pos().x();
        float y = event->pos().y();
        if (x <= 0) x = 1;
        if (y <= 0) y = 1;
        if (x >= width()) x = width()-1;
        if (y >= height()) y = height()-1;
        x /= width();
        y /= height();
        drawDot(x, y);
    }
}

void DrawPad28x28::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        float x = event->pos().x();
        float y = event->pos().y();
        if (x <= 0) x = 1;
        if (y <= 0) y = 1;
        if (x >= width()) x = width()-1;
        if (y >= height()) y = height()-1;
        x /= width();
        y /= height();
        drawDot(x, y);
        qDebug() << "x, y = " << x << ", " << 1.0f-y;
    }
    else if (event->button() == Qt::RightButton)
    {
        for (uint64_t y = 0; y < RES_Y; ++y)
        {
            for (uint64_t x = 0; x < RES_X; ++x)
            {
                texture[y][x] = 0.0f;
            }
        }
        glActiveTexture(GL_TEXTURE0); // 활성화할 텍스쳐 슬롯을 지정합니다.
        glBindTexture(GL_TEXTURE_2D, textureId); // 현재 활성화된 텍스쳐 슬롯에 실질 텍스쳐를 지정합니다.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, RES_X, RES_Y, 0, GL_RED, GL_FLOAT, texture); // 텍스쳐 이미지가 RED 단일 채널이며, float 입니다. border 는 0 만 유효합니다.
        this->update();
    }
}

void DrawPad28x28::mouseReleaseEvent(QMouseEvent *event)
{

}

void DrawPad28x28::drawDot(float normX, float normY)
{
    float middle_x = normX * RES_X;
    float middle_y = normY * RES_Y;

    qDebug() << "middle_x, middle_y = " << middle_x << ", " << middle_y;
    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            float x = middle_x + dx;
            float y = middle_y + dy;
            if (x < 0 || x > RES_X) continue;
            if (y < 0 || y > RES_Y) continue;
            float lx = abs(0.5f + int(x) - middle_x);
            float ly = abs(0.5f + int(y) - middle_y);
            float color = 1.0f - sqrtf(lx*lx + ly*ly)/(3.0f/2.0f*sqrtf(2));
            texture[(uint64_t)y][(uint64_t)x] = std::max(color, texture[(uint64_t)y][(uint64_t)x]);
        }
    }

    glActiveTexture(GL_TEXTURE0); // 활성화할 텍스쳐 슬롯을 지정합니다.
    glBindTexture(GL_TEXTURE_2D, textureId); // 현재 활성화된 텍스쳐 슬롯에 실질 텍스쳐를 지정합니다.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, RES_X, RES_Y, 0, GL_RED, GL_FLOAT, texture); // 텍스쳐 이미지가 RED 단일 채널이며, float 입니다. border 는 0 만 유효합니다.
    emit inputUpdated(&texture[0][0]);
    this->update();
}
