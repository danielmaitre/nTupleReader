import sys

NTRPath='/home/daniel/workspace/NtupleAnalysis/build_fedora/install_dir'
sys.path.append( '%s/lib' % NTRPath)

import NtuplesReader as NTR

r1=NTR.nTupleReader()
r1.addFile('%s/share/RS_s1.root' % NTRPath)

r2=NTR.nTupleReader()
r2.addFile('%s/share/RS_s2.root' % NTRPath)


r1.setPDF('MSTW2008nlo68cl.LHgrid')
r2.setPDF('MSTW2008nlo68cl.LHgrid')

r1.nextEvent()
r2.nextEvent()

r2.computeWeight(r2.getFacScale(),r1.getRenScale())

