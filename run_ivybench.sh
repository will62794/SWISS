#!/bin/sh
./run.sh benchmarks/tla-consensus.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/tla-tcommit.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/TwoPhase.pyv --config auto --threads 1 --minimal-models --with-conjs
