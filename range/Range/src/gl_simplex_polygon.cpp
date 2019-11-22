/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_simplex_polygon.cpp                                   *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   7-th August 2015                                         *
 *                                                                   *
 *  DESCRIPTION: OpenGL simplex polygon class definition             *
 *********************************************************************/

#include "gl_simplex_polygon.h"


void GLSimplexPolygon::_init(const GLSimplexPolygon *pGlPolygon)
{
    if (pGlPolygon)
    {
        this->thickness = pGlPolygon->thickness;
    }
}

GLSimplexPolygon::GLSimplexPolygon(GLWidget *glWidget, const std::vector<RR3Vector> &nodes, double thickness)
    : GLSimplex(glWidget,nodes)
    , thickness(thickness)
{
    this->_init();
}

GLSimplexPolygon::GLSimplexPolygon(const GLSimplexPolygon &glPolygon)
    : GLSimplex(glPolygon)
{
    this->_init(&glPolygon);
}

GLSimplexPolygon::~GLSimplexPolygon()
{

}

GLSimplexPolygon &GLSimplexPolygon::operator =(const GLSimplexPolygon &glPolygon)
{
    this->GLSimplex::operator =(glPolygon);
    this->_init(&glPolygon);
    return (*this);
}

void GLSimplexPolygon::initialize(void)
{

}

void GLSimplexPolygon::finalize(void)
{

}

void GLSimplexPolygon::draw(void)
{
    uint nn = uint(this->nodes.size());

    if (nn < 3)
    {
        return;
    }

    bool volumeElement = (this->thickness > RConstants::eps);
    bool useTexture = (this->nodeTextureCoordinates.size() == nn);

    std::vector<RNode> nodes1;
    std::vector<RNode> nodes2;

    nodes1.resize(nn);
    nodes2.resize(nn);

    for (uint i=0;i<nn;i++)
    {
        nodes1[i] = RNode(this->nodes[i]);
        nodes2[i] = RNode(this->nodes[i]);
    }

    if (volumeElement)
    {
        RR3Vector offset(0.0,0.0,0.0);
        offset = RPolygon(nodes1).getNormal();
        offset *= this->thickness;

        for (uint i=0;i<nn;i++)
        {
            nodes1[i].move(offset);
        }

        offset *= -1.0;

        for (uint i=0;i<nn;i++)
        {
            nodes2[i].move(offset);
        }
    }

    if (drawTypeMask & GLSimplex::NodeIds)
    {
        this->getGLWidget()->qglColor(QColor(Qt::black));

        for (uint i=0;i<nn;i++)
        {
            this->getGLWidget()->renderText(nodes1[i].getX(),
                                            nodes1[i].getY(),
                                            nodes1[i].getZ(),
                                            QString::number(this->nodeIds[i]),
                                            QFont("Courier",8));
        }
    }
    if (drawTypeMask & GLSimplex::ElementId)
    {
        this->getGLWidget()->qglColor(QColor(Qt::black));

        double x = 0.0, y = 0.0, z = 0.0;

        for (uint i=0;i<nn;i++)
        {
            x += nodes1[i].getX();
            y += nodes1[i].getY();
            z += nodes1[i].getZ();
        }

        x /= double(nn);
        y /= double(nn);
        z /= double(nn);

        this->getGLWidget()->renderText(x,y,z,QString::number(this->id),QFont("Courier",8));
    }
    if (drawTypeMask & GLSimplex::ElementEdges)
    {
        this->getGLWidget()->qglColor(Qt::black);
        this->drawWired(nodes1,nodes2,volumeElement,useTexture);
    }
    if (drawTypeMask & GLSimplex::ElementNodes)
    {
        this->getGLWidget()->qglColor(Qt::black);
        this->drawNodes(nodes1,nodes2,volumeElement);
    }
    if (drawTypeMask & GLSimplex::Wired)
    {
        this->getGLWidget()->qglColor(useTexture ? Qt::white : this->color);
        this->drawWired(nodes1,nodes2,volumeElement,useTexture);
    }
    if (drawTypeMask & GLSimplex::Normal)
    {
        this->getGLWidget()->qglColor(useTexture ? Qt::white : this->color);
        this->drawNormal(nodes1,nodes2,volumeElement,useTexture);
    }
}

void GLSimplexPolygon::drawNormal(const std::vector<RNode> &nodes1, const std::vector<RNode> &nodes2, bool volumeElement, bool useTexture)
{
    uint nn = uint(nodes1.size());

    RR3Vector normal(RPolygon(nodes1).getNormal());

    GLObject::glNormalVector(normal);

    if (useTexture)
    {
        GL_SAFE_CALL(glEnable(GL_TEXTURE_1D));
    }

    GLboolean cullState;
    GLint cullMode;

    GL_SAFE_CALL(glGetBooleanv(GL_CULL_FACE,&cullState));
    GL_SAFE_CALL(glGetIntegerv(GL_CULL_FACE_MODE,&cullMode));

    if (this->useGlCullFace)
    {
        GL_SAFE_CALL(glEnable(GL_CULL_FACE));
        GL_SAFE_CALL(glCullFace(GL_BACK));
    }
    else
    {
        GL_SAFE_CALL(glDisable(GL_CULL_FACE));
    }

    GLFunctions::begin(GL_POLYGON);
    for (uint i=0;i<nn;i++)
    {
        if (useTexture)
        {
            GL_SAFE_CALL(glTexCoord1d(this->nodeTextureCoordinates[i]));
        }
        GLObject::glVertexNode(nodes1[i]);
    }
    GLFunctions::end();

    if (volumeElement)
    {
        GLFunctions::begin(GL_QUADS);

        for (uint i=0;i<nn;i++)
        {
            uint n1 = i;
            uint n2 = i+1;
            if (n2 == nn)
            {
                n2 = 0;
            }

            GLObject::glNormalVector(RTriangle(nodes1[n2],nodes1[n1],nodes2[n1]).getNormal());

            if (useTexture)
            {
                GL_SAFE_CALL(glTexCoord1d(this->nodeTextureCoordinates[n2]));
            }
            GLObject::glVertexNode(nodes1[n2]);
            if (useTexture)
            {
                GL_SAFE_CALL(glTexCoord1d(this->nodeTextureCoordinates[n1]));
            }
            GLObject::glVertexNode(nodes1[n1]);
            GLObject::glVertexNode(nodes2[n1]);
            if (useTexture)
            {
                GL_SAFE_CALL(glTexCoord1d(this->nodeTextureCoordinates[n2]));
            }
            GLObject::glVertexNode(nodes2[n2]);
        }

        GLFunctions::end();
    }

    if (this->useGlCullFace)
    {
        GL_SAFE_CALL(glCullFace(GL_FRONT));
    }

    if (useTexture)
    {
        GL_SAFE_CALL(glDisable(GL_TEXTURE_1D));
    }

    if (this->useGlCullFace)
    {
        this->getGLWidget()->qglColor(Qt::white);

        GLObject::glNormalVector(normal);

        GLFunctions::begin(GL_POLYGON);
        for (uint i=0;i<nn;i++)
        {
            GLObject::glVertexNode(nodes2[i]);
        }
        GLFunctions::end();
    }

    GL_SAFE_CALL(glCullFace(GLenum(cullMode)));
    GL_SAFE_CALL(cullState ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE));
}

void GLSimplexPolygon::drawWired(const std::vector<RNode> &nodes1, const std::vector<RNode> &nodes2, bool volumeElement, bool useTexture)
{
    uint nn = uint(nodes1.size());

    RR3Vector normal(RPolygon(nodes1).getNormal());

    GLObject::glNormalVector(normal);

    if (useTexture)
    {
        GL_SAFE_CALL(glEnable(GL_TEXTURE_1D));
    }

    GLFunctions::begin(GL_LINE_LOOP);
    for (uint i=0;i<nn;i++)
    {
        if (useTexture)
        {
            GL_SAFE_CALL(glTexCoord1d(this->nodeTextureCoordinates[i]));
        }
        GLObject::glVertexNode(nodes1[i]);
    }
    GLFunctions::end();

    if (volumeElement)
    {
        GLFunctions::begin(GL_LINE_LOOP);
        for (uint i=0;i<nn;i++)
        {
            if (useTexture)
            {
                GL_SAFE_CALL(glTexCoord1d(this->nodeTextureCoordinates[i]));
            }
            GLObject::glVertexNode(nodes2[i]);
        }
        GLFunctions::end();

        GLFunctions::begin(GL_LINES);
        for (uint i=0;i<nn;i++)
        {
            if (useTexture)
            {
                GL_SAFE_CALL(glTexCoord1d(this->nodeTextureCoordinates[i]));
            }
            GLObject::glVertexNode(nodes1[i]);
            GLObject::glVertexNode(nodes2[i]);
        }
        GLFunctions::end();
    }

    if (useTexture)
    {
        GL_SAFE_CALL(glDisable(GL_TEXTURE_1D));
    }
}

void GLSimplexPolygon::drawNodes(const std::vector<RNode> &nodes1, const std::vector<RNode> &nodes2, bool volumeElement)
{
    uint nn = uint(nodes1.size());

    for (uint i=0;i<nn;i++)
    {
        GLFunctions::begin(GL_POINTS);
        GLObject::glVertexNode(nodes1[i]);
        if (volumeElement)
        {
            GLObject::glVertexNode(nodes2[i]);
        }
        GLFunctions::end();
    }
}
