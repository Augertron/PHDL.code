#! /usr/bin/env python

# This little program reads the PHDLIF JSON file and
# generates the Mentor PADS Layout .asc and .p files.

import sys
import json

# This is the function that generates the output for PADS Layout.
def genPADS(jlist):
    print "Hey! We are in the genPADS procedure."
    print "!PADS-POWERPCB-V9.0-MILS"

    print "\n*PART*"
    # We need to get the list of component types so we can look up the layout footprint.
    comp_list = jlist["component_list"]
    # Now we get the list of actual part instances in the design.
    # Each line is in this format "C31 CAP_0402@CC0402"
    inst_list = jlist["instance_list"]
    for a in range(len(inst_list)):
        comp_name = inst_list[a]["comp_name"]
        print  "{} {}@{}".format(inst_list[a]["refdes"], comp_name, comp_list[comp_name]["footprint"])


    print "\n*CONNECTION*"
    net_list = j1["net_list"]
    for a in range(len(net_list)):
        print "*SIGNAL*", net_list[a]["name"]
        conn_list = net_list[a]["conn_list"]
        # Mentor prints these connections two per line so here is some extra logic to make that happen.
        line_index = 0
        for b in conn_list:
            if line_index%2 == 0:
                print b,
            else:
                print b
            line_index += 1


    print "\n*MISC*"
    print "\n*END*\n"





# Lets use a command line option parser from the optparse library.
# I guess this is where execution starts in a python program, ie., main().
from optparse import OptionParser
usage_str="usage: %prog [options] inputfile"
parser = OptionParser(usage=usage_str, add_help_option=True)
parser.add_option("-n", "--netformat", action="store",       dest="netformat",   default="PADS",       help="Selects output format. Use PADS for Mentor PADS.", metavar="FILE")
parser.add_option("-q",     "--quiet", action="store_false", dest="verbose",     default="store_true", help="don't print status messages to stderr")
(options, args) = parser.parse_args()
if len(args) != 1:
    parser.error("incorrect number of arguments")
else:
    print "reading %s..." % args[0]

    # This loads the JSON file into a python dictionary, I think.
    fp = open(args[0],"r")
    j1 = json.load(fp)
    fp.close()

    if options.netformat == "PADS":
        print "PADS output selected"
        genPADS(jlist=j1)
    else:
        print "nonsupported output selected"



