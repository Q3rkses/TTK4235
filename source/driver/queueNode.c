#include "queueNode.h"

QueueNode QueueNode_Init(void){
    QueueNode temporaryQueue;
    
}

void Attach_QueueNode(int floor){
    if (mQueueNodeCounter > 9) {
        printf("Cannot attach QueueNode because the Queue has %d elements", mQueueNodeCounter);
    }
    int attachmentIndex = Where_To_Attach_QueueNode();

    mQueueNodeCounter++;
}