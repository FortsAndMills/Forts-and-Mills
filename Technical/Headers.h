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

#define UNDEFINED -79  // <undone>
#define INF 0x7FFFFFFF

class Settings;
class Constants;
class Pictures;
class AnimationManager;
class Client;

#define HexType QString
#define UnitType QString
#define OrderType QString
#define PlayerColor QString
#define Resource QString
#define DayTime int
#define WAY QString
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
class GameWindow;
class Unit;
class UnitPanel;

extern Settings * settings;
extern Constants * constants;
extern Pictures * pictures;
extern AnimationManager * animations;
extern Client * client;

#include "Random.h"

#endif
