#!/bin/bash

# This script assumes that you have already built the project.
# How it works:
# 1. It looks for all .txt files in the source directory
# 2. For each source file, it looks for a corresponding query file
# 3. If a query file exists, it runs the autotester on the source file and query file
# 4. It then looks for failed tests in the output file
# 5. If there are failed tests, it prints the output file and exits with a non-zero exit code
# 6. If there are no failed tests, it prints a success message and exits with a zero exit code


RD='\033[0;31m'
NC="\033[0m"
GR="\033[1;32m"
YL="\033[1;33m"

# Assumes either Windows or MacOS

if [[ "$OSTYPE" == "cygwin" || "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    AUTOTESTER_BIN="./src/autotester/Release/autotester.exe"
else
    AUTOTESTER_BIN="./src/autotester/autotester"
fi

# PATHS
# SCRIPT_DIR=$(dirname $(readlink -f "$0"))
SOURCE_DIR="../Team35/Tests35/source"
QUERIES_DIR="../Team35/Tests35/query"
OUTPUT_DIR="../Team35/Tests35/output"

################################################################


for source_file in "${SOURCE_DIR}"/*.txt
do
    filename=$(basename "${source_file}" | cut -f 1 -d '.')
    query_file="${QUERIES_DIR}/${filename}.txt"
    if [ -e ${query_file} ]
    then
        output_file="${OUTPUT_DIR}/${filename}.xml"
        exec 3>&1
        exec >/dev/null
        ${AUTOTESTER_BIN} ${source_file} ${query_file} ${output_file}
        echo -e "${YL}Generated ${output_file}${NC}"
        exec >&3
    fi
done

FAILED_FILES=()

for output_file in "${OUTPUT_DIR}"/*.xml
do
    if grep -q "</failed>" ${output_file}; then
        FAILED_FILES+=(${output_file})
    fi
done

if [ ${#FAILED_FILES[@]} -gt 0 ]; then
    echo -e "${RD}The following output files contain failed tests:"
    for failed_file in "${FAILED_FILES[@]}"
    do
        echo -e "${failed_file}"
    done
    exit 1
else
    echo -e "${GR}All tests passed.${NC}"
    exit 0
fi
