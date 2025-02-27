#ifndef TASK_H
#define TASK_H

#include "NetworkHandler.h"

class Task {
protected:
    NetworkHandler &network;

public:
    explicit Task(NetworkHandler &net);
    virtual ~Task() = default;  
    virtual void execute() = 0;
};

class SumTask : public Task {
public:
    using Task::Task;
    void execute() override;
};
#endif