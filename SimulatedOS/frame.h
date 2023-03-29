#ifndef FRAME_H
#define FRAME_H
#include "SimulatedOS.h"
class Frame{
    public:
        std::vector<Process> process;
        void setFrameNum(int num){
            framenumber = num;
        }
        void setPid(int num){
            pid = num;
        }
        void setPage(int num){
            page = num;
        }
        int returnPid(){ 
            return pid;
        }
        int returnPage(){ 
            return page;
        }
        int returnFrameNum(){ 
            return framenumber;
        }
    private:
        int pid=0;
        int framenumber;
        int page;
};
#endif