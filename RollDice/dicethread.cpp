#include "dicethread.h"

DiceThread::DiceThread()
{
    m_isPaused = true;
    m_isStopped = false;
}

//开始掷骰子
void DiceThread::diceBegin()
{
    m_isPaused = false;
}

//暂停掷骰子
void DiceThread::dicePause()
{
    m_isPaused = true;
}

//停止线程
void DiceThread::stopThread()
{
    m_isStopped = true;
}

//读取数据
bool DiceThread::readPointValue(quint8 *index, quint8 *point)
{
    if(m_mutex.tryLock())
    {
        *index = m_index;
        *point = m_dicePoint;
        m_mutex.unlock();
        return true;
    }
    else
    {
        return false;
    }
}

void DiceThread::run()
{
    m_isStopped = false;
    m_index = 0;
    while(!m_isStopped)
    {
        if(!m_isPaused)
        {
            m_mutex.lock();
            m_dicePoint = QRandomGenerator::global()->bounded(1, 7);
            m_index++;
            m_mutex.unlock();
        }
        msleep(500);
    }
}
