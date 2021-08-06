#!/bin/bash
echo "this is shell test"
echo # this inserts an empty line in output.
echo "we are currently in the following directory"

/bin/pwd
echo
echo "this directory contains the following files"
echo $0
echo $*
/bin/ls

