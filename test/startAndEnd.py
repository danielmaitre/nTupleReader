import sys

NTRPath='/home/daniel/workspace/NtupleAnalysis/build_fedora/install_dir'
sys.path.append( '%s/lib' % NTRPath)

import NtuplesReader as NTR

r=NTR.NtupleReader()
r.addFile('%s/share/sample.root' % NTRPath)

r.getNumberOfEntries()

r.nextEvent()
r.getID()

r.nextEvent()
r.getID()

r.setStartEventIndex(0)
r.nextEvent()
r.getID()

r.setStartEventIndex(0)
[(i,r.nextEvent()) for i in range(0,11) ]

r.setStartEventIndex(0)
r.setEndEventIndex(5)
[(i,r.nextEvent()) for i in range(0,11) ]

r.getEntry(5)

r.nextEvent()
