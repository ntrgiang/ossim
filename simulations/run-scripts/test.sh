#!/bin/sh
# Get the total number of runs
N_RUN=60
LAST_INDEX=$((N_RUN-1))

# Deviding the whole simulation into smaller "chunks"
N_ITERATION=30
N_MEASUREMENT=$1
INDEX_LOW=$((N_ITERATION*(N_MEASUREMENT-1)))
INDEX_HIGH=$((N_ITERATION*N_MEASUREMENT-1))

echo "INDEX_LOW = $INDEX_LOW -- INDEX_HIGH = $INDEX_HIGH"
