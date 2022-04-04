import subprocess,sys
import time

bms = [
    "tla-consensus.pyv",
    "tla-tcommit.pyv",
    "ex-quorum-leader",
    "toy_consensus_forall.pyv ",
    "tla-simple.pyv",
    "ex-lockserv-automaton.pyv",
    "tla-simpleregular.pyv",
    "sharded_kv.pyv",
    "lockserv.pyv",
    "tla-twophase.pyv",
    "learning_switch.pyv",
    "simple-decentralized-lock",
    "two_phase_commit.pyv",
    "consensus_wo_decide.pyv",
    "consensus_forall.pyv",
    "pyv-learning-switch",
    "chord.pyv",
    "sharded_kv_no_lost_keys.pyv",
    "naive_consensus.ivy",
    "client_server_ae.pyv",
    "simple-election.pyv",
    "toy_consensus_epr.pyv",
    "toy_consensus.pyv",
    "client_server_db_ae.pyv",
    "hybrid_reliable_broadcast.pyv",
    "firewall.pyv",
    "majorityset_leader_election.pyv",
    "consensus_epr.pyv"
]

# f = open("distai_ivybench_log.txt", 'w')
bms_to_run = bms
for ind,bm in enumerate(bms_to_run):
    msg = f"=== Running benchmark {ind+1}/{len(bms_to_run)}: '{bm}'"
    print(msg)
    sys.stdout.flush()
    # logdir = "ivybench" + bm.split(".")[0]
    # --logdir
    cmd = f"./run.sh benchmarks/{bm} --config auto --threads 1 --minimal-models --with-conjs"
    ret = subprocess.run(cmd, shell=True)
    print(ret.stdout)
    time.sleep(2)
    # print(ret.stderr)
    # retlines = ret.stdout.splitlines()
