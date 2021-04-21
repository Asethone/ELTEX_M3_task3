// Программа является имитацией пользователя: отправляет на сервер свой PID, 
//   после чего ждет сообщения от сервера с типом сообщения, равным PID пользователя.
// Программа может вызываться многократно для имитации пользовательской активности. Каждый запрос обрабатывается сервером.
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

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
    mybuf.mtype = 1;
    long crntPID = (long)getpid();
    mybuf.data = crntPID;
    // 
    if (msgsnd(msqid, &mybuf, len, 0) < 0){
        printf("Error on sending message!\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }
    if (msgrcv(msqid, &mybuf, len, crntPID, 0) < 0){
        printf("Error on recieving message!\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }
    printf("User PID: %ld, server data: %ld\n", crntPID, mybuf.data);
    exit(0);
    return 0;
}

