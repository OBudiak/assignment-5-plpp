import subprocess
def run_echo(input_data):
 process = subprocess.Popen(
 ['cmake-build-debug/assignment_5_oleksandr_budiak.exe'],
 stdin=subprocess.PIPE,
 stdout=subprocess.PIPE,
 stderr=subprocess.PIPE,
 text=True
 )
 stdout, stderr = process.communicate(input_data)
 return stdout.strip(), stderr.strip()
def test_echo():
 tests = [
  {
   "input": "12345\n",
   "expected_output": "12345"
  },
  {
   "input": "2+3\n",
   "expected_output": "5"
  },
  {
   "input": "2+3*4\n",
   "expected_output": "14"
  },
  {
   "input": "(2+3)*4\n",
   "expected_output": "20"
  },
  {
   "input": "-5+2\n",
   "expected_output": "-3"
  },
  {
   "input": "10/(2+3)\n",
   "expected_output": "2"
  },
  {
   "input": "max(3,7)\n",
   "expected_output": "7"
  },
  {
   "input": "min(3,7)\n",
   "expected_output": "3"
  },
  {
   "input": "pow(2,5)\n",
   "expected_output": "32"
  },
  {
   "input": "abs(-7)\n",
   "expected_output": "7"
  },
  {
   "input": "var x = 5\nx\n",
   "expected_output": "5"
  },
  {
   "input": "var y = 2\ny + 3\n",
   "expected_output": "5"
  },
  {
   "input": "def fun(a){a+1}\nfun(4)\n",
   "expected_output": "5"
  },
  {
   "input": "var z = 8\nmax(z,10)\n",
   "expected_output": "10"
  },
  {
   "input": "def bigger(a){max(a,5)}\nbigger(3)\nbigger(7)\n",
   "expected_output": "5\n7"
  },
  {
   "input": "var b = 3\npow(2,b)\n",
   "expected_output": "8"
  },
  {
   "input": "var a = 3\nvar b = 4\na * pow(b,2) + max(a,b)\n",
   "expected_output": "52"
  },
  {
   "input": "def inc(a){a+1}\ndef inc2(b){inc(b)+1}\ninc2(4)\n",
   "expected_output": "6"
  },
  {
   "input": "abs(-5+2)\n",
   "expected_output": "3"
  },
  {
   "input": "10/4\n",
   "expected_output": "2.5"
  },
  {
   "input": "1+2-3+4-5\n",
   "expected_output": "-1"
  },
  {
   "input": "var n = 1\ndef same(a){a+1}\nsame(n)\nn\n",
   "expected_output": "2\n1"
  },
  {
   "input": "def sum(a,b){a+b}\nsum(3,4)\n",
   "expected_output": "7"
  },
  {
   "input": "min(5,2)+max(3,4)\n",
   "expected_output": "6"
  },
  {
   "input": "pow(2,-1)\n",
   "expected_output": "0.5"
  },
  {
   "input": "var m = 2\nvar m = 3\nm\n",
   "expected_output": "3"
  }
 ]

 for test in tests:
  input_data = test["input"]
  expected_output = test["expected_output"]

  output, error = run_echo(input_data)

  if error:
   print(f"Test failed for input:\n{input_data}")
   print(f"Error:\n{error}")
  elif output.strip() != expected_output:
   print(f"Test failed for input:\n{input_data}")
   print(f"Expected:\n{expected_output}")
   print(f"Got:\n{output}")
  else:
   print(f"Test passed for input:\n{input_data}")
if __name__ == "__main__":
 test_echo()
