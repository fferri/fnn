#!/bin/bash

export PATH="$(cd "$(dirname $0)/.."; pwd):$PATH"

filter | tokenizer -l | wordstats | sort -n
