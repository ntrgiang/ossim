setwd("/home/giang/workspace/422-omnet-inet-donet/scamp/simulations/results/")

require(omnetpp)

vectorFiles <- scalarFiles <- file.path('Donet_Homo_oneRouter_Network-0.vec')

dataset <- loadDataset(vectorFiles, add(select='module("Donet_Homo_oneRouter_Network.activePeerTable") AND name("aptSize:vector")'))

data <- loadVectors(dataset, NULL)

# print(data$vectors[,c('module','name')])

series <- data$vectordata$y

print(series)