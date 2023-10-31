#include <KUiServerV2JobTracker>
#include <QGuiApplication>

#include "job.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setDesktopFileName("org.kde.dolphin");

    auto job = new KustomJob{25};

    KUiServerV2JobTracker tracker{&app};
    tracker.registerJob(job);

    job->start();
    app.exec();
}
