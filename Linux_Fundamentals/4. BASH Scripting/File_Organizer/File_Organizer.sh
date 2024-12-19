#!/usr/bin/bash

: << 'Project_Idea'

The goal of this project is to create a Bash script that organizes files in a specified directory
based on their file types into separate subdirectories.
This can be useful to keep your directories clean and tidy by automatically sorting files
into appropriate categories.

Features:

- The script should take a directory path as an argument and organize the files within that directory.
- It should create subdirectories based on the file extensions 
  (e.g., “txt” for text files, “jpg” for image files).
- Files with unknown or no file extensions should be placed in a “misc” subdirectory.
- If a subdirectory for a particular file type already exists, the script should move the files
  into that existing directory.
- The script should handle edge cases, such as files with no extensions or hidden files 
  (those starting with a dot).

Project_Idea

### Variables ###

    declare DIRECTORY_PATH=$1               # Target directory argument to sort its files

    declare -a EXTENSIONS=("txt" "jpg" "pdf" "misc")     # Checklist of all supported extensions

    declare -a FILES_ARR=()                 # An array of all files present inside directory

    
### Functions ###

function create()
{
    for item in "${EXTENSIONS[@]}"; do
        if [[ ! -d "${DIRECTORY_PATH}/${item}" ]]; then
            mkdir "${DIRECTORY_PATH}/${item}"
        fi
    done

    if [[ ! -d "${DIRECTORY_PATH}/misc" ]]; then
        mkdir "${DIRECTORY_PATH}/misc"
    fi
}

function get()
{
mapfile -t FILES_ARR < <(find "$DIRECTORY_PATH" -maxdepth 1 -type f -exec basename {} \;)
}

function move()
{
    mv "$1" "$2"
}

function main()
{
    # 1. Create directory tree (txt jpg pdf misc)
    create

    # 2. Get files inside directory
    get

    # 3. For each file:

    for file in "${FILES_ARR[@]}"; do

        # Get extension
        File_EXT=${file##*.}
        FLAG=0  # FLAG = False

        # Check extension inside white-list
        # if yes:
                # Move file into this directory
            # if no:
                # Move file into misc 

        for item in "${EXTENSIONS[@]}"; do

            if [[ "${File_EXT}" == "${item}" ]]; then

                mv "${DIRECTORY_PATH}/${file}" "${DIRECTORY_PATH}/${File_EXT}/${file}"

                FLAG=1  # FLAG = True

                break
            fi
        done

        if (( FLAG == 0 )); then    # File extension is not supported ---> move to misc dir
            mv "${DIRECTORY_PATH}/${file}" "${DIRECTORY_PATH}/misc/${file}"
        fi


    done
        
}

### Execute main ###
main