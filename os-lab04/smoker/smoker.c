#include "ipc.h"

int main(int argc, char* argv[]) {
  
int smoker_id = atoi(argv[1]);

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

  
    producer_sem_id = set_sem(producer_sem_key, 0, sem_flg); 
    mutex_sem_id = set_sem(mutex_sem_key, 0, sem_flg); 
    agent_turn_sem_id = set_sem(agent_turn_sem_key, 0, sem_flg);

    smoker_tobacco_sem_id = set_sem(smoker_tobacco_sem_key, 0, sem_flg);
    smoker_paper_sem_id = set_sem(smoker_paper_sem_key, 0, sem_flg);
    smoker_glue_sem_id = set_sem(smoker_glue_sem_key, 0, sem_flg);

    const char* smoker_type;
    int my_sem_id;
    int needed_item1_idx = -1, needed_item2_idx = -1; 

    if (smoker_id == 0) {
        smoker_type = "烟草";
        my_sem_id = smoker_tobacco_sem_id;
        needed_item1_idx = 1;
        needed_item2_idx = 2;
    } else if (smoker_id == 1) {
        smoker_type = "纸";
        my_sem_id = smoker_paper_sem_id;
        needed_item1_idx = 0;
        needed_item2_idx = 2; 
    } else if (smoker_id == 2) { 
        smoker_type = "胶水";
        my_sem_id = smoker_glue_sem_id;
        needed_item1_idx = 0; 
        needed_item2_idx = 1; 
    } 

    while (1) {
        down(my_sem_id);

        down(mutex_sem_id);

        if (buff_ptr[needed_item1_idx] == 1 && buff_ptr[needed_item2_idx] == 1) {
            buff_ptr[needed_item1_idx] = 0;
            buff_ptr[needed_item2_idx] = 0;
            printf("(PID: %d)拿到了%s 成功吸烟 \n",  getpid(),smoker_type);
        } 

        up(mutex_sem_id);

        up(producer_sem_id);
        sleep(1);
    }

    return EXIT_SUCCESS;
}