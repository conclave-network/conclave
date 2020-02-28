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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <string>
#include <iostream>

/**
 * Utility functions for working with JSON, using the boost property tree classes
 */

namespace pt = boost::property_tree;

inline const std::string jsonToString(const pt::ptree& tree, const bool pretty = true)
{
    std::ostringstream oss;
    write_json(oss, tree, pretty);
    std::string str = oss.str();
    return str;
}

inline const pt::ptree parseJson(const std::string& str)
{
    pt::ptree root;
    std::istringstream iss(str);
    try {
        pt::read_json(iss, root);
        return root;
    } catch (...) {
        throw std::runtime_error("Malformed JSON: " + str);
    }
}

template<typename T>
inline const T getPrimitiveFromJson(const pt::ptree& tree, const std::string& key)
{
    return tree.get<T>(key);
}

template<typename T>
inline const T getObjectFromJson(const pt::ptree& tree, const std::string& key)
{
    return T(tree.get_child(key));
}

template<typename T>
inline const std::optional<T> getOptionalPrimitiveFromJson(const pt::ptree& tree, const std::string& key)
{
    boost::optional<T> opt = tree.get_optional<T>(key);
    if (opt.has_value()) {
        return opt.get();
    } else {
        return std::nullopt;
    }
}

template<typename T>
inline const std::optional<T> getOptionalObjectFromJson(const pt::ptree& tree, const std::string& key)
{
    boost::optional<const pt::ptree&> opt = tree.get_child_optional(key);
    if (opt.has_value()) {
        return T(opt.get());
    } else {
        return std::nullopt;
    }
}

template<typename T>
inline const std::vector<T> tryGetVectorOfPrimitives(const pt::ptree& tree, const std::string childName)
{
    std::vector<T> vec;
    for (auto& item : tree.get_child(childName)) {
        vec.push_back(item.second.get_value<T>());
    }
    return vec;
}

template<typename T>
inline const std::vector<T> tryGetVectorOfObjects(const pt::ptree& tree, const std::string childName)
{
    std::vector<T> vec;
    for (auto& item : tree.get_child(childName)) {
        vec.push_back(T(item.second));
    }
    return vec;
}

template<typename T>
inline const pt::ptree vectorOfPrimitivesToArray(const std::vector<T> vec)
{
    pt::ptree tree;
    for (const T& item : vec) {
        pt::ptree ele;
        ele.put("", item);
        tree.push_back(std::make_pair("", ele));
    }
    return tree;
}

template<typename T>
inline const pt::ptree vectorOfObjectsToArray(const std::vector<T> vec)
{
    pt::ptree tree;
    for (const T& item : vec) {
        tree.push_back(std::make_pair("", (pt::ptree) item));
    }
    return tree;
}

template<typename T>
inline const std::vector<T> arrayToVectorOfPrimitives(const pt::ptree& tree)
{
    std::vector<T> vec;
    for (auto& item : tree) {
        vec.push_back(item.second.get_value<T>());
    }
    return vec;
}

template<typename T>
inline const std::vector<T> arrayToVectorOfObjects(const pt::ptree& tree)
{
    std::vector<T> vec;
    for (auto& item : tree) {
        vec.push_back(T(item.second));
    }
    return vec;
}
