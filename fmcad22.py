import subprocess,sys
import time
import os

# Run SWISS benchmarks for FMCAD 22 results.

bms = [
    "tla-Consensus.pyv",
    "tla-TCommit.pyv",
    # "i4-lock_server.pyv  # reported in SWISS NSDI21 pape",
    "ex-quorum-leader-election.pyv",
    # "pyv-toy_consensus_forall.pyv  # reported in SWISS NSDI21 pape",
    "tla-Simple.pyv",
    "ex-lockserv_automaton.pyv",
    "tla-SimpleRegular.pyv",
    # "pyv-sharded_kv.pyv  # reported in SWISS NSDI21 pape",
    # "pyv-lockserv.pyv  # reported in SWISS NSDI21 pape",
    "tla-TwoPhase.pyv",
    "i4-learning_switch.pyv",
    "ex-simple-decentralized-lock.pyv",
    # "i4-two_phase_commit.pyv  # reported in SWISS NSDI21 pape",
    # "pyv-consensus_wo_decide.pyv  # reported in SWISS NSDI21 pape",
    # "pyv-consensus_forall.pyv  # reported in SWISS NSDI21 paper",
    # "pyv-learning_switch.pyv  # reported in SWISS NSDI21 pape",
    # "i4-chord_ring_maintenance.pyv  # reported in SWISS NSDI21 paper,
    # "pyv-sharded_kv_no_lost_keys.pyv  # reported in SWISS NSDI21 paper,
    "ex-naive_consensus.pyv",
    # "pyv-client_server_ae.pyv  # reported in SWISS NSDI21 paper,
    "ex-simple-election.pyv",
    # "pyv-toy_consensus_epr.pyv  # reported in SWISS NSDI21 paper,
    "ex-toy_consensus.pyv",
    # "pyv-client_server_db_ae.pyv  # reported in SWISS NSDI21 pape",
    # "pyv-hybrid_reliable_broadcast.pyv  # reported in SWISS NSDI21 pape",
    "pyv-firewall.pyv",
    "ex-majorityset-leader-election.pyv",
    "pyv-consensus_epr.pyv",
    "mldr_epr_15.ivy"
]

def run_bm(bm, config):
    bms_dir = "benchmarks/fmcad22"
    logdir = f"logs/{config}/" + bm.split(".")[0]
    os.makedirs(logdir, exist_ok=True)

    swiss_cmd = f"./run.sh {bms_dir}/{bm} --seed 10 --config auto --threads 1 --minimal-models --with-conjs --logdir {logdir}"
    # timeout_prefix = "timeout 10m "
    # cmd = timeout_prefix + swiss_cmd
    cmd = swiss_cmd
    print(cmd)
    one_minute = 60
    timeout_secs = 10 * one_minute
    try:
        ret = subprocess.run(cmd, shell=True, timeout=timeout_secs)
    except subprocess.TimeoutExpired:
        print(f"Terminated due to timeout expiring ({timeout_secs}s)")

    # print(ret.stdout)
    # print(ret.stderr)
    # retlines = ret.stdout.splitlines()

    # summary_file = logdir + "/summary"
    # summary_text = open(summary_file).read()
    # print(summary_text)

bms_to_run = bms
for ind,bm in enumerate(bms_to_run):
    msg = f"\n### Running benchmark {ind+1}/{len(bms_to_run)}: '{bm}'"
    print(msg)
    sys.stdout.flush()

    # Run under both configs.
    run_bm(bm, "auto")
    run_bm(bm, "templ")

    # Avoid conflicts.
    time.sleep(1)

