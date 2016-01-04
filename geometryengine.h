#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QGLFunctions>
#include <QGLShaderProgram>

class GeometryEngine : protected QGLFunctions
{
public:
    GeometryEngine();
    virtual ~GeometryEngine();

    void init();
    void drawGeometry(QGLShaderProgram *program);

private:
    void initGeometry();

    GLuint vboIds[2];
};

#endif // GEOMETRYENGINE_H
