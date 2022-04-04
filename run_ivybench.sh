
# bms = [
#     tla-consensus.pyv
#     tla-tcommit.pyv
#     ex-quorum-leader
#     toy_consensus_forall.pyv ,
#     tla-simple.pyv
#     ex-lockserv-automaton.pyv
#     tla-simpleregular.pyv
#     sharded_kv.pyv ,
#     lockserv.pyv ,
#     tla-twophase.pyv
#     learning_switch.pyv ,
#     simple-decentralized-lock
#     two_phase_commit.pyv ,
#     consensus_wo_decide.pyv ,
#     consensus_forall.pyv ,
#     pyv-learning-switch
#     chord.pyv ,
#     sharded_kv_no_lost_keys.pyv ,
#     naive_consensus.ivy ,
#     client_server_ae.pyv ,
#     simple-election.pyv
#     toy_consensus_epr.pyv ,
#     toy_consensus.pyv ,
#     client_server_db_ae.pyv ,
#     hybrid_reliable_broadcast.pyv ,
#     firewall.pyv ,
#     majorityset_leader_election.pyv ,
#     consensus_epr.pyv ,
# ]

./run.sh benchmarks/tla-consensus.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/tla-tcommit.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/ex-quorum-leader-election.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/toy_consensus_forall.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/tla-simple.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/ex-lockserv-automaton.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/tla-simpleregular.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/sharded_kv.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/lockserv.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/tla-twophase.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/learning_switch.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/simple-decentralized-lock.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/two_phase_commit.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/consensus_wo_decide.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/consensus_forall.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/pyv-learning-switch.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/chord.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/sharded_kv_no_lost_keys.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/naive_consensus.ivy --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/client_server_ae.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/simple-election.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/toy_consensus_epr.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/toy_consensus.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/client_server_db_ae.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/hybrid_reliable_broadcast.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/firewall.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/majorityset_leader_election.pyv --config auto --threads 1 --minimal-models --with-conjs
./run.sh benchmarks/consensus_epr.pyv --config auto --threads 1 --minimal-models --with-conjs
