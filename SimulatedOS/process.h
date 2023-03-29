#ifndef PROCESS_H
#define PROCESS_H

class Process{
    public:
        void setPID(int pid){
            PID = pid;
        }
        void setPriority(int priorityNum){
            priority = priorityNum;
        }
        void setusingCPU(bool TorF){
            if(TorF==true){
                usingCPU = true;
            }
            else{
                usingCPU = false;
            }
        }
        int returnPID(){
            return PID;
        }
        int returnPriority(){
            return priority;
        }
        std::string returnFileName(){
            return fileName;
        }
        bool isUsingCpu(){
            if(usingCPU == true){
                return true;
            }
            else{
                return false;
            }
        }
        void clearProcess(){
            PID = 0;
            priority = 0;
        }
        void setFileName(std::string Name){
            fileName = Name;
        }
        void setLastUsedPage(int num){
            lastUsedPage = num;
        }
        int returnLastUsedPage(){
            return lastUsedPage;
        }
     private:
        int PID;
        int priority;
        bool usingCPU;
        int lastUsedPage = 0;
        std::string fileName;

    };


#endif