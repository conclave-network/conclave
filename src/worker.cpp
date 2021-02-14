/*
 * CONCLAVE - Scaling Bitcoin Simply.
 * Copyright (C) 2019-2021 Conclave development team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "worker.h"
#include <iostream>
#include <stdexcept>

Worker::Worker()
{
}

Worker::Worker(const Worker& other)
    : threadShouldRun(other.threadShouldRun), threadRunning(other.threadRunning)
{
    if (other.threadShouldRun || other.threadRunning) {
        throw std::logic_error("Copy constructor called on running Worker");
    }
}

Worker::Worker(const Worker&& other)
    : threadShouldRun(other.threadShouldRun), threadRunning(other.threadRunning)
{
    if (other.threadShouldRun || other.threadRunning) {
        throw std::logic_error("Move constructor called on running Worker");
    }
}

void Worker::prepare()
{
    // Deliberately blank
}

void Worker::work()
{
    workWaitMutex.lock();
}

void Worker::cleanup()
{
    // Deliberately blank
}

Worker::~Worker()
{
    joinIfJoinable();
}

void Worker::start()
{
    startStopMutex.lock();
    if (threadRunning) {
        goto unlock;
    }
    
    // Sometimes the worker thread may have exited and the client calls
    // start() again to restart the thread. In such a case the thread
    // be joinable and must be joined.
    joinIfJoinable();
    threadStartMutex.unlock();
    threadStartMutex.lock();
    threadShouldRun = true;
    thread = std::thread(&Worker::run, this);
    threadStartMutex.lock();
unlock:
    startStopMutex.unlock();
}

void Worker::stop(const bool join)
{
    startStopMutex.lock();
    threadShouldRun = false;
    workWaitMutex.unlock();
    if (join) {
        thread.join();
    }
    startStopMutex.unlock();
}

void Worker::run()
{
    threadRunning = true;
    workWaitMutex.unlock();
    workWaitMutex.lock();
    threadStartMutex.unlock();
    try {
        prepare();
        while (threadShouldRun) {
            work();
        }
    } catch (const std::exception& e) {
        std::cout << "Worker caught:" << e.what() << std::endl;
    }
    cleanup();
    threadRunning = false;
}

void Worker::joinIfJoinable()
{
    if (thread.joinable()) {
        thread.join();
    }
}
