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

#include "hash160.h"
#include "hash256.h"
#include "public_key.h"
#include <bitcoin/system.hpp>

using namespace bc::system;
namespace conclave
{
    using ScriptOp = bc::system::machine::opcode;
    
    template<typename T>
    const machine::operation integralToOperation(T integral)
    {
        static_assert(std::is_integral<T>::value, "Integral required.");
        switch (integral) {
            case -1:
                return machine::operation(ScriptOp::push_negative_1);
            case 0:
                return machine::operation(ScriptOp::push_size_0);
            case 1:
                return machine::operation(ScriptOp::push_positive_1);
            case 2:
                return machine::operation(ScriptOp::push_positive_2);
            case 3:
                return machine::operation(ScriptOp::push_positive_3);
            case 4:
                return machine::operation(ScriptOp::push_positive_4);
            case 5:
                return machine::operation(ScriptOp::push_positive_5);
            case 6:
                return machine::operation(ScriptOp::push_positive_6);
            case 7:
                return machine::operation(ScriptOp::push_positive_7);
            case 8:
                return machine::operation(ScriptOp::push_positive_8);
            case 9:
                return machine::operation(ScriptOp::push_positive_9);
            case 10:
                return machine::operation(ScriptOp::push_positive_10);
            case 11:
                return machine::operation(ScriptOp::push_positive_11);
            case 12:
                return machine::operation(ScriptOp::push_positive_12);
            case 13:
                return machine::operation(ScriptOp::push_positive_13);
            case 14:
                return machine::operation(ScriptOp::push_positive_14);
            case 15:
                return machine::operation(ScriptOp::push_positive_15);
            case 16:
                return machine::operation(ScriptOp::push_positive_16);
            default:
                return machine::operation(ScriptOp::nop);
        }
    }
    
    /***
     * Wrapper around libbitcoin's machine::operation class
     */
    class ScriptElement final
    {
        public:
        ScriptElement(const ScriptOp& scriptOp)
            : operation(scriptOp)
        {
        }
        
        ScriptElement(const std::vector<BYTE>& pushData)
            : operation(pushData)
        {
        }
        
        ScriptElement(const Hash256& hash256)
            : ScriptElement(static_cast<std::vector<BYTE>>(hash256))
        {
        }
        
        ScriptElement(const Hash160& hash160)
            : ScriptElement(static_cast<std::vector<BYTE>>(hash160))
        {
        }
        
        ScriptElement(const PublicKey& publicKey)
            : ScriptElement(static_cast<std::vector<BYTE>>(publicKey))
        {
        }
        
        template<typename T>
        ScriptElement(const T integral)
            :operation(integralToOperation(integral))
        {
        }
        
        operator machine::operation() const
        {
            return operation;
        }
        
        private:
        machine::operation operation;
    };
}
