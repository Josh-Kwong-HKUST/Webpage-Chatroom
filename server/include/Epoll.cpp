#include "Epoll.h"
#include <string.h>
#include <sys/epoll.h>
#include <iostream>

void Epoll::Init(Socket& socket){
    socket.setNonBlocking();
    this->addSocket(socket);
    bzero(events, sizeof(epoll_event) * MAXEVENTS);
}

void Epoll::addSocket(Socket& socket){
    int fd = socket.getSockfd();
    epoll_event event;
    bzero(&event, sizeof(event));  
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if(epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) == -1){
        std::cerr << "=====Error: epoll failed to add file descriptor=====\n";
        exit(-1);
    }
}

std::vector<epoll_event> Epoll::poll(int timeout){
    std::vector<epoll_event> activeEvents;
    int numFds = epoll_wait(epollFd, events, MAXEVENTS, timeout);
    if (numFds == -1){
        std::cerr << "=====Error: epoll failed to wait=====\n";
        exit(-1);
    }
    for (int i = 0; i < numFds; i++){
        activeEvents.push_back(events[i]);
    }
    return activeEvents;
}
