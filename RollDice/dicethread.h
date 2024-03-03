
#ifndef DICETHREAD_H
#define DICETHREAD_H

#include <QThread>
#include <QRandomGenerator>
#include <QMutex>
class DiceThread : public QThread
{
    Q_OBJECT
public:
    DiceThread();
    void diceBegin();
    void dicePause();
    void stopThread();
    bool readPointValue(quint8* index, quint8* point);

protected:
    void run() override;

private:
    QMutex  m_mutex;
    quint8  m_index;
    quint8  m_dicePoint;
    bool    m_isPaused;
    bool    m_isStopped;

// signals:
//     void generateNewPoint(quint8 index, quint8 point);
};

#endif // DICETHREAD_H
