#include <KUiServerV2JobTracker>
#include <QGuiApplication>

#include "job.hpp"
#include "worker.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setDesktopFileName("org.kde.dolphin");

    Worker worker{25};

    KUiServerV2JobTracker tracker{&app};
    tracker.registerJob(new KustomJob{&worker});

    worker.start();
    app.exec();
}
