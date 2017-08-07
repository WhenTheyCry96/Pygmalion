#pragma once
#include <fstream>
#include <string.h>
#include <cstring>
#include <cassert>
#include "geo.h"

float parseFloat(std::ifstream& file);
Point parsePoint(std::ifstream& file);
Line* parseSTL(const std::string filename, unsigned int& _num);