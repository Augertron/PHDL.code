#! /usr/bin/env python

''''''
This little program demonstrates the use of the json library for processing
a JSON format PHDLIF netlist format.
''''''

import sys
import json

print '\nAttempting to open ', sys.argv[1]
try:
    fp = open(sys.argv[1],"r")
except ValueError:
    print "oops! I could not open the input file."


j1 = json.load(fp)
fp.close()


print "\nprinting report to stdout\n" 
try:
    design = j1["design_name"]
except ValueError:
    print "Error: Could not find design name.\n"
print "design = ", design

try:
    source = j1["source_file"]
except ValueError:
    print "Error: Could not find source file name.\n"
print "source file = ", source

try:
    comp_list = j1["component_list"]
except ValueError:
    print "Error: Could not find component list.\n"

for a in range(len(comp_list)):
    print "\n-----component-------------"
    for b in comp_list[a]:
        print  b, " = ", comp_list[a][b]
print "------------------"

try:
    inst_list = j1["instance_list"]
except ValueError:
    print "Error: Could not find instance list.\n"

for a in range(len(inst_list)):
    print "\n-----instance-------------"
    for b in inst_list[a]:
        print  b, " = ", inst_list[a][b]
print "------------------"

try:
    net_list = j1["net_list"]
except ValueError:
    print "Error: Could not find net list.\n"

for a in range(len(net_list)):
    print "\n-----net-------------"
    for b in net_list[a]:
        print  b, " = ", net_list[a][b]
print "------------------"

#print(j1)


