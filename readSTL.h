#pragma once
#include "geo.h"

float parseFloat(std::ifstream& file);
geo::Point parsePoint(std::ifstream& file);
geo::Line* parseSTL2Line(const std::string filename, unsigned int& _num, float extension);
geo::Obj* parseSTL2Obj(const std::string filename, unsigned int& _num, float extension);
