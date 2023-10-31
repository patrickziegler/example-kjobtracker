# pragma once

#include <KJob>
#include <QThread>
#include <QVariant>

#include "worker.hpp"

class KustomJob : public KJob {
    Q_OBJECT
public:
    explicit KustomJob(Worker* worker) : m_worker{worker}
    {
        setProgressUnit(Files);
        setTotalAmount(Files, m_worker->size());
        setCapabilities(Killable | Suspendable);
        setProperty("destUrl", "file://~/");
        setProperty("immediateProgressReporting", true);
        QObject::connect(m_worker, &Worker::started, this, &KustomJob::start);
        QObject::connect(m_worker, &Worker::finished, this, &KustomJob::quit);
        QObject::connect(m_worker, &Worker::updateMessage, this, &KustomJob::updateMessage);
        QObject::connect(m_worker, &Worker::updateProgress, this, &KustomJob::updateProgress);
    }
    ~KustomJob()
    {
    }
public slots:
    void start() override
    {
        infoMessage(this, "Initializing ... ");
    }
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
        emitResult();
    }
protected:
    bool doKill() override
    {
        return m_worker->kill();
    }
    bool doSuspend() override
    {
        return m_worker->suspend();
    }
    bool doResume() override
    {
        return m_worker->resume();
    }
private:
    Worker* m_worker;
};
