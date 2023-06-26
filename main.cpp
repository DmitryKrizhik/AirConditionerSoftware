#include "simulatorsoftware.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication SoftwareApplication(argc, argv);
    SimulatorSoftware SoftwareWindow;
    SoftwareWindow.show();
    return SoftwareApplication.exec();
}
