/**
 * Copyright (C) 2018-2021 Xilinx, Inc
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You may
 * not use this file except in compliance with the License. A copy of the
 * License is located at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#ifndef COMMON_SENSOR_H
#define COMMON_SENSOR_H

// Local - Include Files
#include "config.h"
#include "device.h"

// 3rd Party Library - Include Files
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>

// System - Include Files
#include <iostream>

namespace xrt_core {
namespace sensor {

XRT_CORE_COMMON_EXPORT
boost::property_tree::ptree
read_power_rails(const xrt_core::device * device);

XRT_CORE_COMMON_EXPORT
boost::property_tree::ptree
read_thermals(const xrt_core::device * device);

XRT_CORE_COMMON_EXPORT
boost::property_tree::ptree
read_power_consumption(const xrt_core::device * device);

XRT_CORE_COMMON_EXPORT
boost::property_tree::ptree
read_fans(const xrt_core::device * device);

}} // sensor, xrt_core


// ------ L E G A C Y   X B U T I L   F U N C T I O N S  -----------------
// The following namespace is only used by legacy xbutil dump

namespace sensor_tree {

boost::property_tree::ptree&
instance();

/* 
 * Puts @val at the @path for instance()'s ptree.
 */
template <typename T>
void put(const std::string &path, T val)
{
    instance().put(path,val);
} 

/* 
 * Gets value of type @T at the @path for instance()'s ptree.
 * @defaultVal is returned if @path does not exist.
 */
template <typename T>
inline T get(const std::string &path, const T &defaultVal)
{
    return instance().get( path, defaultVal );
}

/* 
 * Gets value of type @T at the @path for instance()'s ptree.
 */
template <typename T>
inline T get(const std::string &path)
{
    return instance().get<T>( path );
}

/* 
 * Inserts child node @child at @path.
 */
inline void
add_child(const std::string &path, const boost::property_tree::ptree& child)
{
    instance().add_child( path, child );
}

/* 
 * Gets child node at @path.
 */
inline boost::property_tree::ptree
get_child(const std::string &path)
{
    return instance().get_child( path );
}

/* 
 * Dumps json format of ptree to @ostr.
 */
void json_dump(std::ostream &ostr);

/* 
 * Wipe out whole ptree.
 */
inline void clear()
{
    instance().clear();
}

/*
 * Pretty formats @val. If @val is non-string, and is equal to max_value of type @T, 
 * returns @default_val.
 * Returns a string.
 */
template <typename T>
inline std::string pretty( const T &val, const std::string &default_val = "N/A", bool isHex = false )
{   
    if( typeid(val).name() != typeid(std::string).name() ) {
        if( val >= std::numeric_limits<T>::max() || val == 0) {
            return default_val;
        }

        if( isHex ) {
            std::stringstream ss;
            ss << "0x" << std::hex << val;
            return ss.str();
        }
    }
    return boost::lexical_cast<std::string>(val);
}

/*
 * Wrapper around get() that formats as pretty.
 */
template <typename T>
inline std::string
get_pretty( const std::string &path, const std::string &default_val = "N/A", bool isHex = false )
{
    T val = instance().get<T>( path );
    return pretty( val, default_val, isHex );
}

} // namespace sensor_tree

#endif
