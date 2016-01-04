#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    QVector3D position;
    QVector3D realPos;
    QVector2D texCoord;
};

GeometryEngine::GeometryEngine()
{
}

GeometryEngine::~GeometryEngine()
{
    glDeleteBuffers(2, vboIds);
}

void GeometryEngine::init()
{
    initializeGLFunctions();

//! [0]
    // Generate 2 VBOs
    glGenBuffers(2, vboIds);

//! [0]

    // Initializes geometry and transfers it to VBOs
    initGeometry();
}

void GeometryEngine::initGeometry()
{
	// Vertex data contains data for the quad, but also the location of the texture
    VertexData vertices[] = {
        {QVector3D(-1.0, -1.0,  0.0), QVector3D(-10.0, -90.1,  0.0), QVector2D(0,0)},
        {QVector3D( 1.0, -1.0,  0.0), QVector3D( 10.0, -90.1,  0.0), QVector2D(1,0)},
        {QVector3D( 1.0,  1.0,  0.0), QVector3D( 10.0, -0.1,  0.0), QVector2D(1,1)},
        {QVector3D(-1.0,  1.0,  0.0), QVector3D(-10.0, -0.1,  0.0), QVector2D(0,1)},
    };


    //VertexData vertices[] = {
    //    {QVector3D(-1.0, -1.0,  0.0), QVector3D(-20.0, -120.1,  0.0), QVector2D(0,0)},
    //    {QVector3D( 1.0, -1.0,  0.0), QVector3D( 20.0, -120.1,  0.0), QVector2D(1,0)},
    //    {QVector3D( 1.0,  1.0,  0.0), QVector3D( 20.0, -0.1,  0.0), QVector2D(1,1)},
    //    {QVector3D(-1.0,  1.0,  0.0), QVector3D(-20.0, -0.1,  0.0), QVector2D(0,1)},
    //};

    // Indices for drawing faces using quads.
    GLushort indices[] = {
         0,  1,  2,  3
    };

//! [1]
    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData), vertices, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), indices, GL_STATIC_DRAW);
//! [1]
}

//! [2]
void GeometryEngine::drawGeometry(QGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int realPosLocation = program->attributeLocation("a_pos_real");
    program->enableAttributeArray(realPosLocation);
    glVertexAttribPointer(realPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texLocation);
    glVertexAttribPointer(texLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void *)offset);

    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, 0);
}
//! [2]
