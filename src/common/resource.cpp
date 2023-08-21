//
//  resource.cpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/3.
//

#include "resource.hpp"
#include "path.h"

using namespace std;

string R::file(const char * file) {
    return ProjectPath + "/" + file;
}

string R::image(const char * file) {
    return ProjectPath + "/resources/textures/" + file;
}

string R::shader(const char * file) {
    return ProjectPath + "/src/render/" + file;
}
