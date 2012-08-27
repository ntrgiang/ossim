#setwd("../simulations/results/")
setwd("/nix/workspace/omnet422-inet118/so/simulations/results/")

require(omnetpp)

vectorFiles <- scalarFiles <- file.path('Donet_Homo_oneRouter_Network-0.vec')

dataset <- loadDataset(vectorFiles, add(select='module("Donet_Homo_oneRouter_Network.globalStatistic") AND name("NumberOfPartner:vector")'))

data <- loadVectors(dataset, NULL)

# print(data$vectors[,c('module','name')])

series <- data$vectordata$y

print(series)

print(sum(series > 0))