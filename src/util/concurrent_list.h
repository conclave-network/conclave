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

#pragma once

#include <optional>
#include <thread>
#include <vector>
#include <mutex>

template<typename T>
class ConcurrentList
{
    public:
    ConcurrentList()
    {
        // List will be empty so initially block
        // when somebody tries to get an item
        emptyMutex.lock();
        willShutDown = false;
    }
    
    bool addToStart(const T&& item)
    {
        return add(std::move(item), false);
    }
    
    bool addToStart(const T& item)
    {
        return add(std::move(item), false);
    }
    
    bool addToEnd(const T&& item)
    {
        return add(std::move(item), true);
    }
    
    bool addToEnd(const T& item)
    {
        return add(std::move(item), true);
    }
    
    std::optional<T> popFromStart()
    {
        return pop(false);
    }
    
    std::optional<T> popFromEnd()
    {
        return pop(true);
    }
    
    void shutdown()
    {
        willShutDown = true;
        emptyMutex.unlock();
    }
    
    bool isShuttingDown()
    {
        return willShutDown;
    }
    
    size_t size()
    {
        return items.size();
    }
    
    const T& operator[](const size_t index)
    {
        itemsMutex.lock();
        if (index >= items.size()) {
            throw std::runtime_error("Cannot return item at index " + std::to_string(index));
        }
        T& ret = items[index];
        itemsMutex.unlock();
        return ret;
    }
    
    private:
    bool add(const T& item, const bool toEnd)
    {
        itemsMutex.lock();
        if (willShutDown) {
            itemsMutex.unlock();
            return false;
        }
        if (toEnd) {
            items.emplace_back(std::move(item));
        } else {
            items.insert(items.begin(), std::move(item));
        }
        emptyMutex.unlock();
        itemsMutex.unlock();
        return true;
    }
    
    std::optional<T> pop(const bool fromEnd)
    {
        removeMutex.lock();
        emptyMutex.lock();
        if (willShutDown) {
            emptyMutex.unlock();
            itemsMutex.unlock();
            removeMutex.unlock();
            return std::nullopt;
        }
        itemsMutex.lock();
        std::optional<T> ret;
        if (fromEnd) {
            ret.emplace(items[items.size() - 1]);
            items.pop_back();
        } else {
            ret.emplace(items[0]);
            items.erase(items.begin());
        }
        if (items.size() > 0) {
            emptyMutex.unlock();
        }
        itemsMutex.unlock();
        removeMutex.unlock();
        return ret;
    }
    
    std::vector<T> items;
    std::mutex itemsMutex;
    std::mutex emptyMutex;
    std::mutex removeMutex;
    bool willShutDown;
};
