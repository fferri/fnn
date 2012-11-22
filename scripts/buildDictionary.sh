#!/bin/bash

export SCRIPTS_DIR="$(cd "$(dirname $0)/"; pwd)"
export BIN_DIR="$(cd "$(dirname $0)/.."; pwd)"
export PATH="$BIN_DIR:$PATH"

filter | sed -E -f "$SCRIPTS_DIR/subst.sed" | tokenizer -l | sort | uniq
