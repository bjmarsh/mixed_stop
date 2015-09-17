import ROOT


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

yields, errors = getYields("histos.root", ["ht_count_0","ht_count_1","ht_count_2","ht_count_3","hs_count_0","hs_count_1","hs_count_2","hc_count_0","hc_count_1","hc_count_2"])
names = ["ttbar", "WJets", "ttZ", "tW", "450/230/222.5", "650/330/325", "850/105/100", "0 lep", "1 lep", "2 lep"]

print "\n*** Yields ***"
for i in range(len(names)):
    print "{0:15s}: {1:4.1f} +- {2:4.1f}".format(names[i], yields[i], errors[i])

counts, errors = getGenLepCounts("histos.root", ["ht_ngenlep_0", "ht_ngenlep_1", "ht_ngenlep_2", "ht_ngenlep_3"], maxn=2)
names = ["ttbar", "WJets", "ttZ", "tW"]

print "\n*** Gen Lep Counts ***"
for i in range(len(names)):
    print"{0:6s}: {1:4.1f} +- {2:4.1f}\t{3:4.1f} +- {4:4.1f}\t{5:4.1f} +- {6:4.1f}".format(names[i],counts[i][0],errors[i][0],counts[i][1],errors[i][1],counts[i][2],errors[i][2])


