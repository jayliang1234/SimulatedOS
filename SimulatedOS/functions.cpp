#include "SimulatedOS.h"

SimulatedOS::SimulatedOS( int numberOfDisks, int amountOfRAM, int pageSize ){
    numOfFrame = amountOfRAM/pageSize;
    SizeOfPage = pageSize;
    PID = 1;
    for(int i = 0; i< numberOfDisks;i++){ //create all the disks
        Disk newDisk;
        Disks.push_back(newDisk);
    }
    for (int i =0; i< numOfFrame;i++){ //creates the Frames and Frames for frameQueue
        Frame newFrame;
        Frame queue;
        newFrame.setFrameNum(i);
        queue.setFrameNum(i);
        frameQueue.push_back(queue);
        Frames.push_back(newFrame);
    }
}
void SimulatedOS::NewProcess( int priority ){
    Process newProcess;
    bool emptyframe = false;
    newProcess.setPID(PID);
    newProcess.setPriority(priority);
    if(runningCPU.size()>0){
        if(priority > runningCPU.at(0).returnPriority()){
            readyQueue.push_back(runningCPU.at(0));
            runningCPU.clear();
            runningCPU.push_back(newProcess);
        }
        else{
            readyQueue.push_back(newProcess);
        }
    }
    else{
        runningCPU.push_back(newProcess);
    }
    for(int i = 0; i< numOfFrame; i++){
        if(Frames.at(i).returnPid() == 0){
            Frames.at(i).setPid(PID);
            Frames.at(i).setPage(0);
            frameQueue.push_back(Frames.at(i));
            frameQueue.erase(frameQueue.begin()+indexOfFrame(i));
            emptyframe = true;
            break;
        }
    }
    if(emptyframe == false){
        //if Least used Frame's PID is not equal to CPU process PID
        //replace that frame
        bool notEqual = false;
        for(int i = 0; i < numOfFrame; i++){
            if(frameQueue.at(i).returnPid() != runningCPU.at(0).returnPID()){
                notEqual = true;
                Frames.at(frameQueue.at(i).returnFrameNum()).setPid(PID);
                Frames.at(frameQueue.at(i).returnFrameNum()).setPage(0);
                frameQueue.push_back(Frames.at(frameQueue.at(i).returnFrameNum()));
                frameQueue.erase(frameQueue.begin()+i);
                break;
            }
        }
        //if all Frames have PID of CPU process, replace least used frame
        if(notEqual == false){
            Frames.at(frameQueue.at(0).returnFrameNum()).setPid(PID);
            Frames.at(frameQueue.at(0).returnFrameNum()).setPage(0);
            frameQueue.push_back(Frames.at(frameQueue.at(0).returnFrameNum()));
            frameQueue.erase(frameQueue.begin());
        }
    }
    PID++;
}
void SimulatedOS::Exit(){
    //clear frames with running process
    for(int i = 0; i < 3; i++){
        if(Frames.at(i).returnPid() == runningCPU.at(0).returnPID()){
            Frames.at(i).setPage(0);
            Frames.at(i).setPid(0);
        }
    }
    //removing process from cpu
    runningCPU.pop_back();
    for(int i = 0; i< readyQueue.size(); i++){
        if(readyQueue.at(i).returnPID() == highestPriorityProcess()){
            runningCPU.push_back(readyQueue.at(i));
            readyQueue.erase(readyQueue.begin()+i);
            break;
        }
    }
}
void SimulatedOS::DiskReadRequested( int diskNumber, std::string fileName ){
    if(diskNumber > Disks.size()-1){ // if diskNumber is out of range
        std::cout<< "disk does not exist \n";
    }
    else if(runningCPU.size() == 0){ // if no process is running
        std::cout << " instruction ignored because CPU is IDLE \n";
    }
    else{
        for(int i =0; i< Frames.size();i++){
            if(frameQueue.at(i).returnPid() == highestPriorityProcess()){
                frameQueue.push_back(frameQueue.at(i));
                frameQueue.erase(frameQueue.begin()+i);
                break;
            }
        }
        runningCPU.at(0).setFileName(fileName);
        Disks.at(diskNumber).servingProcess.push_back(runningCPU.at(0)); //makes the running process serve the disk  
        runningCPU.clear();
        if(readyQueue.size() > 0){
            int priority = highestPriorityProcess(); //Highest priority in readyQueue
            for(int i = 0; i< readyQueue.size();i++){
                if(readyQueue.at(i).returnPID() == priority){
                    runningCPU.push_back(readyQueue.at(i));
                    readyQueue.erase(readyQueue.begin()+i);
                    break;
                }
            }
        }
    }
}
void SimulatedOS::FetchFrom( unsigned int memoryAddress ){
    int pageNum = memoryAddress/SizeOfPage;
    bool pageInMem = false; // if a certain page is already in memory
    //check if frame has same page and same PID
    for(int i = 0;i<3;i++){
        if(Frames.at(i).returnPage() == pageNum && Frames.at(i).returnPid() == runningCPU.at(0).returnPID() && Frames.at(i).returnPage() != 0){
            pageInMem = true;
            frameQueue.push_back(Frames.at(i));
            frameQueue.erase(frameQueue.begin()+indexOfFrame(Frames.at(i).returnFrameNum()));
            break;
        }
    }
    if(pageInMem == false){
        if(Frames.at(0).returnPid() == 0){
            Frames.at(0).setPage(pageNum);
            Frames.at(0).setPid(runningCPU.at(0).returnPID());
            frameQueue.push_back(Frames.at(0));
            frameQueue.erase(frameQueue.begin()+indexOfFrame(0));
            runningCPU.at(0).setLastUsedPage(pageNum); //UPDATES the last paged used
        }
        else if(Frames.at(1).returnPid() == 0){
            Frames.at(1).setPage(pageNum);
            Frames.at(1).setPid(runningCPU.at(0).returnPID());
            frameQueue.push_back(Frames.at(1));
            frameQueue.erase(frameQueue.begin()+indexOfFrame(1));
            runningCPU.at(0).setLastUsedPage(pageNum); //UPDATES the last paged used
        }
        else if(Frames.at(2).returnPid() == 0){
            Frames.at(2).setPage(pageNum);
            Frames.at(2).setPid(runningCPU.at(0).returnPID());
            frameQueue.push_back(Frames.at(2));
            frameQueue.erase(frameQueue.begin()+indexOfFrame(2));
            runningCPU.at(0).setLastUsedPage(pageNum); //UPDATES the last paged used
        }
        else{
            //if any frame has a PID not equal to CPU process PID
            if(frameQueue.at(0).returnPid() != runningCPU.at(0).returnPID() || frameQueue.at(1).returnPid() != runningCPU.at(0).returnPID() || frameQueue.at(2).returnPid() != runningCPU.at(0).returnPID()){
                for(int i =0;i<3;i++){
                    if(frameQueue.at(i).returnPid() != runningCPU.at(0).returnPID()){//if the PID of the least used frame doesnt match CPU pid
                        Frames.at(frameQueue.at(i).returnFrameNum()).setPage(pageNum);
                        Frames.at(frameQueue.at(i).returnFrameNum()).setPid(runningCPU.at(0).returnPID());
                        frameQueue.push_back(Frames.at(frameQueue.at(i).returnFrameNum()));
                        frameQueue.erase(frameQueue.begin() + i);
                        runningCPU.at(0).setLastUsedPage(pageNum); //UPDATES the last paged used
                        break;
                    }
                }       
            }     
            else{//if all frames have the same PID as process in CPU then change least used frame
                Frames.at(frameQueue.at(0).returnFrameNum()).setPage(pageNum);
                frameQueue.push_back(Frames.at(frameQueue.at(0).returnFrameNum()));
                frameQueue.erase(frameQueue.begin());
                runningCPU.at(0).setLastUsedPage(pageNum); //UPDATES the last paged used
            }

        }
    }
}

void SimulatedOS::DiskJobCompleted( int diskNumber ){

    if(Disks.at(diskNumber).servingProcess.size() > 0){ // if there is a Process in the disk
        
        if(Disks.at(diskNumber).servingProcess.at(0).returnPriority() > runningCPU.at(0).returnPriority()){ //Process is now finished serving will return to CPU
            readyQueue.push_back(runningCPU.at(0));
            runningCPU.pop_back();
            runningCPU.push_back(Disks.at(diskNumber).servingProcess.at(0));
            Disks.at(diskNumber).servingProcess.pop_back();
        }
        else{ //Process is now finished serving will return to ReadyQueue
            readyQueue.push_back(Disks.at(diskNumber).servingProcess.at(0));
            Disks.at(diskNumber).servingProcess.pop_back();
        }
        if(Disks.at(diskNumber).IOqueue.size() > 0){ //if there are Processes in IOqueue waiting to be served
            Disks.at(diskNumber).servingProcess.push_back(Disks.at(diskNumber).IOqueue.at(0)); //pushes process into servingProcess
            Disks.at(diskNumber).IOqueue.erase(Disks.at(diskNumber).IOqueue.begin()); //moves process out of IOqueue
        }
    }
    else{
        std::cout << "instruction ignored, no read requested on this disk \n";
    }
    if((Frames.at(0).returnPid() == runningCPU.at(0).returnPID() && Frames.at(0).returnPage() == runningCPU.at(0).returnLastUsedPage()) || (Frames.at(1).returnPid() == runningCPU.at(0).returnPID() && Frames.at(1).returnPage() == runningCPU.at(0).returnLastUsedPage()) || (Frames.at(2).returnPid() == runningCPU.at(0).returnPID() && Frames.at(2).returnPage() == runningCPU.at(0).returnLastUsedPage())){
        for(int i =0; i< Frames.size(); i++){
            if(frameQueue.at(i).returnPage() == runningCPU.at(0).returnLastUsedPage() && frameQueue.at(i).returnPid() == runningCPU.at(0).returnPID()){
                frameQueue.push_back(frameQueue.at(i));
                frameQueue.erase(frameQueue.begin()+i);
                break;
            }
        }
    }
    else{
        if(Frames.at(0).returnPid() == 0){
            Frames.at(0).setPage(runningCPU.at(0).returnLastUsedPage());
            Frames.at(0).setPid(runningCPU.at(0).returnPID());
            frameQueue.push_back(Frames.at(0));
            frameQueue.erase(frameQueue.begin()+indexOfFrame(0));
        }
        else if(Frames.at(1).returnPid() == 0){
            Frames.at(1).setPage(runningCPU.at(0).returnLastUsedPage());
            Frames.at(1).setPid(runningCPU.at(0).returnPID());
            frameQueue.push_back(Frames.at(1));
            frameQueue.erase(frameQueue.begin()+indexOfFrame(1));
        }
        else if(Frames.at(2).returnPid() == 0){
            Frames.at(2).setPage(runningCPU.at(0).returnLastUsedPage());
            Frames.at(2).setPid(runningCPU.at(0).returnPID());
            frameQueue.push_back(Frames.at(2));
            frameQueue.erase(frameQueue.begin()+indexOfFrame(2));
        }
        else{//if any frame has a PID not equal to CPU process PID
            //Frames.at(frameQueue.at(0).returnFrameNum()).setPage(runningCPU.at(0).returnLastUsedPage());
            //Frames.at(frameQueue.at(0).returnFrameNum()).setPid(runningCPU.at(0).returnPID());
            if(frameQueue.at(0).returnPid() != runningCPU.at(0).returnPID() || frameQueue.at(1).returnPid() != runningCPU.at(0).returnPID() || frameQueue.at(2).returnPid() != runningCPU.at(0).returnPID()){
                for(int i =0;i<3;i++){
                    if(frameQueue.at(i).returnPid() != runningCPU.at(0).returnPID()){//if the PID of the least used frame doesnt match CPU pid
                        Frames.at(frameQueue.at(i).returnFrameNum()).setPage(runningCPU.at(0).returnLastUsedPage());
                        Frames.at(frameQueue.at(i).returnFrameNum()).setPid(runningCPU.at(0).returnPID());
                        frameQueue.push_back(Frames.at(frameQueue.at(i).returnFrameNum()));
                        frameQueue.erase(frameQueue.begin() + i);
                        break;
                    }
                }       
            }
            else{//if all frames have the same PID as process in CPU then change least used frame
                Frames.at(frameQueue.at(0).returnFrameNum()).setPage(runningCPU.at(0).returnLastUsedPage());
                frameQueue.push_back(Frames.at(frameQueue.at(0).returnFrameNum()));
                frameQueue.erase(frameQueue.begin());
            }
        }
    }
}

void SimulatedOS::PrintCPU(){
    if(runningCPU.size() == 0){
        std::cout << "There are no processes currently using cpu \n";
    }
    else{
        std::cout<< "CPU: "<< runningCPU.at(0).returnPID() << "\n";
    }
}

void SimulatedOS::PrintReadyQueue(){
    if(readyQueue.size() > 0){
        std::cout<<"Ready-Queue: ";
        for(int i = 0; i < readyQueue.size(); i++ ){
            std::cout<< readyQueue.at(i).returnPID() << " ";
        }
        std::cout<<"\n";
    }
    else{

    }
}

void SimulatedOS::PrintRAM( ){
    std::cout << "Frame    Page    PID \n";
    for(int i =0; i<3;i++){
        std::cout<< std::left << std::setw(9) << Frames.at(i).returnFrameNum();
        if(Frames.at(i).returnPid()!=0){
            std::cout<< std::left << std::setw(8) << Frames.at(i).returnPage() << Frames.at(i).returnPid() << "\n";
        }
        else{
            std::cout<<"\n";
        }

    }
}

void SimulatedOS::PrintDisk( int diskNumber ){
    if(diskNumber > Disks.size()-1){
        std::cout<< "instruction ignored because disk does not exist \n";
    }
    else{
        if(Disks.at(diskNumber).servingProcess.size() > 0){
            std::cout<< "Disk " << diskNumber << ": PID " << Disks.at(diskNumber).returnPIDofProcessServed() << ", " << Disks.at(diskNumber).servingProcess.at(0).returnFileName() <<"\n";
        }
        else{
            std::cout<< "Disk " << diskNumber << ": Idle \n";
        }
    }   
}

void SimulatedOS::PrintDiskQueue( int diskNumber ){
    if(diskNumber > Disks.size()-1){
        std::cout << "Instruction ignored: no disk with such number exists \n";
    }
    else{
        std::cout << "Disk " << diskNumber << " I/0-queue: ";
        if(Disks.at(diskNumber).IOqueue.size() == 0){
            std::cout<< "Empty\n";
        }
        else{
            for(int i =0; i< Disks.at(diskNumber).IOqueue.size();i++){
                std:: cout << Disks.at(diskNumber).IOqueue.at(i).returnPID() << " ";
            }
            std::cout << "\n";
        }
        
    }

}
int SimulatedOS::highestPriorityProcess(){ //returns Process PID with the highest priority of the readyQueue
    int high = 0;
    int currentPID = 0;

    for(int i = 0; i< readyQueue.size();i++){
        if(readyQueue.at(i).returnPriority() > high){
            high = readyQueue.at(i).returnPriority();
            currentPID = readyQueue.at(i).returnPID();
        }
    }

    return currentPID;
}
int SimulatedOS::indexOfFrame(int num){//returns index of given frameNumber
    for(int i = 0; i< frameQueue.size();i++){
        if(frameQueue.at(i).returnFrameNum()==num){
            return i;
        }
    }
    return 0;
}
void SimulatedOS::temp(){
    std::cout<< "frame Queue: ";
    for (int i =0; i <frameQueue.size(); i++){
        std::cout<< frameQueue.at(i).returnFrameNum() << " ";
    }
    std::cout <<"\n";
}