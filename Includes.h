#pragma once

#include "GLEW/glew.h"

#include "SOIL/SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

using std::string;
using std::vector;
using std::map;

using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::make_shared;

using std::cout;
using std::endl;
using std::setw;
using std::left;
