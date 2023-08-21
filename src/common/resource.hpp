//
//  resource.hpp
//  LearnOpenGL
//
//  Created by ONEMC on 2023/7/3.
//

#ifndef resource_hpp
#define resource_hpp

#include <stdio.h>
#include <string>

class R {
public:
    static std::string file(const char * file);
    static std::string image(const char * file);
    static std::string shader(const char * file);
};


#endif /* resource_hpp */
