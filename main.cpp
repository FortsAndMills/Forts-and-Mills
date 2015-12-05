#include "Technical\Application.h"
#include "Interface\MainWindow.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    Application * app = new Application(argc, argv);

    MainWindow * programm = new MainWindow();
    programm->show();

    return app->exec();
}
