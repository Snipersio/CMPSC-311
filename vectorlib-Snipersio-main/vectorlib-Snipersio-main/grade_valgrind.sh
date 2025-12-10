#!/bin/bash

valgrind -v --leak-check=full --error-exitcode=1 --errors-for-leak-kinds=all ./test && (echo ""; echo "[GRADE] points for correctness: 3/3, valgrind detected no memory errors. PASSED.") || (echo ""; echo "[GRADE] points for correctness: 0/3, valgrind detected memory errors. FAILED.")