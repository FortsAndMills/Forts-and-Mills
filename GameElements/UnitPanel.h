#ifndef UNITPANEL_H
#define UNITPANEL_H

#include "Technical/Headers.h"
#include "BasicElements/StateObject.h"
#include "Order.h"

class UnitPanel : public StateObject
{
    Q_OBJECT
    Game * game;

    QList <OrderVariant *> variants;
    friend Unit;  //  FAIL доступ к variants ради соединений

public:
    explicit UnitPanel(GraphicObject *parent, Game * game) :
        StateObject(parent, "default", "UnitPanel", 0, "", "", false, constants->unitPanelAppearTime)
    {
        this->game = game;
        this->setOpacity(0);
        this->setZValue(constants->unitPanelZPos);

        addGeometry("hidden", QRectF(constants->unitPanelShiftX,
                                                             constants->unitPanelShiftY, 1, 1));
        setGeometryState("hidden", true);
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
        while (variants[i]->pos_order.variant != name) { ++i; }
        return variants[i]->pos();
    }

    void setVariants(const QList<UnitType>& all_variants)
    {
        QList<Game::PossibleChoice> new_variants;
        foreach (UnitType var, all_variants)
            new_variants << Game::PossibleChoice(var, Game::POSSIBLE);
        setVariants(new_variants);
    }
    void setVariants(const QList<Game::PossibleChoice>& new_variants)
    {
        foreach (OrderVariant * var, variants)
            var->Delete();
        variants.clear();

        foreach (Game::PossibleChoice pos_order, new_variants)
        {
            variants << new OrderVariant(this->cur_object(), pos_order);
        }

        reconfigure();
    }

    void appear()
    {
        this->setEnabled(true);
        this->AnimationStart(OPACITY, 1, constants->unitPanelAppearTime);
        this->setGeometryState("default");
    }
    void disappear()
    {
        this->setEnabled(false);
        this->AnimationStart(OPACITY, 0, constants->unitPanelAppearTime);
        this->setGeometryState("hidden");
    }
};

#endif // UNITPANEL_H
