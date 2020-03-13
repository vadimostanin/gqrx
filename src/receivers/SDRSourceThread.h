#ifndef SDRSOURCETHREAD_H
#define SDRSOURCETHREAD_H

#include "receivers/RtlSdrSource.h"
#include <thread>

class SDRSourceThread
{
    RtlSdrSource m_source;
    std::thread m_sourceThread;
    void run();
public:
    SDRSourceThread(int32_t devIndex);
};

#endif // SDRSOURCETHREAD_H
