// Priority Preemptive (PP)

struct PCB handle_process_arrival_pp(struct PCB *ready_queue, int *queue_cnt,
                                     struct PCB current_process,
                                     struct PCB new_process, int time_stamp)
{
    if (current_process.PID == 0) {
        new_process.EST = time_stamp;
        new_process.EET = time_stamp + new_process.TBT;
        return new_process;
    }

    if (new_process.Priority < current_process.Priority) {
        current_process.RBT -= time_stamp - current_process.EST;
        ready_queue[*queue_cnt] = current_process;
        (*queue_cnt)++;

        new_process.EST = time_stamp;
        new_process.EET = time_stamp + new_process.TBT;
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
        if (ready_queue[i].Priority < ready_queue[idx].Priority)
            idx = i;
    }

    struct PCB chosen = ready_queue[idx];

    for (int i = idx; i < *queue_cnt - 1; i++)
        ready_queue[i] = ready_queue[i+1];

    (*queue_cnt)--;

    chosen.EST = time_stamp;
    chosen.EET = time_stamp + chosen.TBT;
    return chosen;
}

// SRTP
struct PCB handle_process_arrival_srtp(struct PCB *ready_queue, int *queue_cnt,
                                       struct PCB current_process,
                                       struct PCB new_process, int time_stamp)
{
    if (current_process.PID == 0) {
        new_process.EST = time_stamp;
        new_process.EET = time_stamp + new_process.TBT;
        return new_process;
    }

    int executed = time_stamp - current_process.EST;
    int current_rbt = current_process.RBT - executed;

    if (new_process.RBT < current_rbt) {
        // preempt current process
        current_process.RBT = current_rbt;
        ready_queue[*queue_cnt] = current_process;
        (*queue_cnt)++;

        new_process.EST = time_stamp;
        new_process.EET = time_stamp + new_process.TBT;
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
        if (ready_queue[i].RBT < ready_queue[idx].RBT)
            idx = i;
    }

    struct PCB chosen = ready_queue[idx];

    for (int i = idx; i < *queue_cnt - 1; i++)
        ready_queue[i] = ready_queue[i+1];

    (*queue_cnt)--;

    chosen.EST = time_stamp;
    chosen.EET = time_stamp + chosen.RBT;
    return chosen;
}

// Round Robin (RR)

struct PCB handle_process_arrival_rr(struct PCB *ready_queue, int *queue_cnt,
                                     struct PCB current_process,
                                     struct PCB new_process,
                                     int time_stamp, int time_quantum)
{
    if (current_process.PID == 0) {
        new_process.EST = time_stamp;
        new_process.EET = time_stamp + new_process.TBT;
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

    chosen.EST = time_stamp;
    chosen.EET = time_stamp + chosen.RBT;
    return chosen;
}
