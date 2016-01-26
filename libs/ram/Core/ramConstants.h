// 
// ramConstants.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "ofMain.h"

const std::string RAM_OSC_ADDR_ACTOR = "/ram/skeleton";
const std::string RAM_OSC_ADDR_RIGID_BODY = "/ram/rigid_body";
const std::string RAM_OSC_ADDR_SET_SCENE = "/ram/set_scene";
const std::string RAM_OSC_ADDR_CLEAR_SCENE = "/ram/clear_scene";

const std::string RAM_OSC_ADDR_COMMUNICATE_NOTEON		= "/ram/communicate/noteon";
const std::string RAM_OSC_ADDR_COMMUNICATE_NOTEOFF	= "/ram/communicate/noteoff";
const std::string RAM_OSC_ADDR_COMMUNICATE_CC			= "/ram/communicate/cc";


const float RAM_OUTDATED_DURATION = 1.0;
