/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   gl_object.cpp                                            *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   23-rd February 2012                                      *
 *                                                                   *
 *  DESCRIPTION: OpenGL object class definition                      *
 *********************************************************************/

#include "gl_object.h"

GLObject::GLObject(GLWidget *glWidget) :
    pParentModel(nullptr),
    glWidget(glWidget),
    applyEnvSettings(true),
    useGlList(true),
    useGlCullFace(true)
{
    this->_init();
}

GLObject::GLObject(const GLObject &glObject)
{
    this->_init(&glObject);
}

GLObject::~GLObject()
{
}

GLObject & GLObject::operator =(const GLObject &glObject)
{
    this->_init(&glObject);
    return (*this);
}

void GLObject::_init(const GLObject *pGlObject)
{
    if (pGlObject)
    {
        this->pParentModel = pGlObject->pParentModel;
        this->glWidget = pGlObject->getGLWidget();
        this->applyEnvSettings = pGlObject->applyEnvSettings;
        this->useGlList = pGlObject->useGlList;
        this->useGlCullFace = pGlObject->useGlCullFace;
    }
}

GLWidget *GLObject::getGLWidget(void) const
{
    return this->glWidget;
}

void GLObject::setParentModel(const Model *pParentModel)
{
    this->pParentModel = pParentModel;
}

bool GLObject::getApplyEnvironmentSettings() const
{
    return this->applyEnvSettings;
}

void GLObject::setApplyEnvironmentSettings(bool applyEnvSettings)
{
    this->applyEnvSettings = applyEnvSettings;
}

bool GLObject::getUseGlList(void) const
{
    return this->useGlList;
}

void GLObject::setUseGlList(bool useGlList)
{
    this->useGlList = useGlList;
}

bool GLObject::getUseGlCullFace(void) const
{
    return this->useGlCullFace;
}

void GLObject::setUseGlCullFace(bool useGlCullFace)
{
    this->useGlCullFace = useGlCullFace;
}

void GLObject::paint(PaintActionMask paintActionMask)
{
    if (paintActionMask & GLObject::Initialize)
    {
        this->initialize();
    }
    if (paintActionMask & GLObject::Draw)
    {
        this->draw();
    }
    if (paintActionMask & GLObject::Finalize)
    {
        this->finalize();
    }
}

void GLObject::glNormalVector(const RR3Vector &vector)
{
    GL_SAFE_CALL(glNormal3d(vector[0],vector[1],vector[2]));
}

void GLObject::glVertexVector(const RR3Vector &vector)
{
    GL_SAFE_CALL(glVertex3d(vector[0],vector[1],vector[2]));
}

void GLObject::glVertexNode(const RNode &node)
{
    GL_SAFE_CALL(glVertex3d(node.getX(),node.getY(),node.getZ()));
}

void GLObject::initialize(void)
{
}

void GLObject::finalize(void)
{
}

void GLObject::draw(void)
{
}
