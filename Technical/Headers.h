#ifndef HEADERS
#define HEADERS

#include <QtGui>
#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QApplication>
#include <Qt>
#include <QString>
#include <QPainter>
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include <QSize>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QBitmap>
#include <QLayout>
#include <QHBoxLayout>
#include <QRgb>
#include <QSpinBox>
#include <QLineEdit>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QGraphicsDropShadowEffect>
#include <QStyle>
#include <QTextStream>
#include <QPainterPath>
#include <QGraphicsSceneResizeEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QPushButton>
#include <QtGlobal>
#include <QQueue>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <ctime>
#include <cstdlib>

#define UNDEFINED -79
#define INF 0x7FFFFFFF

#define LESSONS_AMOUNT 5

class Settings;
class Constants;
class Images;
class AnimationManager;
class HelpManager;
class Client;

#define HexType QString
#define UnitType QString
#define OrderType QString
#define DefaultOrder "Wait"
#define PlayerColor QString
#define Resource QString
#define DayTime int
enum WAY {UP, RIGHT_UP, RIGHT_DOWN, DOWN, LEFT_DOWN, LEFT_UP, NO_WAY};
#define WAYS (QList<WAY>() << UP << RIGHT_UP << RIGHT_DOWN << DOWN << LEFT_DOWN << LEFT_UP)

class GameHex;
class GameOrder;
class GamePlayer;
class GameRules;
class Game;
class GameUnit;

enum ANIMATION_TYPE {X_POS, Y_POS, WIDTH, HEIGHT, OPACITY, ROTATION};
class Object;
class Animation;
class GraphicObject;
class SpriteObject;

class Hex;
class Field;
class ResourcePic;
class GameElementsOrganization;
class Unit;
class UnitPanel;
class Shield;

extern Settings * settings;
extern Constants * constants;
extern Images * images;
extern AnimationManager * animations;
extern HelpManager * help;
extern Client * client;
extern QFile * logFile;
extern QTextStream debug;

#include "Elementary.h"

QTextStream &operator << (QTextStream &stream, const QList<QString> list);
QTextStream &operator >> (QTextStream &stream, QList<QString>& list);


#endif
