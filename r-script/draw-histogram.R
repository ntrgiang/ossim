setwd("/home/giang/workspace/422-omnet-inet-donet/scamp/simulations/results/")
require(omnetpp)
vectorFiles <- scalarFiles <- file.path('Scamp_Homo_oneRouter_Network-0.vec')
dataset <- loadDataset(vectorFiles, add(select='module("Scamp_Homo_oneRouter_Network.globalStatisic") AND name("allFinalPVsizes")'))
data <- loadVectors(dataset, NULL)
# print(data$vectors[,c('module','name')])
series <- data$vectordata$y
hist(series, seq(0, max(series), step=1))
# mean(series)
summary(series)

