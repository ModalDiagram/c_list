#!/bin/bash

echo "" > "copy.h"
blocco=()
tipi_base=(int float double)
while read -r line; do

  if [[ $inside_func == true ]]; then
    # echo "$line"
    if [[ "$line" == *FINEFUNZ* ]]; then
      for tipo in "${tipi_base[@]}"; do
        for line_da_mod in "${blocco[@]}"; do
          # echo "$line_da_mod"
          echo "${line_da_mod//TIPOBASE/$tipo}" >> "copy.h"
        done
        echo "" >> "copy.h"
      done
      echo "fine"
      # echo "BloccoInt" >> "copy.h"
      # echo "BloccoFloat" >> "copy.h"
      inside_func=false
      continue;
    fi
    blocco+=("$line")
    continue
  fi

  if [[ "$line" == *INIZIOFUNZ* ]]; then
    echo "inizio"
    inside_func=true
    continue
  else
    echo "$line" >> "copy.h"
  fi
done < "list.h"
