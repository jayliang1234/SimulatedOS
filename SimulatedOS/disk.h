#ifndef DISK_H
#define DISK_H

class Disk{
    public:
        std::vector<Process> IOqueue;
        std::vector<Process> servingProcess;
        int returnPIDofProcessServed(){
            return servingProcess.at(0).returnPID();
        }
        int returnIOqueuePID (){ //returns the process PID of the highest Priority in the IOqueue
            int high = 0;
            int currentPID;

            for(int i =0; i< IOqueue.size(); i++){
                if(IOqueue.at(i).returnPriority() > high){
                    high = IOqueue.at(i).returnPriority();
                    currentPID = IOqueue.at(i).returnPID();
                }
            }
            return currentPID;
        }
    private:
        int PID;

    };

#endif