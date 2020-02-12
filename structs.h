#ifndef STRUCTS_H
#define STRUCTS_H

typedef unsigned int ui;

struct process_struct {
    ui priority; // set in read process
    ui cpu;  // set in read process
    ui io; // set in read process

    ui curCpu; // init when loaded into cpu; increment when checked
    ui curIo;  // count of time waiting for I/O
    ui wait; // current count of time in ready queue
    ui curPrior; // adjusted for starvation
    ui cpuTotal; // sum of time in cpu
    ui ioTotal; // sum of time doing io
    ui runtime;

    // statistics
    ui waitSum; // total time in ready queue
    ui waitCount; // how many times in ready queue (for average)
    ui waitMin; // smallest time in ready queue
    ui waitMax; // longest time in ready queue
    ui isRunning; // 0 when no longer running
};
typedef struct process_struct process;

struct os_struct { ui quantum; ui wait; };

typedef struct os_struct os;
#endif