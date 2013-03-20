#! /usr/bin/env python

# This little program reads the PHDLIF JSON file and
# generates netlist output for several layout tools.

import sys
import json
import os
from time import gmtime, strftime

#####################################################3
# This function generates the output for KiCAD layout.
def genKiCAD(jlist, basefilename):
    net_filename = basefilename + ".net"
    print "Writing to", net_filename
    fp = open(net_filename,"w")
    fp.write("(export (version D) \n")

    # Write out the design section. It looks like some header information.
    fp.write("  (design \n")
    fp.write("    (source " + basefilename + ".json) \n") 
    fp.write("    (date \"" + strftime("%Y-%m-%d %H:%M:%S", gmtime()) + "\") \n") 
    fp.write("    (tool \"phdl\")) \n") 

    # Write out the components section. It is a list of instances on the board.
    fp.write("  (components \n")
    inst_list = jlist["instance_list"]
    for a in inst_list:
        fp.write("    (comp (ref " + inst_list[a]["refdes"] + ") \n")
        fp.write("      (value ?) \n")
        fp.write("      (libsource (lib ?) (part " + inst_list[a]["comp_name"] + " )) \n")
        fp.write("    ) \n")
    fp.write("  ) \n")

    # Write the libparts section.
    fp.write("  (libparts \n")
    # We need to write one "libpart" entry for each type of part used in the design.
    # Let's create of list of unique parts.
    libpart_list = []
    comp_list = jlist["component_list"]
    for a in inst_list:
        libpart_list.append(inst_list[a]["comp_name"])
    for a in libpart_list:
        fp.write("    (libpart (lib ?) (part " + a + ") \n")
        fp.write("      (footprints (fp " + comp_list[a]["footprint"] + ")) \n")
        fp.write("      (fields ?) \n")
        fp.write("      (pins \n")
        pin_list = comp_list[a]["pin_list"]
        for b in pin_list:
            fp.write("        (pin (num " + pin_list[b]["pin_number"] + ") (name " + b + ") (type ?)) \n")
        fp.write("      ) \n")
        fp.write("    ) \n")
    fp.write("  ) \n")

    # Let's write the libraries section. 
    # I don't know what it is for so I will just write a stub for now.
    fp.write("  (libraries ? \n")
    fp.write("  ) \n")

    # Let's write the nets section. 
    net_list = jlist["net_list"]
    fp.write("  (nets \n")
    for a in net_list:
        fp.write("    (net (code ?) (name " + a + ") \n")
        conn_list = net_list[a]["conn_list"]
        for b in conn_list:
            # This is tricky because connections are stored like "U1.7" in the PHDLIF JSON file bu
            # we need the refdes and the pin number separated, hence splitpath below.
            fp.write("      (node (ref " + os.path.splitext(b)[0] + ") (pin " + os.path.splitext(b)[1][1:] + ")) \n")
        fp.write("    ) \n")
    fp.write("  ) \n")


    fp.write(") \n")
    fp.close()



#####################################################################3
# This is the function that generates the output for PADS Layout.
def genPADS(jlist, basefilename):
    asc_filename = basefilename + ".asc"
    print "Writing to", asc_filename
    fp = open(asc_filename,"w")
    fp.write("!PADS-POWERPCB-V9.0-MILS\n")

    fp.write("\n*PART*\n")
    # We need to get the list of component types so we can look up the layout footprint.
    comp_list = jlist["component_list"]
    # Now we get the list of actual part instances in the design.
    # Each output line is in this format "C31 CAP_0402@CC0402"
    inst_list = jlist["instance_list"]
    for a in inst_list:
        comp_name = inst_list[a]["comp_name"]
        fp.write("{} {}@{}\n".format(inst_list[a]["refdes"], comp_name, comp_list[comp_name]["footprint"]))


    fp.write("\n*CONNECTION*\n")
    net_list = jlist["net_list"]
    for a in net_list:
        # Mentor prints these connections in a way I did not expect. They print 
        # two pins per line but the first one is the same as the second one from
        # the previous line. This means you cannot make a single pin net.
        # I don't think we want PHDLIF to be restricted in that way so I have to 
        # detect that here and delete it.
        conn_list = net_list[a]["conn_list"]
        if len(conn_list) < 2:
            print "Signal " + a + " is a single pin net, deleting"
        else:
            fp.write("*SIGNAL* " + a + "\n")
            conn = conn_list.pop()
            while (len(conn_list) > 0):
                last_conn = conn
                conn = conn_list.pop()
                fp.write(last_conn + " " + conn + "\n")

    fp.write("\n*MISC*\n")
    fp.write("\n*END*\n")
    fp.close()


    # Here we write the Mentor .p file.
    p_filename = basefilename + ".p"
    print "Writing to", p_filename
    fp = open(p_filename,"w")
    fp.write("*PADS-LIBRARY-PART-TYPES-V9*\n")
    comp_list = jlist["component_list"]
    for a in comp_list:
        fp.write("{} {} I TTL 0 1 0 0 0\n".format(a, comp_list[a]["footprint"]))
        fp.write("GATE 0 {} 1\n".format(len(comp_list[a]["pin_list"])))
        for b in comp_list[a]["pin_list"]:
            pin = comp_list[a]["pin_list"][b]
            fp.write("{} 0 L\n".format(pin["pin_number"]))
        fp.write("\n")
    fp.write("*END*\n")
    fp.close()




# Lets use a command line option parser from the optparse library.
# I guess this is where execution starts in a python program, ie., main().
from optparse import OptionParser
usage_str="usage: %prog [options] inputfile"
parser = OptionParser(usage=usage_str, add_help_option=True)
parser.add_option("-n", "--netformat", action="store",       dest="netformat",   default="PADS",       help="Selects output format. Use PADS for Mentor PADS. Use kicad for KiCAD.", metavar="FILE")
parser.add_option("-q",     "--quiet", action="store_false", dest="verbose",     default="store_true", help="don't print status messages to stderr")
(options, args) = parser.parse_args()
if len(args) != 1:
    parser.error("incorrect number of arguments")
else:
    print "reading %s..." % args[0]
    basefilename = os.path.splitext(args[0])[0]

    # This loads the JSON file into a python dictionary, I think.
    fp = open(args[0],"r")
    j1 = json.load(fp)
    fp.close()

    if options.netformat == "PADS":
        print "PADS output selected"
        genPADS(jlist=j1, basefilename=basefilename)
    elif options.netformat == "kicad":
        print "KiCad output selected"
        genKiCAD(jlist=j1, basefilename=basefilename)
    else:
        print "nonsupported output" + options.netformat + "selected"



