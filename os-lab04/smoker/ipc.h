#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSZ 256

int get_ipc_id(char* proc_file, key_t key);
char* set_shm(key_t shm_key, int shm_num, int shm_flag);
int set_msq(key_t msq_key, int msq_flag);
int set_sem(key_t sem_key, int sem_val, int sem_flag);
int down(int sem_id);
int up(int sem_id);

typedef union semuns {
    int val;
} Sem_uns;

typedef struct msgbuf {
    long mtype;
    char mtext[1];
} Msg_buf;

extern key_t agent_turn_sem_key;
extern int agent_turn_sem_id;

extern key_t buff_key;
extern int buff_num;
extern int* buff_ptr;

extern key_t producer_sem_key;
extern key_t mutex_sem_key;
extern int producer_sem_id;
extern int mutex_sem_id;

extern key_t smoker_tobacco_sem_key;
extern key_t smoker_paper_sem_key;
extern key_t smoker_glue_sem_key;
extern int smoker_tobacco_sem_id;
extern int smoker_paper_sem_id;
extern int smoker_glue_sem_id;

extern int sem_val;
extern int sem_flg;
extern int shm_flg;

extern key_t pput_key;
extern int pput_num;
extern int* pput_ptr;
extern key_t cget_key;
extern int cget_num;
extern int* cget_ptr;
