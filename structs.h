#ifndef STRUCTS_H
#define STRUCTS_H

typedef unsigned int ui;

struct process_struct {
    ui priority; // set in read process
    ui cpu;  // set in read process
    ui io; // set in read process

    ui curCpu; // init in loadCpu(); increment in checkCpu()
    ui curIo;  // count of time waiting for I/O
    ui wait; // init in loadCpu(); increment in waitTick()
    ui curPrior; // init in loadCpu(); increment in waitTick()
    ui cpuTotal; // init when read in; increment in cpuCheck()
    ui ioTotal; // init when read in; increment in ioTick()
    ui runtime; // init when read in; cpuTotal + ioTotal > runtime checked in checkCpu()

    // statistics
    ui waitSum; // init when read in; increment in waitTick()
    ui waitCount; // init when read in; increment in enqueue()
    ui waitMin; // init when read in: evaluate => change in 
    ui waitMax; // longest time in ready queue
    ui isRunning; // 0 when no longer running
};
typedef struct process_struct process;

struct os_struct { ui quantum; ui wait; };

typedef struct os_struct os;
#endif