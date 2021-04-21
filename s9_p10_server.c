// Программа является имитацией сервера: принимает входящие данные (PID) от других процессов (пользователей), 
//   возводит их PID в квадрат и отсылает обратно с типом сообщения, равным PID пользователя
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    int msqid;                      // IPC дескриптор для очереди сообщений
    char pathname[] = "s9_p10_server.c";   // Имя файла, использующееся для генерации ключа
    key_t key;                      // IPC ключ
    int len = sizeof(long);         // Длина информативной части сообщения
    char item;                      // Выбранный пункт меню
    struct msgbuf {                 // Инициализация структуры для очереди сообщений
        long mtype;
        long data;
    } mybuf;
    // Получаем IPC-ключ
    key = ftok(pathname, 0);
    if(key < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }
    // Создаем очередь сообщений, либо получаем доступ к уже созданной
    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get msqid\n");
        exit(-1);
    }
    // 
    while (1) {
        if (msgrcv(msqid, &mybuf, len, 1, 0) < 0){
            printf("Error on recieving message from user!\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
        mybuf.mtype = mybuf.data;
        mybuf.data = mybuf.data * mybuf.data;
        if (msgsnd(msqid, &mybuf, len, 0) < 0){
            printf("Error on sending message to user!\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }
    return 0;
}

