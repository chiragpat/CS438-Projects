from xml.dom.minidom import Document
from random import *
import string
import sys


def msg_generator(size=6, chars=string.ascii_uppercase + string.digits):
    return ''.join(choice(chars) for x in range(size))

if len(sys.argv) != 3:
    print "Usuage: python generate_xml_test.py output_filename num_nodes"
    sys.exit(1)

filename = sys.argv[1]
num_nodes = int(sys.argv[2])
doc = Document()

simulation = doc.createElement("Simulation")
topology = doc.createElement("Topology")
simulation.appendChild(topology)
doc.appendChild(simulation)

node_numbers = range(1, num_nodes+1)
links = []

for number in node_numbers:
    for y in range(0, randint(0, len(node_numbers)-1)):
        x = randint(1, len(node_numbers))
        if number != x:
            linkstr = str(x) + ":" + str(number)
            if not (linkstr in links or linkstr[::-1] in links):
                links.append(linkstr)
                link = doc.createElement("Link")
                node1 = doc.createElement("node")
                node2 = doc.createElement("node")
                cost = doc.createElement("cost")
                node1.appendChild(doc.createTextNode(str(number)))
                node2.appendChild(doc.createTextNode(str(x)))
                cost.appendChild(doc.createTextNode(str(randint(1, 100))))
                link.appendChild(node1)
                link.appendChild(node2)
                link.appendChild(cost)
                topology.appendChild(link)

events = doc.createElement("Events")
simulation.appendChild(events)
for x in range(100):
    src = randint(1, len(node_numbers))
    dest = randint(1, len(node_numbers))
    while src == dest:
        dest = randint(1, len(node_numbers))
    msg = msg_generator(randint(1, 100))
    message = doc.createElement("Message")
    srcN = doc.createElement("src")
    destN = doc.createElement("dst")
    data = doc.createElement("data")
    srcN.appendChild(doc.createTextNode(str(src)))
    destN.appendChild(doc.createTextNode(str(dest)))
    data.appendChild(doc.createTextNode(str(msg)))
    message.appendChild(srcN)
    message.appendChild(destN)
    message.appendChild(data)
    events.appendChild(message)

# print doc.toprettyxml(indent="   ")

f = open(filename, "w")
f.write(doc.toxml())
