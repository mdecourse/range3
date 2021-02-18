/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_ellipse.h                                    *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   26-th August 2016                                        *
 *                                                                   *
 *  DESCRIPTION: Draw engine ellipse class declaration               *
 *********************************************************************/

#include "draw_engine_ellipse.h"

DrawEngineEllipse::DrawEngineEllipse(QObject *parent) :
    DrawEngineObject(parent)
{
    this->name = tr("ellipse");

    this->inputParameters.append(DrawEngineInput(RLocalDirection(RR3Vector(0.0,0.0,0.0),RR3Vector(0.0,0.0,1.0)),
                                                 tr("Center node position and direction"),
                                                 tr("Center node position in X,Y,Z coordinate system and its direction (normal)."),
                                                 "m"));
    this->inputParameters.append(DrawEngineInput(0.25,
                                                 tr("Radius R1"),
                                                 tr("Ellipse\'s 1-st radius."),
                                                 "m",
                                                 RConstants::eps));
    this->inputParameters.append(DrawEngineInput(0.15,
                                                 tr("Radius R2"),
                                                 tr("Ellipse\'s 2-nd radius."),
                                                 "m",
                                                 RConstants::eps));
    this->inputParameters.append(DrawEngineInput(0.0,
                                                 tr("Start angle"),
                                                 tr("Opening angle of the ellipse section."),
                                                 "deg",
                                                 0.0,
                                                 360.0));
    this->inputParameters.append(DrawEngineInput(360.0,
                                                 tr("End angle"),
                                                 tr("Closing angle of the ellipse section."),
                                                 "deg",
                                                 0.0,
                                                 360.0));
    this->inputParameters.append(DrawEngineInput(uint(20),
                                                 tr("Anglular divisions"),
                                                 tr("Number of angular divisions."),
                                                 "#",
                                                 uint(3)));

    this->updateModel();
}

RModelRaw DrawEngineEllipse::generate() const
{
    RModelRaw modelRaw;

    RLocalDirection localDirection = this->inputParameters.at(0).toLocalDirection();
    const RR3Vector &center = localDirection.getPosition();
    const RR3Vector &direction = localDirection.getDirection();

    double r1 = this->inputParameters.at(1).toDouble();
    double r2 = this->inputParameters.at(2).toDouble();
    double sa = this->inputParameters.at(3).toDouble();
    double ea = this->inputParameters.at(4).toDouble();
    double nai = this->inputParameters.at(5).toUint();

    double da = 0.0;
    if (sa > ea)
    {
        da = 360.0 - (sa-ea);
    }
    else if (sa == ea)
    {
        da = 360.0;
    }
    else
    {
        da = ea - sa;
    }
    da /= double(nai);

    RNode centerNode(0.0,0.0,0.0);

    for (uint i=0;i<nai;i++)
    {
        double a1 = sa + i*da;
        double a2 = a1 + da;

        if (a1 > 360.0)
        {
            a1 -= 360.0;
        }

        if (a2 > 360.0)
        {
            a2 -= 360.0;
        }

        a1 = R_DEG_TO_RAD(a1);
        a2 = R_DEG_TO_RAD(a2);

        RNode bNode1(0.0,r1*cos(a1),r2*sin(a1));
        RNode bNode2(0.0,r1*cos(a2),r2*sin(a2));

        modelRaw.addTriangle(centerNode,bNode1,bNode2);
    }

    RRMatrix R;
    direction.findRotationMatrix(R);

    for (uint i=0;i<modelRaw.getNNodes();i++)
    {
        modelRaw.getNode(i).transform(R,center);
    }

    return modelRaw;
}
