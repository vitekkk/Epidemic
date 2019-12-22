#ifndef TIMER_H
#define TIMER_H
#include <time.h>

class Timer
{
private:
    bool    isRunning_;
    clock_t startTime_;
public:
    Timer():
      isRunning_(false)
          , startTime_(0)
      {}

      void start()
      {
          isRunning_ = true;
          startTime_ =  clock();
      }

      float stop()
      {
          if(isRunning_)
          {
              isRunning_ = false;
              return (float)((clock() - startTime_)/(float)CLOCKS_PER_SEC);
          }
          return 0.f;
      }
};

#endif // TIMER_H
