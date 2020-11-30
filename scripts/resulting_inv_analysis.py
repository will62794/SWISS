import protocol_parsing
import os
import json
import subprocess
import sys
import tempfile
import paper_benchmarks

def get_protocol_filename(logdir):
  summary_file = os.path.join(logdir, "summary")
  with open(summary_file, "r") as f:
    for line in f:
      if line.startswith("Protocol: "):
        return line[10:].strip()
      if line.startswith("./save.sh "):
        return line.split()[1]

  assert False, "could not find protocol filename"

def does_claim_success(i):
  if "# Success: True" in i:
    return True
  elif "# Success: False" in i:
    return False
  else:
    assert False
        
def validate_run_invariants(logdir):
  with open(os.path.join(logdir, "invariants")) as f:
    inv_contents = f.read()

  claims_success = does_claim_success(inv_contents)

  if not claims_success:
    print("invariants file does not claim invariants are correct")
  else:
    print("invariants file claims to contain correct and complete invariants")
    protocol_filename = get_protocol_filename(logdir)
    print("protocol file: " + protocol_filename)
    j, invs = protocol_parsing.parse_invs(protocol_filename, inv_contents)

    j["conjectures"] = j["conjectures"] + invs

    tmpjson = tempfile.mktemp()
    with open(tmpjson, "w") as f:
      f.write(json.dumps(j))

    proc = subprocess.Popen(["./synthesis", "--input-module", tmpjson, "--check-inductiveness"])
    ret = proc.wait()

def count_terms_of_tmpfile(logdir):
  with open(os.path.join(logdir, "invariants")) as f:
    inv_contents = f.read()
  protocol_filename = get_protocol_filename(logdir)
  j, invs = protocol_parsing.parse_invs(protocol_filename, inv_contents)

  def count_terms(v):
    if v[0] in ('forall', 'exists'):
      return count_terms(v[2])
    elif v[0] in ('and', 'or'):
      return sum(count_terms(t) for t in v[1])
    elif v[0] == 'not':
      return count_terms(v[1])
    elif v[0] == 'apply':
      return 1
    elif v[0] == 'eq':
      return 1
    else:
      print(v)
      assert False

  count = sum(count_terms(i) for i in invs)
  #print("total terms: " + str(count))

  return {"invs": len(invs), "terms": count}

def do_analysis(b):
  print(b)
  d = count_terms_of_tmpfile(b)

  res_filename = os.path.join(b, "inv_analysis")

  print(d)

  with open(res_filename, "w") as f:
    f.write(json.dumps(d))

def run_analyses(input_directory):
  all_main_benches = paper_benchmarks.get_all_main_benchmarks()
  for b in all_main_benches:
    name = b.get_name()
    do_analysis(os.path.join(input_directory, name))

if __name__ == '__main__':
  #validate_run_invariants(sys.argv[1])
  #count_terms_of_tmpfile(sys.argv[1])
  run_analyses(sys.argv[1])
