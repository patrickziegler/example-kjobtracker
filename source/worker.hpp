# pragma once

#include <QThread>
#include <spdlog/spdlog.h>
#include <thread>

class Worker : public QObject {
    Q_OBJECT

public:
    explicit Worker(int items) : m_items(items)
    {
        moveToThread(&m_thread);
        QObject::connect(this, &Worker::started, this, &Worker::work);
        m_thread.start();
    }
    ~Worker() {
        m_thread.quit();
        m_thread.wait();
    }

Q_SIGNALS:
    void started();
    void finished();
    void updateMessage(int progress);
    void updateProgress(int progress);

private Q_SLOTS:
    void work() {
        std::this_thread::sleep_for(
                    std::chrono::milliseconds(1000));
        while(m_progress < m_items && !m_killed) {
            while (m_suspended) {
                std::this_thread::sleep_for(
                            std::chrono::milliseconds(200));
            }
            ++m_progress;
            emit updateMessage(m_progress);
            spdlog::info("working on file {}", m_progress);
            std::this_thread::sleep_for(
                        std::chrono::milliseconds(200));
            emit updateProgress(m_progress);
        }
        m_thread.quit();
        Q_EMIT finished();
    }

public:
    void start()
    {
        Q_EMIT started();
    }
    bool suspend()
    {
        m_suspended.store(true);
        return true;
    }
    bool resume()
    {
        m_suspended.store(false);
        return true;
    }
    bool kill()
    {
        m_killed.store(true);
        return m_thread.wait();
    }
    int size() const
    {
        return m_items;
    }

private:
    QThread m_thread;
    std::atomic<bool> m_suspended = false;
    std::atomic<bool> m_killed = false;
    int m_progress = 0;
    int m_items = 0;
};
