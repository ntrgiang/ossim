setwd("/nix/workspace/omnet422-inet118/so/simulations/results/")
require(omnetpp)

scalarFiles <- file.path('Donet_Homo_oneRouter_Network-*.sca')
dataset_Hit <- loadDataset(scalarFiles, add(select='module("Donet_Homo_oneRouter_Network.globalStatistic") AND name("ChunkHit:count")'))
dataset_Miss <- loadDataset(scalarFiles, add(select='module("Donet_Homo_oneRouter_Network.globalStatistic") AND name("ChunkMiss:count")'))

#print(dataset_Hit)
#print(dataset_Miss)


d_Hit <- makeBarChartDataset(dataset_Hit, rows=c('measurement'), columns=c('name'))
print(d_Hit)

d_Miss <- makeBarChartDataset(dataset_Miss, rows=c('measurement'), columns=c('name'))
#print(d_Miss)

CI <- d_Hit / (d_Hit + d_Miss)
print(CI)

plotBarChart(CI, Legend.Display='true', Legend.Anchoring='NorthWest')

