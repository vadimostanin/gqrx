#include "SDRSourceThread.h"

SDRSourceThread::SDRSourceThread(int32_t devIndex):
    m_source(devIndex),
    m_sourceThread(&SDRSourceThread::run, this)
{
}

void SDRSourceThread::run()
{
    ;
}
