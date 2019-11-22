/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   draw_engine_circle.h                                     *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   12-th February 2015                                      *
 *                                                                   *
 *  DESCRIPTION: Draw engine circle class declaration                *
 *********************************************************************/

#ifndef DRAW_ENGINE_CIRCLE_H
#define DRAW_ENGINE_CIRCLE_H

#include "draw_engine_object.h"

class DrawEngineCircle : public DrawEngineObject
{

    Q_OBJECT

    public:

        //! Constructor.
        explicit DrawEngineCircle(QObject *parent = nullptr);

    protected:

        //! Generate RAW model from input parameters.
        virtual RModelRaw generate() const;

};

#endif // DRAW_ENGINE_CIRCLE_H
