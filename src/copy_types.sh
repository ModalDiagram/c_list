#!/usr/bin/env bash

types=(int long_int long_long_int unsigned long_unsigned long_long_unsigned float double char signed_char pvoid)
members=(i li lli u lu llu f d c sc pv)
types_with_spaces=(int "long int" "long long int" "unsigned" "long unsigned" "long long unsigned" "float" "double" "char" "signed char" "pvoid")

while read -r file; do
  if [[ "$file" == *BASETYPE* ]]; then
    i=0
    # echo "$file"
    for type in "${types[@]}"; do
      mkdir -p "$(dirname "${file//BASETYPE/$type}")"
      touch "${file//BASETYPE/$type}"
      sed "s/MEMBERTYPE/${members[$i]}/g" "$file" > "${file//BASETYPE/$type}"
      sed -i "s/BASETYPE_SPACE/${types_with_spaces[$i]}/g" "${file//BASETYPE/$type}"
      sed -i "s/BASETYPE/$type/g" "${file//BASETYPE/$type}"
      ((i++));
    done
  fi
done < <(find ./list_copied/ -type f)

find ./list_copied/ -name *BASETYPE* -type d | xargs rm -r
