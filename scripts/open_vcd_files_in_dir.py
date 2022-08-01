import subprocess
import glob

vcd_files = glob.glob('*.vcd')
print(vcd_files)

for vcd in vcd_files:
    p = subprocess.Popen(["gtkwave", vcd])
