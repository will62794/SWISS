#!/bin/sh

#
# Run all FMCAD 2022 benchmarks with SWISS.
#

bmdir="benchmarks/fmcad22"

# Sleep to avoid log naming conflicts.

# Run with 'auto' config.
./run.sh $bmdir/tla-Consensus.pyv --config auto --threads 1 --minimal-models --with-conjs --logdir logs/auto/tla-Consensus && sleep 2
./run.sh $bmdir/tla-TCommit.pyv --config auto --threads 1 --minimal-models --with-conjs && sleep 2
# ./run.sh $bmdir/i4-lock_server.pyv --config auto --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper.
./run.sh $bmdir/ex-quorum-leader-election.pyv --config auto --threads 1 --minimal-models --with-conjs && sleep 2
# ./run.sh $bmdir/pyv-toy_consensus_forall.pyv --config auto --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper.
./run.sh $bmdir/tla-Simple.pyv --config auto --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/ex-lockserv_automaton.pyv --config auto --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/tla-SimpleRegular.pyv --config auto --threads 1 --minimal-models --with-conjs && sleep 2
# ./run.sh $bmdir/pyv-sharded_kv.pyv --config auto --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper.
# ./run.sh $bmdir/pyv-lockserv.pyv --config auto --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper.
./run.sh $bmdir/tla-TwoPhase.pyv --config auto --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/i4-learning_switch.pyv --config auto --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/ex-simple-decentralized-lock.pyv --config auto --threads 1 --minimal-models --with-conjs && sleep 2
# ./run.sh $bmdir/i4-two_phase_commit.pyv --config auto --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper.
# ./run.sh $bmdir/pyv-consensus_wo_decide.pyv --config auto --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper.
# ./run.sh $bmdir/pyv-consensus_forall.pyv --config auto --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper.
# ./run.sh $bmdir/pyv-learning_switch.pyv --config auto --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper.
# ./run.sh $bmdir/i4-chord_ring_maintenance.pyv --config auto --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper.
# ./run.sh $bmdir/pyv-sharded_kv_no_lost_keys.pyv --config auto --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper.
./run.sh $bmdir/ex-naive_consensus.pyv --config auto --threads 1 --minimal-models --with-conjs && sleep 2
# ./run.sh $bmdir/pyv-client_server_ae.pyv --config auto --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper.
./run.sh $bmdir/ex-simple-election.pyv --config auto --threads 1 --minimal-models --with-conjs && sleep 2
# ./run.sh $bmdir/pyv-toy_consensus_epr.pyv --config auto --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper.
./run.sh $bmdir/ex-toy_consensus.pyv --config auto --threads 1 --minimal-models --with-conjs && sleep 2
# ./run.sh $bmdir/pyv-client_server_db_ae.pyv --config auto --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper.
# ./run.sh $bmdir/pyv-hybrid_reliable_broadcast.pyv --config auto --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper.
./run.sh $bmdir/pyv-firewall.pyv --config auto --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/ex-majorityset-leader-election.pyv --config auto --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/pyv-consensus_epr.pyv --config auto --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/mldr_epr_15.ivy --config auto --threads 1 --minimal-models --with-conjs && sleep 2

# Run with 'templ' config.
./run.sh $bmdir/tla-Consensus.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/tla-TCommit.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
# ./run.sh $bmdir/i4-lock_server.pyv --config templ --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper. && sleep 2
./run.sh $bmdir/ex-quorum-leader-election.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
# ./run.sh $bmdir/pyv-toy_consensus_forall.pyv --config templ --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper. && sleep 2
./run.sh $bmdir/tla-Simple.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/ex-lockserv_templmaton.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/tla-SimpleRegular.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
# ./run.sh $bmdir/pyv-sharded_kv.pyv --config templ --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper. && sleep 2
# ./run.sh $bmdir/pyv-lockserv.pyv --config templ --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper. && sleep 2
./run.sh $bmdir/tla-TwoPhase.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/i4-learning_switch.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/ex-simple-decentralized-lock.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
# ./run.sh $bmdir/i4-two_phase_commit.pyv --config templ --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper. && sleep 2
# ./run.sh $bmdir/pyv-consensus_wo_decide.pyv --config templ --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper. && sleep 2
# ./run.sh $bmdir/pyv-consensus_forall.pyv --config templ --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper. && sleep 2
# ./run.sh $bmdir/pyv-learning_switch.pyv --config templ --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper. && sleep 2
# ./run.sh $bmdir/i4-chord_ring_maintenance.pyv --config templ --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper. && sleep 2
# ./run.sh $bmdir/pyv-sharded_kv_no_lost_keys.pyv --config templ --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper. && sleep 2
./run.sh $bmdir/ex-naive_consensus.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
# ./run.sh $bmdir/pyv-client_server_ae.pyv --config templ --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper. && sleep 2
./run.sh $bmdir/ex-simple-election.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
# ./run.sh $bmdir/pyv-toy_consensus_epr.pyv --config templ --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper. && sleep 2
./run.sh $bmdir/ex-toy_consensus.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
# ./run.sh $bmdir/pyv-client_server_db_ae.pyv --config templ --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper. && sleep 2
# ./run.sh $bmdir/pyv-hybrid_reliable_broadcast.pyv --config templ --threads 1 --minimal-models --with-conjs # reported in SWISS NSDI21 paper. && sleep 2
./run.sh $bmdir/pyv-firewall.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/ex-majorityset-leader-election.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/pyv-consensus_epr.pyv --config templ --threads 1 --minimal-models --with-conjs && sleep 2
./run.sh $bmdir/mldr_epr_15.ivy --config templ --threads 1 --minimal-models --with-conjs && sleep 2