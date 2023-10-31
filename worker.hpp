# pragma once

#include <QObject>
#include <spdlog/spdlog.h>

class Worker : public QObject {
    Q_OBJECT
public:
    explicit Worker(int size) : m_size(size) {}
    ~Worker() {}
    int size() const
    {
        return m_size;
    }
public slots:
    void process() {
        std::this_thread::sleep_for(
                    std::chrono::milliseconds(1000));
        while(m_progress < m_size && !m_killing) {
            while (m_suspended) {
                std::this_thread::sleep_for(
                            std::chrono::milliseconds(200));
            }
            ++m_progress;
            emit updateMessage(m_progress);
            std::this_thread::sleep_for(
                        std::chrono::milliseconds(200));
            emit updateProgress(m_progress);
        }
        spdlog::info("Done");
        emit finished();
        m_killing = false;
    }
    void suspend()
    {
        m_suspended = true;
    }
    void resume()
    {
        m_suspended = false;
    }
    void kill()
    {
        m_killing = true;
        while (m_killing) {
            std::this_thread::sleep_for(
                        std::chrono::milliseconds(200));
        }
        spdlog::info("Killed");
    }
signals:
    void finished();
    void updateMessage(int progress);
    void updateProgress(int progress);
private:
    std::atomic<bool> m_suspended = false;
    std::atomic<bool> m_killing = false;
    int m_progress = 0;
    int m_size = 0;
};
