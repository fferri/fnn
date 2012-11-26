#!/bin/bash

for i in *.log; do
    grep ^epoch $i | sed \
        -e 's/epoch: //' \
        -e 's/ pseudoMSE: //' \
        -e 's/ validationMSE: //' \
        -e 's/, class.*$//' \
        > "${i/.log/.MSE.csv}";
    echo "written ${i/.log/.MSE.csv}";
    grep ^epoch $i | sed \
        -e 's/epoch: //' \
        -e 's/ pseudoMSE:.*classification err: //' \
        -e 's/ validation err: //' \
        > "${i/.log/.ClsErr.csv}";
    echo "written ${i/.log/.ClsErr.csv}";
done
