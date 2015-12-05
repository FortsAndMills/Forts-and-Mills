#ifndef UNITPANEL_H
#define UNITPANEL_H

#include "Technical/Headers.h"
#include "BasicElements/StateObject.h"
#include "Order.h"

// TODO unitPanel и StateObject
class UnitPanel : public StateObject
{
    Q_OBJECT
    Game * game;

    QList <SpecialButton *> variants;
    friend Unit;  //  FAIL

public:
    explicit UnitPanel(GraphicObject *parent, Game * game) :
        StateObject(parent, "default", 0, "UnitPanel", "", "", constants->unitPanelAppearTime)
    {
        this->game = game;
        this->setOpacity(0);
        this->setZValue(constants->unitPanelZPos);

        geometries["hidden"] = QRectF(constants->unitPanelShiftX,
                                                            constants->unitPanelShiftY, 1, 1);
        setState("hidden", true);
    }

public:
    void reconfigure()
    {
        qreal X = constants->orderVariantTableSideMargin;
        qreal max_x = X;
        qreal Y = constants->orderVariantTableSideMargin;

        for (int i = 0; i < variants.size(); ++i)
        {
            variants[i]->setGeometry(X, Y,
                                     constants->orderVariantSize,
                                     constants->orderVariantSize);

            X += constants->orderVariantSize + constants->orderVariantTableMargin;
            if (X > max_x) max_x = X;

            if ((i + 1) % constants->orderVariantTableMaxColumns == 0)
            {
                X = constants->orderVariantTableSideMargin;
                Y += constants->orderVariantSize + constants->orderVariantTableMargin;
            }
        }

        if (variants.size() % constants->orderVariantTableMaxColumns != 0)
        {
            Y += constants->orderVariantSize + constants->orderVariantTableMargin;
        }

        max_x += constants->orderVariantTableSideMargin;
        Y += constants->orderVariantTableSideMargin;

        this->resize(max_x, Y);
    }
    QPointF variantPos(QString name)
    {
        int i = 0;
        while (variants[i]->name != name) { ++i; }
        return variants[i]->pos();
    }

    void setVariants(QList <QString> variants_names)
    {
        foreach (SpecialButton * var, variants)
            var->Delete();
        variants.clear();

        foreach (QString var, variants_names)
        {
            variants << new SpecialButton(this->cur_object(), var);
        }

        reconfigure();
    }

    void appear()
    {
        this->setEnabled(true);
        this->AnimationStart(OPACITY, 1, constants->unitPanelAppearTime);
        this->setState("default");
    }
    void disappear()
    {
        this->setEnabled(false);
        this->AnimationStart(OPACITY, 0, constants->unitPanelAppearTime);
        this->setState("hidden");
    }
};

#endif // UNITPANEL_H
