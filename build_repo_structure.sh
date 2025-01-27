#!/bin/bash

# Check whether output file is provided
if [ "$#" -ne 1 ]; then
	  echo "Usage: $0 <filename>"
	    exit 1
fi

OUTPUT_FILE=$1

# Check whether given file exists
if [ ! -e "$OUTPUT_FILE" ]; then
	  echo "File $OUTPUT_FILE doesn't exist. Created new one."
	  touch "$OUTPUT_FILE"
fi

build_tree() {
          local dir_path=$1
	  local prefix=$2
	    
	  # Print current dir
	  echo "${prefix}${dir_path##*/}/" >> "$OUTPUT_FILE"
		
	  # Receive all elems from dir
	  local items=("$dir_path"/*)
	  for item in "${items[@]}"; do
	    if [ -d "$item" ]; then
	      build_tree "$item" "$prefix  ├── "
	    elif [ -f "$item" ]; then
	      echo "${prefix}  ├── ${item##*/}" >> "$OUTPUT_FILE"
	    fi
	  done
} 

{
   echo "\nThe structure of folders from $(pwd):"
   build_tree "." ""
} >> "$OUTPUT_FILE"

echo "The structure of folders has been added $OUTPUT_FILE."
