#!/bin/bash

types=(char int float double pvoid)

while read -r file; do
  if [[ "$file" == *BASETYPE* ]]; then
    # echo "$file"
    for type in "${types[@]}"; do
      mkdir -p "$(dirname "${file//BASETYPE/$type}")"
      touch "${file//BASETYPE/$type}"
      sed "s/BASETYPE/$type/" "$file" > "${file//BASETYPE/$type}"
    done
  fi
done < <(find ./list_copied/ -type f)

find ./list_copied/ -name *BASETYPE* -type d | xargs rm -r
