#!/bin/bash

types=(char int float double long pvoid)
members=(c i f d l pv)

while read -r file; do
  if [[ "$file" == *BASETYPE* ]]; then
    i=0
    # echo "$file"
    for type in "${types[@]}"; do
      mkdir -p "$(dirname "${file//BASETYPE/$type}")"
      touch "${file//BASETYPE/$type}"
      sed "s/MEMBERTYPE/${members[$i]}/g" "$file" > "${file//BASETYPE/$type}"
      sed -i "s/BASETYPE/$type/g" "${file//BASETYPE/$type}"
      ((i++));
    done
  fi
done < <(find ./list_copied/ -type f)

find ./list_copied/ -name *BASETYPE* -type d | xargs rm -r
