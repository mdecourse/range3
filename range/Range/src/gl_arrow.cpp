/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_arrow.cpp                                             *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   30-th May 2013                                           *
 *                                                                   *
 *  DESCRIPTION: OpenGL arrow class definition                       *
 *********************************************************************/

#include "gl_arrow.h"
#include "gl_line.h"

void GLArrow::_init(const GLArrow *pGlArrow)
{
    if (pGlArrow)
    {
        this->position = pGlArrow->position;
        this->direction = pGlArrow->direction;
        this->showHead = pGlArrow->showHead;
        this->drawFrom = pGlArrow->drawFrom;
        this->scale = pGlArrow->scale;
    }
}

GLArrow::GLArrow(GLWidget *glWidget, const RR3Vector &position, const RR3Vector &direction, bool showHead, bool drawFrom, double scale)
    : GLObject(glWidget)
    , position(position)
    , direction(direction)
    , showHead(showHead)
    , drawFrom(drawFrom)
    , scale(scale)
{
    this->_init();
}

GLArrow::GLArrow(const GLArrow &glArrow)
    : GLObject(glArrow)
{
    this->_init(&glArrow);
}

GLArrow &GLArrow::operator =(const GLArrow &glArrow)
{
    this->GLObject::operator =(glArrow);
    this->_init(&glArrow);
    return (*this);
}

void GLArrow::initialize(void)
{
    // Save current settings
    GL_SAFE_CALL(glGetBooleanv(GL_NORMALIZE, &this->normalizeEnabled));
    GL_SAFE_CALL(glGetFloatv(GL_LINE_WIDTH, &this->lineWidth));
    GL_SAFE_CALL(glGetBooleanv(GL_CULL_FACE, &this->cullFaceEnabled));
    // Initialize environment
    GL_SAFE_CALL(glEnable(GL_NORMALIZE));
    GL_SAFE_CALL(glDisable(GL_CULL_FACE));
    GL_SAFE_CALL(glLineWidth(1.0f));
}

void GLArrow::finalize(void)
{
    this->normalizeEnabled ? glEnable(GL_NORMALIZE) :glDisable(GL_NORMALIZE);
    this->cullFaceEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    GL_SAFE_CALL(glLineWidth(this->lineWidth));
}

void GLArrow::draw(void)
{
    this->showHead ? this->drawHead() : this->drawShaft();
}

void GLArrow::drawShaft(void) const
{
    double dirScalar = this->drawFrom ? 1.0 : -1.0;

    GLLine line(this->getGLWidget(),
                this->position,
                RR3Vector(this->position[0] + this->direction[0] * dirScalar,
                          this->position[1] + this->direction[1] * dirScalar,
                          this->position[2] + this->direction[2] * dirScalar),
                1.0);
    line.paint();
}

void GLArrow::drawHead(void)
{
    RR3Vector v1;
    RR3Vector v2;

    if (this->drawFrom)
    {
        v1 = this->position;
        v2[0] = this->position[0] + this->direction[0];
        v2[1] = this->position[1] + this->direction[1];
        v2[2] = this->position[2] + this->direction[2];
    }
    else
    {
        v1[0] = this->position[0] - this->direction[0];
        v1[1] = this->position[1] - this->direction[1];
        v1[2] = this->position[2] - this->direction[2];
        v2 = this->position;
    }

    RModelRaw raw = RShapeGenerator::generateArrow(v1,v2,this->scale);
    for (uint i=0;i<raw.getNElements();i++)
    {
        const RElement &rElement = raw.getElement(i);
        if (RElementGroup::getGroupType(rElement.getType()) == R_ENTITY_GROUP_LINE)
        {
            GLboolean lightingEnabled;
            GL_SAFE_CALL(glGetBooleanv(GL_LIGHTING, &lightingEnabled));
            GL_SAFE_CALL(glDisable(GL_LIGHTING));

            GLFunctions::begin(GL_LINES);
            GL_SAFE_CALL(glVertex3d(raw.getNode(rElement.getNodeId(0)).getX(),
                                    raw.getNode(rElement.getNodeId(0)).getY(),
                                    raw.getNode(rElement.getNodeId(0)).getZ()));
            GL_SAFE_CALL(glVertex3d(raw.getNode(rElement.getNodeId(1)).getX(),
                                    raw.getNode(rElement.getNodeId(1)).getY(),
                                    raw.getNode(rElement.getNodeId(1)).getZ()));
            GLFunctions::end();

            if (lightingEnabled)
            {
                GL_SAFE_CALL(glEnable(GL_LIGHTING));
            }
        }
        if (RElementGroup::getGroupType(rElement.getType()) == R_ENTITY_GROUP_SURFACE)
        {
            RTriangle triangle(raw.getNode(rElement.getNodeId(0)),
                               raw.getNode(rElement.getNodeId(1)),
                               raw.getNode(rElement.getNodeId(2)));
            GL_SAFE_CALL(glNormal3d(triangle.getNormal()[0],triangle.getNormal()[1],triangle.getNormal()[2]));
            GLFunctions::begin(GL_TRIANGLES);
            GL_SAFE_CALL(glVertex3d(raw.getNode(rElement.getNodeId(0)).getX(),
                                    raw.getNode(rElement.getNodeId(0)).getY(),
                                    raw.getNode(rElement.getNodeId(0)).getZ()));
            GL_SAFE_CALL(glVertex3d(raw.getNode(rElement.getNodeId(1)).getX(),
                                    raw.getNode(rElement.getNodeId(1)).getY(),
                                    raw.getNode(rElement.getNodeId(1)).getZ()));
            GL_SAFE_CALL(glVertex3d(raw.getNode(rElement.getNodeId(2)).getX(),
                                    raw.getNode(rElement.getNodeId(2)).getY(),
                                    raw.getNode(rElement.getNodeId(2)).getZ()));
            GLFunctions::end();
        }
    }
}
