# pragma once

#include <KJob>
#include <QThread>
#include <QVariant>
#include <spdlog/spdlog.h>

#include "worker.hpp"

class KustomJob : public KJob {
    Q_OBJECT
public:
    explicit KustomJob(int size) : m_worker{size}
    {
        setProgressUnit(Files);
        setTotalAmount(Files, m_worker.size());
        setCapabilities(Killable | Suspendable);
        setProperty("destUrl", "file://~/");
        setProperty("immediateProgressReporting", true);

        m_worker.moveToThread(&m_thread);
        QObject::connect(&m_thread, &QThread::started, &m_worker, &Worker::process);
        QObject::connect(&m_worker, &Worker::finished, &m_thread, &QThread::quit);
        QObject::connect(&m_thread, &QThread::finished, this, &KustomJob::quit);
        QObject::connect(&m_worker, &Worker::updateMessage, this, &KustomJob::updateMessage);
        QObject::connect(&m_worker, &Worker::updateProgress, this, &KustomJob::updateProgress);
    }
    ~KustomJob()
    {
    }
    void start() override
    {
        infoMessage(this, "Initializing ... ");
        m_thread.start();
    }
public slots:
    void updateMessage(int progress)
    {
        infoMessage(this, "Processing ... ");
        description(this, "cmd",
                    qMakePair<QString, QString>(
                        "Source", QString("src-%1").arg(progress)),
                    qMakePair<QString, QString>(
                        "Dest", QString("dst-%1").arg(progress)));
    }
    void updateProgress(int progress)
    {
        setProcessedAmount(Files, progress);
    }
    void quit()
    {
        spdlog::info("Quitting");
        emitResult();
    }
protected:
    bool doKill() override
    {
        m_worker.kill();
        return true;
    }
    bool doSuspend() override
    {
        m_worker.suspend();
        return true;
    }
    bool doResume() override
    {
        m_worker.resume();
        return true;
    }
private:
    QThread m_thread;
    Worker m_worker;
};
