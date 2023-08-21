#!/bin/sh
set -e
set -u
set -o pipefail

PROJECT_FOLDER=$(dirname $(cd "$(dirname "$0")";pwd))
PATH_FILE="${PROJECT_FOLDER}/src/common/path.h"
if [ ! -e ${PATH_FILE} ]; then
    echo "${PATH_FILE} does not exist."
    exit 1  # 报错并退出
fi

cat >${PATH_FILE}<<EOF
#ifndef Path_h
#define Path_h

#include <string>

#ifdef __APPLE__
const std::string ProjectPath = "${PROJECT_FOLDER}";
#endif

#endif /* Path_h */
EOF