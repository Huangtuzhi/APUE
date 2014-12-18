#include <stdio.h>
#include <pthread.h>
#include <iostream>

struct job
{
    struct job *j_next;
    struct job *j_prev;
    pthread_t j_id;
    job(pthread_t id = 0, struct job *p = NULL, struct job *q = NULL):j_next(p), j_prev(q), j_id(id){}
    //Use the construct function to initiate the parameters
}; 

struct queue
{
    struct job *q_head;
    struct job *q_tail;
    pthread_rwlock_t q_lock;
};

int queue_init(struct queue *qp)
{
    int err;
    qp->q_head = NULL;
    qp->q_tail = NULL;
    err = pthread_rwlock_init(&qp->q_lock, NULL);
    if(err != 0)
        return err;
    return 0;
}

/*
 *Insert a job at the head of the queue
 */
void job_insert(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = qp->q_head;
    jp->j_prev = NULL;
    if (qp->q_head != NULL)
        qp->q_head->j_prev = jp;
    else
        qp->q_tail = jp;
    qp->q_head = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

/*
 * Append a job on the tail of the queue
 */
void job_append(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    if(qp->q_tail == NULL)
    {
        qp->q_head = jp;
        jp->j_next = NULL;
        jp->j_prev = NULL;
    }
    
    else
    {
        jp->j_next = NULL;
        jp->j_prev = qp->q_tail;
        qp->q_tail->j_next = jp;
    }
    qp->q_tail = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

void job_remove(struct queue *qp, struct job * jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    if(jp == qp->q_head)
    {
        qp->q_head = qp->q_head->j_next;
        if(qp->q_tail == jp)
            qp->q_tail = NULL;
        else
            qp->q_head->j_prev = NULL;
    }
    else if(jp == qp->q_tail)
    {
        qp->q_tail = jp->j_prev;
        qp->q_tail->j_next = NULL;
    }
    else
    {
        jp->j_prev->j_next = jp->j_next;
        jp->j_next->j_prev = jp->j_prev;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}

struct job* job_find(struct queue* qp, pthread_t id)
{
    struct job *jp;

    if(pthread_rwlock_rdlock(&qp->q_lock) != 0)
        return NULL;

    for(jp = qp->q_head; jp != NULL; jp = jp->j_next)
        if(pthread_equal(jp->j_id, id))
        {
            std::cout<<"The id:"<< jp->j_id<<"\n";
            break;
        }
    pthread_rwlock_unlock(&qp->q_lock);
    return(jp);
}

int main()
{
    struct job Ajob(5), Bjob(6);
    struct queue *Queue;
    //Ajob.j_id = 5;
    //Ajob->j_prev = Ajob->j_next =NULL;
    //Bjob.j_id = 6;
    //Bjob->j_prev = Bjob->j_next =NULL;
    queue_init(Queue);
    job_insert(Queue, &Ajob);
    job_insert(Queue, &Bjob);   
    job_find(Queue, 6);

}
