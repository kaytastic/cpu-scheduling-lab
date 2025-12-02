#include "oslabs.h"

// Priority-based Preemptive Scheduling (PP)

struct PCB handle_process_arrival_pp(struct PCB *ready_queue, int *queue_cnt,
                                     struct PCB current_process,
                                     struct PCB new_process, int time_stamp)
{
    if (current_process.process_id == 0) {
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        return new_process;
    }

    if (new_process.process_priority < current_process.process_priority) {
        current_process.remaining_bursttime -= time_stamp - current_process.execution_starttime;
        ready_queue[*queue_cnt] = current_process;
        (*queue_cnt)++;

        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        return new_process;
    }

    ready_queue[*queue_cnt] = new_process;
    (*queue_cnt)++;
    return current_process;
}

struct PCB handle_process_completion_pp(struct PCB *ready_queue, int *queue_cnt,
                                        int time_stamp)
{
    if (*queue_cnt == 0) {
        struct PCB nullpcb = {0,0,0,0,0,0,0};
        return nullpcb;
    }

    int idx = 0;
    for (int i = 1; i < *queue_cnt; i++) {
        if (ready_queue[i].process_priority < ready_queue[idx].process_priority)
            idx = i;
    }

    struct PCB chosen = ready_queue[idx];

    for (int i = idx; i < *queue_cnt - 1; i++)
        ready_queue[i] = ready_queue[i+1];

    (*queue_cnt)--;

    chosen.execution_starttime = time_stamp;
    chosen.execution_endtime = time_stamp + chosen.remaining_bursttime;
    return chosen;
}

// Shortest-Remaining-Time-Next Preemptive Scheduling (SRTP)

struct PCB handle_process_arrival_srtp(struct PCB *ready_queue, int *queue_cnt,
                                       struct PCB current_process,
                                       struct PCB new_process, int time_stamp)
{
    if (current_process.process_id == 0) {
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        return new_process;
    }

    int executed = time_stamp - current_process.execution_starttime;
    int current_rbt = current_process.remaining_bursttime - executed;

    if (new_process.remaining_bursttime < current_rbt) {
        // preempt current process
        current_process.remaining_bursttime = current_rbt;
        ready_queue[*queue_cnt] = current_process;
        (*queue_cnt)++;

        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        return new_process;
    }

    ready_queue[*queue_cnt] = new_process;
    (*queue_cnt)++;
    return current_process;
}

struct PCB handle_process_completion_srtp(struct PCB *ready_queue, int *queue_cnt,
                                          int time_stamp)
{
    if (*queue_cnt == 0) {
        struct PCB nullpcb = {0,0,0,0,0,0,0};
        return nullpcb;
    }

    // select smallest RBT
    int idx = 0;
    for (int i = 1; i < *queue_cnt; i++) {
        if (ready_queue[i].remaining_bursttime < ready_queue[idx].remaining_bursttime)
            idx = i;
    }

    struct PCB chosen = ready_queue[idx];

    for (int i = idx; i < *queue_cnt - 1; i++)
        ready_queue[i] = ready_queue[i+1];

    (*queue_cnt)--;

    chosen.execution_starttime = time_stamp;
    chosen.execution_endtime = time_stamp + chosen.remaining_bursttime;
    return chosen;
}

// Round Robin (RR)

struct PCB handle_process_arrival_rr(struct PCB *ready_queue, int *queue_cnt,
                                     struct PCB current_process,
                                     struct PCB new_process,
                                     int time_stamp, int time_quantum)
{
    if (current_process.process_id == 0) {
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        return new_process;
    }

    ready_queue[*queue_cnt] = new_process;
    (*queue_cnt)++;

    return current_process;
}

struct PCB handle_process_completion_rr(struct PCB *ready_queue, int *queue_cnt,
                                        int time_stamp, int time_quantum)
{
    if (*queue_cnt == 0) {
        struct PCB nullpcb = {0,0,0,0,0,0,0};
        return nullpcb;
    }

    struct PCB chosen = ready_queue[0];

    for (int i = 0; i < *queue_cnt - 1; i++)
        ready_queue[i] = ready_queue[i+1];

    (*queue_cnt)--;

    chosen.execution_starttime = time_stamp;
    chosen.execution_endtime = time_stamp + chosen.remaining_bursttime;
    return chosen;
}
