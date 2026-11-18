import numpy as np
import matplotlib.font_manager as font
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import ROOT

def npTH1(histo, overflow=False):
    # Get binning structure
    x_axis = histo.GetXaxis()
    n_bins = int(x_axis.GetNbins())
    first = x_axis.GetFirst()
    last = x_axis.GetLast()
    np_x_axis = np.linspace(first,last,n_bins,endpoint=True)
    # print(np_x_axis)

    # Get bin content
    content = np.zeros(n_bins) # more edges than bins
    for i in range(n_bins):
        content[i] = histo.GetBinContent(i+1)
    content = np.array(content)[:]
        
    return content, np_x_axis

# 200Hg + 48Ti
file1 = ROOT.TFile("~/projects/root-files/GSphere18-11-2026-10-59-21.root")
hist1 = file1.Get("Geom-eff")

content, binning = npTH1(hist1)

sum = int(content[0])+int(content[1])+int(content[2])

print("Gammasphere run breakdown")
print("total:              ",sum," events")
print("misses:              ",int(content[0])," events")
print("Ge only (photopeak): ",int(content[1])," events")
print("BGO triggered:       ",int(content[2])," events")
