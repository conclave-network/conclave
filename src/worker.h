/*
 * CONCLAVE - Making Bitcoin Scale And Be Useful
 * Copyright (C) 2019-2020 Noel P. O'Donnell <noel.odonnell.2020@mumail.ie>
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

#pragma once

#include <thread>
#include <mutex>

class Worker
{
    public:
    Worker();
    Worker(const Worker&);
    Worker(const Worker&&);
    ~Worker();
    void start();
    void stop(const bool = true);
    private:
    /**
     * These three virtual methods may be re-implemented by by the subclass. All
     * three get executed in the worker thread from the run() method.
     *
     * `prepare()` is run once on `start()` to prepare resources.
     * `work()` performs one unit of work and is run continuously in a loop
     * until `threadShouldRun` becomes false.
     * `cleanup()` is called after work loop is finished and is meant to clean up
     * resources.
	 *
	 * If the `work()` method is overridden, it's up to the implementer to ensure
	 * it runs for a reasonable amount of time.
     */
    virtual void prepare();
    virtual void work();
    virtual void cleanup();
    void run();
    void joinIfJoinable();
    std::thread thread;
    std::mutex startStopMutex;
    std::mutex threadStartMutex;
    std::mutex workWaitMutex;
    bool threadShouldRun = false;
    bool threadRunning = false;
};
