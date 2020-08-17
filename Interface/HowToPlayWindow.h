#ifndef HOWTOPLAYWINDOW_H
#define HOWTOPLAYWINDOW_H

#include "Technical/Headers.h"
#include "Technical/Constants.h"
#include "Technical/ProgrammSettings.h"
#include "BasicElements/GraphicObject.h"
#include "SpecialButton.h"

class LessonLabel : public GraphicObject
{
    Q_OBJECT

    int n;
    Object * text;
public:
    explicit LessonLabel(GraphicObject * parent, int n) :
        GraphicObject(parent, CLICKABLE | HOVER, "LessonFrame", "LessonFrameSelection", "SimpleLayer")
    {
        this->n = n;

        text = new Object(this, "Lesson" + QString::number(n + 1));

        CheckForLessonsPassed();
    }
    void CheckForLessonsPassed()
    {
        if (settings->lessonsPassed[n])
            this->setOpacity(0.7);
    }

    void resizeChildren(qreal W, qreal H)
    {
        qreal width = text->pixmap().width();
        qreal height = text->pixmap().height();
        qreal K = H * 0.8 / height;
        height *= K;
        width *= K;

        text->setGeometry((W - width) / 2, (H - height) / 2, width, height);
    }

    void leftClick() { emit clicked(n); }

signals:
    void clicked(int);
};
class HowToPlayWindow : public GraphicObject
{
    Q_OBJECT

    Object * logo;

public:
    QVector < LessonLabel * > lessons;

    SpecialButton * HomeButton;

    explicit HowToPlayWindow(GraphicObject * parent) : GraphicObject(parent)
    {
        setOpacity(0);
        logo = new Object(this, "Logo");

        for (int i = 0; i < 5; ++i)
            lessons.push_back(new LessonLabel(this, i));

        HomeButton = new SpecialButton(this, "HomeButton");
    }
    void turn(bool on)
    {
        if (on)
        {
            AnimationStart(OPACITY, 1);
            //dialog->showHelp = true;
        }
        else
        {
            AnimationStart(OPACITY, 0);
            //dialog->showHelp = false;
        }
    }


    void CheckForLessonsPassed()
    {
        foreach (LessonLabel * LL, lessons)
        {
            LL->CheckForLessonsPassed();
        }
    }

    void resizeChildren(qreal W, qreal H)
    {
        logo->setGeometry(W / 2 - H / 12, H / 40, H / 6, H / 6);

        HomeButton->setGeometry(constants->cancelButtonX * W,
                                             constants->cancelButtonY * H,
                                             constants->cancelButtonHeight * H,
                                             constants->cancelButtonHeight * H);

        resizeLessons();
    }

    QRectF lessonLabelGeometry(int i)
    {
        return QRectF(width() * constants->lessonLabelX,
                                 height() * (constants->gameLabelsYStart + i * (constants->gameLabelsMarginY + constants->gameLabelsHeight)),
                                 width() * (1 - 2 * constants->lessonLabelX),
                                height() * constants->gameLabelsHeight);
    }
    void resizeLessons()
    {
        for (int i = 0; i < lessons.size(); ++i)
        {
            lessons[i]->setGeometry(lessonLabelGeometry(i));
        }
    }
};

#endif // HOWTOPLAYWINDOW_H
