import ROOT
import math
import numpy as np

def getYields(fname, histnames=[]):
    f = ROOT.TFile(fname)
    yields = [f.Get(hn).GetBinContent(1) for hn in histnames]
    errors = [f.Get(hn).GetBinError(1) for hn in histnames]
    return yields, errors

def getGenLepCounts(fname, histnames=[], maxn=2):
    f = ROOT.TFile(fname)
    counts = []
    errors = []
    for hn in histnames:
        counts.append([f.Get(hn).GetBinContent(i) for i in range(1,maxn+2)])
        errors.append([f.Get(hn).GetBinError(i) for i in range(1,maxn+2)])
    return counts,errors

def getSBCounts(fname, bg_histnames=[], sig_histnames=[], ncat=4):
    f = ROOT.TFile(fname)
    sig = []
    bg = []
    for shn in sig_histnames:
        sig.append([])
        for i in range(1,ncat+1):
            s = f.Get(shn).GetBinContent(i)
            sig[-1].append(s)
    for i in range(1,ncat+1):
        b = sum([f.Get(h).GetBinContent(i) for h in bg_histnames])
        bg.append(b)

    return sig,bg

yields, errors = getYields("histos.root", ["ht_count_0","ht_count_1","ht_count_2","ht_count_3","hs_count_0","hs_count_1","hs_count_2","hc_count_0","hc_count_1","hc_count_2"])
names = ["ttbar", "WJets", "ttZ", "tW", "450/230/222.5", "650/330/325", "850/105/100", "0 lep", "1 lep", "2 lep"]

print "\n*** Yields ***"
for i in range(len(names)):
    print "{0:15s} & ${1:5.1f}\\pm {2:5.1f}$ \\\\ \\hline".format(names[i], yields[i], errors[i])

counts, errors = getGenLepCounts("histos.root", ["ht_ngenlep_0", "ht_ngenlep_1", "ht_ngenlep_2", "ht_ngenlep_3"], maxn=2)
names = ["ttbar", "WJets", "ttZ", "tW"]

print "\n*** Gen Lep Counts ***"
for i in range(len(names)):
    print"{0:6s} & ${1:4.1f}\\pm{2:4.1f}$ & ${3:4.1f}\\pm{4:4.1f}$ & ${5:4.1f}\pm{6:4.1f}$ \\\\ \\hline".format(names[i],counts[i][0],errors[i][0],counts[i][1],errors[i][1],counts[i][2],errors[i][2])


sig, bg = getSBCounts("histos.root", ["ht_count_0","ht_count_1","ht_count_2","ht_count_3"], ["hs_count_0","hs_count_1","hs_count_2"])
sig = np.array(sig)
bg = np.array(bg)
sig_names=["450/230/222.5", "650/330/325", "850/105/100"]

print "\n*** s/b ratios ***"
print "Signal  & Base selection & btag $p_T$ & topness & both \\\\ \\hline"
print "{0:15s} & {1:.1f} & {2:.1f} & {3:.1f} & {4:.1f} \\\\ \\hline".format("Background",bg[0],bg[1],bg[2],bg[3])
for i in range(len(sig)):
    print "{0:15s} & {1:.1f} & {2:.1f} & {3:.1f} & {4:.1f} \\\\ \\hline".format(sig_names[i],sig[i,0],sig[i,1],sig[i,2],sig[i,3])

bg = np.dot(np.ones(sig.shape),np.diag(bg))
ratio = sig/bg
print "\n{\\bf S/B ratio}  & Base selection & btag $p_T$ & topness & both \\\\ \\hline"
for i in range(len(sig)):
    print "{0:15s} & {1:.3f} & {2:.3f} & {3:.3f} & {4:.3f} \\\\ \\hline".format(sig_names[i],ratio[i][0], ratio[i][1], ratio[i][2], ratio[i][3])

ratio = sig/np.sqrt(sig+bg)
print "\n{\\bf S/sqrt(S+B) ratio}  & Base selection & btag $p_T$ & topness & both \\\\ \\hline"
for i in range(len(sig)):
    print "{0:15s} & {1:.3f} & {2:.3f} & {3:.3f} & {4:.3f} \\\\ \\hline".format(sig_names[i],ratio[i][0], ratio[i][1], ratio[i][2], ratio[i][3])
