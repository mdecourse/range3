/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   bc_manager_tree.h                                        *
 *  GROUP:  Range                                                    *
 *  TYPE:   header file (*.h)                                        *
 *  DATE:   4-th February 2012                                       *
 *                                                                   *
 *  DESCRIPTION: Boundary condition manager tree class declaration   *
 *********************************************************************/

#ifndef BC_MANAGER_TREE_H
#define BC_MANAGER_TREE_H

#include <QTreeWidget>

#include <rmlib.h>

#include "session_entity_id.h"

class BCManagerTree : public QTreeWidget
{
    Q_OBJECT

    private:

        //! List of selected types.
        //! Used only for reselecting already selected items
        //! when model selection has changed.
        QList<RBoundaryConditionType> selectedTypes;
        //! If set to true populat function will return immediately.
        bool doNotPopulate;

    public:

        //! Constructor.
        explicit BCManagerTree(QWidget *parent = nullptr);

    protected:

        //! Populate tree.
        void populate(void);

        //! Return map of applied boundary conditions to number of occurences based on model ID list.
        QMap<RBoundaryConditionType,uint> getBcMap(const QList<SessionEntityID> &entityIDs) const;

        //! Resolve conflicting boundary condition selections.
        bool resolveConflicts(RBoundaryConditionType bcType);

    signals:

        //! Boundary condition was selected.
        void bcSelected(RBoundaryConditionType type, bool applied);

    protected slots:

        //! Called on entity selection changed.
        void onModelSelectionChanged(uint);

        //! Called on problem mask changed.
        void onProblemChanged(uint);

        //! Called when data has been changed.
        void onItemChanged(QTreeWidgetItem *item, int column);

        //! Called when item selection has changed.
        void onItemSelectionChanged(void);

        //! Called when bcSelected signal is emitted.
        void onBcSelected(void);

};

#endif /* BC_MANAGER_TREE_H */
