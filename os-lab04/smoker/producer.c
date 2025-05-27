#include "ipc.h"
static int current_choice = 0;

int main(int argc, char* argv[]) {

    shm_flg = IPC_CREAT | 0644;
    sem_flg = IPC_CREAT | 0644;

    buff_key = 1234;
    buff_num = 3;
    buff_ptr = (int*)set_shm(buff_key, buff_num * sizeof(int), shm_flg);

    producer_sem_key = 200;
    mutex_sem_key = 201;
    agent_turn_sem_key = 202; 

    smoker_tobacco_sem_key = 301;
    smoker_paper_sem_key = 302;
    smoker_glue_sem_key = 303;

    producer_sem_id = set_sem(producer_sem_key, 1, sem_flg);
    mutex_sem_id = set_sem(mutex_sem_key, 1, sem_flg);
    agent_turn_sem_id = set_sem(agent_turn_sem_key, 1, sem_flg);
    smoker_tobacco_sem_id = set_sem(smoker_tobacco_sem_key, 0, sem_flg);
    smoker_paper_sem_id = set_sem(smoker_paper_sem_key, 0, sem_flg);
    smoker_glue_sem_id = set_sem(smoker_glue_sem_key, 0, sem_flg);

    printf("生产者(PID: %d) 已就绪。\n", getpid());

    while (1) {
        down(agent_turn_sem_id);

        down(producer_sem_id);

        down(mutex_sem_id);

        buff_ptr[0] = 0;
        buff_ptr[1] = 0;
        buff_ptr[2] = 0;

        int choice = current_choice % 3;

        if (choice == 0) {
            buff_ptr[1] = 1;
            buff_ptr[2] = 1;
            printf("生产者(PID: %d) 放置: 纸和胶水到桌子。\n", getpid());
            up(smoker_tobacco_sem_id);
        } else if (choice == 1) {
            buff_ptr[0] = 1;
            buff_ptr[2] = 1;
            printf("生产者(PID: %d) 放置: 烟草和胶水到桌子。\n", getpid());
            up(smoker_paper_sem_id);
        } else {
            buff_ptr[0] = 1;
            buff_ptr[1] = 1;
            printf("生产者(PID: %d) 放置: 烟草和纸到桌子。\n", getpid());
            up(smoker_glue_sem_id);
        }

        current_choice++;

        up(mutex_sem_id);

        up(agent_turn_sem_id);

        sleep(1);
    }

    return EXIT_SUCCESS;
}
